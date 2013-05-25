#include "Changer.h"

Changer::Changer( void ) : QObject()
{
  ID.clear();
  Name.clear();
  UnitID1.clear();
  UnitID2.clear();
  Holders1 = Holders2 = 0;
  Center1 = Center2 = 0;
  Spacing1 = Spacing2 = 0;
}
