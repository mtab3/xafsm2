
#include "MainWindow.h"

// QXafs 関連のイニシャライズ setupMwMeas より前に呼ばれてる
void MainWindow::setupQXafsMode( void )
{
  connect( QMaxSpeed, SIGNAL( toggled( bool ) ), this, SLOT( CheckQXafsParams() ) );
  connect( QMinTime, SIGNAL( toggled( bool ) ), this, SLOT( CheckQXafsParams() ) );
  connect( SelRPT, SIGNAL( valueChanged( int ) ), this, SLOT( CheckQXafsParams() ) );
  connect( QLimitedDisplay, SIGNAL( toggled( bool ) ), this, SLOT( SetNewRPTLimit() ) );
  connect( QIntervalAtStart, SIGNAL( editingFinished() ),
	   this, SLOT( CheckQXafsParams() ) );
  connect( QIntervalAtEnd, SIGNAL( editingFinished() ),
	   this, SLOT( CheckQXafsParams() ) );

  OrigHSpeed = HSpeed = 6000;    // 6000 pps, 0.1arcsec/pulse = 2.777..x10-5 deg/pulse
  QConditionBox->setHidden( true );
  QIntervalTimer = new QTimer;
  QIntervalTimer->setSingleShot( true );
  connect( QIntervalTimer, SIGNAL( timeout() ), this, SLOT( QIntervalTimeout() ) );
#if 0
  QMeasOnBackward->setHidden( true );
  QMinMaxBox->setHidden( true );
  QMaxSpeed->setHidden( true );
  QMinTime->setHidden( true );
  QSepLine->setHidden( true );
  QLimitedDisplay->setHidden( true );
#endif
}

void MainWindow::QIntervalTimeout( void )
{
  QIntervalTimer->stop();
  QIntervalBlock = false;
}

