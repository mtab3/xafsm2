
#include <QtGui>

#include "S2DView.h"

S2DView::S2DView( QWidget *p ) : QFrame( p )
{
  setupUi( this );

  rType = AS_SCREEN;
  setRange( 0, 1, 0, 1 );
  setSteps( 10, 10 );
  cc.SetRealCoord( minx, maxx, miny, maxy );
}

void S2DView::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}

void S2DView::Draw( QPainter *p )
{
  
}
