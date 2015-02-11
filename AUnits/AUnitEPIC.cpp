
#include "AUnitEPIC.h"

AUnitCCG:AUnitEPIC( void )
{
}

void AUnitEPIC::init0( Stars *s )
{
  s->SendCMD2( "Init", DevCh, "IsBusy" );
}
