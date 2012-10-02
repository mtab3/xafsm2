#include <QWidget>

#include "MainWindow.h"

/* Timer イベントは、動作のシミュレーション以外にも使うけど、とりあえずここに置いとく */

void MainWindow::timerEvent( QTimerEvent *event )
{
  int Id = event->timerId();

  if ( Id == WatchCurPosID ) { /* 定常的な角度監視 */ 
                               /* 本当に監視しつづけるのが良いかどうかはまた別 */
    WatchPos();
  }
  if ( Id == MoveID ) { /* 軸移動中の軸の値の監視 */
    MotorMove();
  }
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

void MainWindow::WatchPos( void )
{
  QString buf;

  double newv;
  newv = GetCurPosKeV();
  if ( newv != CurPosKeV ) {
    CurPosKeV = newv;
    ShowCurPos();
  }
  
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

void MainWindow::MeasSequence( void )
{
  switch( MeasStage ) {
    /* 
       0: 測定開始
       1: 測定の開始点に移動
       2: 目的の角度になったら計測
       3: 計測が終了したら次の角度に移動
       --- 2 と 3 を繰り返す ---
       4: 最終ブロックの最終点まで来たらリピート処理(1へ戻る)
       リピートも終わったら後始末をして終了
       5: pause の時用のステージ
    */
  case 0:
    NowView->SetWindow( SBlockStart[0], 0, SBlockStart[ SBlocks ], 0 );
    statusbar->showMessage( tr( "Start Measurement!" ) );
    MeasStage = 1;
    MeasR = 0;
    break;
  case 1:
    MeasB = 0;
    MeasS = 0;
    statusbar->showMessage( tr( "Writing Header." ) );
    WriteHeader( MeasR );
    statusbar->showMessage( tr( "Going to initial position." ) );
    MoveCurPosKeV( GoToKeV = SBlockStart[0] );
    MeasStage = 2;
    break;
  case 2:
    if ( isFinishedCurMove() == 1 ) {  // 目的の角度になったら
      inMove = 0;
      // 今の戻り値は Busy=0, Ready=1, Error=2
      // なので Error の時は状態遷移が進まなくなる
      switch( (int)SMeasMode ) {
      case TRANS:
	MeasureICH( 1, SBlockDwell[ MeasB ] );   // デバイスタイプごとに違う
	break;                                    // インターフェイス関数にした方が
      case FLUO:                                  // 多分実用的。
	MeasureSSD( SBlockDwell[ MeasB ] ); // (SSD といえば 19チャンネル同時に
	//  計測リクエストがかかるとか、
	//  個別のデバイスの事情を考慮できる)
	break;
      case AUX:
	MeasureAUX( 0, SBlockDwell[ MeasB ] );
	break;
      }
      MeasureICH( 0, SBlockDwell[ MeasB ] ); // モードに関わらず I0 は必ず計測する
      MeasStage = 3;
    }
    break;
  case 3:
    if ( isFinishedMeasICH( IONCH0 ) == 1 ) {  // I0 チャンバの計測を最後に
      // これが終了したら全部終了のはず
      switch( (int)SMeasMode ) {
      case TRANS:
	ReadOutTransData( GoToKeV );                        // 面倒なので今はまだ
	break;                                    // 野村ファイルフォーマットにはしない
      case FLUO:
	ReadOutSSDData( GoToKeV );
	break;
      case AUX:
	ReadOutAUXData( AUXCH0, GoToKeV );
	break;
      }
      NowView->ReDraw();
      MeasS++;
      if ( ( MeasB < SBlocks-1 ) && ( MeasS >= SBlockPoints[ MeasB ] ) ){
	MeasS = 0;
	MeasB++;
      }
      if ( ( MeasB >= SBlocks-1 ) && ( MeasS > SBlockPoints[ MeasB ] ) ){
	MeasS = 0;
	MeasB++;
      }
      if ( MeasB < Blocks ) {
	double Delta = keV2any( SBLKUnit, SBlockStart[MeasB+1] )
	  - keV2any( SBLKUnit, SBlockStart[MeasB] );
	GoToKeV = any2keV( SBLKUnit, Delta / SBlockPoints[MeasB] * MeasS
			   + keV2any( SBLKUnit, SBlockStart[MeasB] ) );
	MoveCurPosKeV( GoToKeV );
	MeasStage = 2;
	if ( inPause == 1 ) {
	  SMeasStage = 2;
	  MeasStage = 5;
	}
      } else {
	MeasStage = 4;
	if ( inPause == 1 ) {
	  SMeasStage = 4;
	  MeasStage = 5;
	}
      }
    }
    break;
  case 4:
    MeasR++;
    if ( MeasR < SelRPT->value() ) {
      MeasStage = 1;
      NewLogMsg( QString( tr( "Meas: Repeat %1\n" ) ).arg( MeasR + 1 ) );
      ClearNowView();
    } else {
      statusbar->showMessage( tr( "The Measurement has Finished" ), 4000 );
      NewLogMsg( QString( tr( "Meas: Finished\n" ) ) );
      killTimer( MeasID );
      inMeas = 0;
      MeasStart->setText( tr( "Start" ) );
      MeasStart->setStyleSheet( "" );
      MeasPause->setEnabled( false );
      if ( OnFinishP->currentIndex() == (int)RETURN ) {
	MoveCurPosKeV( InitialKeV );
      }
    }
    break;
  case 5:
    if ( inPause == 0 ) {
      MeasStage = SMeasStage;
    }
    break;
  }
}

void MainWindow::SPSSequence( void )
{
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
    statusbar->showMessage( tr( "Scan/Search Start!" ) );
    ScanStage = 1;
    break;
  case 1:
    NowScanP = ScanSP;
    statusbar->showMessage( tr( "Going to initial position." ) );
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
}

void MainWindow::MonSequence( void )
{
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
}
