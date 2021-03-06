
#include "AUnitPZ.h"

AUnitPZ::AUnitPZ( void )
{
  MaxV = 0;          // only for PZ
  MinV = 0;          // only for PZ
}

void AUnitPZ::init0( void )
{
  connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	   Qt::UniqueConnection );

  connect( s, SIGNAL( AnsGoMaxAbs( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGoMaxRel( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsShutterOff( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  s->SendCMD2( Uid, DevCh, "Init" );

}
