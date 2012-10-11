#include <QWidget>
#include <QFile>

#include "MainWindow.h"

/* Timer イベントは、動作のシミュレーション以外にも使うけど、とりあえずここに置いとく */

void MainWindow::timerEvent( QTimerEvent *event )
{
  int Id = event->timerId();

#if 0
  if ( Id == WatchCurPosID ) { /* 定常的な角度監視 */ 
                               /* 本当に監視しつづけるのが良いかどうかはまた別 */
    WatchPos();
  }
  if ( Id == MoveID ) { /* 軸移動中の軸の値の監視 */
    MotorMove();
  }
#endif

  if ( Id == MeasID ) { /* 測定ステップの進行 */
    MeasSequence();
  }
  if ( Id == SPSID ) { /* スキャン/ピークサーチの進行 */
    SPSSequence();
  }
  if ( Id == MonID ) { /* 特定の測定器の値の時間変化の監視 */
    MonSequence();
  }
}

#if 0     // 現在全く不要のはず
void MainWindow::WatchPos( void )
{
  QString buf;

  double newv;
  
  if ( inMove == 1 ) {
    if ( isFinishedCurMove() ) {
      inMove = 0;
      if ( inMeas == 0 )
	NewLogMsg( QString( tr( "Mono: Reach %1 keV\n" ) ).arg( CurPosKeV ) );
    }
  }
  
  if ( inMeas == 1 ) {
    if ( inPause ) {
      buf = QString( tr( "Pausing ! at Rpt [%1] Blk [%2] Stp[%3]" ) )
	.arg( MeasR+1 ).arg( MeasB+1 ).arg( MeasS+1 );
    } else {
      buf = QString( tr( "Now: Rpt [%1] Blk [%2] Stp[%3]" ) )
	.arg( MeasR+1 ).arg( MeasB+1 ).arg( MeasS+1 );
    }
    statusbar->showMessage( buf );
  }
  return;
}
#endif

#if 0    // 現在全く不要のはず
void MainWindow::MotorMove( void )
{
  char *now = NULL;
  
  if ( sks->IsBusy( Motors[ MovingM ].devName ) == 0 ) {
    GoMStop0( now );
    NewLogMsg( QString( tr( "Setup: %1 : Reached at %2\n" ) )
	       .arg( Motors[ MotorN->currentIndex() + 1 ].MName )
	       .arg( now ) );
    statusbar->showMessage( QString( tr( "Setup: %1 : Reached at %2\n" ) )
			    .arg( Motors[ MotorN->currentIndex() + 1 ].MName )
			    .arg( now ), 1000 );
  } else {
    MCurPos
      ->setText( sks->GetValue( Motors[ MovingM ].devName ) );
  }
}
#endif

void MainWindow::ClearSensorStages( void )
{
  for ( int i = 0; i < 5; i++ ) {
    if ( MeasSensF[i] )
      MeasSens[i]->InitLocalStage();
  }
}

