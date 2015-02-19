
#include <math.h>

#include <QStylePainter>
#include <QDebug>

#include "global.h"
#include "XafsM.h"
#include "MCAView.h"
#include "Diff.h"

#define PEAKSEARCH

// 横軸には 3つの単位がある。
// MCA pixel, eV/keV (実Energy), 描画 pixel
// MCAView 内の横軸は基本的に keV
// MCA pixel -> keV : k2p->p2E( MCA pixel ) = keV
// keV -> MCA pixel : k2p->p2E( keV ) = MCA pixel
//    k2p は keV-to-Pixel のつもり。ネーミングがまずいか...
// 描画 pixel との換算は一度 keV を通って cc.r2sx, cc.s2rx,...

#define NEAR1 ( 10 )     // ROI のエッジに近いと判断する距離(画面 pixel)
#define NEAR2 ( 10 )    // 据え置きカーソルに近いと判断する距離
#define NEAR3 ( 3 )     // 蛍光ピーク位置が近いと判断する距離

MCAView::MCAView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  Parent = parent;
  setToolTip( "" );

  k2p = NULL;
  MCA = NULL;
  rMCA = NULL;
  SMCA = NULL;  // スムージング
  DMCA = DMCA2 = DMCA3 = DMCA4 = NULL;  // 1〜4次微分
  dMCA = NULL;  // 統計変動
  E = NULL;

  //  PF = new PeakFit;
  Fit = NULL;
  // フィッティング条件のデフォルト。
  // GUI に書き込んだ値のほうが優先
  fitMaxLoop = 40;
  fitDampFact = 0.5;
  fitPrec1 = 0.1;
  fitPrec2 = 1e-3;

  MCALen = 0;
  MCACh = -1;
  realTime = 0;
  liveTime = 0;
  lowerLimit = 0;
  MaxE = 20.;
  MinE = 0.;
  mMode = M_NO;
  yRatio = 1.0;
  MaxEnergy = 20;

  ShowDiff = true;
  ShowSmoothed = true;
  DoPeakSearch = true;
  DoPeakSearch = false;
  LimitPSEnergy = true;
  PSSens = 0.5; // ピークサーチの感度
  I0Energy = 10.0;   // keV

  valid = false;
  dispLog = false;
  popDock = true;   // ボタンは年中光らせておく

  PopDialog = new QDialog;
  PopDialog->resize( 700, 400 );
  QGridLayout *bl = new QGridLayout;
  PopDialog->setLayout( bl );
  popping = false;
  layout = NULL;

  connect( PopDialog, SIGNAL( finished(int) ), this, SLOT( PopUp() ),
	   Qt::UniqueConnection );
  connect( this, SIGNAL( pop() ), this, SLOT( PopUp() ), 
	   Qt::UniqueConnection );
  
  Black = QColor( 0, 0, 0 );
  White = QColor( 255, 255, 255 );

  MCursorC    = QColor( 255,   0,   0 );  // マウスカーソル色
  MCursorC2   = QColor(   0, 150, 250 );  // 据え置きカーソル色
  MCursorC3   = QColor( 250, 180,   0 );  // マウスが近い時の据え置きカーソル色
  ROIRangeC   = QColor( 170, 210,   0 );  // ROI 内のスペクトル色
  ExROIRangeC = QColor(   0, 100, 230 );  // ROI 外のスペクトル色
  ROIEdgeC    = QColor( 250, 180,   0 );  // ROI の端点表示
  GridC       = QColor( 100, 100, 190 );  // グリッドの色
  AListC      = QColor( 200,   0, 100 );  // 元素名リスト 
  SMCAC       = QColor( 255,   0,   0 );  // スムージングしたスペクトル
  DMCAC       = QColor( 225, 180, 100 );  // 微分
  DMCAC2      = QColor( 100, 225, 180 );  // 2階微分
  DMCAC3      = QColor( 180, 100, 225 );  // 3階微分
  PEAKPOINTC   = QColor( 100, 100, 100 );  // ピークポイント情報
  PEAKPOINTLC  = QColor( 255, 150,   0 );  // ピークポイントライン
  FLC         = QColor(   0, 180,   0 );  // ピーク合成ライン
  ELC         = QColor( 100, 100, 200 );  // ピーク個別ライン
  RLC         = QColor( 200, 100, 100 );  // 残差ライン

  rROIsx = 0;
  rROIex = 20;

  // data-disp で表示した時、にマズイかも
  // ---> まずくない。 Parent が対応した slot を持っていないのでコネクトできないだけ。
  connect( this, SIGNAL( CurrentValues( int, int ) ),
	   Parent, SLOT( showCurrentValues( int, int ) ),
	   Qt::UniqueConnection );
  connect( this, SIGNAL( newROI( int, int ) ),
	   Parent, SLOT( setNewROI( int, int ) ),
	   Qt::UniqueConnection );
  connect( this, SIGNAL( newROIinEng( double, double ) ),
	   Parent, SLOT( S2DSetROIs( void ) ),
	   Qt::UniqueConnection );
  connect( this, SIGNAL( newPeakList( QVector<MCAPeak>* ) ),
	   Parent, SLOT( gotNewPeakList( QVector<MCAPeak>* ) ),
	   Qt::UniqueConnection );
  connect( Parent, SIGNAL( NewEnergy( double ) ), this, SLOT( NewEnergy( double ) ),
	   Qt::UniqueConnection );

  connect( Parent, SIGNAL( SignalMCAViewSetDisplayLog( bool ) ),
	   this, SLOT( setLog( bool ) ),
	   Qt::UniqueConnection );
  connect( Parent, SIGNAL( SignalMCAViewSetShowElements( bool ) ),
	   this, SLOT( setShowElements( bool ) ),
	   Qt::UniqueConnection );
  connect( Parent, SIGNAL( SignalMCAViewShowAlwaysSelElm( bool ) ),
	   this, SLOT( setShowElementsAlways( bool ) ),
	   Qt::UniqueConnection );
  connect( Parent, SIGNAL( SignalMCAViewShowElmEnergy( bool ) ),
	   this, SLOT( setShowElementsEnergy( bool ) ),
	   Qt::UniqueConnection );
}

MCAView::~MCAView( void )
{
  if ( MCA != NULL )
    delete [] MCA;
  if ( rMCA != NULL )
    delete [] rMCA;
  if ( SMCA != NULL )
    delete [] SMCA;
  if ( DMCA != NULL )
    delete [] DMCA;
  if ( DMCA2 != NULL )
    delete [] DMCA2;
  if ( DMCA3 != NULL )
    delete [] DMCA3;
  if ( DMCA4 != NULL )
    delete [] DMCA4;

  if ( dMCA != NULL )
    delete [] dMCA;
  if ( E != NULL )
    delete [] E;

  disconnect( this, SIGNAL( CurrentValues( int, int ) ),
	   Parent, SLOT( showCurrentValues( int, int ) ) );
  disconnect( this, SIGNAL( newROI( int, int ) ),
	   Parent, SLOT( setNewROI( int, int ) ) );
  disconnect( this, SIGNAL( newPeakList( QVector<MCAPeak>* ) ),
	   Parent, SLOT( gotNewPeakList( QVector<MCAPeak>* ) ) );
  disconnect( Parent, SIGNAL( NewEnergy( double ) ), this, SLOT( NewEnergy( double ) ) );

  PopDialog->deleteLater();
}

