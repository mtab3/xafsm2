
#include "MainWindow.h"

bool MainWindow::MeasureDark( void )
{
  AUnit *as;

  if ( inMeasDark ) {
    if ( AskingShutterClose ) {
      AskingShutterClose = false;
      MeasBackGround->setText( tr( "BG Measuring" ) );
      MeasBackGround->setStyleSheet( "background-color: yellow" );
      return false;
    } else if ( AskingShutterOpen ) {
      AskingShutterOpen = false;
      return false;
    } else {
      statusbar->showMessage( tr( "Already Measuring Dark!" ), 2000 );
      return false;
    }
  }
  if ( inMeas != 0 ) {
    statusbar->showMessage( tr( "Measurement is going on!" ), 2000 );
    return false;
  }
  double dt = MeasBackTime->text().toDouble();
  if ( dt < 1 ) {
    dt = 1;
    MeasBackTime->setText( "1" );
  }
  mUnits.clearUnits();

  mUnits.addUnit( ASensors.value( SelectI0->currentIndex() ) );
  if ( UseI1->isChecked() )
    mUnits.addUnit( ASensors.value( SelectI1->currentIndex() ) );
  if ( Use19chSSD->isChecked() )
    mUnits.addUnit( SFluo );
  if ( UseAux1->isChecked() )
    mUnits.addUnit( ASensors.value( SelectAux1->currentIndex() ) );
  if ( UseAux2->isChecked() )
    mUnits.addUnit( ASensors.value( SelectAux2->currentIndex() ) );
  mUnits.setDwellTimes( dt );
  mUnits.setDwellTime();

  for ( int i = 0; i < mUnits.count(); i++ ) {
    as = mUnits.at(i);
    if ( ! as->isEnable() ) { // 指定されたセンサーが Stars 経由で生きていないとダメ
      QString msg = tr( "Scan cannot Start : (%1) is disabled" ).arg( as->getName() );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
      return false;
    }
  }
  // CNT2, OTC2 はカウンタの向こうに Keithley が繋がってる。
  // CNT2, OTC2 では Keithley をレンジ固定で、直接ではオートレンジで使うので
  // 両方を同時には測定に使えない
  for ( int i = 0; i < mUnits.count(); i++ ) {
    if (( mUnits.at(i)->getType() == "CNT2" )||( mUnits.at(i)->getType() == "OTC2" )) {
      for ( int j = 0; j < mUnits.count(); j++ ) {
	if ( mUnits.at(i)->get2ndUid() == mUnits.at(j)->getUid() ) {
	  QString msg = tr( "Selected sensors [%1] and [%2] are conflicting." )
	    .arg( mUnits.at(i)->getName() )
	    .arg( mUnits.at(j)->getName() );
	  statusbar->showMessage( msg, 2000 );
	  NewLogMsg( msg );
	  return false;
	}
      }
    }
  }

  inMeasDark = true;
  AskingShutterClose = true;
  AskingShutterOpen = false;
  MeasBackGround->setText( tr( "Close Shutter!!" ) );
  MeasBackGround->setStyleSheet( "background-color: red" );
  MeasDarkStage = 0;
  MeasDarkTimer->start( 100 );

  return true;
}

void MainWindow::MeasDarkSequence( void )
{
  if (( mUnits.isBusy() )||( AskingShutterOpen ))
    return;

  switch( MeasDarkStage ) {
  case 0:
    mUnits.clearStage();
    MeasDarkStage = 1;
    break;
  case 1:
    if ( mUnits.init() == false ) {  // true :: initializing
      mUnits.clearStage();
      MeasDarkStage = 2;
    }
    break;
  case 2: 
    mUnits.setDwellTime();
    if ( mUnits.isParent() ) {
      MeasDarkStage = 3;
    } else {
      MeasDarkStage = 4;
    }
    break;
  case 3:
    if ( mUnits.getValue0() == false ) { // only for counters and SSDs
      mUnits.clearStage();
      MeasDarkStage = 4;
    }
    break;
  case 4:
    if ( mUnits.getValue() == false ) {  // true :: Getting
      mUnits.clearStage();
      MeasDarkStage = 5;
    }
    break;
  case 5:
    double setTime;
    mUnits.readValue( MeasVals, false );   // false :: not correct dark
    for ( int i = 0; i < mUnits.count(); i++ ) {
      setTime = mUnits.at(i)->GetSetTime();
      if ( setTime > 0 ) {
	mUnits.at(i)->setDark( MeasVals[i] / setTime );
	qDebug() << "Dark for" << mUnits.at(i)->getName()
		 << "is" << MeasVals[i] / setTime
		 << " raw value " << MeasVals[i] << " meas time " << setTime;
      } else {
	statusbar
	  ->showMessage( tr( "Invalid dwell time [%1] was set for [%2]."
			     "However, the background was set "
			     "as if the time was set at 1sec." )
			 .arg( setTime ).arg( mUnits.at(i)->getName() ), 2000 );
	mUnits.at(i)->setDark( MeasVals[i] );
      }
    }
    AskingShutterOpen = true;
    MeasBackGround->setText( tr( "Open Shutter!!" ) );
    MeasBackGround->setStyleSheet( "background-color: red" );
    MeasDarkStage = 6;
    break;
  case 6:
    inMeasDark = false;
    MeasBackGround
      ->setStyleSheet( "background-color: "
		       "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 "
		       "rgba(225, 235, 225, 255), stop:1 "
		       "rgba(255, 255, 255, 255));" );
    MeasDarkStage = 0;
    MeasDarkTimer->stop();
    break;
  }
}
