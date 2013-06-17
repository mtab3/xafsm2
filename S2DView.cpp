
#include <QtGui>

#include "S2DView.h"

S2DView::S2DView( QWidget *p ) : QFrame( p )
{
  setupUi( this );

  data = NULL;
  rType = AS_SCREEN;
  setRange( -10., 10., -10., 10., 2., 2. );
  cc.SetRealCoord( minx, maxx, miny, maxy );

  AutoScale = true;
  for ( int i = 0; i < 256 * 4; i++ )
    cbar[ i ] = QColor( 0, 0, 0 );
  for ( int r = 0; r < 256; r++ )
    cbar[ r ] = QColor( r, 0, 0 );
  for ( int g = 0; g < 256; g++ )
    cbar[ 256 + g ] = QColor( 255, g, 0 );
  for ( int b = 0; b < 256; b++ )
    cbar[ 512 + b ] = QColor( 255, 255, b );
  cmax = 767;
  cmin = 0;
}

int S2DView::cNum( double v )
{
  int cnum;
#if 0
  int vvmin, vvmax;

  if ( vmin < vmax ) {
    vvmin = vmin;
    vvmax = vmax;
  } else {
    vvmin = vmax;
    vvmax = vmin;
  }
#endif
  //  cnum = (int)((double)( v - vvmin )/( vvmax - vvmin )*( cmax - cmin ) + cmin );
  cnum = (int)((double)( v - minz )/( maxz - minz )*( cmax - cmin ) + cmin );

  if ( cnum < cmin ) cnum = cmin;
  if ( cnum > cmax ) cnum = cmax;

  return cnum;
}

void S2DView::setRange( double Sx, double Sy, double Dx, double Dy, int ix, int iy )
{
  double ex, ey;
  sx = Sx;
  sy = Sy;
  dx = Dx;
  dy = Dy;

  ex = sx + dx * ix;
  ey = sy + dy * iy;
  if ( sx > ex ) { maxx = sx; minx = ex; } else { maxx = ex; minx = sx; }
  if ( sy > ey ) { maxy = sy; miny = ey; } else { maxy = ey; miny = sy; }
  cc.SetRealCoord( minx, maxx, miny, maxy );

  if ( data != NULL ) {
    for ( int i = 0; i < maxix; i++ ) {
      delete [] data[i];
    }
    delete [] data;
  }
  maxix = ix;
  maxiy = iy;
  data = new double * [ maxix + 1 ];        // x, y 両方向に 1ずつ大きく取る
  for ( int i = 0; i < maxix + 1; i++ ) {
    data[i] = new double [ maxiy + 1 ];
    for ( int j = 0; j < maxiy + 1; j++ ) {
      data[i][j] = 0;
    }
  }
}

void S2DView::setData( int ix, int iy, double v )
{
  if (( ix >= 0 )&&( iy >= 0 )&&( ix < maxix )&&( iy < maxiy )) {
    data[ix][iy] = v;
    update();
  }
}

void S2DView::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}
 
void S2DView::Draw( QPainter *p )
{
  QColor White( 255, 255, 255 );
  QColor GridC( 128, 128, 128 );

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

  double screen_w = HW;
  double screen_h = VW;
  double real_w = maxx - minx;
  double real_h = maxy - miny;
  switch ( rType ) {
  case REAL_RATIO:
    if ( ( screen_w / real_w * real_h ) > screen_h ) {
      // (HW x VW) のスクリーンに横を合わせると縦がはみ出るなら
      // 縦を元々の VW のままにして、横(HW)を定義し直す
      HW = screen_h / real_h * real_w;
      // LM = ( w - HW ) / 2;
    } else {
      // (HW x VW) のスクリーンに横を合わせると縦がはみ出ないなら
      // 横を元々の HW のままにして、縦(VM)を定義し直す
      VW = screen_w / real_w * real_h;
      // TM = ( h - VW ) / 2;
    }
    break;
  case AS_SCREEN:
  default:
    break;
  }
  cc.SetScreenCoord( LM, h-BM-VW, LM+HW, h-BM );
  cc.SetRealCoord( minx, miny, maxx, maxy );

  p->fillRect( 0, 0, w, h, White );
  p->setPen( GridC );
  p->drawRect( cc.r2sx( minx ), cc.r2sy( maxy ),
	       cc.r2sdx( maxx - minx ), cc.r2sdy( maxy - miny ) );
  
  if ( AutoScale ) {
    minz = 1e300;
    maxz = -1e300;
    for ( int ix = 0; ix < maxix; ix++ ) {
      for ( int iy = 0; iy < maxiy; iy++ ) {
	if ( data[ix][iy] < minz ) minz = data[ix][iy];
	if ( data[ix][iy] > maxz ) maxz = data[ix][iy];
      }
    }
  }

  int x0, y0, xd, yd;
  int x1, x2, y1, y2;
  for ( int ix = 0; ix < maxix; ix++ ) {
    for ( int iy = 0; iy < maxiy; iy++ ) {
      x1 = cc.r2sx( sx + dx * ix );
      x2 = cc.r2sx( sx + dx * ix + dx );
      y1 = cc.r2sy( sy + dy * iy );
      y2 = cc.r2sy( sy + dy * iy + dy );
      x0 = ( x1 < x2 ) ? x1 : x2;
      y0 = ( y1 < y2 ) ? y1 : y2;
      xd = abs( x1 - x2 );
      yd = abs( y1 - y2 );
      p->fillRect( x0, y0, xd, yd, cbar[ cNum( data[ix][iy] ) ] );
    }
  }
	
  for ( int ix = 0; ix < maxix; ix++ ) {
    p->drawLine( cc.r2sx( sx + dx * ix ), cc.r2sy( maxy ),
		 cc.r2sx( sx + dx * ix ), cc.r2sy( miny ) );
  }
  for ( int iy = 0; iy < maxiy; iy++ ) {
    p->drawLine( cc.r2sx( minx ), cc.r2sy( sy + dy * iy ),
		 cc.r2sx( maxx ), cc.r2sy( sy + dy * iy ) );
  }

}
