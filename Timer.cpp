#include <QWidget>
#include <QFile>

#include "MainWindow.h"

/* Timer �C�x���g�́A����̃V�~�����[�V�����ȊO�ɂ��g�����ǁA�Ƃ肠���������ɒu���Ƃ� */
/* ���j�I�ɂ́AStars �Ƃ̒ʐM������Ă��Ȃ����������ɁAStars �̃f�o�C�X�� */
/* �V�~�����[�V�����������ł���Ă��̂ɂ��̊Ԃɂ��A�{�Ԃł� */
/* �厖�ȓ�������������悤�ɂȂ��� */

void MainWindow::timerEvent( QTimerEvent *event )
{
  int Id = event->timerId();

  if ( Id == MeasID ) { /* ����X�e�b�v�̐i�s */
    MeasSequence();
  }
  if ( Id == MoveID ) { /* �w�肵�����̈ړ� */
    MotorMove();
  }
  if ( Id == SPSID ) { /* �X�L����/�s�[�N�T�[�`�̐i�s */
    SPSSequence();
  }
  if ( Id == MonID ) { /* ����̑����̒l�̎��ԕω��̊Ď� */
    MonSequence();
  }
}

void MainWindow::MotorMove( void )
{
  AUnit *am = AMotors.value( MotorN->currentIndex() );
  
  if ( !am->isBusy() ) {
    GoMStop0();
    NewLogMsg( tr( "Setup: %1 : Reached at %2\n" )
	       .arg( am->getName() ).arg( am->value() ) );
    statusbar->showMessage( tr( "Setup: %1 : Reached at %2\n" )
			    .arg( am->getName() ).arg( am->value() ), 1000 );
  }
}

void MainWindow::ClearSensorStages( void )
{
  if ( OneOfTheSensorIsCounter )
    TheCounter->InitLocalStage();
  if ( OneOfTheSensorIsSSD ) {
    SFluo->InitLocalStage();
  }
  for ( int i = 0; i < MCHANNELS; i++ ) {
    if ( MeasSensF[i] )
      MeasSens[i]->InitLocalStage();
  }
}

bool MainWindow::InitSensors( void )
{
  bool ff = false;

  if ( OneOfTheSensorIsCounter )
    ff |= TheCounter->InitSensor();
  if ( OneOfTheSensorIsSSD ) {
    ff |= SFluo->InitSensor();
  }
  for ( int i = 0; i < MCHANNELS; i++ ) {
    if ( MeasSensF[i] ) {
      ff |= MeasSens[i]->InitSensor();
    }
  }

  return ff;
}

bool MainWindow::isBusyMotorInMeas( void )
{
  return MMainTh->isBusy() || MMainTh->isBusy2();
}

bool MainWindow::isBusySensors( void )
{
  bool ff = false;

  if ( OneOfTheSensorIsCounter )
    ff |= TheCounter->isBusy() || TheCounter->isBusy2();
  if ( OneOfTheSensorIsSSD ) {
    ff |= SFluo->isBusy() || SFluo->isBusy();
  }
  for ( int i = 0; i < MCHANNELS; i++ ) {
    if ( MeasSensF[i] ) {
      ff |= MeasSens[i]->isBusy() || MeasSens[i]->isBusy2();
    }
  }

  return ff;
}

void MainWindow::SetDwellTime( double dtime )  // ������z���g�͕ԓ���҂`�ɂ���ׂ�
{
  for ( int i = 0; i < MCHANNELS; i++ ) {
    if ( MeasSensF[i] )
      MeasSens[i]->SetTime( dtime );
  }
}

void MainWindow::SetDwellTime2( void )  // ������z���g�͕ԓ���҂`�ɂ���ׂ�
{
  for ( int i = 0; i < MCHANNELS; i++ ) {
    if ( MeasSensF[i] )
      MeasSens[i]->SetTime( MeasSensDT[i] );
  }
}

bool MainWindow::GetSensValues0( void )
{
  bool rv = false;

  if ( OneOfTheSensorIsCounter )
    rv |= TheCounter->GetValue0();
  if ( OneOfTheSensorIsSSD ) {
    rv |= SFluo->GetValue0();
  }

  return rv;
}

