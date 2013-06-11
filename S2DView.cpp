
#include <QtGui>

#include "S2DView.h"

S2DView::S2DView( QWidget *p ) : QFrame( p )
{
  setupUi( this );

  data = NULL;
  rType = AS_SCREEN;
  setRange( -10., 10., -10., 10., 2., 2. );
  cc.SetRealCoord( minx, maxx, miny, maxy );
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
  data = new double * [ maxix ];
  for ( int i = 0; i < maxix; i++ ) {
    data[i] = new double [ maxiy ];
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
  
}
