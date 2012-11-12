#include <QWidget>
#include <QFile>

#include "MainWindow.h"

/* Timer イベントは、動作のシミュレーション以外にも使うけど、とりあえずここに置いとく */
/* 歴史的には、Stars との通信をやっていなかった時期に、Stars のデバイスの */
/* シミュレーションをここでやってたのにいつの間にか、本番でも */
/* 大事な働きをさせられるようになった */

void MainWindow::timerEvent( QTimerEvent *event )
{
  int Id = event->timerId();

  if ( Id == MeasID ) { /* 測定ステップの進行 */
    MeasSequence();
  }
  if ( Id == MoveID ) { /* 指定した軸の移動 */
    MotorMove();
  }
  if ( Id == SPSID ) { /* スキャン/ピークサーチの進行 */
    SPSSequence();
  }
  if ( Id == MonID ) { /* 特定の測定器の値の時間変化の監視 */
    MonSequence();
  }
}

void MainWindow::MotorMove( void )
{
  AUnit *am = AMotors.value( MotorN->currentIndex() );
  
  if ( !am->getIsBusy() ) {
    GoMStop0();
    NewLogMsg( tr( "Setup: %1 : Reached at %2\n" )
	       .arg( am->getName() ).arg( am->value() ) );
    statusbar->showMessage( tr( "Setup: %1 : Reached at %2\n" )
			    .arg( am->getName() ).arg( am->value() ), 1000 );
  }
}

void MainWindow::ClearSensorStages( void )
{
  for ( int i = 0; i < MCHANNELS; i++ ) {
    if ( MeasSensF[i] )
      MeasSens[i]->InitLocalStage();
  }
}

bool MainWindow::InitSensors( void )
{
  bool ff = false;

  for ( int i = 0; i < MCHANNELS; i++ ) {
    if ( MeasSensF[i] ) {
      ff |= MeasSens[i]->InitSensor();
    }
  }

  return ff;
}

bool MainWindow::isBusyMotorInMeas( void )
{
  return MMainTh->getIsBusy() || MMainTh->getIsBusy2();
}

bool MainWindow::isBusySensors( void )
{
  bool ff = false;

  for ( int i = 0; i < MCHANNELS; i++ ) {
    if ( MeasSensF[i] ) {
      ff |= MeasSens[i]->getIsBusy() || MeasSens[i]->getIsBusy2();
    }
  }

  return ff;
}

void MainWindow::SetDwellTime( double dtime )  // これもホントは返答を待つ形にするべき
{
  for ( int i = 0; i < MCHANNELS; i++ ) {
    if ( MeasSensF[i] )
      MeasSens[i]->SetTime( dtime );
  }
}

bool MainWindow::GetSensValues0( void )
{
  return TheCounter->GetValue0();
}

bool MainWindow::GetSensValues( void )
{
  bool ff = false;

  for ( int i = 0; i < MCHANNELS; i++ ) {
    if ( MeasSensF[i] ) {
      ff |= MeasSens[i]->GetValue();
      // qDebug() << "getSensVals" << i << MeasSensF[i] << ff << MeasSens[i]->GetValue();
    }
  }
  
  return ff;
}

void MainWindow::ReadSensValues( void )
{
  MeasVals[ MC_I0 ] = MeasSens[ MC_I0 ]->value().toDouble();
  for ( int i = 1; i < MCHANNELS; i++ ) {
    if ( MeasSensF[i] ) {
      MeasVals[i] = MeasSens[i]->value().toDouble();
    }
  }
}

void MainWindow::DispMeasDatas( void )
{
  double I0;
  double Val;
  int LineCount = 1;

  I0 = MeasVals[ MC_I0 ];
  NowView->NewPoint( 0, GoToKeV, I0 );
  for ( int i = 1; i < MCHANNELS; i++ ) {
    if ( MeasSensF[i] ) {
      Val = MeasVals[i];
      if ( MeasDispMode[i] == TRANS ) {
	if ( Val < 1e-10 )
	  Val = 1e-10;
	if ( ( I0 / Val ) > 0 )
	  NowView->NewPoint( LineCount, GoToKeV, log( I0/Val ) );
	else 
	  NowView->NewPoint( LineCount, GoToKeV, 0 );
      } else {  // MeasDispMode == FLUO
	if ( I0 < 1e-20 )
	  I0 = 1e-20;
	NowView->NewPoint( LineCount, GoToKeV, Val/I0 );
      }
      LineCount++;
    }
  }
}

