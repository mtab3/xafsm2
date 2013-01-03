
#include "ChCoord.h"

#include <math.h>

ChCoord::ChCoord( void )
{
  sminx = smaxx = sminy = smaxy = 0;
  rminx = rmaxx = rminy = rmaxy = 0.0;
}

void ChCoord::SetScreenCoord( double x1, double y1, double x2, double y2 )
{
  sminx = x1;
  smaxx = x2;
  sminy = y1;
  smaxy = y2;
}

void ChCoord::SetRealCoord( double x1, double y1, double x2, double y2 )
{
  rminx = x1;
  rmaxx = x2;
  rminy = y1;
  rmaxy = y2;
}

void ChCoord::SetRealX( double x1, double x2 )
{
  rminx = x1;
  rmaxx = x2;
}

void ChCoord::SetRealY( double y1, double y2 )
{
  rminy = y1;
  rmaxy = y2;
}

double ChCoord::r2sx( double rx )
{
  return ( smaxx - sminx ) / ( rmaxx - rminx ) * ( rx - rminx ) + sminx; 
}

double ChCoord::r2sy( double ry )
{
  return smaxy - ( smaxy - sminy ) / ( rmaxy - rminy ) * ( ry - rminy );
}

double ChCoord::r2sdx( double rx )
{
  return ( smaxx - sminx ) / ( rmaxx - rminx ) * rx;
}

double ChCoord::r2sdy( double ry )
{
  return ( smaxy - sminy ) / ( rmaxy - rminy ) * ry;
}

double ChCoord::s2rx( double sx )
{
  return ( rmaxx - rminx ) / ( smaxx - sminx ) * ( sx - sminx ) + rminx;
}

double ChCoord::s2ry( double sy )
{
  return ( rmaxy - rminy ) / ( smaxy - sminy ) * ( smaxy - sy ) + rminy;
}

double ChCoord::s2rxLimit( double sx )
{
  double rv = ( rmaxx - rminx ) / ( smaxx - sminx ) * ( sx - sminx ) + rminx;
  if ( rmaxx > rminx ) {
    if ( rv > rmaxx ) rv = rmaxx;
    if ( rv < rminx ) rv = rminx;
  } else {
    if ( rv > rminx ) rv = rminx;
    if ( rv < rmaxx ) rv = rmaxx;
  }
  return rv;
}

double ChCoord::s2ryLimit( double sy )
{
  double rv = ( rmaxy - rminy ) / ( smaxy - sminy ) * ( smaxy - sy ) + rminy;
  if ( rmaxy > rminy ) {
    if ( rv > rmaxy ) rv = rmaxy;
    if ( rv < rminy ) rv = rminy;
  } else {
    if ( rv > rminy ) rv = rminy;
    if ( rv < rmaxy ) rv = rmaxy;
  }
  return rv;
}

double ChCoord::s2rdx( double sx )
{
  return ( rmaxx - rminx ) / ( smaxx - sminx ) * sx;
}

double ChCoord::s2rdy( double sy )
{
  return ( rmaxy - rminy ) / ( smaxy - sminy ) * sy;
}


void ChCoord::getSEDy0( double *sy, double *ey, double *dy, double div )
{
  if ( rminy == rmaxy ) {
    *sy = 0;
    *ey = 1;
    *dy = 1;
  } else if ( rminy > rmaxy ) {
    *sy = rmaxy;
    *ey = rminy;
    *dy = ( rminy - rmaxy ) / div;
  } else {
    *sy = rminy;
    *ey = rmaxy;
    *dy = ( rmaxy - rminy ) / div;
  }
}

void ChCoord::getSEDy( double *sy, double *ey, double *dy, double div )
{
  if ( rminy == rmaxy ) {
    *sy = 0;
    *ey = 1;
    *dy = 1;
  } else if ( rminy > rmaxy ) {
    // calcScale( div, rmaxy, rminy, &sy, &dy )
    *sy = rmaxy;
    *ey = rminy;
    *dy = ( rminy - rmaxy ) / div;
  } else {
    // calcScale( div, rminy, rmaxy, &sy, &dy )
    *sy = rminy;
    *ey = rmaxy;
    *dy = ( rmaxy - rminy ) / div;
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

/*************************************************************************************/
/*   The following functions have almost no relation with the coordination change.   */
/*   But useful as common function for drawing applications.                         */
/*************************************************************************************/

void ChCoord::DrawText( QPainter *p, 
			QRectF rec, QFont font, int flags, DRAWTXTF f, QString msg )
/* Draw Text within a given rectangle, 'QRectF rec'. 
 *   When the 'DRAWTEXTF (draw text flag) f' is 
 *   FIXSIZE   : the text will draw with preparantially set font size via 'QFont font'
 *   SCALESIZE : the text will be automatically scaled to match within the rectange. 
 */
{
  double xr, yr;
  QRectF brec, dummyrec = QRectF( 0, 0, 1000, 1000 );
  double fSize = font.pointSizeF();

  if ( f == SCALESIZE ) {
    brec = p->boundingRect( dummyrec, flags, msg );
    // There was a bug, that call the boundingRect with given 'rec'
    // Then it returns limitted by the size of rec when the 
    //   drawn text size should bigger than the rectange,
    //   and the results of the resize were wrong. 
    xr = brec.width() / rec.width();
    yr = brec.height() / rec.height();
    font.setPointSize( fSize / ( ( xr > yr ) ? xr : yr ) );
  }
  p->setFont( font );
  p->drawText( rec, flags, msg );
  font.setPointSizeF( fSize );
  p->setFont( font );
}
