
#include "AUnitCCG.h"

AUnitCCG::AUnitCCG( void )
{
}

void AUnitCCG::init0( void )
{
  setMeasIntegral( false );  // ¬���ͤ��礭���Ϸ�¬���֤����㤷�ʤ�
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

// CCG �ϡ��Ѷ�Ū���ͤ�ʹ���˹Ԥ����Ȥ�̵����
// Event�ǿ��ή����Ƥ����ͤ�ί��Ƥ����� value() ���Ф��������������
bool AUnitCCG::GetValue( void )
{
  return false;
}
