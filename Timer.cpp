#include <QWidget>

#include "MainWindow.h"

/* Timer �C�x���g�́A����̃V�~�����[�V�����ȊO�ɂ��g�����ǁA�Ƃ肠���������ɒu���Ƃ� */

void MainWindow::timerEvent( QTimerEvent *event )
{
  int Id = event->timerId();

#if 0
  if ( Id == WatchCurPosID ) { /* ���I�Ȋp�x�Ď� */ 
                               /* �{���ɊĎ����Â���̂��ǂ����ǂ����͂܂��� */
    WatchPos();
  }
  if ( Id == MoveID ) { /* ���ړ����̎��̒l�̊Ď� */
    MotorMove();
  }
#endif

  if ( Id == MeasID ) { /* ����X�e�b�v�̐i�s */
    MeasSequence();
  }
  if ( Id == SPSID ) { /* �X�L����/�s�[�N�T�[�`�̐i�s */
    SPSSequence();
  }
  if ( Id == MonID ) { /* ����̑����̒l�̎��ԕω��̊Ď� */
    MonSequence();
  }
}

#if 0     // ���ݑS���s�v�̂͂�
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

#if 0    // ���ݑS���s�v�̂͂�
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


bool MainWindow::isBusyMotorInMeas( void )
{
  return false;                               // !!!!!!!!!! �_�~�[
  return MMainTh->getIsBusy();
}

bool MainWindow::isBusySensorInMeas( void )
{
  return false;                               // !!!!!!!!!! �_�~�[
  bool isB0, isB1, isB2, isB3, isB4;
  isB0 = ASensors.value( MeasSens0 )->getIsBusy();   // I0
  isB1 = MeasSensF1 & ASensors.value( MeasSens1 )->getIsBusy();   // I1
  isB2 = MeasSensF2 & ASensors.value( MeasSens2 )->getIsBusy();   // Aux1
  isB3 = MeasSensF3 & ASensors.value( MeasSens3 )->getIsBusy();   // Aux2
  isB4 = MeasSensF4 & SFluo->getIsBusy();
  return ( isB0 | isB1 | isB2 | isB3 | isB4 );
}

void MainWindow::SetDwellTime( double dtime )       // �_�~�[�֐�
{
  
}

void MainWindow::MeasureI0( void )     // �_�~�[�֐�
{
}

void MainWindow::MeasureI1( void )     // �_�~�[�֐�
{
}

void MainWindow::Measure19chSSD( void )     // �_�~�[�֐�
{
}

void MainWindow::MeasureAux1( void )     // �_�~�[�֐�
{
}

void MainWindow::MeasureAux2( void )     // �_�~�[�֐�
{
}

void MainWindow::ReadI0( void )          // �_�~�[�֐�
{
}

void MainWindow::ReadI1( void )          // �_�~�[�֐�
{
}

void MainWindow::Read19chSSD( void )          // �_�~�[�֐�
{
}

void MainWindow::ReadAux1( void )          // �_�~�[�֐�
{
}

void MainWindow::ReadAux2( void )          // �_�~�[�֐�
{
}

