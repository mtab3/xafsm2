
#include "ChCoord.h"

#include <math.h>

ChCoord::ChCoord( void )
{
  sminx = smaxx = sminy = smaxy = 0;
  rminx = rmaxx = rminy = rmaxy = 0.0;

  ASBBorderC = QColor( 0, 220, 220 );  // auto scale button border color
  ASBOnC = QColor( 170, 255, 170 );    // auto scale button on color
  ASBOffC = QColor( 140, 180, 140 );   // auto scale button off color
  BLACK = QColor( 0, 0, 0 );
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

void ChCoord::SetRealCoord0( double x1, double y1, double x2, double y2 )
{
  rminx0 = x1;
  rmaxx0 = x2;
  rminy0 = y1;
  rmaxy0 = y2;
}

void ChCoord::SetRealX0( double x1, double x2 )
{
  rminx0 = x1;
  rmaxx0 = x2;
}

void ChCoord::SetRealY0( double y1, double y2 )
{
  rminy0 = y1;
  rmaxy0 = y2;
}

void ChCoord::RecallRealCoord( void )
{
  rminx = rminx0;
  rmaxx = rmaxx0;
  rminy = rminy0;
  rmaxy = rmaxy0;
}

void ChCoord::RecallRealX( void )
{
  rminx = rminx0;
  rmaxx = rmaxx0;
}

void ChCoord::RecallRealY( void )
{
  rminy = rminy0;
  rmaxy = rmaxy0;
}

double ChCoord::r2sx( double rx )
{
  return ( smaxx - sminx ) / ( rmaxx - rminx ) * ( rx - rminx ) + sminx; 
}

double ChCoord::r2sy( double ry )
{
  return smaxy - ( smaxy - sminy ) / ( rmaxy - rminy ) * ( ry - rminy );
}

// 最大最小が (min, max) の範囲にある関数 y を、現在の real window の範囲に合わせる
double ChCoord::fixy( double y, double min, double max )
{
  double ry = ( y - min ) / ( max - min ) * ( rmaxy - rminy ) + rminy;
  return r2sy( ry );
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

double ChCoord::s2rx0( double sx )
{
  return ( rmaxx - rminx ) / ( smaxx - sminx ) * ( sx - sminx );
}

double ChCoord::s2ry0( double sy )
{
  return ( rmaxy - rminy ) / ( smaxy - sminy ) * ( smaxy - sy );
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
    max = tmp;
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

void ChCoord::DrawTexts( QPainter *p, QRectF &rec, double dx, double dy,
			 QFont &font, int flags, DRAWTXTF f, QStringList texts )
// rec と font は内部で変更する(参照で変更を返す)
{
  if ( f == FIXSIZE ) {
    double minFs = 100000;    // 必要な最小フォントサイズを確認
    for ( int i = 0; i < texts.count(); i++ ) {
      double fs = getFontSize( p, rec, font, Qt::AlignLeft | Qt::AlignVCenter, texts[i] );
      if ( fs < minFs )
	minFs = fs;
    }
    font.setPointSize( minFs );
  }

  for ( int i = 0; i < texts.count(); i++ ) {
    DrawText( p, rec, font, flags, f, texts[i] );
    rec.translate( dx, dy );
  }
}

QRectF ChCoord::DrawText( QPainter *p, 
			QRectF rec, QFont font, int flags, DRAWTXTF f, QString msg )
/* Draw Text within a given rectangle, 'QRectF rec'. 
 *   When the 'DRAWTEXTF (draw text flag) f' is 
 *   FIXSIZE   : the text will draw with preparantially set font size via 'QFont font'
 *   SCALESIZE : the text will be automatically scaled to match within the rectange. 
 */
{
  double xr, yr;
  QRectF brec, dummyrec = QRectF( rec.left(), rec.bottom(), 1000, 1000 );
  double fSize = font.pointSizeF();

  brec = p->boundingRect( dummyrec, flags, msg );
  if ( f == SCALESIZE ) {
    // There 'was' a bug, that call the boundingRect with given 'rec'
    // Then it returns limitted by the size of rec when the 
    //   drawn text size should bigger than the rectange,
    //   and the results of the resize were wrong. 
    if ( rec.width() == 0 ) 
      xr = 1;
    else 
      xr = brec.width() / rec.width();
    if ( rec.height() == 0 )
      yr = 1;
    else 
      yr = brec.height() / rec.height();
    if (( xr == 0 )&&( yr == 0 ))
      xr = yr = 1;

    font.setPointSize( fSize / ( ( xr > yr ) ? xr : yr ) );
  }
  p->setFont( font );
  p->drawText( rec, flags, msg );
  font.setPointSizeF( fSize );
  p->setFont( font );

  return brec;
}

QRectF ChCoord::UnDrawText( QPainter *p, 
			QRectF rec, QFont font, int flags, DRAWTXTF f, QString msg )
/* Draw Text within a given rectangle, 'QRectF rec'. 
 *   When the 'DRAWTEXTF (draw text flag) f' is 
 *   FIXSIZE   : the text will draw with preparantially set font size via 'QFont font'
 *   SCALESIZE : the text will be automatically scaled to match within the rectange. 
 */
{
  double xr, yr;
  QRectF brec, dummyrec = QRectF( rec.left(), rec.bottom(), 1000, 1000 );
  double fSize = font.pointSizeF();

  brec = p->boundingRect( dummyrec, flags, msg );
  if ( f == SCALESIZE ) {
    // There 'was' a bug, that call the boundingRect with given 'rec'
    // Then it returns limitted by the size of rec when the 
    //   drawn text size should bigger than the rectange,
    //   and the results of the resize were wrong. 
    if ( rec.width() == 0 ) 
      xr = 1;
    else 
      xr = brec.width() / rec.width();
    if ( rec.height() == 0 )
      yr = 1;
    else 
      yr = brec.height() / rec.height();
    if (( xr == 0 )&&( yr == 0 ))
      xr = yr = 1;

    font.setPointSize( fSize / ( ( xr > yr ) ? xr : yr ) );
  }
  p->setFont( font );
  brec = p->boundingRect( rec, flags, msg );
  font.setPointSizeF( fSize );
  p->setFont( font );

  return brec;
}


double ChCoord::getFontSize( QPainter *p, QRectF rec, QFont font, int flags, QString msg )
{
  double xr, yr;
  QRectF brec, dummyrec = QRectF( rec.left(), rec.bottom(), 1000, 1000 );
  double fSize = font.pointSizeF();

  brec = p->boundingRect( dummyrec, flags, msg );
  if ( rec.width() == 0 ) 
    xr = 1;
  else 
    xr = brec.width() / rec.width();
  if ( rec.height() == 0 )
    yr = 1;
  else 
    yr = brec.height() / rec.height();
  if (( xr == 0 )&&( yr == 0 ))
    xr = yr = 1;

  return fSize / ( ( xr > yr ) ? xr : yr );
}

void ChCoord::ShowAButton( QPainter *p, bool BState, QString BName,
			   int x0, int w, int y0 )
{
  QFont F1;

  if ( BState )
    p->fillRect( x0 + 5, y0 - 19, 14, 14, ASBOnC );
  else 
    p->fillRect( x0 + 5, y0 - 19, 14, 14, ASBOffC );
  p->setPen( ASBBorderC );
  p->drawRect( x0 + 5, y0 - 19, 14, 14 );

  p->setPen( BLACK );
  QRectF rec = QRectF( x0 + 24, y0-17, w, 11 );
  DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, BName );
}

// 既存の QRectF の集合で出来た Polygon と与えられた rec の重なりを調べ、
// 上または右にどれだけずれると重なりを解消できる「可能性」があるかを返す
bool ChCoord::IntersectRecs( QRectF rec )
{
  for ( int i = 0; i < recs.count(); i++ ) {
    if ( recs[i].intersects( rec ) )
      return true;
  }
  return false;
}

void ChCoord::AddARec( QRectF rec )
{
  recs << rec;
}
