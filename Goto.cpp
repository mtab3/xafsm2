#include "MainWindow.h"

void MainWindow::MotorMove( void )
{
  // �������򤵤�Ƥ����˥åȤ���ɸ���ˤ��ɤ��夤�ƤƤ���
  // (ư���Ƥ���ե饰��Ω�äƤ�Τˡ�busy ��̵���ʤäƤ���)
  // ɽ�����Ѥ����꿧�������
  int mNo = SPSMotorSelect->currentIndex();
  AMotor *am = AMotors[ mNo ];

  // GOMOTOR ���ȤäƤ��˥åȤ�
  if ( UUnits.user( am ) == GOMOTOR_ID ) {
    // busy ��̵���ʤäƤ���
    if ( ! ( am->isBusy() ) ) {
      UUnits.removeAnUnit( GOMOTOR_ID, am );
      setGoBAsNotMoving();
      NewLogMsg( tr( "Setup: %1 : Reached at %2" )
		 .arg( am->getName() ).arg( am->value() ) );
      statusbar->showMessage( tr( "Setup: %1 : Reached at %2" )
			      .arg( am->getName() ).arg( am->value() ), 1000 );
    }
  }

  // GOMOTOR ���ȤäƤ��˥åȤ��ޤ����ä������
  if ( UUnits.isAnyOneUsedBy( GOMOTOR_ID ) )
    return;

  // ̵���ä��餳�δؿ���ƤӽФ������ޡ���ߤ�Ƥ���
  GoTimer->stop();
}
