
#include "MainWindow.h"

// QXafs 関連のイニシャライズ setupMeMeas より前に呼ばれてる
void MainWindow::setupQXafsMode( void )
{
  OrigHSpeed = HSpeed = 6000;    // 6000 pps, 0.1arcsec/pulse = 2.777..x10-5 deg/pulse
  QMeasOnBackward->setHidden( true );
}

void MainWindow::ToggleQXafsMode( bool )
{
  if ( QXafsMode->isChecked() ) {
    SaveSelectedI0 = SelectI0->currentIndex();
    SaveSelectedI1 = SelectI1->currentIndex();
    for ( int i = 0; i < I0Sensors.count(); i++ ) {
      if ( I0Sensors[i]->getID() == "QXAFS-I0" ) {
	SelectI0->setCurrentIndex( i );
	break;
      }
    }
    for ( int i = 0; i < I1Sensors.count(); i++ ) {
      if ( I1Sensors[i]->getID() == "QXAFS-I1" ) {
	SelectI1->setCurrentIndex( i );
	break;
      }
    }

    SaveNowBlocks = SelBLKs->value();
    CheckQXafsParams();
    ChangeBLKs( 1 );
    SelBLKs->setEnabled( false );
    HideBLKs( true );
    QMeasOnBackward->setHidden( false );

    SaveUse19ChSSD = Use19chSSD->isChecked();
    SaveUseAux1 = UseAux1->isChecked();
    SaveUseAux2 = UseAux2->isChecked();
    Use19chSSD->setChecked( false );
    UseAux1->setChecked( false );
    UseAux2->setChecked( false );
    Use19chSSD->setEnabled( false );
    UseAux1->setEnabled( false );
    UseAux2->setEnabled( false );

  } else {

    SelBLKs->setEnabled( true );
    ChangeBLKs( SaveNowBlocks );
    HideBLKs( false );
    QMeasOnBackward->setHidden( true );

    SelectI0->setCurrentIndex( SaveSelectedI0 );
    SelectI1->setCurrentIndex( SaveSelectedI1 );
    Use19chSSD->setChecked( SaveUse19ChSSD );
    UseAux1->setChecked( SaveUseAux1 );
    UseAux2->setChecked( SaveUseAux1 );
    Use19chSSD->setEnabled( true );
    UseAux1->setEnabled( true );
    UseAux2->setEnabled( true );
  }
}

void MainWindow::HideBLKs( bool f )
{
  for ( int i = 2; i < BLKstart.count(); i++ ) {
    BLKstart[i]->setHidden( f );
  }
  for ( int i = 1; i < BLKstep.count(); i++ ) {
    BLKstep[i]->setHidden( f );
    BLKdwell[i]->setHidden( f );
    BLKpoints[i]->setHidden( f );
  }
  DwellAll->setHidden( f );
  for ( int i = 2; i < BLKlabels.count(); i++ ) {
    BLKlabels[i]->setHidden( f );
  }
}

void MainWindow::CheckQXafsParams( void )  // BlockPoints は Widget から直読みしない
{
  double sdeg = u->keV2deg( u->any2keV( BLKUnit, BLKstart[0]->text().toDouble() ) );
  double edeg = u->keV2deg( u->any2keV( BLKUnit, BLKstart[1]->text().toDouble() ) );
  int steps = BlockPoints[0];
  double dtime = BLKdwell[0]->text().toDouble();

  HSpeed = fabs( ( edeg - sdeg ) / dtime / MMainTh->getUPP() );
  if (( HSpeed > MaxHSpeed )||( HSpeed < 0 )) {
    HSpeed = MaxHSpeed;
    dtime = fabs( edeg - sdeg ) / ( HSpeed * MMainTh->getUPP() );
    BlockDwell[0] = dtime;
  }
}

void MainWindow::GetPM16CParamsForQXAFS( void )
{
  double sdeg = u->keV2deg( SBlockStart[0] );
  double edeg = u->keV2deg( SBlockStart[1] );
  int steps = abs( SBlockPoints[0] );
  double dtime = SBlockDwell[0];

  MMainTh->SetHighSpeed( HSpeed );

  QXafsSP0 = sdeg / MMainTh->getUPP() + MMainTh->getCenter();
  QXafsEP0 = edeg / MMainTh->getUPP() + MMainTh->getCenter();
  if ( abs( QXafsSP0 - QXafsEP0 ) > steps )
    QXafsInterval = (int)(abs( QXafsSP0 - QXafsEP0 ) / steps);
  else 
    QXafsInterval = 1;
  QXafsSteps = abs( QXafsSP0 - QXafsEP0 ) / QXafsInterval;
  QXafsDwellTime = ( (double)QXafsInterval / HSpeed ) * 0.9;

  RunUpTime = ( HSpeed - LowSpeed ) * RunUpRate / 1000;
  int RunUpPulses = ( HSpeed - LowSpeed ) * ( HSpeed + LowSpeed ) * RunUpRate / 2000.;
  if ( QXafsSP0 > QXafsEP0 )
    RunUpPulses *= -1;
  qDebug() << "RunUpPulses " << HSpeed << LowSpeed << RunUpRate << RunUpPulses << RunUpTime;

  QXafsSP = QXafsSP0 - RunUpPulses;
  QXafsEP = QXafsEP0 + RunUpPulses;

  qDebug() << QString( "Measure Range [ %1 [ %2 %3 ] %4 ], RunUpRate %5" )
    .arg( QXafsSP ).arg( QXafsSP0 ).arg( QXafsEP0 ).arg( QXafsEP ).arg( RunUpRate );
  qDebug() << QString( "Interval and Steps %1 %2" )
    .arg( QXafsInterval ).arg( QXafsSteps );
}