bool MainWindow::GetSensValues( void )
{
  bool ff = false;

  for ( int i = 0; i < MCHANNELS; i++ ) {
    if ( MeasSensF[i] ) {
      ff |= MeasSens[i]->GetValue();
    }
  }
  
  return ff;
}

void MainWindow::ReadSensValues( void )
{
  //  MeasVals[ MC_I0 ] = MeasSens[ MC_I0 ]->value().toDouble();
  for ( int i = 0; i < MCHANNELS; i++ ) {
    if ( MeasSensF[i] ) {
      if ( MeasSens[i]->getType() == "SSD" ) {
	MeasVals[i] = MeasSens[i]->values().at(0).toDouble();
      } else {
	MeasVals[i] = MeasSens[i]->value().toDouble();
      }
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
       0: ����J�n Repeat = 0
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
       99: pause �̎��p�̃X�e�[�W
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
    // break;       MeasStage == 1 �̓���̓��X�|���X��҂K�v�Ȃ�
  case 3: 
    MeasS = 0;    // Measurement Step count in each block
    ClearSensorStages();
    SetDwellTime( NowDwell = SBlockDwell[0] );
    MeasStage = 4;
    // break;       MeasStage == 2 �����X�|���X��҂K�v�Ȃ�
    //              (�����ő��삵���̂̓Z���T�[��, Stage == 3 �ŃZ���T�[�𑀍삵�Ȃ�����)
  case 4:
    Delta = keV2any( SBLKUnit, SBlockStart[MeasB+1] )
      - keV2any( SBLKUnit, SBlockStart[MeasB] );
    GoToKeV = any2keV( SBLKUnit, Delta / SBlockPoints[MeasB] * MeasS
		       + keV2any( SBLKUnit, SBlockStart[MeasB] ) );
    MoveCurThPosKeV( GoToKeV );     // ���̈ړ�
    ClearSensorStages();
    if ( OneOfTheSensorIsCounter || OneOfTheSensorIsSSD )
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
      } else {               // �I��
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

  switch( MonStage ) {
    /* 
       0: �l�̓ǂݏo���ƕ\��
       1: nct08 ���g����: �v���J�n�̑O����
          ����ȊO: �v���J�n
       2: nct08 ���g����: �v���J�n
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
    SetDwellTime2();
    MonStage = 3;
    break;
  case 3:
    if ( OneOfTheSensorIsCounter || OneOfTheSensorIsSSD ) {
      if ( GetSensValues0() == false ) { // only for counters and SSDs
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
  AUnit *am = AMotors.value( MotorN->currentIndex() );

  if ( am->isBusy() || am->isBusy2() || isBusySensors() )
    return;

  switch( ScanStage ) {
    /* 
       0: ����J�n
       1: ����̊J�n�_�Ɉړ�
       2: �ړI�̊p�x�ɂȂ�����v��
       3: �v�����I�������玟�̊p�x�Ɉړ�
       --- 2 �� 3 ���J��Ԃ� ---
       4: �ŏI�u���b�N�̍ŏI�_�܂ŗ����烊�s�[�g����(1�֖߂�)
       ���s�[�g���I��������n�������ďI��
    */
  case 0:
    statusbar->showMessage( tr( "Going to initial position." ), 1000 );
    NowScanP = ScanSP;
    am->SetValue( ScanSP );
    SetSPSViewWindow();
    ClearSensorStages();
    ScanStage = 1;
    // break;                   // break �s�v
  case 1:
    if ( InitSensors() == false ) {  // true :: initializing
      ClearSensorStages();
      ScanStage = 2;
    }
    break;
  case 2: 
    ClearSensorStages();
    SetDwellTime2();
    ScanStage = 3;
    break;
  case 3:
    if ( OneOfTheSensorIsCounter || OneOfTheSensorIsSSD ) {
      if ( GetSensValues0() == false ) { // only for counters and SSDs
	ClearSensorStages();
	ScanStage = 4;
      }
    } else {
      if ( GetSensValues() == false ) {  // true :: Getting
	ClearSensorStages();
	ScanStage = 5;
      }
    }
    break;
  case 4:
    if ( GetSensValues() == false ) {  // true :: Getting
      ClearSensorStages();
      ScanStage = 5;
    }
    break;
  case 5:
    ReadSensValues();
    qDebug() << "Plot" << NowScanP << MeasVals[0];
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
    killTimer( SPSID );
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
