
#include "AUnitEPIC.h"

AUnitEPIC::AUnitEPIC( void )
{
}

void AUnitEPIC::init0( void )
{
  setMeasIntegral( false );  // 測定値の大きさは計測時間に比例しない
  s->SendCMD2( "Init", DevCh, "IsBusy" );
}

#if 0
//void AUnitEPIC::SetIsBusyByMsg( SMsg msg )
{
  if ( ( msg.From() == DevCh )
       && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
    IsBusy = ( msg.Val().toInt() == 1 );
    if ( IsBusy )
      LastFunc = "SetIsBusyByMsg";
    else
      LastFunc = "";
    emit ChangedIsBusy1( DevCh );
  }
}
#endif

double AUnitEPIC::SetTime( double dtime ) // in sec // この関数は、複数ステップ化できない
{
  setTime = dtime;          // setTime できたと見せかけるだけ。

  return setTime;
}
