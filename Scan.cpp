#include "MainWindow.h"

void MainWindow::ScanSequence( void )
{
  AUnit *am = AMotors.value( MotorN->currentIndex() );

  if ( am->isBusy() || am->isBusy2() || mUnits.isBusy() )
    return;

  switch( ScanStage ) {
    /* 
       0: ¬�곫��
       1: ¬��γ������˰�ư
       2: ��Ū�γ��٤ˤʤä����¬
       3: ��¬����λ�����鼡�γ��٤˰�ư
       --- 2 �� 3 �򷫤��֤� ---
       4: �ǽ��֥�å��κǽ����ޤ��褿���ԡ��Ƚ���(1�����)
       ��ԡ��Ȥ⽪��ä��������򤷤ƽ�λ
    */
  case 0:
    statusbar->showMessage( tr( "Going to initial position." ), 1000 );
    NowScanP = ScanSP;
    am->SetValue( ScanSP );
    SetSPSViewWindow();
    mUnits.clearStage();
    ScanStage = 1;
    // break;                   // break ����
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