void MainWindow::MeasSequence( void )
{
  double Delta;

  if ( isBusyMotorInMeas() || isBusySensorInMeas() )
    return;

  switch( MeasStage ) {
    /* 
       0: ����J�n Repeat = 0
       1: Block = 0
       2: Step = 0, setDwellTIme
       3: Goto a Position with a Block and a Step
       4: Triger Sensors
       5: Read out Sensors
          Step++; if ( Step < MaxStep ) goto 3
          Block++; if ( Block < MaxBlock ) goto 2
          Repeat++; if ( Repeat < MaxRepeat ) toto 1
          when reach here, finish.
       6: pause �̎��p�̃X�e�[�W
    */
  case 0:
    NowView->SetWindow( SBlockStart[0], 0, SBlockStart[ SBlocks ], 0 );
    statusbar->showMessage( tr( "Start Measurement!" ) );
    MeasR = 0;    // Measurement Repeat count
    MeasStage = 1;
    //break;        MeasStage == 0 �̓��e�ɂ́A���X�|���X��҂��Ȃ��Ƃ����Ȃ�
    //              ���m���Ȃ��̂� break ���� MeasStage == 1 �Ɉڍs����
  case 1:
    MeasB = 0;    // Measurement Block count
    statusbar->showMessage( tr( "Writing Header." ), 2000 );
    WriteHeader( MeasR );
    MeasStage = 2;
    // break;       MeasStage == 1 �̓��e�ɂ����X�|���X��҂K�v�Ȃ�
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
    MoveCurThPosKeV( GoToKeV );     // ���̈ړ�
    MeasStage = 4;
    break;
  case 4:
    MeasureI0();
    if ( UseI1->isChecked() ) MeasureI1();
    if ( Use19chSSD->isChecked() ) Measure19chSSD();
    if ( UseAux1->isChecked() ) MeasureAux1();
    if ( UseAux2->isChecked() ) MeasureAux2();
    MeasStage = 5;
    break;
  case 5:
    ReadI0();
    ReadI1();
    Read19chSSD();
    ReadAux1();
    ReadAux2();
    NowView->ReDraw();
    MeasS++;
    if ( inPause == 1 ) {
      MeasStage = 7;          // PauseStage
    }
    // break;                 // case 6: �̓|�[�Y���̕��A�|�C���g
  case 6:
    if ( inPause == 0 ) {
      qDebug() << tr( "R=%1 B=%2 S=%3" ).arg( MeasR ).arg( MeasB ).arg( MeasS );
      if ( MeasS < SBlockPoints[ MeasB ] ) {
	MeasStage = 3;
      } else if ( MeasB < SBlocks ) {
	MeasB++;
	MeasStage = 2;
      } else if ( MeasR < SelRPT->value() ) {
	NewLogMsg( QString( tr( "Meas: Repeat %1\n" ) ).arg( MeasR + 1 ) );
	ClearNowView();
	MeasR++;
	MeasStage = 1;
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
  case 7:
    if ( inPause == 0 )
      MeasStage = 6;
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
       0: ����J�n
       1: ����̊J�n�_�Ɉړ�
       2: �ړI�̊p�x�ɂȂ�����v��
       3: �v�����I�������玟�̊p�x�Ɉړ�
       --- 2 �� 3 ���J��Ԃ� ---
       4: �ŏI�u���b�N�̍ŏI�_�܂ŗ����烊�s�[�g����(1�֖߂�)
       ���s�[�g���I��������n�������ďI��
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
    if ( sks->IsBusy( Motors[ MovingM ].devName ) == 0 ) {  // �ړI�̊p�x�ɂȂ�����
      // ���̖߂�l�� Busy=1, Ready=0, Error=-1
      // �Ȃ̂� Error �̎��͏�ԑJ�ڂ��i�܂Ȃ��Ȃ�
      sks->StartMeas( MDevs[ SPSMon ].devName, ScanDW, MDevs[ SPSMon ].devCh );
      MeasureICH( 0, ScanDW );
      ScanStage = 3;
    }
    break;
  case 3:
    if ( sks->IsBusy( MDevs[ SPSMon ].devName, MDevs[ SPSMon ].devCh ) == 0 ) { 
      ReadOutScanData( NowScanP );                        // �ʓ|�Ȃ̂ō��͂܂�
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
      ScanStage = 5;  // ���[�^���s�[�N�ʒu�ɂ��ǂ蒅���̂�҂��ďI��
    } else {
      SPSStop0();
    }
    break;
  case 5:
    if ( sks->IsBusy( Motors[ MovingM ].devName ) == 0 ) { 
      ScanStage = 6;  // �����Ŏ~�߂Ă��������ǁA���[�^�̌��݈ʒu�\�����m���ɂ��邽�߂�
                      // ����1�C���^�[�o���܂킷�B
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
       0: ����J�n
       1: �v�����ʎ擾
       ���̃T�C�N���Ɏ����I�ȏI���͖��� (�����I�ɃX�g�b�v�̎w��������܂Ŏ~�܂�Ȃ�)
    */
  case 0:
    if ( MonUsedOldV == 0 ) { // �O�񑪒肪�Ԃɍ����ĂȂ������玟�̑���̓X�^�[�g���Ȃ�
      sks->StartMeas( MDevs[ MonDev ].devName, MonMeasTime, MDevs[ MonDev ].devCh2 );
    } else {
      MonUsedOldV = 0;
    }
    if ( MonUsedOldV0 == 0 ) { // �O�񑪒肪�Ԃɍ����ĂȂ������玟�̑���̓X�^�[�g���Ȃ�
      sks->StartMeas( MDevs[ IONCH0 ].devName, MonMeasTime, MDevs[ IONCH0 ].devCh2 );
    } else {
      MonUsedOldV0 = 0;
    }
    MonStage = 1;
    break;
  case 1:
    if ( sks->IsBusy( MDevs[ MonDev ].devName, MDevs[ MonDev ].devCh2 ) == 1 ) {
      NewMonV = OldMonV;   // �����肪�Ԃɍ����ĂȂ�������, �Â����茋�ʂ𗬗p
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
      NewMonV0 = OldMonV0;   // �����肪�Ԃɍ����ĂȂ�������, �Â����茋�ʂ𗬗p
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

