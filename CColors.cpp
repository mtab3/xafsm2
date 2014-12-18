
#include "CColors.h"

CColors::CColors( void )
{
  QVector<QColor*> cs;

  cs << new QColor( 0, 0, 0 )
     << QColor( 0, 0, 255 ) << QColor( 255, 0, 0 ) << QColor( 0, 255, 0 );
  generateColors( cs, 40 );
}

CColors::CColors( QVector<QColor> cs, int colors )
{
  generateColors( cs, colors );
}

void CColors::generateColors( QVector<QColor*> cs, int colors )
{
  for ( int i = 0; i < cs.count(); i++ ) {
    ccolors << cs[i];
  }
  while ( ccolors.count() < colors ) {
    int now
    for ( int i = 0; ( i < ccolors.count() ) && ( ccolors.count() < colors ); i++ ) {
    }
  }
}
