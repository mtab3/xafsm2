
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

#if 0
// CCG �ϡ��Ѷ�Ū���ͤ�ʹ���˹Ԥ����Ȥ�̵����
// Event�ǿ��ή����Ƥ����ͤ�ί��Ƥ����� value() ���Ф��������������
// ---> �ǤϤʤ��ä���
//  GetValue ��ư��� AnsGetValue �˷Ҥ��� slot �ϡ�AUnit0 ������ɸ��Τ�Τ��ɤ��Ϥ�
bool AUnitCCG::GetValue( void )
{
  return false;
}
#endif