quint32 *MCAView::setMCAdataPointer( int len )
{
  MCA = new quint32[ MCALen = len ];
  rMCA = new double[ MCALen ];
  E = new double[ MCALen ];
  SMCA = new double[ MCALen ];
  DMCA = new double[ MCALen ];
  DMCA2 = new double[ MCALen ];
  DMCA3 = new double[ MCALen ];
  DMCA4 = new double[ MCALen ];
  dMCA = new double[ MCALen ];
  return MCA;
}

void MCAView::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}

void MCAView::print( QPrinter *p )
{
  QPainter pp( p );
  Draw( &pp );
}

double LOGS[ 9 ] = {
  0.0,         // log10( 1 )
  0.30103,     // log10( 2 )
  0.47712,     // log10( 3 )
  0.60206,     // log10( 4 )
  0.69897,     // log10( 5 )
  0.77815,     // log10( 6 )
  0.84510,     // log10( 7 )
  0.90309,     // log10( 8 )
  0.95424,     // log10( 9 )
};

//
// ピークサーチのためのスペクトルの平滑化と微分のクオリティ決定
//
#if 1       // ものすごく平滑化する例
#define SMOOTHINGRANGE  ( 5 )
double SWeight[ SMOOTHINGRANGE + 1 ] = { 1, 1, 1, 1, 1, 1 };  // 0 - 5 で 6点いる
#define DIFFOFFSET  ( -5 )  // 微分を計算する係数
#define DIFFRANGE  ( 11 )
double DWeight[ DIFFRANGE ] = { -12, -15, -20, -30, -60, 0, 60, 30, 20, 15, 12 };
double DiffNorm = 300;
#endif

#if 0       // 中間ぐらい (微分の方で平滑化を行う)
#define SMOOTHINGRANGE  ( 2 )
double SWeight[ SMOOTHINGRANGE + 1 ] = { 6, 4, 1 };
#define DIFFOFFSET  ( -5 )  // 微分を計算する係数
#define DIFFRANGE  ( 11 )
double DWeight[ DIFFRANGE ] = { -12, -15, -20, -30, -60, 0, 60, 30, 20, 15, 12 };
double DiffNorm = 300;
#endif

#if 0       // 中間ぐらい (微分ではあまり平滑化しない)
#define SMOOTHINGRANGE  ( 5 )
double SWeight[ SMOOTHINGRANGE + 1 ] = { 1, 1, 1, 1, 1, 1 };
#define DIFFOFFSET  ( -2 )  // 微分を計算する係数
#define DIFFRANGE  ( 5 )
double DWeight[ DIFFRANGE ] = { -2, -1, 0, 1, 2 };
double DiffNorm = 10;
#endif

#if 0       // ほとんど平滑化しない例
#define SMOOTHINGRANGE  ( 2 )
double SWeight[ SMOOTHINGRANGE + 1 ] = { 6, 4, 1 };
#define DIFFOFFSET  ( -2 )  // 微分を計算する係数
#define DIFFRANGE  ( 5 )
double DWeight[ DIFFRANGE ] = { -2, -1, 0, 1, 2 };
double DiffNorm = 10;
#endif

#define DELOFFSET ( -1 )  // 統計誤差を見積もる係数
#define DELRANGE  ( 3 )
double DelWeight[ DELRANGE ] = { 1, 0, 1 };

