#include "MainWindow.h"

void MainWindow::ScanSequence( void )
{
  ScanInfo si = ScanView->getSInfo();
  AUnit *am = si.am;

  if ( am->isBusy() || am->isBusy2() || mScanUnits.isBusy() )
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
    NowScanP = si.sx;
    am->SetValue( si.sx );
    SetScanViewWindow();
    mScanUnits.clearStage();
    ScanStage = 1;
    // break;                   // break 不要
  case 1:
    if ( mScanUnits.init() == false ) {  // true :: initializing
      mScanUnits.clearStage();
      ScanStage = 2;
    }
    break;
  case 2: 
    mScanUnits.setDwellTime();
    if ( mScanUnits.isParent() ) {
      ScanStage = 3;
    } else {
      ScanStage = 4;
    }
    break;
  case 3:
    if ( mScanUnits.getValue0() == false ) { // only for counters and SSDs
      mScanUnits.clearStage();
      ScanStage = 4;
    }
    break;
  case 4:
    if ( mScanUnits.getValue() == false ) {  // true :: Getting
      mScanUnits.clearStage();
      ScanStage = 5;
    }
    break;
  case 5:
    mScanUnits.readValue( MeasVals, MeasCPSs, false );   // false :: not correct dark
    if ( si.normalize ) {
      if ( MeasVals[1] != 0 )
	ScanView->NewPoint( 0, NowScanP, MeasVals[0] / MeasVals[1] );
      else 
	ScanView->NewPoint( 0, NowScanP, MeasVals[0] );
    } else {
      ScanView->NewPoint( 0, NowScanP, MeasVals[0] );
    }
    for ( int i = 1; i < mScanUnits.count(); i++ ) {
      ScanView->NewPoint( i, NowScanP, MeasVals[i] );
    }
    ScanView->update();
    NowScanP += si.dx;
    if ( ( ( si.dx > 0 )&&( NowScanP > si.ex ) )
	 ||( ( si.dx < 0 )&&( NowScanP < si.ex ) ) ) {
      ScanStage = 10;
    } else {
      am->SetValue( NowScanP );
      ScanStage = 3;
    }
    break;
  case 10:
    SPSScanB->setText( tr( "Scan" ) );
    SPSScanB->setStyleSheet( NormalEXECB );
    statusbar->showMessage( tr( "The Scan has Finished" ), 4000 );
    NewLogMsg( QString( tr( "Scan Finished" ) ) );
    am->Stop();
    ScanStage = 11;
    break;
  case 11:
    am->SetValue( si.origin );
    ScanStage = 12;
    break;
  case 12:
    UUnits.removeUnits( SCAN_ID );
    inSPSing = 0;
    ScanTimer->stop();
    SPSScanB->setEnabled( true );
    GoMotorB->setEnabled( true );
    ScanViewC->setDeletable( true );
    break;
  }
}

void MainWindow::SetScanViewWindow( void )
{
  ScanInfo si = ScanView->getSInfo();
  if ( si.ex > si.sx ) {
    ScanView->SetWindow0( si.sx, 0, si.ex, 0 );
  } else {
    ScanView->SetWindow0( si.ex, 0, si.sx, 0 );
  }
}
