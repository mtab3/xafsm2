#include "XafsM.h"
#include "MCAView.h"

// 横軸には 3つの単位がある。
// MCA pixel, eV (実Energy), 描画 pixel
// MCAView 内の横軸は基本的に keV
// MCA pixel -> keV : k2p->p2E( MCA pixel ) = keV
// keV -> MCA pixel : k2p->p2E( keV ) = MCA pixel
//    k2p は keV-to-Pixel のつもり。ネーミングがまずいか...
// 描画 pixel との換算は一度 keV を通って cc.r2sx, cc.s2rx,...

#define NEAR ( 10 )     // ROI のエッジに近いと判断する距離(画面 pixel)
#define NEAR2 ( 10 )    // 据え置きカーソルに近いと判断する距離

MCAView::MCAView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  Parent = parent;
  setToolTip( "" );

  k2p = NULL;
  MCA = NULL;
  MCALen = 0;
  MCACh = -1;
  realTime = 0;
  liveTime = 0;
  MaxE = 20.;
  MinE = 0.;
  mMode = M_NO;

  valid = false;
  dispLog = false;

  Black = QColor( 0, 0, 0 );
  White = QColor( 255, 255, 255 );

  MCursorC    = QColor( 255,   0,   0 );  // マウスカーソル色
  MCursorC2   = QColor(   0, 200, 245 );  // 据え置きカーソル色
  MCursorC3   = QColor( 250, 180,   0 );  // マウスが近い時の据え置きカーソル色
  ROIRangeC   = QColor( 170, 210,   0 );  // ROI 内のスペクトル色
  ExROIRangeC = QColor(   0, 100, 230 );  // ROI 外のスペクトル色
  ROIEdgeC    = QColor( 250, 180,   0 );  // ROI の端点表示
  GridC       = QColor( 100, 100, 190 );  // グリッドの色
  AListC      = QColor( 200,   0, 100 );  // 元素名リスト 

  rROIsx = 0;
  rROIex = 20;

  // data-disp で表示した時、にマズイかも
  connect( this, SIGNAL( CurrentValues( int, int ) ),
	   Parent, SLOT( showCurrentValues( int, int ) ) );
  connect( this, SIGNAL( newROI( int, int ) ),
	   Parent, SLOT( setNewROI( int, int ) ) );
}

MCAView::~MCAView( void )
{
  if ( MCA != NULL )
    delete MCA;
  disconnect( this, SIGNAL( CurrentValues( int, int ) ),
	   Parent, SLOT( showCurrentValues( int, int ) ) );
  disconnect( this, SIGNAL( newROI( int, int ) ),
	   Parent, SLOT( setNewROI( int, int ) ) );
}

int *MCAView::setMCAdataPointer( int len )
{
  MCA = new int[ MCALen = len ];
  return MCA;
}

void MCAView::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
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

