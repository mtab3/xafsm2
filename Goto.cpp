#include "MainWindow.h"

void MainWindow::MotorMove( void )
{
  AUnit *am = AMotors.value( SPSMotorSelect->currentIndex() );
  
  if ( !am->isBusy() ) {
    GoMStop0();
    NewLogMsg( tr( "Setup: %1 : Reached at %2" )
	       .arg( am->getName() ).arg( am->value() ) );
    statusbar->showMessage( tr( "Setup: %1 : Reached at %2" )
			    .arg( am->getName() ).arg( am->value() ), 1000 );
  }
}