void MCAView::Draw( QPainter *p )
{
  if ( valid != true ) 
    return;

  QFont f;
  QRectF rec;

  int w = width();
  int h = height();

  int LM = w * 0.25;        // 画面を描くときの基準になる定数を幾つか決めておく
  if ( LM > 180 ) LM = 180;
  int RM = w * 0.05;
  if ( RM > 40 ) RM = 40;
  int HW = w - LM - RM;
  int TM = h * 0.05;
  if ( TM > 40 ) TM = 40;
  int BM = h * 0.1;
  if ( BM > 80 ) BM = 80;
  int VW = h - TM - BM;
  double dLM = LM / 10;
  double dVW = VW / 20;     // 1行の高さ(文字の高さ)
  double dVW2 = dVW * 1.2;  // 行間

  cc.ClearRecs();
  cc.SetScreenCoord( LM, TM, LM+HW, TM+VW );
  p->fillRect( 0, 0, w, h, White );

  // double min,
  double max, min;
  double min0 = 1e300, max0 = 0;
  // y 軸方向の表示スケール決定のため表示範囲での最大値を探す
  for ( int i = 0; i < MCALen; i++ ) {
    rMCA[i] = (double)MCA[i];
    double E = k2p->p2E( MCACh, i );
    if (( E > MinE )&&( E < MaxE )) {
      if ( MCA[i] > max0 ) {
	max0 = MCA[i];
      }
      if ( MCA[i] < min0 ) {
	min0 = MCA[i];
      }
    }
  }
  if ( dispLog ) {   // log スケールだったら最大値も log をとって縦軸のスケールにする
    if ( max0 > 0 )
      max = log10( max0 );
    else 
      max = 1;
    if ( min0 > 0 )
      min = log10( min0 );
    else 
      min = 1;
  } else {
    max = max0;
    min = min0;
  }
  // 今や横軸は MCA pixel ではなく、エネルギ-[keV]
  //  cc.SetRealCoord( MinE, min, MaxE, ( max - min ) * yRatio + min );
  cc.SetRealCoord( MinE, 0, MaxE, max * yRatio );

  double rmx = cc.s2rx( m.x() );
  double wrROIsx = rROIsx;  // wrROI.. working-real-ROI.., rROI.. real-ROI..
  double wrROIex;
  if ( mMode == M_ROI ) {      // マウスボタンを押してる時は、「終点」はマウスの現在位置
    wrROIex = rmx;
  } else {
    wrROIex = rROIex;       // そうでなければ最後にボタンを離した位置
  }
  double tmp;
  if ( wrROIsx > wrROIex ) { // 起点、終点の大小関係が逆になってたら入れ替えておく
    tmp = wrROIsx;
    wrROIsx = wrROIex;
    wrROIex = tmp;
  }
  if ( m.inPress() && ( mMode == M_ROI ) ) {
    emit newROI( k2p->E2p( MCACh, wrROIsx ), k2p->E2p( MCACh, wrROIex ) );
  }
  // 平滑化
  Smooth( true, (int *)MCA, SMCA, MCALen, 10 /* SMOOTHINGRANGE */, NULL /* SWeight */ );

  // 微係数の計算
  double maxD, minD, maxD2, minD2, maxD3, minD3, maxD4, minD4;
  int minI = 0, maxI = 0;
  if ( ShowDiff || DoPeakSearch ) {
    maxD = minD = maxD2 = minD2 = maxD3 = minD3 = 0;
    minI = k2p->E2p( MCACh, MinE );
    if ( minI < 0 ) minI = 0; if ( minI >= MCALen ) minI = MCALen - 1;
    maxI = k2p->E2p( MCACh, MaxE );
    if ( maxI < 0 ) maxI = 0; if ( maxI >= MCALen ) maxI = MCALen - 1;
    if ( ShowDiff || DoPeakSearch ) {
      // 4次の微係数まで一気に計算
      //    SDiff3( true, MCA, DMCA, DMCA2, DMCA3, MCALen, 10,
      SDiff4( false, SMCA, DMCA, DMCA2, DMCA3, DMCA4, MCALen, 10,
              WT3, WT3, WT3, WT3,
              &minD, &maxD, &minD2, &maxD2, &minD3, &maxD3, &minD4, &maxD4, minI, maxI );
      maxD = ( fabs( minD ) > fabs( maxD ) ) ? fabs( minD ) : fabs( maxD );
      minD = -maxD;
      maxD2 = ( fabs( minD2 ) > fabs( maxD2 ) ) ? fabs( minD2 ) : fabs( maxD2 );
      minD2 = -maxD2;
      maxD3 = ( fabs( minD3 ) > fabs( maxD3 ) ) ? fabs( minD3 ) : fabs( maxD3 );
      minD3 = -maxD3;
      maxD4 = ( fabs( minD4 ) > fabs( maxD4 ) ) ? fabs( minD4 ) : fabs( maxD4 );
      minD4 = -maxD4;
    }
  }
  // 局所的な統計変動
  if ( DoPeakSearch ) {
    double maxd = 0;
    for ( int i = 0; i < MCALen; i++ ) {
      double d = 1;
      if ( ( ( i + DELOFFSET ) >= 0 ) && ( ( i - DELOFFSET ) < MCALen ) ) {
        for ( int j = 0; j < DIFFRANGE; j++ ) {
          d += DelWeight[j] * SMCA[ i + j + DELOFFSET ];
        }
      }
      dMCA[i] = d = sqrt( d ) / 2;
      if ( d > maxd ) { maxd = d; }
    }
    PeakSearch( wrROIsx, wrROIex );
  }

  /* ここまでデータの準備とか色々 */
  /* ここから描画開始 */

  double lastE = 0;
  double sum = 0;
  for ( int i = 0; i < MCALen; i++ ) {       // ROI の範囲の積算と MCA スペクトルの描画
    double E = k2p->p2E( MCACh, i );         // MCA pixel から エネルギーへの換算
    if (( E >= wrROIsx )&&( E <= wrROIex )) {
      p->setPen( ROIRangeC );
      sum += MCA[i];
    } else {
      p->setPen( ExROIRangeC );
    }
    if ( dispLog ) {  // Log 表示
      if ( i > 0 ) {
	if ( MCA[i] > 0 ) {
	  p->drawLine( cc.r2sx( E ), cc.r2sy( log10( MCA[i] ) ),
		       cc.r2sx( E ), cc.r2sy( 0 ) );
	}
	if ( ShowSmoothed ) {
	  if ( ( SMCA[i] > 0 ) && ( SMCA[i-1] > 0 ) ) {
	    p->setPen( SMCAC );
	    p->drawLine( cc.r2sx( lastE ), cc.r2sy( log10( SMCA[i-1] ) ),
			 cc.r2sx( E ), cc.r2sy( log10( SMCA[i] ) ) );
	  }
	}
	if ( Fit != NULL ) {
	  p->setPen( FLC );
	  if (( FittedLine[i] > 0 )&&( FittedLine[i-1] > 0 )) {
	    p->drawLine( cc.r2sx( lastE ), cc.r2sy( log10( FittedLine[i-1] ) ),
			 cc.r2sx( E ), cc.r2sy( log10( FittedLine[i] ) ) );
	  }
	  p->setPen( ELC );
	  for ( int j = 0; j < EachLine.count(); j++ ) {
	    if (( EachLine[j][i] > 0 )&&( EachLine[j][i-1] > 0 )) {
	      p->drawLine( cc.r2sx( lastE ), cc.r2sy( log10( EachLine[j][i-1] ) ),
			   cc.r2sx( E ), cc.r2sy( log10( EachLine[j][i] ) ) );
	    }
	  }
	  p->setPen( RLC );
	  double r1 = ( DoPeakFitToRaw ? MCA[i-1] : SMCA[i-1] ) - FittedLine[i-1];
	  double r2 = ( DoPeakFitToRaw ? MCA[i] : SMCA[i] ) - FittedLine[i];
	  if ( ( r1 > 0 ) && ( r2 > 0 ) ) {
	    p->drawLine( cc.r2sx( lastE ), cc.r2sy( log10( r1 ) ),
			 cc.r2sx( E ), cc.r2sy( log10( r2 ) ) );
	  }
	}
	if ( ShowDiff ) {
	  if ( ( DMCA[i] > 0 ) && ( DMCA[i-1] > 0 ) ) {
	    p->setPen( DMCAC );
	    p->drawLine( cc.r2sx( lastE ),
			 cc.r2sy( log10( ( DMCA[i-1] - minD )
					 * max * yRatio / ( maxD - minD ) ) ),
			 cc.r2sx( E ),
			 cc.r2sy( log10( ( DMCA[i] - minD )
					 * max * yRatio / ( maxD - minD ) ) ) );
	  }
	  if ( ( DMCA2[i] > 0 ) && ( DMCA2[i-1] > 0 ) ) {
	    p->setPen( DMCAC2 ); // 2階微分
	    p->drawLine( cc.r2sx( lastE ),
			 cc.r2sy( log10( ( DMCA2[i-1] - minD2 )
					 * max * yRatio / ( maxD2 - minD2 ) ) ),
			 cc.r2sx( E ),
			 cc.r2sy( log10( ( DMCA2[i] - minD2 )
					 * max * yRatio / ( maxD2 - minD2 ) ) ) );
	  }
	  if ( ( DMCA3[i] > 0 ) && ( DMCA3[i-1] > 0 ) ) {
	    p->setPen( DMCAC3 ); // 3階微分
	    p->drawLine( cc.r2sx( lastE ),
			 cc.r2sy( log10( ( DMCA3[i-1] - minD3 )
					 * max * yRatio / ( maxD3 - minD3 ) ) ),
			 cc.r2sx( E ),
			 cc.r2sy( log10( ( DMCA3[i] - minD3 )
					 * max * yRatio / ( maxD3 - minD3 ) ) ) );
	  }
	}
      }
    } else {  // リニア表示
      p->drawLine( cc.r2sx( E ), cc.r2sy( MCA[i] ), cc.r2sx( E ), cc.r2sy( 0 ) );
      if ( i > 0 ) {
	if ( ShowSmoothed ) {
	  p->setPen( SMCAC );
	  p->drawLine( cc.r2sx( lastE ), cc.r2sy( SMCA[i-1] ),
		       cc.r2sx( E ), cc.r2sy( SMCA[i] ) );
	}
	if ( Fit != NULL ) {
	  p->setPen( FLC );
	  p->drawLine( cc.r2sx( lastE ), cc.r2sy( FittedLine[i-1] ),
			 cc.r2sx( E ), cc.r2sy( FittedLine[i] ) );
	  p->setPen( ELC );
	  for ( int j = 0; j < EachLine.count(); j++ ) {
	    p->drawLine( cc.r2sx( lastE ), cc.r2sy( EachLine[j][i-1] ),
			 cc.r2sx( E ), cc.r2sy( EachLine[j][i] ) );
	  }
	  p->setPen( RLC );
	  double r1 = ( DoPeakFitToRaw ? MCA[i-1] : SMCA[i-1] ) - FittedLine[i-1];
	  double r2 = ( DoPeakFitToRaw ? MCA[i] : SMCA[i] ) - FittedLine[i];
	  p->drawLine( cc.r2sx( lastE ), cc.r2sy( r1 ),
		       cc.r2sx( E ), cc.r2sy( r2 ) );
	}
	if ( ShowDiff ) { // 微分表示
	  p->setPen( DMCAC ); // 1階微分
	  p->drawLine( cc.r2sx( lastE ),
		       cc.r2sy( ( DMCA[i-1] - minD )
				* max * yRatio / ( maxD - minD ) ),
		       cc.r2sx( E ),
		       cc.r2sy( ( DMCA[i] - minD )
				* max * yRatio / ( maxD - minD ) ) );
	  
	  p->setPen( DMCAC2 ); // 2階微分
	  p->drawLine( cc.r2sx( lastE ),
		       cc.r2sy( ( DMCA2[i-1] - minD2 )
				* max * yRatio / ( maxD2 - minD2 ) ),
		       cc.r2sx( E ),
		       cc.r2sy( ( DMCA2[i] - minD2 )
				* max * yRatio / ( maxD2 - minD2 ) ) );
	  
	  p->setPen( DMCAC3 ); // 3階微分
	  p->drawLine( cc.r2sx( lastE ),
		       cc.r2sy( ( DMCA3[i-1] - minD3 )
				* max * yRatio / ( maxD3 - minD3 ) ),
		       cc.r2sx( E ),
		       cc.r2sy( ( DMCA3[i] - minD3 )
				* max * yRatio / ( maxD3 - minD3 ) ) );
	}
      }
    }
    lastE = E;
  }

  if ( ShowDiff ) {
    int y1, y2, y3;
    y1 = cc.r2sy( ( DMCA[maxI] - minD ) * max * yRatio / ( maxD - minD ) );
    y2 = cc.r2sy( ( DMCA2[maxI] - minD2 ) * max * yRatio / ( maxD2 - minD2 ) );
    y3 = cc.r2sy( ( DMCA3[maxI] - minD3 ) * max * yRatio / ( maxD3 - minD3 ) );
    if ( fabs( y2 - y1 ) < dVW )
      y2 = y1 + ( ( y2 > y1 ) ? dVW : -dVW ); 
    if ( fabs( y3 - y1 ) < dVW )
      y3 = y1 + ( ( y3 > y1 ) ? dVW : -dVW );
    if ( fabs( y3 - y2 ) < dVW ) {
      if ( y2 > y1 ) {
	if ( fabs( y3 - y2 ) > fabs( y3 - y1 ) )
	  y3 = y1 - dVW;
	else 
	  y3 = y2 + dVW;
      } else {
	if ( fabs( y3 - y2 ) > fabs( y3 - y1 ) )
	  y3 = y1 + dVW;
	else
	  y3 = y2 - dVW;
      }
    }

    p->setPen( DMCAC ); // 1階微分
    rec.setRect( cc.r2sx( MaxE ) + dLM/2, y1, dLM * 4, dVW );
    cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
		 "1st" );

    p->setPen( DMCAC2 ); // 1階微分
    rec.setRect( cc.r2sx( MaxE ) + dLM/2, y2, dLM * 4, dVW );
    cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
		 "2nd" );

    p->setPen( DMCAC3 ); // 1階微分
    rec.setRect( cc.r2sx( MaxE ) + dLM/2, y3, dLM * 4, dVW );
    cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
		 "3rd" );
  }

  // MCAPeaks に登録されたピークの情報表示
  if ( DoPeakSearch || ( Fit != NULL )) {
    for ( int i = 0; i < MCAPeaks.count(); i++ ) {
      int Y = dispLog ? log10( MCAPeaks[i].A ) : MCAPeaks[i].A;

      // ひげ線の先にピーク情報を書く箱を準備
      QString msg = QString( "%1 : %2[keV]\nW %3[keV], A %4" )
	.arg( i )
	.arg( prec( MCAPeaks[i].BinE, 3 ) )
	.arg( prec( MCAPeaks[i].WinE, 3 ) )
	.arg( prec( MCAPeaks[i].Area(), 1 ) );
      rec.setRect( cc.r2sx( MCAPeaks[i].BinE ) + 10, cc.r2sy( Y ) - dVW * 1.4 - 8,
		   dLM * 10, dVW * 1.4 );
      // 描画予定の文字のバウンディングボックスだけ取得
      QRectF sr = cc.UnDrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter,
				 SCALESIZE, msg );
      // 得られたバウンディングボックスを少しずつずらして
      // 既存の表示と重ならない位置を探す
      bool ok = false;
      for ( int dx = 0; dx < w; dx += dLM ) {
	for ( int dy = 0; dy < h; dy += dVW * 0.7 ) {
	  QRectF cr = sr.translated( dx, -dy );
	  if ( ! cc.IntersectRecs( cr ) ) {
	    rec = cr;
	    ok = true;
	    break;
	  }
	}
	if ( ok )
	  break;
      }
      // 重ならない位置が見つかったら(見つかってなくても)その場所を登録
      cc.AddARec( rec );

      // ピーク情報表示
      p->setPen( PEAKPOINTC );
      cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, msg );

      // 表示したピーク情報とピークを結ぶ線の表示
      p->setPen( PEAKPOINTLC );
      p->drawLine( cc.r2sx( MCAPeaks[i].BinE ), cc.r2sy( Y ),
		   cc.r2sx( MCAPeaks[i].BinE ), rec.bottom() + 4 );
      p->drawLine( cc.r2sx( MCAPeaks[i].BinE ), rec.bottom() + 4,
		   rec.left(), rec.bottom() );

    }
  }
  
  p->setPen( Black );                      // グラフ外枠の四角描画
  p->drawRect( LM, TM, HW, VW );
  p->setPen( GridC );                      // グラフの罫線描画
  for ( double E = 2; E < MaxE; E += 2.0 ) { // 2keV 刻みで主線と目盛り数字
    p->drawLine( cc.r2sx( E ), TM, cc.r2sx( E ), TM + VW );
    rec.setRect( cc.r2sx( E ) - dLM * 2, TM + VW + dVW * 0.2,
		 dLM * 4, dVW );
    cc.DrawText( p, rec, f, Qt::AlignHCenter | Qt::AlignVCenter, SCALESIZE, 
		 QString::number( E ) );
  }
  rec.setRect( cc.r2sx( MaxE ) - dLM * 2, TM + VW + dVW * 0.2,
	       dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignHCenter | Qt::AlignVCenter, SCALESIZE, "[keV]" );
  for ( double E = 1; E < 20; E += 2.0 ) { // その隙間に副線
    p->drawLine( cc.r2sx( E ), TM, cc. r2sx( E ), TM + dVW / 2 );
    p->drawLine( cc.r2sx( E ), TM + VW, cc. r2sx( E ), TM + VW - dVW / 2 );
  }

  // 縦軸ピーク値
  rec.setRect( LM - dLM*8, TM - dVW / 2, dLM * 7.5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( max0 ) );
  if ( !dispLog ) {
    for ( double y = max / 10.0; y < max; y += max / 10. ) {
      p->drawLine( LM, cc.r2sy( y ), LM + HW, cc.r2sy( y ) );
    }
  } else {
    double ten = 0;
    int sub = 0;
    double y;
    while ( 1 == 1 ) {
      if ( ( y = ten + LOGS[ sub ] ) > max )
	break;
      p->drawLine( LM, cc.r2sy( y ), LM + HW, cc.r2sy( y ) );
      sub++;
      if ( sub > 8 ) {
	sub = 0;
	ten += 1;
      }
    }
  }

  if ( ( m.x() > LM )&&( m.x() < LM+HW ) ) {   // マウスカーソル位置に縦棒描画
    p->setPen( MCursorC );
    p->drawLine( m.x(), TM, m.x(), TM+VW );
  }

  // エネルギー換算したマウスカーソル位置に対応する MCA pixel
  int curp = k2p->E2p( MCACh, cc.s2rxLimit( m.x() ) );
  emit CurrentValues( MCA[ curp ], sum );

  if ( nearf ) {              // マウスカーソルが ROI の両端に近いと認識しているときは
                              // 近いと思っている方の橋を強調? 表示
    p->setPen( ROIEdgeC );
    p->drawLine( nearX, TM, nearX, TM+VW );
  }

  for ( int i = 0; i < cPoints.count(); i++ ) {  // 据え置きカーソル位置に縦棒
    int cpx = cc.r2sx( cPoints[i] );
    if ( abs( cpx - m.x() ) < NEAR2 )
      p->setPen( MCursorC3 );
    else
      p->setPen( MCursorC2 );
    p->drawLine( cpx, TM, cpx, TM+VW );
  }

  if ( showElements ) {   // 元素名表示
    if ( showElementsAlways ) {
      // 元素名常時表示の場合
      int dispx1, dispw;
      int lines = 16;            // 元素名は最大16段表示
      QVector<int> lend;         // 格段の最終表示位置
      for ( int i = 0; i < lines; i++ ) { lend << 0; }
      if ( ! ( m.getMod() & Qt::ShiftModifier ) ) {
	QVector<Fluo> inRange = fdbase->inRange( MinE, MaxE,
						 cc.s2rx( NEAR3 ) - cc.s2rx( 0 ) );
	p->setPen( AListC );
	for ( int i = 0; i < inRange.count(); i++ ) {
	  if ( inRange[i].dispf ) {
	    double v = inRange[i].val;
	    QString show = inRange[i].fullName;
	    if ( showElementsEnergy )
	      show += " " + QString::number( inRange[i].val );
	    dispx1 = cc.r2sx( v ) - dLM * 10;
	    dispw = dLM * 9.5;
	    int j;
	    for ( j = 0; j < lines; j++ ) {
	      if ( lend[j] < dispx1 ) {
		p->drawLine( cc.r2sx( v ), TM+VW, cc.r2sx( v ), TM+VW-dVW*(3+j) );
		p->drawLine( cc.r2sx( v ), TM+VW-dVW*(3+j),
			     cc.r2sx( v ) - dLM * 0.5, TM+VW-dVW*(4+j) );
		rec.setRect( dispx1, TM+VW-dVW*(4+j+0.5), dispw, dVW );
		cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE,
			     show );
		lend[j] = dispx1 + dispw;
		break;
	      }
	    }
	    if ( j >= lines ) {
	      p->drawLine( cc.r2sx( v ), TM+VW, cc.r2sx( v ), TM+VW-dVW*(3+j) );
	      p->drawLine( cc.r2sx( v ), TM+VW-dVW*(3+j),
			   cc.r2sx( v ) - dLM * 0.5, TM+VW-dVW*(4+j) );
	      rec.setRect( dispx1, TM+VW-dVW*(4+j+0.5), dispw, dVW );
	      cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE,
			   show );
	      lend[j] = dispx1 + dispw;
	    }
	  }
	}
      }
    } else {
      if ( ! ( m.getMod() & Qt::ShiftModifier ) ) {
	// マウスカーソル付近のみ元素名表示の場合
	QVector<Fluo> nears = fdbase->nears( rmx, cc.s2rx( NEAR3 ) - cc.s2rx( 0 ) );
	p->setPen( AListC );
	bool isUpper = ( rmx > ( MaxE + MinE ) / 2 );
	for ( int i = 0; i < nears.count(); i++ ) {
	  int pp;
	  if ( nears[i].dispf ) {
	    double v = nears[i].val;
	    pp = ( isUpper ) ? i : ( nears.count() - i - 1 );
	    p->drawLine( cc.r2sx( v ), TM+VW, cc.r2sx( v ), TM+VW-dVW*(3+pp) );
	    p->drawLine( cc.r2sx( v ), TM+VW-dVW*(3+pp), 
			 cc.r2sx( v ) + ( ( isUpper ) ? -dLM * 0.5 : dLM * 0.5 ),
			 TM+VW-dVW*(4+pp) );
	    QString show = nears[i].fullName;
	    if ( showElementsEnergy )
	      show += " " + QString::number( nears[i].val );
	    if ( isUpper ) {
	      rec.setRect( cc.r2sx( v ) - dLM * 10, TM+VW-dVW*(4+pp+0.5),
			   dLM * 9.5, dVW );
	      cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE,
			   show );
	    } else {
	      rec.setRect( cc.r2sx( v ) + dLM * 0.5, TM+VW-dVW*(4+pp+0.5),
			   dLM * 9.5, dVW );
	      cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE,
			   show );
	    }
	  }
	}
      }
    }
  }

  p->setPen( Black );

  QStringList titles, vals;
  if ( MCACh >= 0 ) {   // MCA ch 番号
    titles << tr( "MCA Ch. : " );
    vals << QString::number( MCACh );
  }
  // カーソル(タイトル)
  titles << tr( "Cursor" );
  vals << "";

  // カーソル位置( 実エネルギー換算 )
  titles << tr( " Pos. [keV] : " );
  vals << QString::number( cc.s2rxLimit( m.x() ), 'f', 3 );

  // カーソル位置( MCA pixel )
  titles << tr( " Pos. [ch] : " );
  vals << QString::number( k2p->E2p( MCACh, cc.s2rxLimit( m.x() ) ) );

  // カーソル位置の MCA 値
  titles << tr( " Val. : " );
  vals << QString::number( MCA[ curp ] );

  // ROI タイトル
  titles << tr( "ROI " );
  vals << "";

  // ROI のスタート位置 ( MCA pixel )
  titles << tr( " Start [keV] : " );
  vals << QString::number( wrROIsx, 'g', 3 );

  // ROI の終了位置 ( MCA pixel )
  titles << tr( " End [keV] : " );
  vals << QString::number( wrROIex , 'g', 3 );

  // ROI 内の積分値
  titles << tr( " Count : " );
  vals  << QString::number( (double)sum, 'g', 4 );

  // ROI 内の積分値を LiveTime で割った cps
  titles << tr( "   CPS : " );
  vals << QString::number( sum / (( realTime == 0 )? 1 : realTime ), 'g', 4 );

  // Real Time
  titles << tr( "Real Time : " );
  vals << QString::number( realTime, 'f', 3 );

  // Live Time
  titles << tr( "Live Time : " );
  vals << QString::number( liveTime, 'f', 3 );

  // デッドタイム
  double dt = ( realTime != 0 ) ? ( realTime - liveTime )/realTime * 100: 0;
  titles << tr( "Dead Time : " );
  vals << QString::number( dt, 'f', 3 );

  rec.setRect( dLM,   TM+dVW, dLM * 4, dVW );
  cc.DrawTexts( p, rec, 0, dVW2, f, Qt::AlignLeft | Qt::AlignVCenter, FIXSIZE, titles );
  rec.setRect( dLM*5, TM+dVW, dLM * 4, dVW );
  cc.DrawTexts( p, rec, 0, dVW2, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, vals );
  
  //　　 青   ->  緑   ->  黄   -> オレンジ -> 赤　　にするとしたら？
  //    0 0 1 -> 0 1 0 -> 1 1 0 ->    ->      1 0 0
  //     0 %      10%      20%        ->       100%
  int r, g, b;
  r = g = b = 0;
  if ( dt < 0 ) dt = 0;
  if ( dt >= 100 ) dt = 100;
  if ( dt < 10 ) { r = 0; g = (int)( dt / 10 * 255 ); b = 255 - g; };
  if (( 10 <= dt )&&( dt < 20 )) { r = (int)( ( dt - 10 ) / 10 * 255 ); g = 255; b = 0; };
  if ( dt >= 20 ) { r = 255; g = 255 - (int)( ( dt - 20 ) / 80 * 255 ); b = 0; };
  if ( r < 0 ) r = 0;
  if ( g < 0 ) g = 0;
  if ( b < 0 ) b = 0;
  QColor DTC = QColor( r, g, b );
  QRectF bbox = rec;
  double rw = rec.width();
  rec.translate( (double)rw / 100 * ( 100 - dt ), 0 );
  rec.setWidth( (double)rw / 100 * dt );
  p->fillRect( rec, DTC );
  p->setPen( Black );
  p->drawRect( bbox );
  
  cc.ShowAButton( p, popDock, tr( "Pop/Dock" ), 0, 100, height() );
}

