
#include "AUnitFP23.h"

AUnitFP23::AUnitFP23( void )
{
}

void AUnitFP23::init0( void )
{
  setMeasIntegral( false );  // 測定値の大きさは計測時間に比例しない
  s->SendCMD2( Uid, Dev, "flgon", Ch );
  s->SendCMD2( Uid, "System", "flgon", Dev );

  connect( s, SIGNAL( EvReportValue(SMsg) ), this, SLOT( OnReportValue(SMsg) ),
	   Qt::UniqueConnection );
  s->SendCMD2( Uid, DevCh, "IsBusy" );
}

void AUnitFP23::OnReportValue( SMsg msg )
{
  if ( msg.From() == DevCh ) {
    Values = msg.Val().simplified().split( QRegExp( "\\s" ) );
    LastValue = Value;
    Value = Values[ Values.count() - 1 ];
    emit NewValue( Value );
  }
}

#if 0
// FP23 は、積極的に値を聞きに行くことは無い。
// Eventで垂れ流されている値を溜めておいて value() に対して返答するだけ
bool AUnitFP23::GetValue( void )
{
  return false;
}
#endif
