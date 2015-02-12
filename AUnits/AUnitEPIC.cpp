
#include "AUnitEPIC.h"

AUnitEPIC::AUnitEPIC( void )
{
}

void AUnitEPIC::init0( void )
{
  s->SendCMD2( "Init", DevCh, "IsBusy" );
}