void MCAView::PeakSearch( double Es, double Ee )
{
  MCAPeaks.clear();
  MCAPeak aPeak;

  QVector<int> Xps;
  QVector<int> Signs;
  int sXp = -1, Xp = -1;
  int oSign = 0, Sign = 0;

  if ( LimitPSEnergy ) // ピークサーチを入射X線のエネルギーまでに制限する
    if ( Ee > I0Energy + 2.0 )
      Ee = I0Energy + 0.1;
  int minI = k2p->E2p( MCACh, Es );
  int maxI = k2p->E2p( MCACh, Ee );

  // こっちが本来のピークサーチ
  for ( int i = 1; i < MCALen; i++ ) {
    if (( i >= minI )&&( i <= maxI )) {
      if ( ( DMCA3[i] == 0 ) || ( DMCA3[ i-1 ] * DMCA3[ i ] < 0 ) ) {
	// 3階微分が 0クロスする点を発見 !
	// 同じ点の 2階微分の符号
	// (これが負ならピーク、これが正なら裾野の果て)
	Sign = ( DMCA2[i] < 0 ) ? -1 : 1;
	// 境界付近でバタバタして何回も 0 クロスする可能性があるので、
	// Sign が変わらないままの一連の 0 クロス点はまとめて一つにする
	if ( oSign != Sign ) { // 2階微分の符号が変わってた
	  if ( sXp >= 0 ) { // その前までの(同符号の)ピークをまとめる
	    Xps << (int)(( sXp + Xp ) / 2 );
	    Signs << oSign;
	  }
	  sXp = i;
	}
	Xp = i;
	oSign = Sign;
      }
    }
  }

  int w1, w2, w;
  for ( int i = 0; i < Xps.count(); i++ ) {  // 見つけたピークに関して吟味
    if ( Signs[i] < 0 ) {
      w1 = ( i > 0 ) ? ( Xps[i] - Xps[i-1] ) : 0;
      w2 = ( i < Xps.count() - 1 ) ? ( Xps[i+1] - Xps[i] ) : 0;

      // ピーク近辺のカウントの平均
      double avr = 0;
      int cnt = 0;
      for ( int j = Xps[i] - w1; j <= Xps[i] + w2; j++ ) {
	avr += SMCA[i];
	cnt++;
      }
      avr /= cnt;
      // ピーク高さの見積り
#if 1
      double ph = 0;
      double ph1 = SMCA[Xps[i]] - SMCA[Xps[i]-w1];
      double ph2 = SMCA[Xps[i]] - SMCA[Xps[i]+w2];
      if (( ph1 > 0 )&&( ph2 > 0 ))
	ph = ( ph1 + ph2 ) / 2;
      if (( ph1 < 0 )&&( ph2 > 0 ))
	ph = ph2;
      if (( ph1 > 0 )&&( ph2 < 0 ))
	ph = ph1;
      if (( ph1 < 0 )&&( ph2 < 0 ))
	ph = SMCA[Xps[i]];
#else
      double ph = SMCA[ Xps[i] ] - ( SMCA[ Xps[i]-w1 ] + SMCA[ Xps[i]-w2 ] ) / 2;
#endif

      // 推定誤差( x 感度 )より大きなピークなら
      // ほんとにピークと認める
      if ( ph > sqrt( avr ) * PSSens ) {
	if ( w1 == 0 ) w = w2;
	else if ( w2 == 0 ) w = w1;
	else w = ( w1 < w2 ) ? w1 : w2;   // 半値幅は小さい方を採用
	w *= 0.7;
	aPeak.A = SMCA[ Xps[i] ];
	aPeak.BinP = Xps[i];
	aPeak.BinE = k2p->p2E( MCACh, aPeak.BinP );
	int sp = (( Xps[i] - w ) >= 0 ) ? ( Xps[i] - w ) : 0;
	int ep = (( Xps[i] + w ) < MCALen ) ? ( Xps[i] + w ) : MCALen - 1;
	aPeak.WinP = ep - sp;
	double sE = k2p->p2E( MCACh, sp );
	double eE = k2p->p2E( MCACh, ep );
	aPeak.WinE = eE - sE;
	aPeak.CinE = 4. * log( 2. ) / ( aPeak.WinE * aPeak.WinE );
	aPeak.CinP = 4. * log( 2. ) / ( aPeak.WinP * aPeak.WinP );
	MCAPeaks << aPeak;
      }
    }
  }

  // 上の方法だと見つからないピークがあるのが悔しいので 4階微分まで使ってみる。
  {
    int ooXp, oXp, Xp;
    int ooSign, oSign, Sign;
    ooXp = oXp = Xp = -1;
    ooSign = oSign = Sign = 0;
    for ( int i = 1; i < MCALen; i++ ) {
      if (( i >= minI )&&( i <= maxI )) {
	if ( ( DMCA4[i] == 0 ) || ( DMCA4[ i-1 ] * DMCA4[ i ] < 0 ) ) {
	  // 4階微分が 0クロする点を発見 !
	  if ( ( i - oXp ) >= 8 ) { // 前の点と十分離れてたら
	    // 新しい交点だと認める
	    Xp = i;
	    Sign = ( DMCA3[i] < 0 ) ? -1 : 1;
	    // 同じ点の 3階微分の符号が連続して同じなら間にピークがあるのではと疑う
	    if (( oSign == Sign )&&( ooSign == oSign )) {
	      if ( ( SMCA[ oXp ] - ( SMCA[ ooXp ] + SMCA[ Xp ] ) / 2. )
		   > sqrt( SMCA[oXp] ) * PSSens ) {
		aPeak.A = SMCA[ oXp ];
		aPeak.BinP = oXp;
		aPeak.BinE = k2p->p2E( MCACh, oXp );
		int sp = ooXp;
		int ep = Xp;
		aPeak.WinP = ep - sp;
		double sE = k2p->p2E( MCACh, sp );
		double eE = k2p->p2E( MCACh, ep );
		aPeak.WinE = eE - sE;
		aPeak.CinE = 4. * log( 2. ) / ( aPeak.WinE * aPeak.WinE );
		aPeak.CinP = 4. * log( 2. ) / ( aPeak.WinP * aPeak.WinP );
		MCAPeaks << aPeak;
	      }
	    }
	    ooXp = oXp;
	    oXp = Xp;
	    ooSign = oSign;
	    oSign = Sign;
	  }
	}
      }
    }
  }
  emit newPeakList( &MCAPeaks );
}


