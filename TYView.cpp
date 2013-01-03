#include <QtGui>

#include "XafsM.h"
#include "TYView.h"

TYView::TYView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  valid = false;
  cc.SetRealCoord( 0, 0, 1, 1 );
  bgColor = QColor( 255, 255, 255 );
  BLACK = QColor( 0, 0, 0 );
  MCLineC = QColor( 210, 180, 0 );     // mouse cursor line color

  LC << QColor(   0,   0,   0 )
     << QColor( 255,   0,   0 ) << QColor(   0,   0, 255 ) << QColor(   0, 255,   0 )
     << QColor( 255, 255,   0 ) << QColor( 255,   0, 255 ) << QColor(   0, 255, 255 )
     << QColor( 127,   0,   0 ) << QColor(   0, 127,   0 ) << QColor(   0,   0, 127 )
     << QColor(   0,   0,   0 )
     << QColor( 255,   0,   0 ) << QColor(   0, 255,   0 ) << QColor(   0,   0, 255 )
     << QColor( 255, 255,   0 ) << QColor( 255,   0, 255 ) << QColor(   0, 255, 255 )
     << QColor( 127,   0,   0 ) << QColor(   0, 127,   0 ) << QColor(   0,   0, 127 )
     << QColor(   0,   0,   0 )
     << QColor( 255,   0,   0 ) << QColor(   0, 255,   0 ) << QColor(   0,   0, 255 )
     << QColor( 255, 255,   0 ) << QColor( 255,   0, 255 ) << QColor(   0, 255, 255 )
     << QColor( 127,   0,   0 ) << QColor(   0, 127,   0 ) << QColor(   0,   0, 127 );
  MonScale = 0;
  nx = ny = 0;
  ep = 0;
  datas = 0;
}

void TYView::ClearDataR( void )
{
  for ( int j = 0; j < RingMax; j++ ) {
    for ( int i = 0; i < MaxMon; i++ ) {
      mony[ i ][ j ] = 0;
    }
    mont[ j ] = 0;
  }
  ep = 0;
  datas = 0;
}

void TYView::SetLines( int Lines )
{
  lines = Lines;
}

void TYView::SetMonScale( int ms )
{
  MonScale = ms;
}

// リングバッファへのデータ追加
void TYView::NewPointR( int tt, double yy0, double yy1, double yy2 )
{
  mont[ ep ] = tt;
  mony[0][ ep ] = yy0;
  mony[1][ ep ] = yy1;
  mony[2][ ep ] = yy2;
  ep++;
  if ( ep >= RingMax )
    ep = 0;
  if ( datas < RingMax )
    datas++;
}

void TYView::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}

