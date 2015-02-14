
#include "MainWindow.h"

bool MainWindow::MeasureDark( void )
{
  ASensor *as;

  if ( inMeasDark ) {
    if ( AskingShutterClose ) {
      AskingShutterClose = false;
      MeasBackGround->setText( tr( "BG Measuring" ) );
      MeasBackGround->setStyleSheet( InActive );
      return false;
    } else if ( AskingShutterOpen ) {
      AskingShutterOpen = false;
      return false;
    } else {
      statusbar->showMessage( tr( "Already Measuring Dark!" ), 2000 );
      return false;
    }
  }
  if ( inMeas ) {
    statusbar->showMessage( tr( "Measurement is going on!" ), 2000 );
    return false;
  }

  double dt = MeasBackTime->text().toDouble();
  if ( dt < 1 ) {
    dt = 1;
    MeasBackTime->setText( "1" );
  }
  dUnits.clearUnits();

  dUnits.addUnit( I0Sensors[ SelectI0->currentIndex() ] );
  if ( UseI1->isChecked() )
    dUnits.addUnit( I1Sensors[ SelectI1->currentIndex() ] );
  if ( Use19chSSD->isChecked() ) {
    if ( inMCAMeas ) {
      QString msg = tr( "MCA measurement is going on" );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
      return false;
    }
    dUnits.addUnit( SFluo );
  }
  if ( UseAux1->isChecked() )
    dUnits.addUnit( A1Sensors[ SelectAux1->currentIndex() ] );
  if ( UseAux2->isChecked() )
    dUnits.addUnit( A2Sensors[ SelectAux2->currentIndex() ] );
  dUnits.setDwellTimes( dt );
  dUnits.setDwellTime();

  for ( int i = 0; i < dUnits.count(); i++ ) {
    as = dUnits.at(i);
    if ( ! as->isEnable() ) { // 指定されたセンサーが Stars 経由で生きていないとダメ
      QString msg = tr( "Scan cannot Start : (%1) is disabled" ).arg( as->name() );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
      return false;
    }
  }
  // CNT2, OTC2 はカウンタの向こうに Keithley が繋がってる。
  // CNT2, OTC2 では Keithley をレンジ固定で、直接ではオートレンジで使うので
  // 両方を同時には測定に使えない
  for ( int i = 0; i < dUnits.count(); i++ ) {
    if (( dUnits.at(i)->type() == "CNT2" )||( dUnits.at(i)->type() == "OTC2" )) {
      for ( int j = 0; j < dUnits.count(); j++ ) {
	if ( dUnits.at(i)->uid2() == dUnits.at(j)->uid() ) {
	  QString msg = tr( "Selected sensors [%1] and [%2] are conflicting." )
	    .arg( dUnits.at(i)->name() )
	    .arg( dUnits.at(j)->name() );
	  statusbar->showMessage( msg, 2000 );
	  NewLogMsg( msg );
	  return false;
	}
      }
    }
  }

  if ( MMStab != NULL )
    UseAutoShutter = AutoShutter->isChecked();
  else 
    UseAutoShutter = false;

  inMeasDark = true;
  if ( !UseAutoShutter ) {
    AskingShutterClose = true;
    AskingShutterOpen = false;
    MeasBackGround->setText( tr( "Close Shutter!!" ) );
    MeasBackGround->setStyleSheet( AlartRed );
    statusbar->showMessage( tr( "Make sure that shutte is closed."
				"  Then push the 'red' button." ), 0 );
  } else {
    MMStab->CloseShutter( true );
    MeasBackGround->setText( tr( "BG Measuring" ) );
    MeasBackGround->setStyleSheet( InActive );
  }
  MeasDarkStage = 0;
  MeasDarkTimer->start( 100 );

  return true;
}

void MainWindow::MeasDarkSequence( void )
{
  if (( dUnits.isBusy() )||( AskingShutterClose )||( AskingShutterOpen ))
    return;

  switch( MeasDarkStage ) {
  case 0:
    statusbar->showMessage( "", 0 );
    dUnits.clearStage();
    MeasDarkStage = 1;
    break;
  case 1:
    if ( dUnits.init() )  // true :: initializing
      break;
    dUnits.clearStage();
    MeasDarkStage = 2;
    break;
  case 2: 
    dUnits.setDwellTime();
    if ( dUnits.isParent() ) {
      MeasDarkStage = 3;
    } else {
      MeasDarkStage = 4;
    }
    break;
  case 3:
    if ( dUnits.getValue0() )  // only for counters and SSDs
      break;
    dUnits.clearStage();
    MeasDarkStage = 4;
    break;
  case 4:
    if ( dUnits.getValue() )  // true :: Getting
      break;
    dUnits.clearStage();
    MeasDarkStage = 5;
    break;
  case 5:
    double setTime;
    dUnits.readValue( MeasVals, MeasCPSs, false );   // false :: not correct dark
    // 前は MeasCPSs は無かったので MeasVals (count) を cps に直す計算をここでやってる。
    // 直しても良いけどそのままにしておく
    for ( int i = 0; i < dUnits.count(); i++ ) {
      setTime = dUnits.at(i)->getSetTime();
      if ( setTime > 0 ) {
	dUnits.at(i)->setDark( MeasVals[i] / setTime );
      } else {
	statusbar
	  ->showMessage( tr( "Invalid dwell time [%1] was set for [%2]."
			     "However, the background was set "
			     "as if the time was set at 1sec." )
			 .arg( setTime ).arg( dUnits.at(i)->name() ), 2000 );
	dUnits.at(i)->setDark( MeasVals[i] );
      }
      if ( dUnits.at(i) == SFluo ) {
	SFluo->setDark();      // 19ch 分のダークを内部空間に保存
      }
    }
    if ( !UseAutoShutter ) {
      AskingShutterOpen = true;
      MeasBackGround->setText( tr( "Open Shutter!!" ) );
      MeasBackGround->setStyleSheet( AlartRed );
      statusbar->showMessage( tr( "Make sure that shutte is opened."
				  "  Then push the 'red' button." ), 0 );
    } else {
      MMStab->CloseShutter( false );
    }
    MeasDarkStage = 6;
    break;
  case 6:
    haveMeasuredDark = true;
    inMeasDark = false;
    statusbar->showMessage( "", 0 );
    MeasBackGround->setText( tr( "Measure Background" ) );
    MeasBackGround
      ->setStyleSheet( NormalB );
    MeasDarkStage = 0;
    MeasDarkTimer->stop();
    break;
  }
}

void MainWindow::ShowNewDark( double dark )
{
  if ( sender() == ASensors.at( SelectD3->currentIndex() ) ) {
    InputDark->setText( QString::number( dark ) );
  }
}

void MainWindow::NewDarkChSelected( int i )
{
  InputDark
    ->setText( QString::number( ASensors.at( i )->getDark() ) );
}

void MainWindow::AskedToSetDark( void )
{
  ASensors.at( SelectD3->currentIndex() )->setDark( InputDark->text().toDouble() );
}