void MCAView::mouseMoveEvent( QMouseEvent *e )
{
  if ( m.CheckABPosition( e, 0, height() ) ) {
    return;
  }

  m.Moved( e );
  nearf = false;
  if ( !m.inPress() ) {
    int dsx = fabs( e->x() - m.sx() );
    int dex = fabs( e->x() - m.ex() );

    if ( ( dsx < NEAR1 ) || ( dex < NEAR1 ) ) {
      if ( dsx < dex ) {
	nearX = m.sx();
      } else {
	nearX = m.ex();
      }
      nearf = true;
    }
  } else {
    double dE;
    switch( mMode ) {
    case M_H_SHIFT:
      dE = ( e->x() - m.sx() ) / ( cc.Smaxx() - cc.Sminx() ) * ( MaxE - MinE );
      MinE -= dE;
      MaxE -= dE;
      m.setSx( e->x() );
      break;
    default:
      break;
    }
  }
  update();
}

void MCAView::mousePressEvent( QMouseEvent *e )
{
  if ( m.CheckABPosition( e, 0, height() ) ) {
    return;
  }

  if ( e->modifiers() & Qt::ShiftModifier ) { // シフトキーを押しながら
    if ( e->button() == Qt::LeftButton ) {   // shift + 左ボタンで 並行移動モード
      mMode = M_H_SHIFT;
      m.Pressed( e );
    }
  } else {                                    // シフトキー無し
    if ( e->button() == Qt::LeftButton ) {   // 左ボタンで ROI 設定モード
      mMode = M_ROI;
      int dsx = fabs( e->x() - m.sx() );
      int dex = fabs( e->x() - m.ex() );
      int setX;
      if ( ( dsx < NEAR1 ) || ( dex < NEAR1 ) ) {   // 近くに既設の ROI の端点があるか
	if ( dsx < dex ) {                        // あれば ROI 変更
	  setX = m.ex();
	} else {
	  setX = m.sx();
	}
      } else {                                    // なければ ROI 新規
	setX = e->x();
      }
      m.Pressed( e );
      m.setSx( setX );
      rROIsx = cc.s2rx( m.sx() );
    }
    if ( e->button() == Qt::RightButton ) {  // 右ボタンで カーソル設置/削除
      double checkp = cc.s2rx( e->x() );
      bool del = false;
      int nearp = -1;
      double nearv = 1e300;
      for ( int i = 0; i < cPoints.count(); i++ ) { // 既設のカーソルが近くにあるか
	if ( fabs( cPoints[i] - checkp ) < nearv ) {  // 一番近い既設カーソルを探す
	  nearp = i;
	  nearv = fabs( cPoints[i] - checkp );
	}
      }
      if ( nearp >= 0 ) {
	if ( fabs( cc.r2sx( cPoints[ nearp ] ) - e->x() ) < NEAR2 ) {
	  // 最近接がNEAR以下の場所だったら
	  cPoints.remove( nearp );                  // その点を削除
	  del = true;
	}
      }
      if ( ! del ) {    // 削除したカーソルがなければカーソル設置
	cPoints << checkp;
      }
      doPeakFitWCPoints();
    }
  }
}

