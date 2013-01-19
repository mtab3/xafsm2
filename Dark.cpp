
#include "MainWindow.h"

void MainWindow::MeasureDark( void )
{
  AUnit *as;

  if ( inMeasDark ) {
    statusbar->showMessage( tr( "Already Measuring Dark!" ), 2000 );
    return;
  }
  if ( inMeas != 0 ) {
    statusbar->showMessage( tr( "Measurement is going on!" ), 2000 );
    return;
  }

  mUnits.clearUnits();

  mUnits.addUnit( ASensors.value( SelectI0->currentIndex() ), 0 );
  if ( UseI1->isChecked() )
    mUnits.addUnit( ASensors.value( SelectI1->currentIndex() ), 0 );
  if ( Use19chSSD->isChecked() )
    mUnits.addUnit( SFluo, 0 );
  if ( UseAux1->isChecked() )
    mUnits.addUnit( ASensors.value( SelectAux1->currentIndex() ), 0 );
  if ( UseAux2->isChecked() )
    mUnits.addUnit( ASensors.value( SelectAux2->currentIndex() ), 0 );

  for ( int i = 0; i < mUnits.count(); i++ ) {
    as = mUnits.at(i);
    if ( ! as->isEnable() ) { // 指定されたセンサーが Stars 経由で生きていないとダメ
      QString msg = tr( "Scan cannot Start : (%1) is disabled" ).arg( as->getName() );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
      return;
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
	  return;
	}
      }
    }
  }
  
  inMeasDark = true;
  MeasBackGround->setStyleSheet( "background-color: yellow" );
  MeasDarkStage = 0;
  MeasDarkTimer->start( 100 );
}

void MainWindow::MeasDarkSequence( void )
{
  
}
