
#include "CheckUnits.h"

void CheckUnits::add( AUnit0 *au, bool b1, bool b2 )
{
  for ( int i = 0; i < units.count(); i++ ) {
    if ( units[i]->au == au ) {
      return;
    }
  }

  CUnit *cu = new CUnit;
  cu->au = au;
  cu->b1f = b1;
  cu->b2f = b2;

  units << cu;
}

void CheckUnits::clear( void )
{
  units.clear();
}

bool CheckUnits::isBusy( void )
{
  for ( int i = 0; i < units.count(); i++ ) {
    if ( ( units[i]->b1f )&&( units[i]->au->isBusy() ) )
      return true;
    if ( ( units[i]->b2f )&&( units[i]->au->isBusy2() ) )
      return true;
  }

  return false;
}
