
#include "ChCoord.h"

#include <math.h>

ChCoord::ChCoord( void )
{
  minx = maxx = miny = maxy = 0;
  wminx = wmaxx = wminy = wmaxy = 0.0;
}

void ChCoord::SetView( int x1, int y1, int x2, int y2 )
{
  minx = x1;
  maxx = x2;
  miny = y1;
  maxy = y2;
}

void ChCoord::SetWindow( double x1, double y1, double x2, double y2 )
{
  wminx = x1;
  wmaxx = x2;
  wminy = y1;
  wmaxy = y2;
}

void ChCoord::SetWindowX( double x1, double x2 )
{
  wminx = x1;
  wmaxx = x2;
}

void ChCoord::SetWindowY( double y1, double y2 )
{
  wminy = y1;
  wmaxy = y2;
}

int ChCoord::w2rx( double x )
{
  return ( maxx - minx ) / ( wmaxx - wminx ) * ( x - wminx ) + minx; 
}

int ChCoord::w2ry( double y )
{
  return maxy - ( maxy - miny ) / ( wmaxy - wminy ) * ( y - wminy );
}

int ChCoord::w2rdx( double x )
{
  return ( maxx - minx ) / ( wmaxx - wminx ) * x;
}

int ChCoord::w2rdy( double y )
{
  return ( maxy - miny ) / ( wmaxy - wminy ) * y;
}

double ChCoord::r2wx( int x )
{
  return ( wmaxx - wminx ) / ( maxx - minx ) * ( x - minx ) + wminx;
}

double ChCoord::r2wy( int y )
{
  return ( wmaxy - wminy ) / ( maxy - miny ) * ( maxy - y ) + wminy;
}

double ChCoord::r2wdx( int x )
{
  return ( wmaxx - wminx ) / ( maxx - minx ) * x;
}

double ChCoord::r2wdy( int y )
{
  return ( wmaxy - wminy ) / ( maxy - miny ) * y;
}


void ChCoord::getSEDy( double *sy, double *ey, double *dy )
{
    if ( wminy == wmaxy ) {
      *sy = 0;
      *ey = 1;
      *dy = 1;
    } else if ( wminy > wmaxy ) {
      *sy = wmaxy;
      *ey = wminy;
      *dy = ( wminy - wmaxy ) / 5;
    } else {
      // calcScale( 5, wminy, wmaxy, &sy, &dy )
      *sy = wminy;
      *ey = wmaxy;
      *dy = ( wmaxy - wminy ) / 5.;
    }
}

void ChCoord::calcScale( double div, double min, double max, double *s, double *d )
{
  if ( max < min ) {
    double tmp = min;
    min = max;
    max = min;
  }
  if ( max == min ) {
    *s = min;
    *d = 1;
    return;
  }

  double d0 = ( max - min ) / div;
  double Ld0 = 0;

  while ( d0 >= 10 ) {
    Ld0++;
    d0 /= 10.;
  }
  while( d0 < 1 ) {
    Ld0--;
    d0 *= 10;
  }
  if ( d0 <= 2.5 )
    d0 = 1.0;
  else if ( d0 <= 7.5 )
    d0 = 5.0;
  else 
    d0 = 10.0;
  d0 *= pow( 10, Ld0 );

  *s = (int)(min / d0 + 1) * d0;
  *d = d0;
}

/**********************************************************************/

void ChCoord::DrawText( QPainter *p, 
			QRectF rec, QFont font, int flags, QString msg, DRAWTXTF f )
{
  double xr, yr;
  QRectF brec;
  double fSize = font.pointSizeF();

  brec = p->boundingRect( rec, flags, msg );
  xr = brec.width() / (int)rec.width();
  yr = brec.height() / (int)rec.height();
  font.setPointSize( (int)( fSize / ( ( xr > yr ) ? xr : yr ) + 1 ) );
  p->setFont( font );
  p->drawText( rec, flags, msg );
  font.setPointSizeF( fSize );
  p->setFont( font );
}
