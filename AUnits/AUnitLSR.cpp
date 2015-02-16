
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

void AUnitLSR::OnReportCurrent( SMsg msg )
{
  if ( msg.From() == DevCh ) {
    Values = msg.Val().simplified().split( QRegExp( "\\s" ) );
    LastValue = Value;
    Value = Values[ Values.count() - 1 ];
    emit NewRingCurrent( Value, Values );
    emit NewValue( Value );
  }
}

void AUnitLSR::OnReportInjection( SMsg msg )
{
  if ( msg.From() == DevCh ) {
    Values = msg.Val().simplified().split( QRegExp( "\\s" ) );
    LastValue = Value;
    Value = Values[ Values.count() - 1 ];
    emit NewInjectionReport( Value, Values );
    emit NewValue( Value );
  }
}
