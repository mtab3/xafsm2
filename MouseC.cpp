
#include "MouseC.h"

MouseC::MouseC( void )
{
  nx = ny = Sx = Sy = Ex = Ey = 0;
  inpress = false;
}

void MouseC::Moved( QMouseEvent *e )
{
  nx = e->x();
  ny = e->y();

  Button = e->button();
  Mod = e->modifiers();
}

void MouseC::Pressed( QMouseEvent *e )
{
  Sx = e->x();
  Sy = e->y();

  inpress = true;
  Button = e->button();
  Mod = e->modifiers();
}

void MouseC::Released( QMouseEvent *e )
{
  Ex = e->x();
  Ey = e->y();

  inpress = false;
  Button = e->button();
  Mod = e->modifiers();
}

void MouseC::DClicked( QMouseEvent *e )
{
  DCx = e->x();
  DCy = e->y();

  Button = e->button();
  Mod = e->modifiers();
}

bool MouseC::CheckABPush( int x0, int y0 )
{
  if ( ( Ex > ( x0 + 5 ) )&&( Ex < ( x0 + 19 ) )
       &&( Ey > ( y0 - 19 ) )&&( Ey < ( y0 - 5 ) ) ) {
    return true;
  } else {
    return false;
  }
}
