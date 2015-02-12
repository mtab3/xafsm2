
#include "AUnitXMAP2.h"

void AUnitXMAP2::init0( void )
{
  connect( s, SIGNAL( AnsSetPresetValue( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetROIs( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  s->SendCMD2( "Init", "System", "flgon", DevCh );
}

double AUnitXMAP2::stat( STATELM i )
{
  return ((AUnitXMAP*)TheParent)->stat( Ch.toInt(), i );
}

bool AUnitXMAP2::SetRealTime( double val )
{
  bool rv = false;

  IsBusy2On( Dev, "SetRealTime1" );
  s->SendCMD2( Uid, DevCh, "SetRealTime", QString::number( val ) );
  rv = false;

  return rv;
}
