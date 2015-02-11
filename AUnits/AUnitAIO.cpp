
#include "AUnitAIO.h"

AUnitAIOi::AUnitAIOi( void )
{
}

AUnitAIOo::AUnitAIOo( void )
{
  MaxV = 0;
  MinV = 0;
}

void AUnitAIOi::init00( Stars *s )
{
  s->SendCMD2( "Init", Dev, "IsBusy" );
}

void AUnitAIOo::init00( Stars *s )
{
  connect( s, SIGNAL( AnsGoMaxAbs( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGoMaxRel( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGoMaxAbsQ( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGoMaxRelQ( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsShutterOff( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );

  s->SendCMD2( "Init", DevCh, "Init" );
  s->SendCMD2( "Init", Dev, "IsBusy" );
}


/** MStab ************************************************************************/

void AUnitAIOo::CloseShutter( bool close )
{
  if ( close ) {
    s->SendCMD2( Uid, Driver, "ShutterOff 1" );
  } else {
    s->SendCMD2( Uid, Driver, "ShutterOff 0" );
  }
}

void AUnitAIOo::GoMaxAbs( double start, double end, int steps )
{
  IsBusy2On( Driver, "GoMaxAbs" );
  s->SendCMD2( Uid, Driver,
	       QString( "GoMaxAbs 0 %1 %2 %3" ).arg( start ).arg( end ).arg( steps ) );
}

void AUnitAIOo::GoMaxAbsQ( double start, double end, int steps, double time )
{
  if ( Type == "AIOo" ) {
    IsBusy2On( Driver, "GoMaxAbsQ" );
    s->SendCMD2( Uid, Driver,
		 QString( "GoMaxAbs 1 %1 %2 %3 %4" )
		 .arg( start ).arg( end ).arg( steps ).arg( time ) );
  }
}

void AUnitAIOo::GoMaxRel( double width, int steps )
{
  if ( Type == "AIOo" ) {
    IsBusy2On( Driver, "GoMaxRel" );
    s->SendCMD2( Uid, Driver,
		 QString( "GoMaxRel 0 %1 %2" ).arg( width ).arg( steps ) );
  }
}

void AUnitAIOo::GoMaxRelQ( double width, int steps, double time )
{
  if ( Type == "AIOo" ) {
    IsBusy2On( Driver, "GoMaxRelQ" );
    s->SendCMD2( Uid, Driver,
		 QString( "GoMaxRel 1 %1 %2 %3" ).arg( width ).arg( steps ).arg( time ) );
  }
}
