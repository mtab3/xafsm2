
#include <QtGui>

#include "XafsM.h"
#include "MCAView.h"

// 横軸には 3つの単位がある。
// MCA pixel, eV (実Energy), 描画 pixel
// MCAView 内の横軸は基本的に keV
// MCA pixel -> keV : MCA pixel * a = keV
// keV -> MCA pixel : keV / a = MCA pixel
// 描画 pixel との換算は一度 keV を通って cc.r2sx, cc.s2rx,...

MCAView::MCAView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  MCA = NULL;
  MCALen = 0;
  MCACh = -1;
  realTime = 0;
  liveTime = 0;
  AA = 0.01;
  BB = 0;
  MaxE = 20.;

  valid = false;
  dispLog = false;
  fdbase = new FluoDBase;

  Black = QColor( 0, 0, 0 );
  White = QColor( 255, 255, 255 );

  MCursorC = QColor( 255, 0, 0 );
  ROIRangeC = QColor( 170, 210, 0 );
  ExROIRangeC = QColor( 0, 100, 230 );
  ROIEdgeC = QColor( 255, 200, 0 );   // ROI の端点表示
  GridC = QColor( 100, 100, 190 );   // グリッドの色

  rROIsx = 0;
  rROIex = 20;
}

MCAView::~MCAView( void )
{
  if ( MCA != NULL )
    delete MCA;
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
  double max0 = 0;    // y 軸方向の表示スケール決定のため最大値を探す
  for ( int i = 0; i < MCALen; i++ ) {
    if ( MCA[i] > max0 ) {
      max0 = MCA[i];
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
  cc.SetRealCoord( 0, 0, MaxE, max );          // 今や横軸は MCA pixel ではなく、
                                               // エネルギ-[keV]
  // 正しく調整されていると MCP pixel = エネルギー[eV]/10 になっているはず。

  double wrROIsx = rROIsx;  // wrROI.. working-real-ROI.., rROI.. real-ROI..
  double wrROIex;
  if ( m.inPress() ) {      // マウスボタンを押してる時は、「終点」はマウスの現在位置
    wrROIex = cc.s2rx( m.x() );
  } else {
    wrROIex = rROIex;       // そうでなければ最後にボタンを離した位置
  }
  double tmp;
  if ( wrROIsx > wrROIex ) { // 起点、終点の大小関係が逆になってたら入れ替えておく
    tmp = wrROIsx;
    wrROIsx = wrROIex;
    wrROIex = tmp;
  }
  if ( m.inPress() ) {
    emit newROI( (int)( ( wrROIsx - BB ) / AA ), (int)( ( wrROIex - BB ) / AA ) );
  }

  int sum = 0;
  for ( int i = 0; i < MCALen; i++ ) {       // ROI の範囲の積算と MCA スペクトルの描画
    double E = i * AA + BB;                  // 画面 pixel から MCA pixel への換算
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
  int curp = ( cc.s2rxLimit( m.x() ) - BB ) / AA;  // マウスカーソル位置の MCA 値 pixel
  if ( curp >= MCALen ) curp = MCALen - 1;
  emit CurrentValues( MCA[ curp ], sum );

  if ( nearf ) {              // マウスカーソルが ROI の両端に近いと認識しているときは
                              // 近いと思っている方の橋を強調? 表示
    p->setPen( ROIEdgeC );
    p->drawLine( nearX, TM, nearX, TM+VW );
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

  // カーソル位置( 実エネルギー換算 )
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Cur. [keV] : " ) );
  rec.setRect( dLM*5, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( cc.s2rxLimit( m.x() ) ) );
  LINE++;

  // カーソル位置( MCA pixel )
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Cur. [ch] : " ) );
  rec.setRect( dLM*5, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( (int)(( cc.s2rxLimit( m.x() ) - BB ) / AA ) ) );
  LINE++;

  // カーソル位置の MCA 値
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Val. Cur : " ) );
  rec.setRect( dLM*5, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( MCA[ curp ] ) );
  LINE++;

  // ROI のスタート位置 ( MCA pixel )
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "ROI s [keV] : " ) );
  rec.setRect( dLM*5, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( wrROIsx ) );
  LINE++;

  // ROI の終了位置 ( MCA pixel )
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "ROI e [keV] : " ) );
  rec.setRect( dLM*5, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( wrROIex ) );
  LINE++;

  // ROI 内の積分値
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "ROI value : " ) );
  rec.setRect( dLM*5, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( sum ) );
  LINE++;

  // ROI 内の積分値を LiveTime で割った cps
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "CPS in ROI : " ) );
  rec.setRect( dLM*5, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( sum / (( realTime == 0 )? 1 : realTime )) );
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

#define NEAR ( 10 )

void MCAView::mouseMoveEvent( QMouseEvent *e )
{
  m.Moved( e );
  nearf = false;
  if ( !m.inPress() ) {
    int dsx = abs( e->x() - m.sx() );
    int dex = abs( e->x() - m.ex() );

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
  int dsx = abs( e->x() - m.sx() );
  int dex = abs( e->x() - m.ex() );
  int setX;
  if ( ( dsx < NEAR ) || ( dex < NEAR ) ) {
    if ( dsx < dex ) {
      setX = m.ex();
    } else {
      setX = m.sx();
    }
  } else {
    setX = e->x();
  }
  m.Pressed( e );
  m.setSx( setX );
  rROIsx = cc.s2rx( m.sx() );
  update();
}

void MCAView::mouseReleaseEvent( QMouseEvent *e )
{
  m.Released( e );
  rROIex = cc.s2rx( m.ex() );
  update();
}

void MCAView::mouseDoubleClickEvent( QMouseEvent * ) {}

void MCAView::setROI( int s, int e )   // MCA pixel
{
  rROIsx = (double)s * AA + BB;
  rROIex = (double)e * AA + BB;
}
