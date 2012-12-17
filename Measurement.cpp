#include "MainWindow.h"

void MainWindow::MeasSequence( void )
{
  double Delta;
  bool a1, a2;

  if ( AskingOverwrite )
    return;

  if ( ( a1 = isBusyMotorInMeas() ) || ( a2 = mUnits.isBusy() ) ) {
    return;
  }

  switch( MeasStage ) {
    /* 
       0: ¬�곫�� Repeat = 0
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
       99: pause �λ��ѤΥ��ơ���
    */
  case 0:
    mUnits.clearStage();
    MeasStage = 1;
  case 1:
    NowView->SetWindow( SBlockStart[0], 0, SBlockStart[ SBlocks ], 0 );
    statusbar->showMessage( tr( "Start Measurement!" ) );
    MeasR = 0;    // Measurement Repeat count
    if ( mUnits.init() == false )   // true :: initializing
      mUnits.clearStage();
      MeasStage = 2;
    break;
  case 2:
    MeasB = 0;    // Measurement Block count
    statusbar->showMessage( tr( "Writing Header." ), 2000 );
    WriteHeader( MeasR );
    MeasStage = 3;
    // break;       MeasStage == 1 ��ư��ϥ쥹�ݥ󥹤��Ԥ�ɬ�פʤ�
  case 3: 
    MeasS = 0;    // Measurement Step count in each block
    mUnits.setDwellTimes( NowDwell = SBlockDwell[0] );
    MeasStage = 4;
    // break;       MeasStage == 2 ��쥹�ݥ󥹤��Ԥ�ɬ�פʤ�
    //              (�����������Τϥ��󥵡���, Stage == 3 �ǥ��󥵡������ʤ�����)
  case 4:
    Delta = keV2any( SBLKUnit, SBlockStart[MeasB+1] )
      - keV2any( SBLKUnit, SBlockStart[MeasB] );
    GoToKeV = any2keV( SBLKUnit, Delta / SBlockPoints[MeasB] * MeasS
		       + keV2any( SBLKUnit, SBlockStart[MeasB] ) );
    MoveCurThPosKeV( GoToKeV );     // ���ΰ�ư
    mUnits.clearStage();
    if ( mUnits.isParent() )
      MeasStage = 5;
    else
      MeasStage = 6;
    break;
  case 5:
    if ( mUnits.getValue0() == false ) { // only for counters
      mUnits.clearStage();
      MeasStage = 6;
    }
    break;
  case 6:
    if ( mUnits.getValue() == false ) {  // true :: Getting
      mUnits.clearStage();
      MeasStage = 7;
    }
    break;
  case 7:
    mUnits.readValue( MeasVals );
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
      } else {               // ��λ
	statusbar->showMessage( tr( "The Measurement has Finished" ), 4000 );
	NewLogMsg( QString( tr( "Meas: Finished\n" ) ) );
	MeasTimer->stop();
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

bool MainWindow::isBusyMotorInMeas( void )
{
  return MMainTh->isBusy() || MMainTh->isBusy2();
}

void MainWindow::DispMeasDatas( void )
{
  double I0;
  double Val;

  I0 = MeasVals[ MC_I0 ];
  NowView->NewPoint( 0, GoToKeV, I0 );
  for ( int i = 1; i < mUnits.count(); i++ ) {
    Val = MeasVals[i];
    if ( MeasDispMode[i] == TRANS ) {
      if ( Val < 1e-10 )
	Val = 1e-10;
      if ( ( I0 / Val ) > 0 )
	NowView->NewPoint( i, GoToKeV, log( I0/Val ) );
      else 
	NowView->NewPoint( i, GoToKeV, 0 );
    } else {  // MeasDispMode == FLUO
      if ( I0 < 1e-20 )
	I0 = 1e-20;
      NowView->NewPoint( i, GoToKeV, Val/I0 );
    }
  }
}