void TYView::Draw( QPainter *p )
{
  if ( valid != true ) return;

  QString buf, buf2;
  double RM, LM, TM, BM, HDiv, VDiv;
  QPen pen0, pen1;
  QFont F1;
  QRectF rec;
  int ms = MScales[ MonScale ].div * 1000;   // １グリッドの時間 秒 x 1000 = ミリ秒

  double HDNum = 6;    // 水平方向のグリッド数
  double VDNum = 10;   // 垂直方向のグリッド数

  cc.SetRealCoord( -ms * HDNum, 0, 0, 1 );   // 実座標の指定(とりあえず垂直方向は 1)

  p->fillRect( 0, 0, width(), height(), bgColor ); // 背景全体の塗りつぶし
  pen0.setWidth( 1 );
  pen0.setColor( QColor( 0, 0, 0 ) );
  p->setPen( pen0 );
  p->setFont( F1 );

  RM = width() * 0.03;    // 描画領域の中でのグラフの右マージン
  LM = width() * 0.15;    // 描画領域の中でのグラフの左マージン
  TM = height() * 0.05;   // 描画領域の中でのグラフの上(top)マージン
  BM = height() * 0.10;   // 描画領域の中でのグラフの下(bottom)マージン
  HDiv = ( width() - RM - LM ) / HDNum;     // グリッド幅
  VDiv = ( height() - TM - BM ) / VDNum;    // グリッド高さ

  cc.SetScreenCoord( LM, TM, width()-RM, height()-BM );
  // screen 座標の指定。左上、右下の座標指定
  // (ここで上下裏がえった座標を指定することで real 座標で普通に左下が 0,0 の座標になる)

  p->drawRect( LM, TM, width()-RM-LM, height()-BM-TM );  // グラフの枠線

  for ( double xx = cc.Rminx(); xx <= cc.Rmaxx(); xx += ms ) {   // 横方向方向
    p->drawLine( cc.r2sx( xx ), TM, cc.r2sx( xx ), height()-BM );  // 縦罫線
                                                                   // 横軸のメモリ数字
    rec = QRect( cc.r2sx( xx )-HDiv/2, height()-BM*0.95, HDiv, BM*0.4 );
    cc.DrawText( p, rec, F1, Qt::AlignHCenter | Qt::AlignVCenter, SCALESIZE, 
		 QString( tr( "%1" ) )
		 .arg( (int)((cc.Rmaxx()-xx)/ms) * MScales[ MonScale ].dispDiv ) );
  }
  rec = QRect( width()-VDiv, height()-BM*0.5, RM*0.9, BM*0.4 );  // 横軸のラベル
  cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE,
	       MScales[ MonScale ].unit );

  pen1.setWidth( 1 );
  pen1.setColor( BLACK );
  p->setPen( pen1 );
  for ( double yy = 0; yy < 1; yy += 0.1 ) {
    p->drawLine( LM, cc.r2sy( yy ), width()-RM, cc.r2sy( yy ) );   // 横の罫線
  }

  double sy, ey, dy;
  UpDateYWindowRing();   // 最大値最小値(5%マージン)を探す (Rminy, Rmaxy に返す)

  for ( int j = 0; j < lines; j++ ) {
    //      sy = Rwminy[j];
    //      dy = ( Rwmaxy[j] - Rwminy[j] ) / 10.;
    cc.SetRealY( Rwminy[j], Rwmaxy[j] );
    cc.getSEDy( &sy, &ey, &dy, 5 );
    pen1.setColor( LC[ j ] );
    p->setPen( pen1 );

    /*  ここまで確認した  */

    for ( double yy = sy; yy <= cc.Rmaxy(); yy += dy ) {
      rec = QRectF( LM - LM * 0.32 * ( j + 1 ), cc.r2sy( yy )-VDiv*0.45,
		    LM * 0.3, VDiv * 0.9 ); // メモリ数字
      buf.sprintf( "%6.4g", yy );
      cc.DrawText( p, rec, F1, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, buf );
    }
    rec = QRectF( LM + HDiv * 0.1 + HDiv * 2 * j, TM * 0.05, 
		  HDiv * 2, TM * 0.9 );  // 軸のラベル
    
    int pp1, pp2;
    int t0 = mont[ ( ep == 0 ) ? RingMax - 1 : ep - 1 ];
    pen1.setWidth( 2 );
    pen1.setColor( LC[ j ] );
    p->setPen( pen1 );
    int nowt = cc.s2rx( nx ) + t0;  // マウスポインタの位置に相当する時刻
    int nowtp = 0;
    
    for ( int i = 0; i < datas - 1; i++ ) { // データプロット
      pp1 = ep - 1 - i;
      pp2 = ep - 1 - ( i + 1 );
      if ( pp1 < 0 ) pp1 += RingMax;
      if ( pp2 < 0 ) pp2 += RingMax;
      if ( ( mont[ pp2 ] - t0 ) < ( - ms * 6 ) ) {
	break;
      }
      p->drawLine( cc.r2sx( mont[pp1] - t0 ), cc.r2sy( mony[j][pp1] ),
		   cc.r2sx( mont[pp2] - t0 ), cc.r2sy( mony[j][pp2] ) );

      // マウスポインタの指す時刻に出来るだけ近い実測点
      if (( mont[pp1] >= nowt )&&( mont[pp2] < nowt ))
	nowtp = pp1;

    }
    cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
		 LNames[j] + " : " + QString::number(mony[j][nowtp]) );
  }

  // マウスポインタの位置に縦線を引く
  if ( ( nx > LM ) && ( nx < width()-RM ) ) {
    p->setPen( MCLineC );
    p->drawLine( nx, TM, nx, height()-BM );
  }
}

void TYView::UpDateYWindowRing( void )
{
  int dx = MScales[ MonScale ].div;
  int p, t0;

  t0 = mont[ ( ep == 0 ) ? RingMax -1 : ep - 1 ];

  for ( int j = 0; j < lines; j++ ) {
    double nmaxy = -1e300;
    double nminy = 1e300;
    for ( int i = 0; i < datas; i++ ) {
      p = ep - 1 - i;
      if ( p < 0 ) p += RingMax;
      if ( (  mont[ p ] - t0 ) > ( - dx * 6 * 1000 ) ) {
	if ( mony[j][ p ] < nminy )
	  nminy = mony[j][ p ];
	if ( mony[j][ p ] > nmaxy ) 
	  nmaxy = mony[j][ p ];
      }
    }
    double dy = nmaxy - nminy;
    if ( dy == 0 )
      dy = 1.;
    Rwminy[j] = nminy - dy * 0.05;
    Rwmaxy[j] = nmaxy + dy * 0.05;
  }
}

/*******************************************************************/

void TYView::mouseMoveEvent( QMouseEvent *e )
{
  nx = e->x();
  ny = e->y();

  update();
}

void TYView::mousePressEvent( QMouseEvent * )
{
}

void TYView::mouseReleaseEvent( QMouseEvent * )
{
}

void TYView::mouseDoubleClickEvent( QMouseEvent * )
{
}
