#include "MainWindow.h"

void MainWindow::MotorMove( void )
{
  // 現在選択されているユニットが目標点にたどり着いててたら
  // (動いているフラグが立ってるのに、busy で無くなってたら)
  // 表示を変えたり色々後始末
  int mNo = SPSMotorSelect->currentIndex();
  AMotor *am = AMotors[ mNo ];

  // GOMOTOR が使ってるユニットが
  if ( UUnits.user( am ) == GOMOTOR_ID ) {
    // busy で無くなってたら
    if ( ! ( am->isBusy() ) ) {
      UUnits.removeAnUnit( GOMOTOR_ID, am );
      setGoBAsNotMoving();
      NewLogMsg( tr( "Setup: %1 : Reached at %2" )
		 .arg( am->getName() ).arg( am->value() ) );
      statusbar->showMessage( tr( "Setup: %1 : Reached at %2" )
			      .arg( am->getName() ).arg( am->value() ), 1000 );
    }
  }

  // GOMOTOR が使ってるユニットがまだあったら戻る
  if ( UUnits.isAnyOneUsedBy( GOMOTOR_ID ) )
    return;

  // 無かったらこの関数を呼び出すタイマーを止めておく
  GoTimer->stop();
}