void MCAView::mouseReleaseEvent( QMouseEvent *e )
{
  if ( m.CheckABPosition( e, 0, height() ) ) {
    //    popDock = ! popDock;
    emit pop();
    return;
  }

  if ( mMode == M_ROI ) {
    m.Released( e );
    rROIex = cc.s2rx( m.ex() );

    double tmp;
    double wrROIsx = rROIsx;
    double wrROIex = rROIex;
    if ( wrROIsx > wrROIex ) { // 起点、終点の大小関係が逆になってたら入れ替えておく
      tmp = wrROIsx;
      wrROIsx = wrROIex;
      wrROIex = tmp;
    }
    emit newROIinEng( wrROIsx, wrROIex );
    if ( cPoints.count() > 0 ) {
      doPeakFitWCPoints();
    }
    update();
  }
  mMode = M_NO;
}

void MCAView::mouseDoubleClickEvent( QMouseEvent * )
{
}

void MCAView::setROI( int s, int e )   // MCA pixel
{
  rROIsx = k2p->p2E( MCACh, s );
  rROIex = k2p->p2E( MCACh, e );
}

void MCAView::wheelEvent( QWheelEvent *e )
{
  double step = ( e->delta() / 8. ) / 15.;    // deg := e->delta / 8.
  if ( e->modifiers() & Qt::ShiftModifier ) { // シフトキーを押しながらでは縦軸の拡大縮小
    if ( step > 0 ) {
      yRatio *= 0.9;
    } else {
      yRatio /= 0.9;
    }
    if ( yRatio > 1.0 )
      yRatio = 1.0;
  } else {                                    // そうでなければ横軸の拡大縮小
    double rx = cc.s2rx( e->x() );
    double drx = MaxE - MinE;
    if ( step > 0 ) {
      drx *= 0.9;
    } else {
      drx /= 0.9;
    }
    MinE = rx - ( e->x() - cc.Sminx() ) / ( cc.Smaxx() - cc.Sminx() ) * drx;
    MaxE = MinE + drx;

    if ( step < 0 ) {
      if ( MinE < 0 ) MinE = 0;
      if ( MaxE > MaxEnergy ) MaxE = MaxEnergy;
    }
  }
  update();
}