void MCAView::Draw( QPainter *p )
{
  if ( valid != true ) 
    return;

  QFont f;
  QRectF rec;

  k2p->setMCALen( MCALen );

  int w = width();
  int h = height();

  int LM = w * 0.25;
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

  cc.SetScreenCoord( LM, TM, LM+HW, TM+VW );
  p->fillRect( 0, 0, w, h, White );

  double max;
  double max0 = 0;    // y 軸方向の表示スケール決定のため表示範囲での最大値を探す
  for ( int i = 0; i < MCALen; i++ ) {
    double E = k2p->p2E( MCACh, i );
    if (( E > MinE )&&( E < MaxE )) {
      if ( MCA[i] > max0 ) {
	max0 = MCA[i];
      }
    }
  }
  if ( dispLog ) {   // log スケールだったら最大値も log をとって縦軸のスケールにする
    if ( max0 > 0 )
      max = log10( max0 );
    else 
      max = 1;
  } else {
    max = max0;
  }
  //  cc.SetRealCoord( 0, 0, MCALen-1, max );
  cc.SetRealCoord( MinE, 0, MaxE, max );          // 今や横軸は MCA pixel ではなく、
                                               // エネルギ-[keV]
  // 正しく調整されていると MCP pixel = エネルギー[eV]/10 になっているはず。

  double rmx = cc.s2rx( m.x() );
  double wrROIsx = rROIsx;  // wrROI.. working-real-ROI.., rROI.. real-ROI..
  double wrROIex;
  if ( m.inPress() ) {      // マウスボタンを押してる時は、「終点」はマウスの現在位置
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

  int sum = 0;
  for ( int i = 0; i < MCALen; i++ ) {       // ROI の範囲の積算と MCA スペクトルの描画
    double E = k2p->p2E( MCACh, i );         // MCA pixel から エネルギーへの換算
    if (( E >= wrROIsx )&&( E <= wrROIex )) {
      p->setPen( ROIRangeC );
      sum += MCA[i];
    } else {
      p->setPen( ExROIRangeC );
    }
    if ( dispLog ) {
      if ( MCA[i] > 0 )
	p->drawLine( cc.r2sx( E ), cc.r2sy( log10( MCA[i] ) ),
		     cc.r2sx( E ), cc.r2sy( 0 ) );
    } else {
      p->drawLine( cc.r2sx( E ), cc.r2sy( MCA[i] ), cc.r2sx( E ), cc.r2sy( 0 ) );
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

  if ( showElements ) {
    if ( showElementsAlways ) {
      if ( ! ( m.getMod() & Qt::ShiftModifier ) ) {
	QVector<Fluo> inRange = fdbase->inRange( MinE, MaxE );
	p->setPen( AListC );
	for ( int i = 0; i < inRange.count(); i++ ) {
	  double v = inRange[i].val;
	  p->drawLine( cc.r2sx( v ), TM+VW, cc.r2sx( v ), TM+VW-dVW*3 );
	  QString show = QString( "%1 %2" )
	    .arg( inRange[i].fullName ).arg( inRange[i].val );
	  rec.setRect( cc.r2sx( v ) - dLM * 10, TM+VW-dVW*(4+0.5), dLM * 9.5, dVW );
	  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, show );
	}
      }
    } else {
      if ( ! ( m.getMod() & Qt::ShiftModifier ) ) {
	// マウスカーソル付近の元素リスト
	QVector<Fluo> nears = fdbase->nears( rmx );
	p->setPen( AListC );
	bool isUpper = ( rmx > ( MaxE + MinE ) / 2 );
	for ( int i = 0; i < nears.count(); i++ ) {
	  int pp;
	  double v = nears[i].val;
	  pp = ( isUpper ) ? i : ( nears.count() - i - 1 );
	  p->drawLine( cc.r2sx( v ), TM+VW, cc.r2sx( v ), TM+VW-dVW*(3+pp) );
	  p->drawLine( cc.r2sx( v ), TM+VW-dVW*(3+pp), 
		       cc.r2sx( v ) + ( ( isUpper ) ? -dLM * 0.5 : dLM * 0.5 ),
		       TM+VW-dVW*(4+pp) );
	  QString show = QString( "%1 %2" )
	    .arg( nears[i].fullName ).arg( nears[i].val );
	  if ( isUpper ) {
	    rec.setRect( cc.r2sx( v ) - dLM * 10, TM+VW-dVW*(4+pp+0.5), dLM * 9.5, dVW );
	    cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, show );
	  } else {
	    rec.setRect( cc.r2sx( v ) + dLM * 0.5, TM+VW-dVW*(4+pp+0.5), dLM * 9.5, dVW );
	    cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, show );
	  }
	}
      }
    }
  }

  p->setPen( Black );

  int LINE = 1;
  if ( MCACh >= 0 ) {   // MCA ch 番号
    rec.setRect( dLM, TM + dVW2 * LINE, dLM * 4, dVW );
    cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
		 tr( "MCA Ch. : " ) );
    rec.setRect( dLM*5, TM + dVW2 * LINE, dLM * 4, dVW );
    cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
		 QString::number( MCACh ) );
    LINE++;
  }

  // カーソル(タイトル)
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 6, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Cursor" ) );
  LINE++;

  // カーソル位置( 実エネルギー換算 )
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( " Pos. [keV] : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( cc.s2rxLimit( m.x() ), 'f', 3 ) );
  LINE++;

  // カーソル位置( MCA pixel )
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( " Pos. [ch] : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( k2p->E2p( MCACh, cc.s2rxLimit( m.x() ) ) ) );
  LINE++;

  // カーソル位置の MCA 値
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( " Val. : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( MCA[ curp ] ) );
  LINE++;

  // ROI タイトル
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "ROI " ) );
  LINE++;

  // ROI のスタート位置 ( MCA pixel )
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( " Start [keV] : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( wrROIsx, 'g', 3 ) );
  LINE++;

  // ROI の終了位置 ( MCA pixel )
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( " End [keV] : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( wrROIex , 'g', 3 ) );
  LINE++;

  // ROI 内の積分値
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( " Count : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( sum ) );
  LINE++;

  // ROI 内の積分値を LiveTime で割った cps
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "   CPS : " ) );
  rec.setRect( dLM*5, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( sum / (( realTime == 0 )? 1 : realTime ), 'f', 2 ) );
  LINE++;

  // Real Time
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Real Time : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( realTime, 'f', 2 ) );
  LINE++;

  // Live Time
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Live Time : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( liveTime, 'f', 2 ) );
  LINE++;

  // デッドタイム
  double dt = ( realTime != 0 ) ? ( realTime - liveTime )/realTime * 100: 0;
  //　　 青   ->  緑   ->  黄   -> オレンジ -> 赤　　にするとしたら？
  //    0 0 1 -> 0 1 0 -> 1 1 0 ->    ->      1 0 0
  //     0 %      10%      20%        ->       100%
  int r, g, b;
  r = g = b = 0;
  if ( dt < 10 ) { r = 0; g = (int)( dt / 10 * 255 ); b = 255 - g; };
  if (( 10 <= dt )&&( dt < 20 )) { r = (int)( ( dt - 10 ) / 10 * 255 ); g = 255; b = 0; };
  if ( dt >= 20 ) { r = 255; g = 255 - (int)( ( dt - 20 ) / 80 * 255 ); b = 0; };
  if ( r < 0 ) r = 0;
  if ( g < 0 ) g = 0;
  if ( b < 0 ) b = 0;
  QColor DTC = QColor( r, g, b );
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Dead Time : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  p->setPen( DTC );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( dt, 'f', 2 ) );
  LINE++;
}