void MainWindow::MeasSequence( void )
{
  double Delta;
  bool a1, a2;

  if ( AskingOverwrite )
    return;

  if ( ( a1 = isBusyMotorInMeas() ) || ( a2 = isBusySensors() ) ) {
    return;
  }

  switch( MeasStage ) {
    /* 
       0: 測定開始 Repeat = 0
       1: Block = 0
       2: Step = 0, setDwellTIme
       3: Goto a Position with a Block and a Step
       4: prepare to triger Sensors (only for cnt08)
       5: Triger Sensors (for all)
       6: Read out Sensors
       10: Draw (Resume point from 99:)
          Step++; if ( Step < MaxStep ) goto 3
          Block++; if ( Block < MaxBlock ) goto 2
          Repeat++; if ( Repeat < MaxRepeat ) toto 1
          when reach here, finish.
       99: pause の時用のステージ
    */
  case 0:
    ClearSensorStages();
    MeasStage = 1;
  case 1:
    NowView->SetWindow( SBlockStart[0], 0, SBlockStart[ SBlocks ], 0 );
    statusbar->showMessage( tr( "Start Measurement!" ) );
    MeasR = 0;    // Measurement Repeat count
    if ( InitSensors() == false )   // true :: initializing
      MeasStage = 2;
    break;
  case 2:
    MeasB = 0;    // Measurement Block count
    statusbar->showMessage( tr( "Writing Header." ), 2000 );
    WriteHeader( MeasR );
    MeasStage = 3;
    // break;       MeasStage == 1 の動作はレスポンスを待つ必要なし
  case 3: 
    MeasS = 0;    // Measurement Step count in each block
    ClearSensorStages();
    SetDwellTime( NowDwell = SBlockDwell[0] );
    MeasStage = 4;
    // break;       MeasStage == 2 もレスポンスを待つ必要なし
    //              (ここで操作したのはセンサーで, Stage == 3 でセンサーを操作しないから)
  case 4:
    Delta = keV2any( SBLKUnit, SBlockStart[MeasB+1] )
      - keV2any( SBLKUnit, SBlockStart[MeasB] );
    GoToKeV = any2keV( SBLKUnit, Delta / SBlockPoints[MeasB] * MeasS
		       + keV2any( SBLKUnit, SBlockStart[MeasB] ) );
    MoveCurThPosKeV( GoToKeV );     // 軸の移動
    ClearSensorStages();
    if ( OneOfTheSensorIsCounter )
      MeasStage = 5;
    else
      MeasStage = 6;
    break;
  case 5:
    if ( GetSensValues0() == false ) { // only for counters
      ClearSensorStages();
      MeasStage = 6;
    }
    break;
  case 6:
    if ( GetSensValues() == false ) {  // true :: Getting
      ClearSensorStages();
      MeasStage = 7;
    }
    break;
  case 7:
    ReadSensValues();
    DispMeasDatas();
    RecordData();
    MeasStage = 10;
    if ( inPause == 1 ) {
      MeasStage = 99;          // PauseStage
    }
    // don't break
  case 10:                     // This label is resume point from pausing
    NowView->ReDraw();
    MeasS++;
    if ( inPause == 0 ) {
      if ( MeasS < SBlockPoints[ MeasB ] ) {
	MeasStage = 4;
      } else if ( MeasB < SBlocks-1 ) {
	MeasB++;
	MeasStage = 3;
      } else if ( MeasR < SelRPT->value()-1 ) {
	NewLogMsg( QString( tr( "Meas: Repeat %1\n" ) ).arg( MeasR + 1 ) );
	ClearNowView();
	MeasR++;
	MeasStage = 2;
      } else {               // 終了
	statusbar->showMessage( tr( "The Measurement has Finished" ), 4000 );
	NewLogMsg( QString( tr( "Meas: Finished\n" ) ) );
	killTimer( MeasID );
	inMeas = 0;
	MeasStart->setText( tr( "Start" ) );
	MeasStart->setStyleSheet( "" );
	MeasPause->setEnabled( false );
	if ( OnFinishP->currentIndex() == (int)RETURN ) {
	  MoveCurThPosKeV( InitialKeV );
	}
      }
    }
    break;
  case 99:
    if ( inPause == 0 )
      MeasStage = 10;
    break;
  }
}