void MCAView::setNewPSSens( QString newSens )
{
  double s = newSens.toDouble();

  if ( s > 0 ) {
    PSSens = s;
    update();
  }
}

QStringList MCAView::getSelectedElms( void )
{
  QStringList ret;
  QVector<Fluo> inRange = fdbase->inRange( MinE, MaxE,
					   cc.s2rx( NEAR3 ) - cc.s2rx( 0 ) );
  for ( int i = 0; i < inRange.count(); i++ ) {
    if ( inRange[i].dispf ) {
      QString show = inRange[i].fullName;
      show += " " + QString::number( inRange[i].val );
      ret << show;
    }
  }    

  return ret;
}

void MCAView::setFittedLines( int peaks, double *E )
{
  FittedLine.clear();
  for ( int i = 0; i < EachLine.count(); i++ )
    EachLine[i].clear();
  EachLine.clear();
  FittedLine.resize( MCALen );
  EachLine.resize( peaks );
  for ( int i = 0; i < EachLine.count(); i++ )
    EachLine[i].resize( MCALen );
  
  for ( int i = 0; i < MCALen; i++ ) {
    double e = E[i];
    double sum = 0;
    for ( int j = 0; j < peaks; j++ ) {
      sum += EachLine[j][i] = Fit->f( j, e );
    }
    FittedLine[i] = sum;
  }
}