bool MainWindow::InitSensors( void )
{
  bool ff = false;

  for ( int i = 0; i < 5; i++ ) {
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

bool MainWindow::isBusySensorInMeas( void )
{
  bool ff = false;

  for ( int i = 0; i < 5; i++ ) {
    if ( MeasSensF[i] ) {
      ff |= MeasSens[i]->getIsBusy() | MeasSens[i]->getIsBusy2();
    }
  }

  return ff;
}

void MainWindow::SetDwellTime( double dtime )  // これもホントは返答を待つ形にするべき
{
  for ( int i = 0; i < 5; i++ ) {
    if ( MeasSensF[i] )
      MeasSens[i]->SetTime( dtime );
  }
}

bool MainWindow::GetSensValues0( void )
{
  return MeasSens[ MeasCntNo ]->GetValue0();
}

bool MainWindow::GetSensValues( void )
{
  bool ff = false;

  for ( int i = 0; i < 5; i++ ) {
    if ( MeasSensF[i] ) {
      ff |= MeasSens[i]->GetValue();
    }
  }
  
  return ff;
}

void MainWindow::ReadSensValues( void )          // ダミー関数
{
  double I0, I1, A1, A2, SS;

  I1 = A1 = A2 = SS = 0;

  I0 = MeasSens[0]->value().toDouble();   // I0
  if ( MeasSensF[1] )
    I1 = MeasSens[1]->value().toDouble(); // I1
  if ( MeasSensF[2] )
    SS = MeasSens[2]->value().toDouble();  // SSD
  if ( MeasSensF[3] )
    A1 = MeasSens[3]->value().toDouble(); // Aux1
  if ( MeasSensF[4] )
    A2 = MeasSens[4]->value().toDouble(); // Aux2

  NowView->NewPoint( 0, GoToKeV, I0 );
  if ( MeasSensF[1] ) {
    if ( I1 < 1e-10 )
      I1 = 1e-10;
    if ( ( I0 / I1 ) > 0 )
      NowView->NewPoint( 1, GoToKeV, log( I0/I1 ) );     // I1 ほんとは log とるべき
    else 
      NowView->NewPoint( 1, GoToKeV, 0 );     // I1 ほんとは log とるべき


    SetDFName( MeasR );
    QFile file( DFName.toAscii() );
    if ( file.open( QIODevice::Append | QIODevice::Text ) ) {
      QTextStream out(&file);
      out << GoToKeV << " " << I0 << " " << I1 << " ";
      if ( ( I0 / I1 ) > 0 )
	out << log( I0/I1 ) << endl;
      else 
	out << 0 << endl;
      file.close();
    }
    
  }
#if 0
  if ( MeasSensF2 )
    NowView->NewPoint( 2, GoToKeV, A1 );
  if ( MeasSensF3 )
    NowView->NewPoint( 3, GoToKeV, A2 );
  if ( MeasSensF4 )
    NowView->NewPoint( 4, GoToKeV, S );
#endif

}

void MainWindow::MeasSequence( void )
{
  double Delta;

  if ( isBusyMotorInMeas() || isBusySensorInMeas() )
    return;

  switch( MeasStage ) {
    /* 
       0: 測定開始 Repeat = 0
       1: Block = 0
       2: Step = 0, setDwellTIme
       3: Goto a Position with a Block and a Step
       4: Triger Sensors
       5: Read out Sensors
          Step++; if ( Step < MaxStep ) goto 3
          Block++; if ( Block < MaxBlock ) goto 2
          Repeat++; if ( Repeat < MaxRepeat ) toto 1
          when reach here, finish.
       6: pause の時用のステージ
    */
  case 0:
    NowView->SetWindow( SBlockStart[0], 0, SBlockStart[ SBlocks ], 0 );
    statusbar->showMessage( tr( "Start Measurement!" ) );
    MeasR = 0;    // Measurement Repeat count
    ClearSensorStages();
    if ( InitSensors() == false )   // true :: initializing
      MeasStage = 1;
    break;
  case 1:
    MeasB = 0;    // Measurement Block count
    statusbar->showMessage( tr( "Writing Header." ), 2000 );
    WriteHeader( MeasR );
    MeasStage = 2;
    // break;       MeasStage == 1 の内容にもレスポンスを待つ必要なし
  case 2: 
    MeasS = 0;    // Measurement Step count in each block
    SetDwellTime( SBlockDwell[0] );
    MeasStage = 3;
    break;
  case 3:
    Delta = keV2any( SBLKUnit, SBlockStart[MeasB+1] )
      - keV2any( SBLKUnit, SBlockStart[MeasB] );
    GoToKeV = any2keV( SBLKUnit, Delta / SBlockPoints[MeasB] * MeasS
		       + keV2any( SBLKUnit, SBlockStart[MeasB] ) );
    MoveCurThPosKeV( GoToKeV );     // 軸の移動
    ClearSensorStages();
    if ( MeasCntIs )
      MeasStage = 4;
    else
      MeasStage = 5;
    break;
  case 4:
    if ( GetSensValues0() == false ) {  // true :: Getting
      ClearSensorStages();
      MeasStage = 5;
    }
    break;
  case 5:
    if ( GetSensValues() == false ) { // only for counters
      ClearSensorStages();
      MeasStage = 6;
    }
  case 6:
    ReadSensValues();
    MeasStage = 10;
    if ( inPause == 1 ) {
      MeasStage = 99;          // PauseStage
    }
    // don't break
  case 10:
    NowView->ReDraw();
    MeasS++;
    if ( inPause == 0 ) {
      if ( MeasS < SBlockPoints[ MeasB ] ) {
	MeasStage = 3;
      } else if ( MeasB < SBlocks-1 ) {
	MeasB++;
	MeasStage = 2;
      } else if ( MeasR < SelRPT->value()-1 ) {
	NewLogMsg( QString( tr( "Meas: Repeat %1\n" ) ).arg( MeasR + 1 ) );
	ClearNowView();
	MeasR++;
	MeasStage = 1;
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

void MainWindow::MonSequence( void )
{
#if 0
  switch( MonStage ) {
    /* 
       0: 測定開始
       1: 計測結果取得
       このサイクルに自動的な終了は無い (明示的にストップの指示があるまで止まらない)
    */
  case 0:
    if ( MonUsedOldV == 0 ) { // 前回測定が間に合ってなかったら次の測定はスタートしない
      sks->StartMeas( MDevs[ MonDev ].devName, MonMeasTime, MDevs[ MonDev ].devCh2 );
    } else {
      MonUsedOldV = 0;
    }
    if ( MonUsedOldV0 == 0 ) { // 前回測定が間に合ってなかったら次の測定はスタートしない
      sks->StartMeas( MDevs[ IONCH0 ].devName, MonMeasTime, MDevs[ IONCH0 ].devCh2 );
    } else {
      MonUsedOldV0 = 0;
    }
    MonStage = 1;
    break;
  case 1:
    if ( sks->IsBusy( MDevs[ MonDev ].devName, MDevs[ MonDev ].devCh2 ) == 1 ) {
      NewMonV = OldMonV;   // 万測定が間に合ってなかったら, 古い測定結果を流用
      MonUsedOldV = 1;
    } else {
      if ( strcmp( MDevs[ MonDev ].devName, "SSD" ) == 0 ) {
	QString rv = sks->GetValue( MDevs[ MonDev ].devName );
	QStringList rvs = rv.simplified().split( QChar( ' ' ) );
	if ( MDevs[ MonDev ].MDid == SSDALL ) {
	  NewMonV = 0;
	  for ( int i = 0; i < 19; i++ ) {
	    NewMonV += rvs[ i ].toDouble();
	  }
	} else {
	  NewMonV = rvs[ MDevs[ MonDev ].devCh ].toDouble();
	}
      } else {
	NewMonV = atof( sks->GetValue( MDevs[ MonDev ].devName,
				       MDevs[ MonDev ].devCh2 ) );
      }
    }

    if ( sks->IsBusy( MDevs[ IONCH0 ].devName, MDevs[ IONCH0 ].devCh2 ) == 1 ) {
      NewMonV0 = OldMonV0;   // 万測定が間に合ってなかったら, 古い測定結果を流用
      MonUsedOldV0 = 1;
    } else {
	NewMonV0 = atof( sks->GetValue( MDevs[ IONCH0 ].devName,
				       MDevs[ IONCH0 ].devCh2 ) );
    }

    MonView->NewPointR( NewMonV0, NewMonV );
    MonView->ReDraw();

    OldMonV = NewMonV;
    OldMonV0 = NewMonV0;
    ScanStage = 0;
    break;
  }
#endif
}

