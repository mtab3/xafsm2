
#include "AUnitFP23.h"

AUnitFP23::AUnitFP23( void )
{
}

void AUnitFP23::init0( void )
{
  s->SendCMD2( "Init", Dev, "flgon", Ch );
  s->SendCMD2( "Init", "System", "flgon", Dev );

  connect( s, SIGNAL( EvReportValue(SMsg) ), this, SLOT( OnReportValue(SMsg) ),
	   Qt::UniqueConnection );
  s->SendCMD2( "Init", DevCh, "IsBusy" );
}
