
#include "AUnitAIO.h"

AUnitAIOi::AUnitAIOi( void )
{
}

AUnitAIOo::AUnitAIOo( void )
{
  MaxV = 0;
  MinV = 0;
}

void AUnitAIOi::init0( void )
{
  s->SendCMD2( "Init", Dev, "IsBusy" );
}

void AUnitAIOo::init0( void )
{
  connect( s, SIGNAL( AnsGoMaxAbs( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGoMaxRel( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsShutterOff( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );

  s->SendCMD2( "Init", DevCh, "Init" );
  s->SendCMD2( "Init", Dev, "IsBusy" );
}

void AUnitAIOi::SetIsBusyByMsg( SMsg msg )
{
  if ( ( msg.From() == Dev )
       && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
    IsBusy = ( msg.Val().toInt() == 1 );
    if ( IsBusy )
      LastFunc = "SetIsBusyByMsg";
    else
      LastFunc = "";
    emit ChangedIsBusy1( Dev );
  }
}

void AUnitAIOo::SetIsBusyByMsg( SMsg msg )
{
  if ( ( msg.From() == Dev )
       && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
    IsBusy = ( msg.Val().toInt() == 1 );
    if ( IsBusy )
      LastFunc = "SetIsBusyByMsg";
    else
      LastFunc = "";
    emit ChangedIsBusy1( Dev );
  }
}

double AUnitAIOi::SetTime( double dtime ) // in sec // この関数は、複数ステップ化できない
{
  setTime = dtime;          // setTime できたと見せかけるだけ。

  return setTime;
}

/** MStab ************************************************************************/

void AUnitAIOo::CloseShutter( bool close )
{
  if ( close ) {
    s->SendCMD2( Uid, Dev, "ShutterOff 1" );
  } else {
    s->SendCMD2( Uid, Dev, "ShutterOff 0" );
  }
}

void AUnitAIOo::GoMaxAbs( double start, double end, int steps )
{
  IsBusy2On( Dev, "GoMaxAbs" );
  s->SendCMD2( Uid, Dev,
	       QString( "GoMaxAbs 0 %1 %2 %3" ).arg( start ).arg( end ).arg( steps ) );
}

void AUnitAIOo::GoMaxAbsQ( double start, double end, int steps, double time )
{
  if ( Type == "AIOo" ) {
    IsBusy2On( Dev, "GoMaxAbsQ" );
    s->SendCMD2( Uid, Dev,
		 QString( "GoMaxAbs 1 %1 %2 %3 %4" )
		 .arg( start ).arg( end ).arg( steps ).arg( time ) );
  }
}

void AUnitAIOo::GoMaxRel( double width, int steps )
{
  if ( Type == "AIOo" ) {
    IsBusy2On( Dev, "GoMaxRel" );
    s->SendCMD2( Uid, Dev,
		 QString( "GoMaxRel 0 %1 %2" ).arg( width ).arg( steps ) );
  }
}

void AUnitAIOo::GoMaxRelQ( double width, int steps, double time )
{
  if ( Type == "AIOo" ) {
    IsBusy2On( Dev, "GoMaxRelQ" );
    s->SendCMD2( Uid, Dev,
		 QString( "GoMaxRel 1 %1 %2 %3" )
		 .arg( width ).arg( steps ).arg( time ) );
  }
}
