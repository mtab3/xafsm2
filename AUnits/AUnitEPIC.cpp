
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

double AUnitEPIC::SetTime( double dtime ) // in sec // $B$3$N4X?t$O!"J#?t%9%F%C%W2=$G$-$J$$(B
{
  setTime = dtime;          // setTime $B$G$-$?$H8+$;$+$1$k$@$1!#(B

  return setTime;
}
