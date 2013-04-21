
#include "MainWindow.h"

// QXafs 関連のイニシャライズ setupMeMeas より前に呼ばれてる
void MainWindow::setupQXafsMode( void )
{
  connect( QMaxSpeed, SIGNAL( toggled( bool ) ), this, SLOT( CheckQXafsParams() ) );
  connect( QMinTime, SIGNAL( toggled( bool ) ), this, SLOT( CheckQXafsParams() ) );
  connect( SelRPT, SIGNAL( valueChanged( int ) ), this, SLOT( CheckQXafsParams() ) );

  OrigHSpeed = HSpeed = 6000;    // 6000 pps, 0.1arcsec/pulse = 2.777..x10-5 deg/pulse
  QMeasOnBackward->setHidden( true );
  QMinMaxBox->setHidden( true );
  QMaxSpeed->setHidden( true );
  QMinTime->setHidden( true );
  QSepLine->setHidden( true );
}

void MainWindow::ToggleQXafsMode( bool )
{
  if ( QXafsMode->isChecked() ) {

    // QXAFS モード

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
    QMinMaxBox->setHidden( false );
    QMaxSpeed->setHidden( false );
    QMinTime->setHidden( false );
    QSepLine->setHidden( false );

    SaveUse19ChSSD = Use19chSSD->isChecked();
    SaveUseAux1 = UseAux1->isChecked();
    SaveUseAux2 = UseAux2->isChecked();
    Use19chSSD->setChecked( false );
    UseAux1->setChecked( false );
    UseAux2->setChecked( false );
    Use19chSSD->setEnabled( false );
    UseAux1->setEnabled( false );
    UseAux2->setEnabled( false );

    SelRPT->setMaximum( 9999 );
    qDebug() << "rpt max " << SelRPT->maximum();

  } else {

    // NORML XAFS モード

    SelBLKs->setEnabled( true );
    ChangeBLKs( SaveNowBlocks );
    HideBLKs( false );
    QMeasOnBackward->setHidden( true );
    QMinMaxBox->setHidden( true );
    QMaxSpeed->setHidden( true );
    QMinTime->setHidden( true );
    QSepLine->setHidden( true );

    SelectI0->setCurrentIndex( SaveSelectedI0 );
    SelectI1->setCurrentIndex( SaveSelectedI1 );
    Use19chSSD->setChecked( SaveUse19ChSSD );
    UseAux1->setChecked( SaveUseAux1 );
    UseAux2->setChecked( SaveUseAux1 );
    Use19chSSD->setEnabled( true );
    UseAux1->setEnabled( true );
    UseAux2->setEnabled( true );

    SelRPT->setMaximum( 99 );
    if ( SelRPT->value() > 99 ) {
      SelRPT->setValue( 99 );
    }
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
  double dtime = BLKdwell[0]->text().toDouble();

  HSpeed = fabs( ( edeg - sdeg ) / dtime / MMainTh->getUPP() );
  double MinTimeSpeed = sqrt( fabs( edeg - sdeg ) / MMainTh->getUPP()
			       / ( 2. * RunUpRate / 1000. ) );

  if ( QMinTime->isChecked() ) {
    if (( HSpeed > MinTimeSpeed )||( HSpeed < 0 ))
      HSpeed = MinTimeSpeed;
  }
  if (( HSpeed > MaxHSpeed )||( HSpeed < 0 ))
    HSpeed = MaxHSpeed;

  double WidthInPuls = fabs( edeg - sdeg ) / MMainTh->getUPP();
  dtime = WidthInPuls / HSpeed;
  BlockDwell[0] = dtime;

  QString buf;
  buf.sprintf( "% 5.2f", BlockDwell[0] );
  BLKdwell[0]->setText( buf );
  ShowQTime( dtime, WidthInPuls );
}

void MainWindow::ShowQTime( double dtime, double WidthInPuls )
{
  // HSpeed までの加速にかかる時間
  double RunUpTime = ( HSpeed - LowSpeed ) * RunUpRate / 1000;
  // その時間で走るパルス数
  int RunUpPulses = ( HSpeed - LowSpeed ) * ( HSpeed + LowSpeed ) * RunUpRate / 2000.;

  int Th, Tm, Ts;
  double TT, TTT;
  if ( dtime > 0 ) {
    if ( QMeasOnBackward->isChecked() ) {
      TT = ( dtime + RunUpTime * 2 ) * 2 + 3;      // +1 は実測の補正値
    } else {
      TT = dtime + RunUpTime * 2 + ( RunUpPulses + WidthInPuls ) / MaxHSpeed + 3;
    }
    TTT = TT * SelRPT->value();
  } else {
    TTT = TT = 0;
  }
  EstimatedMeasurementTimeInSec = TTT;

  QString buf;

  Th = (int)( TT / 3600 );
  TT -= Th * 3600;
  Tm = (int)( TT / 60 );
  TT -= Tm * 60;
  Ts = (int)TT;
  TT -= Ts;
  buf.sprintf( "%02d:%02d:%02d.%02d", Th, Tm, Ts, (int)(TT*100) );
  TPoints->setText( tr( "Cycle: " ) + buf );

  Th = (int)( TTT / 3600 );
  TTT -= Th * 3600;
  Tm = (int)( TTT / 60 );
  TTT -= Tm * 60;
   Ts = (int)TTT;
  TTT -= Ts;
  buf.sprintf( "%02d:%02d:%02d.%02d", Th, Tm, Ts, (int)(TT*100) );
  TTime->setText( tr( "Total: " ) + buf );
}

void MainWindow::GetPM16CParamsForQXAFS( void )
// 本当は入力時にも計算できる内容だが、
// 確実を期すため測定直前に "Saved" パラメータを使って計算する
{
  double sdeg = u->keV2deg( SBlockStart[0] );
  double edeg = u->keV2deg( SBlockStart[1] );
  int steps = abs( SBlockPoints[0] );
  double dtime = SBlockDwell[0];

  double WidthInPuls = fabs( edeg - sdeg ) / MMainTh->getUPP();
  HSpeed = WidthInPuls / dtime;
  if (( HSpeed > MaxHSpeed )||( HSpeed < 0 )) {
    HSpeed = MaxHSpeed;                           // PM16C に設定する H のスピード
  }

  QXafsSP0 = sdeg / MMainTh->getUPP() + MMainTh->getCenter();  // 測定範囲の始点
  QXafsEP0 = edeg / MMainTh->getUPP() + MMainTh->getCenter();  // 測定範囲の終点
  if ( abs( QXafsSP0 - QXafsEP0 ) > steps )
    QXafsInterval = (int)(abs( QXafsSP0 - QXafsEP0 ) / steps); // Trigger パルスを出す間隔
  else 
    QXafsInterval = 1;

  if ( (double)QXafsInterval / HSpeed < 2e-5 ) {
    // PM16C が出す Trigger は 10us 幅にするので
    // Interval の時間は念の為 20us とる。
    QXafsInterval = 2e-5 * HSpeed;         // これより短くなるなら、インターバルを変更
    SBlockPoints[0] = (int)(abs( QXafsSP0 - QXafsEP0 ) / QXafsInterval);
    statusbar
      ->showMessage( tr( "Selected Steps were too many!  It was changed to be %1" )
		     .arg( SBlockPoints[0] ), 3000 );
  }

  QXafsSteps = abs( QXafsSP0 - QXafsEP0 ) / QXafsInterval;    // 測定ステップ数を再計算
  QXafsDwellTime = ( (double)QXafsInterval / HSpeed ) * 0.9;
  // 1点の積分時間を Trigger パルス間隔の 90% にする

  RunUpTime = ( HSpeed - LowSpeed ) * RunUpRate / 1000;  // HSpeed までの加速にかかる時間

  int RunUpPulses = ( HSpeed - LowSpeed ) * ( HSpeed + LowSpeed ) * RunUpRate / 2000.;
  // HSpeed までの加速に必要なパルス数
  if ( QXafsSP0 > QXafsEP0 )
    RunUpPulses *= -1;
  qDebug() << "RunUpPulses "
	   << HSpeed << LowSpeed << RunUpRate << RunUpPulses << RunUpTime;

  QXafsSP = QXafsSP0 - RunUpPulses;   // 測定範囲を加速パルス分広げた範囲の
  QXafsEP = QXafsEP0 + RunUpPulses;   // 始点と終点

  qDebug() << QString( "Measure Range [ %1 [ %2 %3 ] %4 ], RunUpRate %5" )
    .arg( QXafsSP ).arg( QXafsSP0 ).arg( QXafsEP0 ).arg( QXafsEP ).arg( RunUpRate );
  qDebug() << QString( "Interval and Steps %1 %2" )
    .arg( QXafsInterval ).arg( QXafsSteps );

  ShowQTime( dtime, WidthInPuls );
}

void MainWindow::SetUpMainThToGenerageTriggerSignal( int sp, int ep )
{
  MMainTh->SetTimingOutMode( 3 );          // 10um puls at every interval
  MMainTh->SetTimingOutStart( sp );
  MMainTh->SetTimingOutEnd( ep );
  MMainTh->SetTimingOutInterval( QXafsInterval );
  MMainTh->SetTimingOutReady( 1 );
}

void MainWindow::QXafsMeasSequence( void )
{
  int g;
  qDebug() << "in " << MeasStage;

  switch( MeasStage ) {
  case 0:
    MeasView->SetRLine( 0 );            // まず、0 番目のラインを右軸に表示
    MeasView->SetLLine( 2 );            //       2 番目のラインを左軸に表示
    CurrentRpt->setText( QString::number( 1 ) );
    //    WriteInfoFile();
    mUnits.clearStage();
    MeasView->SetWindow0( SBlockStart[0], 0, SBlockStart[ SBlocks ], 0 );
    statusbar->showMessage( tr( "Start QXAFS Measurement!" ) );
    GetPM16CParamsForQXAFS();
    MMainTh->SetHighSpeed( OrigHSpeed );
    MMainTh->SetSpeed( HIGH );
    MeasStage++;
    break;
  case 1:
    EncMainTh->GetValue();
    if ( mUnits.init() ) // DV は Reset だけ, ENC2 は GetValue だけ
      break;
    MeasR = 0;    // Measurement Repeat count
    MeasStage++;
    break;
  case 2:
    EncValue0 = EncMainTh->value();
    if ( Enc2 != NULL ) Enc2Value0 = Enc2->value();
    qDebug() << "Enc and Enc2 " << EncValue0 << Enc2Value0;
    mUnits.setDwellTimes( QXafsDwellTime );  
    mUnits.setDwellTime();
    mUnits.clearStage();
    MeasStage++;
    break;
  case 3:
    MMainTh->SetValue( QXafsSP );   // 助走距離を含めたスタート地点へ
    MeasStage++;
    break;
  case 4:      // Repeat Point
    if ( mUnits.QStart() )
      break;
    mUnits.clearStage();
    MeasR++;
    WriteQHeader( MeasR, FORWARD );
    if ( MeasR > SelRPT->value() ) { // 規定回数回り終わってれば終了処理に入る!!
      MeasStage = 99;
      break;
    }
    CurrentRpt->setText( QString::number( MeasR ) );
    CurrentPnt->setText( tr( "Fwd" ) );
    MMainTh->SetHighSpeed( HSpeed );
    MeasStage++;
    //break;
  case 5:
    SetUpMainThToGenerageTriggerSignal( QXafsSP0, QXafsEP0 );
    MeasStage++;
    break;
  case 6:
    MMainTh->SetValue( QXafsEP );   // 減速距離を含めた終了地点へ
    mUnits.clearDoneF();      // QRead を一台ずつ行うため
    MeasStage++;
    break;
  case 7:
    if ( mUnits.QRead() )
      break;
    MeasStage++;
    mUnits.clearStage();
    break;
  case 8:
    qDebug() << mUnits.at(0)->values()[0] << mUnits.at(0)->values().count()
	     << mUnits.at(1)->values()[0] << mUnits.at(1)->values().count()
	     << mUnits.at(2)->values()[0] << mUnits.at(2)->values().count();
    WriteQBody();
    g = ( QMeasOnBackward->isChecked() ) ? ( ( MeasR - 1 ) * 2 ) : ( MeasR - 1 );
    DispQSpectrum( g );
    MeasStage++;
    break;
  case 9:
    if ( mUnits.QEnd() )
      break;
    mUnits.clearStage();
    if ( QMeasOnBackward->isChecked() ) {   // 戻りも測定する
      SetUpMainThToGenerageTriggerSignal( QXafsEP0, QXafsSP0 );
      CurrentPnt->setText( tr( "Bwd" ) );
      MeasStage++;
    } else {
      MMainTh->SetHighSpeed( MaxHSpeed );
      MMainTh->SetTimingOutMode( 0 );
      MMainTh->SetTimingOutReady( 0 );
      MeasStage = 11;
    }
    break;
  case 10:
    if ( mUnits.QStart() )
      break;
    MeasStage++;
    break;
  case 11:
    MMainTh->SetValue( QXafsSP );   // 助走距離を含めたスタート地点へ
    if ( QMeasOnBackward->isChecked() ) {   // 戻りも測定する
      WriteQHeader( MeasR, BACKWARD );
      MeasStage++;
    } else {
      MeasStage = 4; // Repeat Point
    }
    mUnits.clearDoneF();      // QRead を一台ずつ行うため
    break;
  case 12:
    if ( mUnits.QRead() )
      break;
    mUnits.clearStage();
    MeasStage++;
    break;
  case 13:
    qDebug() << mUnits.at(0)->values()[0] << mUnits.at(0)->values().count()
	     << mUnits.at(1)->values()[0] << mUnits.at(1)->values().count()
	     << mUnits.at(2)->values()[0] << mUnits.at(2)->values().count();
    WriteQBody();
    g = ( MeasR - 1 ) * 2 + 1;
    DispQSpectrum( g );
    MeasStage++;
    break;
  case 14:
    if ( mUnits.QEnd() )
      break;
    mUnits.clearStage();
    MeasStage = 4;   // Repeat Point
    break;

  case 99:
    QXafsFinish();
    break;
  }
  qDebug() << "out " << MeasStage;
}

void MainWindow::QXafsFinish( void )
{
  mUnits.setOneByOne( false );           // 「ユニット一つずつ順番に」モードやめる
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
}

void MainWindow::DispQSpectrum( int g )  // ダーク補正どうする？
{
  QStringList vals0 = mUnits.at(0)->values();
  QStringList vals1 = mUnits.at(1)->values();
  QStringList vals2;
  if ( Enc2 != NULL )
    vals2 = Enc2->values();
  else 
    vals2.clear();
  int num = findMini( vals0, vals1, vals2 );

  int p = QXafsSP0;
  int d = QXafsInterval;
  int c = MMainTh->getCenter();
  double upp = MMainTh->getUPP();
  double deg2;
  double upp2 = 0;
  double E, I0, I1;
  if ( Enc2 != NULL ) {
    upp2 = Enc2->getUPP();
  }
  qDebug() << "upp2 " << upp2 << EncValue0.toDouble() << Enc2Value0.toInt();

  MeasView->SetLR( g*3, RIGHT_AX );                        // I0 
  MeasView->SetScaleType( g*3, I0TYPE );
  MeasView->SetLineName( g*3, "I0" );
  
  MeasView->SetLR( g*3+1, LEFT_AX );                   // I1
  MeasView->SetScaleType( g*3+1, FULLSCALE );
  MeasView->SetLineName( g*3+1, "I1" );

  MeasView->SetLR( g*3+2, LEFT_AX );                     // mu
  MeasView->SetScaleType( g*3+2, FULLSCALE );
  MeasView->SetLineName( g*3+2, tr( "mu" ) );

  for ( int i = 0; i < num; i++ ) {
    if ( Enc2 == NULL ) {
      deg2 = ( p - c ) * upp + d * i;
    } else {
      deg2 = EncValue0.toDouble() + ( vals2[i+1].toInt() - Enc2Value0.toInt() ) * upp2;
    }
    E = u->deg2keV( deg2 );

    MeasView->NewPoint( g*3 + 0, E, I0 = vals0[i+1].toDouble() );
    MeasView->NewPoint( g*3 + 1, E, I1 = vals1[i+1].toDouble() );
    if ( ( I1 != 0 ) && ( ( I0 / I1 ) > 0 ) ) {
      MeasView->NewPoint( g*3 + 2, E, log( I0/I1 ) );
    } else {
      MeasView->NewPoint( g*3 + 2, E, 0 );
    }
  }
  MeasView->update();
}


  
