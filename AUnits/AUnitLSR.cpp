
#include "AUnitLSR.h"

AUnitLSR::AUnitLSR( void )
{
}

void AUnitLSR::init0( void )
{
  setMeasIntegral( false );  // 測定値の大きさは計測時間に比例しない
  connect( s, SIGNAL(EvReportCurrent( SMsg )), this, SLOT(OnReportCurrent( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(EvReportInjection( SMsg )), this, SLOT(OnReportInjection( SMsg )),
	   Qt::UniqueConnection );
  s->SendCMD2( Uid, Dev, "flgon", Ch );
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

// LSR は、積極的に値を聞きに行くことは無い。
// Eventで垂れ流されている値を溜めておいて value() に対して返答するだけ
bool AUnitLSR::GetValue( void )
{
  return false;
}
