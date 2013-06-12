
#include <QtGui>

#include "cBar.h"

cBar::cBar( QWidget *p ) : QFrame( p )
{
  setupUi( this );
}

void cBar::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}

void cBar::Draw( QPainter *p )
{
}
