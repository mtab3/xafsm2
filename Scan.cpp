#include "MainWindow.h"

void MainWindow::ScanSequence( void )
{
  AUnit *am = AMotors.value( MotorN->currentIndex() );

  if ( am->isBusy() || am->isBusy2() || mUnits.isBusy() )
    return;

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
    statusbar->showMessage( tr( "Going to initial position." ), 1000 );
    NowScanP = ScanSP;
    am->SetValue( ScanSP );
    SetSPSViewWindow();
    mUnits.clearStage();
    ScanStage = 1;
    // break;                   // break 不要
  case 1:
    if ( mUnits.init() == false ) {  // true :: initializing
      mUnits.clearStage();
      ScanStage = 2;
    }
    break;
  case 2: 
    mUnits.setDwellTime();
    if ( mUnits.isParent() ) {
      ScanStage = 3;
    } else {
      ScanStage = 4;
    }
    break;
  case 3:
    if ( mUnits.getValue0() == false ) { // only for counters and SSDs
      mUnits.clearStage();
      ScanStage = 4;
    }
    break;
  case 4:
    if ( mUnits.getValue() == false ) {  // true :: Getting
      mUnits.clearStage();
      ScanStage = 5;
    }
    break;
  case 5:
    mUnits.readValue( MeasVals );
    SPSView->NewPoint( 1, NowScanP, MeasVals[0] );
    SPSView->ReDraw();
    NowScanP += ScanSTP;
    if ( ( ( ScanSTP > 0 )&&( NowScanP > ScanEP ) )
	 ||( ( ScanSTP < 0 )&&( NowScanP < ScanEP ) ) ) {
      ScanStage = 10;
    } else {
      am->SetValue( NowScanP );
      ScanStage = 3;
    }
    break;
  case 10:
    statusbar->showMessage( tr( "The Scan has Finished" ), 4000 );
    NewLogMsg( QString( tr( "Scan Finished\n" ) ) );
    // sks->SetValue( Motors[ MovingM ].devName, p = SPSView->PeakSearch( 1 ) );
    am->Stop();
    ScanStage = 11;
    break;
  case 11:
    am->SetValue( ScanOrigin );
    ScanStage = 12;
    break;
  case 12:
    inSPSing = 0;
    ScanTimer->stop();
    SPSScan->setText( tr( "Scan" ) );
    SPSScan->setStyleSheet( "" );
    SPSScan->setEnabled( true );
    GoMotor->setEnabled( true );
    break;
  }
}

void MainWindow::SetSPSViewWindow( void )
{
  if ( ScanEP > ScanSP ) {
    SPSView->SetWindow( ScanSP, 0, ScanEP, 0 );
  } else {
    SPSView->SetWindow( ScanEP, 0, ScanSP, 0 );
  }
}
