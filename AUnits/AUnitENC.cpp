
#include "AUnitENC.h"

void AUnitENC::init0( void  )
{
  connect( s, SIGNAL(AnsGetStat( SMsg )), this, SLOT( RcvStat( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsTrigger( SMsg )), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsStandBy( SMsg )), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsGetData( SMsg )), this, SLOT( RcvQGetData( SMsg ) ),
	   Qt::UniqueConnection );

  init00();
}

void AUnitENC::init00( void )
{
  connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	   Qt::UniqueConnection );
  s->SendCMD2( "Init", DevCh, "IsBusy" );
  s->SendCMD2( "Init", DevCh, "GetValue" );
}

void AUnitENC2::init00( void )
{
}

void AUnitENC::SetValue( double v )
{
  s->SendCMD2( Uid, DevCh, "SetValue", QString::number( DLastSetV = v ) );
}

void AUnitENC2::AskIsBusy( void )
{
  s->SendCMD2( Uid, DevCh, "IsBusy" );
}

bool AUnitENC2::QStart( void )
{
  IsBusy2On( Dev, "Start" );
  s->SendCMD2( Uid, DevCh, "StandBy" );
  return false;
}

bool AUnitENC2::QRead( void )
{
  IsBusy2On( Dev, "Read" );
  s->SendCMD2( Uid, DevCh, "GetData" );

  return false;
}
