
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

  colors = cbar.count();
  cmin = 0;
  cmax = colors;
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
  p->fillRect( 0, cc.r2sy( cmin ), w, cc.r2sy( 0 ) - cc.r2sy( cmin ), Black );
  for ( int i = 0; i < colors; i++ ) {
    p->fillRect( 0, cc.r2sy( i ), w, ph, *cbar[i] );
  }
  p->fillRect( 0, cc.r2sy( colors ),
	       w, cc.r2sy( cmax ) - cc.r2sy( colors ), White );
  
}

void cBar::mouseMoveEvent( QMouseEvent *e )
{
  m.Moved( e );

  if ( m.inPress() ) {
  }
  
  update();
}

void cBar::mousePressEvent( QMouseEvent *e )
{
  m.Pressed( e );

  update();
}

void cBar::mouseReleaseEvent( QMouseEvent *e )
{
  m.Released( e );

  update();
}