void MainWindow::MonSequence( void )
{
  if ( isBusySensors() ) {
    return;
  }

  MonMeasTime = 0.01;   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  switch( MonStage ) {
    /* 
       0: 値の読み出しと表示
       1: nct08 を使う時: 計測開始の前準備
          それ以外: 計測開始
       2: nct08 を使う時: 計測開始
    */
  case 0:
    ClearSensorStages();
    MonStage = 1;
    break;
  case 1:
    if ( InitSensors() == false ) {  // true :: initializing
      ClearSensorStages();
      MonStage = 2;
    }
    break;
  case 2: 
    ClearSensorStages();
    SetDwellTime( MonMeasTime );
    MonStage = 3;
    break;
  case 3:
    if ( OneOfTheSensorIsCounter ) {
      if ( GetSensValues0() == false ) { // only for counters
	ClearSensorStages();
	MonStage = 4;
      }
    } else {
      if ( GetSensValues() == false ) {  // true :: Getting
	ClearSensorStages();
	MonStage = 5;
      }
    }
    break;
  case 4:
    if ( GetSensValues() == false ) {  // true :: Getting
      ClearSensorStages();
      MonStage = 5;
    }
    break;
  case 5:
    ReadSensValues();
    MonView->NewPointR( MonTime.elapsed(), MeasVals[0], MeasVals[1], MeasVals[2] );
    MonView->ReDraw();
    MonStage = 10;
#if 0
    if ( inPause == 1 ) {
      MonStage = 99;          // PauseStage
    }
#endif
    // don't break
  case 10:                     // This label is resume point from pausing
    MonView->ReDraw();
    if ( inPause == 0 ) {
      MonStage = 3;
    }
    break;
  }
}

void MainWindow::SPSSequence( void )
{
#if 0
  MCurPos
    ->setText( sks->GetValue( Motors[ MovingM ].devName ) );

  switch( ScanStage ) {
    /* 
       0: 測定開始
       1: 測定の開始点に移動
       2: 目的の角度になったら計測
       3: 計測が終了したら次の角度に移動
       --- 2 と 3 を繰り返す ---
       4: 最終ブロックの最終点まで来たらリピート処理(1へ戻る)
       リピートも終わったら後始末をして終了
    */
  case 0:
    if ( ScanEP > ScanSP ) {
      SPSView->SetWindow( ScanSP, 0, ScanEP, 0 );
    } else {
      SPSView->SetWindow( ScanEP, 0, ScanSP, 0 );
    }
    statusbar->showMessage( tr( "Scan/Search Start!" ), 1000 );
    ScanStage = 1;
    break;
  case 1:
    NowScanP = ScanSP;
    statusbar->showMessage( tr( "Going to initial position." ), 1000 );
    sks->SetValue( Motors[ MovingM ].devName, ScanSP );
    ScanStage = 2;
    break;
  case 2:
    if ( sks->IsBusy( Motors[ MovingM ].devName ) == 0 ) {  // 目的の角度になったら
      // 今の戻り値は Busy=1, Ready=0, Error=-1
      // なので Error の時は状態遷移が進まなくなる
      sks->StartMeas( MDevs[ SPSMon ].devName, ScanDW, MDevs[ SPSMon ].devCh );
      MeasureICH( 0, ScanDW );
      ScanStage = 3;
    }
    break;
  case 3:
    if ( sks->IsBusy( MDevs[ SPSMon ].devName, MDevs[ SPSMon ].devCh ) == 0 ) { 
      ReadOutScanData( NowScanP );                        // 面倒なので今はまだ
      SPSView->ReDraw();
      NowScanP += ScanSTP;
      if ( ( ( ScanSTP > 0 )&&( NowScanP > ScanEP ) )
	||( ( ScanSTP < 0 )&&( NowScanP < ScanEP ) ) ) {
	ScanStage = 4;
      } else {
	sks->SetValue( Motors[ MovingM ].devName, NowScanP );
	ScanStage = 2;
      }
    }
    break;
  case 4:
    statusbar->showMessage( tr( "The Scan/Search has Finished" ), 4000 );
    NewLogMsg( QString( tr( "SoPS: Finished\n" ) ) );
    if ( SorPS->currentIndex() == PEAKSEARCH ) {
      int p;
      sks->SetSpeed( Motors[ MovingM ].devName, MSpeeds[ MovingS ].MSid );
      sks->SetValue( Motors[ MovingM ].devName, p = SPSView->PeakSearch( 1 ) );
      statusbar->showMessage( QString( tr( "Moving to %1." ) ).arg( p ), 4000 );
      NewLogMsg( QString( tr( "Moving Motor[%1] to %2.\n" ) )
		 .arg( Motors[ MovingM ].devName ).arg( p ) );
      ScanStage = 5;  // モータがピーク位置にたどり着くのを待って終了
    } else {
      SPSStop0();
    }
    break;
  case 5:
    if ( sks->IsBusy( Motors[ MovingM ].devName ) == 0 ) { 
      ScanStage = 6;  // ここで止めてもいいけど、モータの現在位置表示を確実にするために
                      // あと1インターバルまわす。
    }
    break;
  case 6:
    SPSStop0();
    break;
  }
#endif
}
