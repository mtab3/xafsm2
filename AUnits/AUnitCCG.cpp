
#include "AUnitCCG.h"

AUnitCCG:AUnitCCG( void )
{
}

void AUnitCCG::init0( Stars *s )
{
  s->SendCMD2( "Init", DevCh, "IsBusy" );
}
