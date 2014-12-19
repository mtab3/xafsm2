
#include <QDebug>

#include "CColors.h"

CColors::CColors( void )
{
  QVector<QColor> cs;

  cs << QColor( 0, 0, 0 )
     << QColor( 0, 0, 255 )
     << QColor( 255, 0, 0 )
     << QColor( 0, 255, 0 );

  generateColors( cs, 40 );
}

CColors::CColors( int colors )
{
  QVector<QColor> cs;

  cs << QColor( 0, 0, 0 )
     << QColor( 0, 0, 255 )
     << QColor( 255, 0, 0 )
     << QColor( 0, 255, 0 );

  generateColors( cs, colors );
}

CColors::CColors( QVector<QColor> cs, int colors )
{
  generateColors( cs, colors );
}

void CColors::generateColors( QVector<QColor> cs, int colors )
{
  for ( int i = 0; i < cs.count(); i++ ) {
    ccolors << cs[i];
  }
  int now;
  while ( ( now = ccolors.count() ) < colors ) {
    for ( int i = 0; ( i < now ) && ( ccolors.count() < colors ); i++ ) {
      int c1 = i;
      int c2 = i + 1;
      if ( c2 >= now ) c2 -= now;
      int r = mix( ccolors[c1].red(), ccolors[c2].red() );
      int g = mix( ccolors[c1].green(), ccolors[c2].green() );
      int b = mix( ccolors[c1].blue(), ccolors[c2].blue() );
      if ( ( r + g + b ) > 500 ) {
	r *= 0.8;
	g *= 0.6;
	b *= 0.8;
      }
      QColor newC = QColor( r, g, b );
      ccolors << newC;
    }
  }
}

int CColors::mix( int c1, int c2 )
{
  if ( c1 > c2 )
    return ( c1 * 2 + c2 ) / 3;

  return ( c2 * 2 + c2 ) / 3;
}