void MCAView::mouseMoveEvent( QMouseEvent *e )
{
  m.Moved( e );
  nearf = false;
  if ( !m.inPress() ) {
    int dsx = fabs( e->x() - m.sx() );
    int dex = fabs( e->x() - m.ex() );

    if ( ( dsx < NEAR ) || ( dex < NEAR ) ) {
      if ( dsx < dex ) {
	nearX = m.sx();
      } else {
	nearX = m.ex();
      }
      nearf = true;
    }
  }
  update();
}

void MCAView::mousePressEvent( QMouseEvent *e )
{
  if ( e->button() == Qt::LeftButton ) {   // 左ボタンで ROI 設定モード
    mMode = M_ROI;
    int dsx = fabs( e->x() - m.sx() );
    int dex = fabs( e->x() - m.ex() );
    int setX;
    if ( ( dsx < NEAR ) || ( dex < NEAR ) ) {   // 近くに既設の ROI の端点があるか
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
    update();
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
  }
}

void MCAView::mouseReleaseEvent( QMouseEvent *e )
{
  if ( mMode == M_ROI ) {
    mMode = M_NO;
    m.Released( e );
    rROIex = cc.s2rx( m.ex() );
    update();
  }
}

void MCAView::mouseDoubleClickEvent( QMouseEvent * ) {}

void MCAView::setROI( int s, int e )   // MCA pixel
{
  rROIsx = k2p->p2E( MCACh, s );
  rROIex = k2p->p2E( MCACh, e );
}

void MCAView::wheelEvent( QWheelEvent *e )
{
  double step = ( e->delta() / 8. ) / 15.;     // deg := e->delta / 8.
  double rx = cc.s2rx( e->x() );
  double drx = MaxE - MinE;
  if ( step > 0 ) {
    drx *= 0.9;
  } else {
    drx /= 0.9;
  }
  MinE = rx - ( e->x() - cc.Sminx() ) / ( cc.Smaxx() - cc.Sminx() ) * drx;
  MaxE = MinE + drx;

  if ( MinE < 0 ) MinE = 0;
  if ( MaxE > 20 ) MaxE = 20;

  update();
}
