
#include "AUnitEPIC.h"

AUnitEPIC::AUnitEPIC( void )
{
}

void AUnitEPIC::init0( void )
{
  s->SendCMD2( "Init", DevCh, "IsBusy" );
}

void AUnitEPIC::SetIsBusyByMsg( SMsg msg )
{
  if ( ( msg.From() == DevCh )
       && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
    IsBusy = ( msg.Val().toInt() == 1 );
    if ( IsBusy )
      LastFunc = "SetIsBusyByMsg";
    else
      LastFunc = "";
    emit ChangedIsBusy1( Dev );
  }
}

double AUnitEPIC::SetTime( double dtime ) // in sec // この関数は、複数ステップ化できない
{
  setTime = dtime;          // setTime できたと見せかけるだけ。

  return setTime;
}
