
#includ "AUnitPAM.h"

bool AUnitPAM::GetValue( void )
{
  bool rv = false;
  
  IsBusy2On( Driver, "GetValue" );
  s->SendCMD2( Uid, DevCh, "Read" );

  return false;
}
