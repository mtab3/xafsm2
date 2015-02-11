
#include "AUnitFP23.h"

AUnitCCG:AUnitFP23( void )
{
}

void AUnitFP23::init0( Stars *s )
{
  s->SendCMD2( "Init", Dev, "flgon", Ch );
  s->SendCMD2( "Init", "System", "flgon", Dev );

  connect( s, SIGNAL( EvReportValue(SMsg) ), this, SLOT( OnReportValue(SMsg) ),
	   Qt::UniqueConnection );
  s->SendCMD2( "Init", DevCh, "IsBusy" );
}