void MainWindow::QXafsMeasSequence( void )
{
  switch( MeasStage ) {
  case 0:
    CurrentRpt->setText( QString::number( 1 ) );
    //    WriteInfoFile();
    mUnits.clearStage();
    MeasView->SetWindow0( SBlockStart[0], 0, SBlockStart[ SBlocks ], 0 );
    statusbar->showMessage( tr( "Start QXAFS Measurement!" ) );
    GetPM16CParamsForQXAFS();
    MMainTh->SetSpeed( HIGH );
    MeasStage = 1;
    break;
  case 1:
    if ( ( mUnits.init() == false ) // SetDataSlope までこの中にある
	 && ( MMainTh->busy2Count() == 0 ) ) {  // true :: initializing
      MeasR = 0;    // Measurement Repeat count
      MeasStage = 2;
    }
    break;
  case 2:
    mUnits.setDwellTimes( QXafsDwellTime );
    mUnits.setDwellTime();
    MeasStage = 3;
    break;
  case 3:
    mUnits.start();
    MMainTh->SetValue( QXafsSP );   // 助走距離を含めたスタート地点へ
    MeasStage = 4;
    break;
  case 4:
    MeasR++;
    if ( MeasR > SelRPT->value() ) {// 終了処理に入る!!
      MeasStage = 99;
      break;
    }
    //    WriteHeader( MeasR );
    MeasStage = 5;
  case 5:
    MMainTh->SetTimingOutMode( 3 );
    MMainTh->SetTimingOutStart( QXafsSP0 );
    MMainTh->SetTimingOutEnd( QXafsEP0 );
    MMainTh->SetTimingOutInterval( QXafsInterval );
    MMainTh->SetTimingOutReady( 1 );
    MeasStage = 6;
    break;
  case 6:
    MMainTh->SetValue( QXafsEP );   // 減速距離を含めた終了地点へ
    MeasStage = 7;
    break;
  case 7:
    // ReadOutStage 
    MeasStage = 8;
    break;
  case 8:
    if ( QMeasOnBackward->isChecked() ) {   // 戻りも測定する
      MMainTh->SetTimingOutMode( 3 );
      MMainTh->SetTimingOutStart( QXafsEP0 );
      MMainTh->SetTimingOutEnd( QXafsSP0 );
      MMainTh->SetTimingOutInterval( QXafsInterval );
      MMainTh->SetTimingOutReady( 1 );
    } else {
      MMainTh->SetTimingOutMode( 0 );
      MMainTh->SetTimingOutReady( 0 );
    }
    MeasStage = 9;
    break;
  case 9:
    MMainTh->SetValue( QXafsSP );   // 助走距離を含めたスタート地点へ
    if ( QMeasOnBackward->isChecked() ) {   // 戻りも測定する
      MeasStage = 10;
    } else {
      MeasStage = 4;
    }
    break;
  case 10:
    // ReadOutStage
    MeasStage = 4;
    break;
    // don't break
  case 99:
    MMainTh->SetHighSpeed( OrigHSpeed );   // H のスピードを標準に戻す
    MMainTh->SetSpeed( GoMSpeed );         // 選択されていたスピードに戻す
    statusbar->showMessage( tr( "The Measurement has Finished" ), 4000 );
    NewLogMsg( QString( tr( "Meas: QXafs Finished" ) ) );
    //    WriteHeader2( MeasR );
    //    WriteInfoFile2();
    MeasTimer->stop();
    inMeas = 0;
    MeasStart->setText( tr( "Start" ) );
    MeasStart
      ->setStyleSheet( "background-color: "
		       "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 "
		       "rgba(225, 235, 225, 255), stop:1 "
		       "rgba(255, 255, 255, 255));" );
    MeasPause->setEnabled( false );
    onMeasFinishWorks();
    break;
  }
}
