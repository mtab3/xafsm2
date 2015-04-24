
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

#if 0
// CCG は、積極的に値を聞きに行くことは無い。
// Eventで垂れ流されている値を溜めておいて value() に対して返答するだけ
// ---> ではなかった。
//  GetValue の動作と AnsGetValue に繋がる slot は、AUnit0 が持つ標準のもので良いはず
bool AUnitCCG::GetValue( void )
{
  return false;
}
#endif
