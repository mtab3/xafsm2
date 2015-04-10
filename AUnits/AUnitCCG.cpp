
#include "AUnitCCG.h"

AUnitCCG::AUnitCCG( void )
{
}

void AUnitCCG::init0( void )
{
  setMeasIntegral( false );  // 測定値の大きさは計測時間に比例しない
  s->SendCMD2( "Init", DevCh, "IsBusy" );
}

#if 0
void AUnitCCG::SetCurPos( SMsg msg )
{
  QString buf;
  
  if ( ( msg.From() == DevCh )
       && ( ( msg.Msgt() == GETVALUE ) || ( msg.Msgt() == EvCHANGEDVALUE )
            || ( msg.Msgt() == READ ) ) ) {
    Value = msg.Vals().at(0);
    emit NewValue( Value );
    IsBusy2Off( Dev );
  }
}
#endif

// CCG は、積極的に値を聞きに行くことは無い。
// Eventで垂れ流されている値を溜めておいて value() に対して返答するだけ
bool AUnitCCG::GetValue( void )
{
  return false;
}
