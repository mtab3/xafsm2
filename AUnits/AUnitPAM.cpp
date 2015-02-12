
#include "AUnitPAM.h"

void AUnitPAM::init0( void )
{
  connect( s, SIGNAL( AnsRead( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetAutoRange( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetDataFormat( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetZeroCheck( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetNPLCycles( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );

  init00();
}

void AUnitPAM::init00( void )
{
  s->SendCMD2( "Init", DevCh, "IsBusy" );
}

void AUnitPAM2::init00( void )    // PAM と PAM2 で違ってる
{
  connect( s, SIGNAL( AnsRead( SMsg ) ),this, SLOT( RcvAnsGetValueOfDriver( SMsg ) ),
	   Qt::UniqueConnection );

  s->SendCMD2( "Init", Dev, "IsBusy" );
}

bool AUnitPAM::GetValue( void )
{
  bool rv = false;
  
  IsBusy2On( Dev, "GetValue" );
  s->SendCMD2( Uid, DevCh, "Read" );

  return false;
}

