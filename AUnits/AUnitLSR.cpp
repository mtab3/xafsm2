
#include "AUnitLSR.h"

AUnitLSR::AUnitLSR( void )
{
}

void AUnitLSR::init0( void )
{
  connect( s, SIGNAL(EvReportCurrent( SMsg )), this, SLOT(OnReportCurrent( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(EvReportInjection( SMsg )), this, SLOT(OnReportInjection( SMsg )),
	   Qt::UniqueConnection );
  s->SendCMD2( "Init", Dev, "flgon", Ch );
}