void MainWindow::SetNewRPTLimit( void )
{
  if ( QXafsMode->isChecked() ) {
    if ( QLimitedDisplay->isChecked() ) {
      SelRPT->setMaximum( 999999 ); // 別に unlimit でもいい。事実上の unlimit のつもり。
    } else {
      if ( QMeasOnBackward->isChecked() ) {
	SelRPT->setMaximum( 4999 );
      } else {
	SelRPT->setMaximum( 9999 );
      }
    }
  } else {
    SelRPT->setMaximum( 99 );
  }
  if ( SelRPT->value() > SelRPT->maximum() ) {
    SelRPT->setValue( SelRPT->maximum() );
  }
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
    QConditionBox->setHidden( false );
#if 0
    QMeasOnBackward->setHidden( false );
    QMinMaxBox->setHidden( false );
    QMaxSpeed->setHidden( false );
    QMinTime->setHidden( false );
    QSepLine->setHidden( false );
    QLimitedDisplay->setHidden( false );
#endif

    SaveUse19ChSSD = Use19chSSD->isChecked();
    SaveUseAux1 = UseAux1->isChecked();
    SaveUseAux2 = UseAux2->isChecked();
    Use19chSSD->setChecked( false );
    UseAux1->setChecked( false );
    UseAux2->setChecked( false );
    Use19chSSD->setEnabled( false );
    UseAux1->setEnabled( false );
    UseAux2->setEnabled( false );

    SetNewRPTLimit();

  } else {

    // NORML XAFS モード

    SelBLKs->setEnabled( true );
    ChangeBLKs( SaveNowBlocks );
    HideBLKs( false );

    QConditionBox->setHidden( true );
    SelectI0->setCurrentIndex( SaveSelectedI0 );
    SelectI1->setCurrentIndex( SaveSelectedI1 );
    Use19chSSD->setChecked( SaveUse19ChSSD );
    UseAux1->setChecked( SaveUseAux1 );
    UseAux2->setChecked( SaveUseAux1 );
    Use19chSSD->setEnabled( true );
    UseAux1->setEnabled( true );
    UseAux2->setEnabled( true );

    SetNewRPTLimit();
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

  DispQHSpeed->setText( QString::number( HSpeed ) );
  double WidthInPuls = fabs( edeg - sdeg ) / MMainTh->getUPP();

  if ( WidthInPuls / BlockPoints[0] / HSpeed < 2e-5 ) {
    // PM16C が出す Trigger は 10us 幅にするので
    // Interval の時間は念の為 20us とる。
    // それよりも短くなるなら、インターバルを変更
    int Interval = 2e-5 * HSpeed;      
    BlockPoints[0] = (int)( WidthInPuls / Interval );
    BLKpoints[0]->setText( QString::number( BlockPoints[0] ) );
  }
  if ( BlockPoints[0] > WidthInPuls ) {
    BlockPoints[0] = WidthInPuls;
    BLKpoints[0]->setText( QString::number( WidthInPuls ) );
    BLKstep[0]->setText( QString::number( fabs( edeg - sdeg ) / WidthInPuls ) );
  }
  dtime = WidthInPuls / HSpeed;
  BlockDwell[0] = dtime;

  BLKdwell[0]->setText( QString::number( BlockDwell[0] ) );
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
  // TT は一回のスキャンだけの時間。
  // TTT はインターバルを入れて、繰り返し回数をかけた時間
  if ( dtime > 0 ) {
    if ( QMeasOnBackward->isChecked() ) {
      TT = ( dtime + RunUpTime * 2 ) * 2 + 3;      // +1 は実測の補正値
    } else {
      TT = dtime + RunUpTime * 2 + ( RunUpPulses + WidthInPuls ) / MaxHSpeed + 3;
    }
    TTT = TT * SelRPT->value()
      + QIntervalAtStart->text().toDouble() * ( SelRPT->value() - 1 )
      + QIntervalAtEnd->text().toDouble() * SelRPT->value();
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
  int points = abs( SBlockPoints[0] );
  double dtime = SBlockDwell[0];

  double WidthInPuls = fabs( edeg - sdeg ) / MMainTh->getUPP();
  HSpeed = WidthInPuls / dtime;
  if (( HSpeed > MaxHSpeed )||( HSpeed < 0 )) {
    HSpeed = MaxHSpeed;                           // PM16C に設定する H のスピード
  }
  DispQHSpeed->setText( QString::number( HSpeed ) );

  QXafsSP0 = sdeg / MMainTh->getUPP() + MMainTh->getCenter();  // 測定範囲の始点
  QXafsEP0 = edeg / MMainTh->getUPP() + MMainTh->getCenter();  // 測定範囲の終点
  if ( abs( QXafsSP0 - QXafsEP0 ) > points )
    QXafsInterval = (int)(abs( QXafsSP0 - QXafsEP0 ) / points);
  // Trigger パルスを出す間隔
  else 
    QXafsInterval = 1;

  if ( (double)QXafsInterval / HSpeed < 2e-5 ) {
    // PM16C が出す Trigger は 10us 幅にするので
    // Interval の時間は念の為 20us とる。
    QXafsInterval = 2e-5 * HSpeed;         // これより短くなるなら、インターバルを変更
    SBlockPoints[0] = (int)(abs( QXafsSP0 - QXafsEP0 ) / QXafsInterval);
    statusbar
      ->showMessage( tr( "Selected Points were too many!  It was changed to be %1" )
		     .arg( SBlockPoints[0] ), 3000 );
  }

  QXafsPoints = abs( QXafsSP0 - QXafsEP0 ) / QXafsInterval;    // 測定ステップ数を再計算
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
    .arg( QXafsSP ).arg( QXafsSP0 ).arg( QXafsEP0 ).arg( QXafsEP ).arg( RunUpRate )
	   << QString( "Interval and Points %1 %2" )
    .arg( QXafsInterval ).arg( QXafsPoints );

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
  QDateTime DebugTime1, DebugTime2;
  QString DebugBuf;

  if ( QIntervalBlock )
    return;

  switch( MeasStage ) {
  case 0:
    GetPM16CParamsForQXAFS();
    MakeDelegateFile();
    MeasView->SetRLine( 0 );            // まず、0 番目のラインを右軸に表示
    MeasView->SetLLine( 2 );            //       2 番目のラインを左軸に表示
    if ( AutoModeButton->isChecked() ) {
      CurrentRpt->setText( QString( "%1 - %2" ).arg( 1 ).arg( MeasA+1 ) );
    } else {
      CurrentRpt->setText( QString::number( 1 ) );
    }
    //    WriteInfoFile();
    mUnits.clearStage();
    MeasView->SetWindow0( SBlockStart[0], 0, SBlockStart[ SBlocks ], 0 );
    statusbar->showMessage( tr( "Start QXAFS Measurement!" ) );
    MMainTh->SetHighSpeed( OrigHSpeed );
    MMainTh->SetSpeed( HIGH );
    MeasStage++;
    break;
  case 1:
    EncMainTh->GetValue();
    if ( mUnits.init() ) // D.V. は Reset だけ, ENC2 は GetValue だけ
      break;
    MeasR = 0;    // Measurement Repeat count
    MeasStage++;
    break;
  case 2:
    EncValue0 = EncMainTh->value();
    if ( Enc2 != NULL ) Enc2Value0 = Enc2->value();
    mUnits.setDwellTimes( QXafsDwellTime );  
    mUnits.setDwellTime();
    mUnits.clearStage();
    MeasStage++;
    break;
  case 3:
    MMainTh->SetValue( QXafsSP );   // 助走距離を含めたスタート地点へ
    MeasStage = 5;    // 最初は始点でのインターバル指定があってもインターバルを取らない
    break;
  case 4:      // Repeat Point 1
    QIntervalBlock = true;
    QIntervalTimer->start( QIntervalAtStart->text().toDouble() * 1000 );
    break;
  case 5:      // Repeat Point 2
    DebugTime1 = QDateTime::currentDateTime();    // debug
    if ( mUnits.QStart() )
      break;
    mUnits.clearStage();
    MeasR++;
    if ( MeasR > SelRPT->value() ) { // 規定回数回り終わってれば終了処理に入る!!
      MeasStage = 99;
      break;
    }
    WriteQHeader( MeasR, FORWARD );
    if ( AutoModeButton->isChecked() ) {
      CurrentRpt->setText( QString( "%1 - %2" ).arg( MeasR + 1 ).arg( MeasA+1 ) );
    } else {
      CurrentRpt->setText( QString::number( MeasR ) );
    }
    CurrentPnt->setText( tr( "Fwd" ) );
    MMainTh->SetHighSpeed( HSpeed );
    MeasStage++;
    //break;
  case 6:
    SetUpMainThToGenerageTriggerSignal( QXafsSP0, QXafsEP0 );
    MeasStage++;
    break;
  case 7:
    DebugTime2 = QDateTime::currentDateTime();    // debug
    MMainTh->SetValue( QXafsEP );   // 減速距離を含めた終了地点へ

    DebugBuf = "Interval to return at start point: "
      + DebugTime1.toString("yy.MM.dd hh:mm.zzz") + " "
      + DebugTime2.toString("yy.MM.dd hh:mm.zzz");
    NewLogMsg( DebugBuf );

    mUnits.clearDoneF();      // QRead を一台ずつ行うためのしかけ // 現状不要のはず。
    MeasStage++;
    break;
  case 8:
    DebugTime1 = QDateTime::currentDateTime();    // debug
    if ( mUnits.QRead() )
      break;
    MeasStage++;
    mUnits.clearStage();
    break;
  case 9:
#if 0
    qDebug() << "aa" << mUnits.count();
    qDebug() << mUnits.at(0)->values()[0] << mUnits.at(0)->values().count()
	     << mUnits.at(1)->values()[0] << mUnits.at(1)->values().count()
	     << mUnits.at(2)->values()[0] << mUnits.at(2)->values().count();
#endif
    WriteQHeader2( MeasR, FORWARD );
    WriteQBody();
    g = ( QMeasOnBackward->isChecked() ) ? ( ( MeasR - 1 ) * 2 ) : ( MeasR - 1 );
    DispQSpectrum( g );
    MeasStage++;
    break;
  case 10:
    if ( mUnits.QEnd() )
      break;
    mUnits.clearStage();
    if ( QMeasOnBackward->isChecked() ) {   // 戻りも測定する
      SetUpMainThToGenerageTriggerSignal( QXafsEP0, QXafsSP0 );
      CurrentPnt->setText( tr( "Bwd" ) );
      if ( QIntervalAtEnd->text().toDouble() > 0 )
	MeasStage = 11;      // 終了点での Interval時間指定があった場合
      else
	MeasStage = 12;      // 終了点での Interval時間指定が無かった場合
    } else {
      MMainTh->SetHighSpeed( MaxHSpeed );
      MMainTh->SetTimingOutMode( 0 );
      MMainTh->SetTimingOutReady( 0 );
      MeasStage = 13;
    }
    break;
  case 11:
    QIntervalBlock = true;
    QIntervalTimer->start( QIntervalAtEnd->text().toDouble() * 1000 );
    break;
  case 12:
    if ( mUnits.QStart() )
      break;
    MeasStage++;
    break;
  case 13:
    DebugTime2 = QDateTime::currentDateTime();    // debug
    MMainTh->SetValue( QXafsSP );   // 助走距離を含めたスタート地点へ

    DebugBuf =  "Interval to return at end point: "
      + DebugTime1.toString("yy.MM.dd hh:mm.zzz") + " "
      + DebugTime2.toString("yy.MM.dd hh:mm.zzz");
    NewLogMsg( DebugBuf );

    if ( QMeasOnBackward->isChecked() ) {   // 戻りも測定する
      WriteQHeader( MeasR, BACKWARD );
      MeasStage++;
    } else {
      if ( QIntervalAtStart->text().toDouble() > 0 )
	MeasStage = 4; // 始点でのインターバル指定がある場合 : Repeat Point 1
      else
	MeasStage = 5; // 始点でのインターバル指定がない場合 : Repeat Point 2
    }
    mUnits.clearDoneF();      // QRead を一台ずつ行うため  // 現状不要のはず
    break;
  case 14:
    if ( mUnits.QRead() )
      break;
    mUnits.clearStage();
    MeasStage++;
    break;
  case 15:
#if 0
    qDebug() << mUnits.at(0)->values()[0] << mUnits.at(0)->values().count()
	     << mUnits.at(1)->values()[0] << mUnits.at(1)->values().count()
	     << mUnits.at(2)->values()[0] << mUnits.at(2)->values().count();
#endif
    WriteQHeader2( MeasR, BACKWARD );
    WriteQBody();
    g = ( MeasR - 1 ) * 2 + 1;
    DispQSpectrum( g );
    MeasStage++;
    break;
  case 16:
    if ( mUnits.QEnd() )
      break;
    mUnits.clearStage();
    if ( QIntervalAtStart->text().toDouble() > 0 )
      MeasStage = 4; // 始点でのインターバル指定がある場合 : Repeat Point 1
    else
      MeasStage = 5; // 始点でのインターバル指定がない場合 : Repeat Point 2
    break;

  case 99:
    PlayEndingSound();
    QXafsFinish();
    break;
  }
  qDebug() << "out " << MeasStage;
}

void MainWindow::QXafsFinish0( void )
{
  mUnits.setOneByOne( false );           // 「ユニット一つずつ順番に」モードやめる
  MMainTh->SetHighSpeed( OrigHSpeed );   // H のスピードを標準に戻す
  MMainTh->SetSpeed( GoMSpeed );         // 選択されていたスピードに戻す
}

void MainWindow::QXafsFinish( void )
{
  statusbar->showMessage( tr( "The Measurement has Finished" ), 4000 );
  NewLogMsg( QString( tr( "Meas: QXafs Finished" ) ) );
  MeasTimer->stop();
  inMeas = 0;
  MeasStart->setText( tr( "Start" ) );
  MeasStart->setStyleSheet( NormalB );
  MeasPause->setEnabled( false );
  QXafsFinish0();
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

  double dark0 = mUnits.at(0)->getDark() * QXafsDwellTime;
  double dark1 = mUnits.at(1)->getDark() * QXafsDwellTime;

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

  if ( QLimitedDisplay->isChecked() ) {
    g = g % QLastLines->value();
  }

  MeasView->SetLR( g*3, RIGHT_AX );                    // I0 
  MeasView->SetScaleType( g*3, I0TYPE );
  MeasView->SetLineName( g*3, "I0" );
  MeasView->SetDispF( g*3, true );
  MeasView->SetPoints( g*3, 0 );
  
  MeasView->SetLR( g*3+1, LEFT_AX );                   // I1
  MeasView->SetScaleType( g*3+1, FULLSCALE );
  MeasView->SetLineName( g*3+1, "I1" );
  MeasView->SetDispF( g*3+1, false );
  MeasView->SetPoints( g*3+1, 0 );

  MeasView->SetLR( g*3+2, LEFT_AX );                   // mu
  MeasView->SetScaleType( g*3+2, FULLSCALE );
  MeasView->SetLineName( g*3+2, tr( "mu" ) );
  MeasView->SetDispF( g*3+2, true );
  MeasView->SetPoints( g*3+2, 0 );

  for ( int i = 0; i < num; i++ ) {
    if ( Enc2 == NULL ) {
      deg2 = ( p - c ) * upp + d * i;
    } else {
      deg2 = EncValue0.toDouble() + ( vals2[i+1].toInt() - Enc2Value0.toInt() ) * upp2;
    }

    E = u->deg2keV( deg2 );
    I0 = vals0[i+1].toDouble() - dark0;
    I1 = vals1[i+1].toDouble() - dark1;
    MeasView->NewPoint( g*3 + 0, E, I0 );
    MeasView->NewPoint( g*3 + 1, E, I1 );
    if ( ( I1 != 0 ) && ( ( I0 / I1 ) > 0 ) ) {
      MeasView->NewPoint( g*3 + 2, E, log( I0/I1 ) );
    } else {
      MeasView->NewPoint( g*3 + 2, E, 0 );
    }
  }
  MeasView->update();
}


  
