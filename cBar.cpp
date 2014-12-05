
#include <QStylePainter>

#include "cBar.h"

cBar::cBar( QWidget *p ) : QFrame( p )
{
  setupUi( this );
  initColor();
}

void cBar::initColor( void )
{
  White = QColor( 255, 255, 255 );
  Black = QColor( 0, 0, 0 );
  
  for ( int i = 0; i < 256; i++ )
    cbar << new QColor( i, 0, 0 );
  for ( int i = 0; i < 256; i++ )
    cbar << new QColor( 255, i, 0 );
  for ( int i = 0; i < 256; i++ )
    cbar << new QColor( 255, 255, i );

  cmin = 0;
  cmax = cbar.count();
}

void cBar::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}

void cBar::Draw( QPainter *p )
{
  int h = height();
  int w = width();
  
  cc.SetScreenCoord( 0, 0, w, h );
  cc.SetRealCoord( 0, cmin, 1, cmax );
  //  p->fillRect( 0, 0, w, h, QColor( 255, 255, 255 ) );

  int ph = cc.r2sdy( 1 )+1;
  if ( ph < 1 ) ph = 1;
  //  p->fillRect( 0, 0, w, cc.r2sy( 0 ), Black );
  for ( int i = 0; i < cbar.count(); i++ ) {
    p->fillRect( 0, cc.r2sy( i ), w, ph, *cbar[i] );
  }
  //  p->fillRect( 0, cc.r2sy( cbar.count()-1 ),
  //	       w, cc.r2sy( cc.s2ry( h ) - cbar.count() + 1 ), White );
  
}
