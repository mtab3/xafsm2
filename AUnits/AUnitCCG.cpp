
#include "AUnitCCG.h"

AUnitCCG::AUnitCCG( void )
{
}

void AUnitCCG::init0( void )
{
  s->SendCMD2( "Init", DevCh, "IsBusy" );
}
