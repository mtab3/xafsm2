
#include "AUnitCCG.h"

AUnitCCG::AUnitCCG( void )
{
}

void AUnitCCG::init0( void )
{
  s->SendCMD2( "Init", DevCh, "IsBusy" );
}

#if 0
void AUnitCCG::SetCurPos( SMsg msg )
{
  QString buf;
  
  if ( ( msg.From() == DevCh )
       && ( ( msg.Msgt() == GETVALUE ) || ( msg.Msgt() == EvCHANGEDVALUE )
            || ( msg.Msgt() == READ ) ) ) {
    Value = msg.Vals().at(0);
    emit NewValue( Value );
    IsBusy2Off( Dev );
  }
}
#endif