void MCAView::doPeakFitWCPoints( void )
{
  double Rs = rROIsx;
  double Re = rROIex;
  if ( Rs > Re ) {
    double tmp = Rs;
    Rs = Re;
    Re = tmp;
  }

  int ROIs = k2p->E2p( MCACh, Rs );
  int ROIe = k2p->E2p( MCACh, Re );
  if ( ROIs < 0 )
    ROIs = 0;
  if ( ROIe >= MCALen )
    ROIe = MCALen - 1;
  
  double *E = new double [ MCALen ];               // 最大長をとっておく
  double *p = new double [ cPoints.count() * 3 ];  // 最大個数でとっておく

  for ( int i = 0; i < MCALen; i++ ) {
    E[i] = k2p->p2E( MCACh, i );
  }

  int peaks = 0;
  for ( int i = 0; i < cPoints.count(); i++ ) {
    double e = cPoints[i];
    int ei = k2p->E2p( MCACh, e );
    if ( ei < 0 ) ei = 0;
    if ( ei >= MCALen ) ei = MCALen - 1;
    if ( ( e >= Rs ) && ( e <= Re ) ) {
      p[peaks*3]   = sqrt( fabs( SMCA[ ei ] ) );
      p[peaks*3+1] = e;
      p[peaks*3+2] = sqrt( fabs( 5.0 ) );
      peaks++;
    }
  }

  newFit( peaks );
  Fit->fit( ROIe - ROIs + 1, E + ROIs,
	    ( DoPeakFitToRaw ) ? ( rMCA + ROIs ) : ( SMCA + ROIs ),
	    p, fitMaxLoop, fitDampFact, fitPrec1, fitPrec2 );

  setMCAPeaksByFit();
  setFittedLines( peaks, E );

#if 1
  QFile f( "MCAPeakFitStat.dat" );
  f.open( QIODevice::Text | QIODevice::Append );
  QTextStream out( &f );
  out << "= = = = =\n";
  f.close();
#endif
  
  delete [] E;
  delete [] p;
  update();
}

void MCAView::newFit( int peaks )
{
  if ( Fit != NULL ) {
    delete Fit;
    disconnect( Fit );
  }
  Fit = new Gs( peaks );
  connect( Fit, SIGNAL( nowStat( QString & ) ),
	   Parent, SLOT( nowFitStat( QString & ) ),
	   Qt::UniqueConnection );
}

void MCAView::setMCAPeaksByFit( void )
{
  int peaks = Fit->peaks();

  MCAPeaks.clear();
  for ( int i = 0; i < peaks; i++ ) {
    MCAPeak ap;
    ap.A = Fit->ag( i )->a();
    ap.A *= ap.A;
    ap.BinE = Fit->ag(i)->b();     // エネルギー単位のピーク位置
    ap.BinP = k2p->E2p( MCACh, ap.BinE );
    ap.CinE = Fit->ag(i)->c();
    ap.CinE *= ap.CinE;
    ap.WinE = Fit->ag(i)->w();     // エネルギー単位の半値全幅
    double sp = k2p->E2p( MCACh, ap.BinE - ap.WinE / 2. );
    double ep = k2p->E2p( MCACh, ap.BinE + ap.WinE / 2. );
    ap.WinP = ( ep - sp );        // pixel単位の半値全幅
    ap.CinP = 4. * log( 2 ) / ( ap.WinP * ap.WinP );
    MCAPeaks << ap;
  }
  emit newPeakList( &MCAPeaks );
}

void MCAView::clearMCAPeaks( void )
{
  cPoints.clear();
  MCAPeaks.clear();
  emit newPeakList( &MCAPeaks );
}

void MCAView::PopUp( void )
{
  if ( popping ) {
    layout->addWidget( this );
    PopDialog->hide();
  } else {
    PopDialog->layout()->addWidget( this );
    PopDialog->show();
  }
  popping = ! popping;
}
