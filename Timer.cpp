#include <QWidget>

#include "MainWindow.h"

/* Timer �C�x���g�́A����̃V�~�����[�V�����ȊO�ɂ��g�����ǁA�Ƃ肠���������ɒu���Ƃ� */

void MainWindow::timerEvent( QTimerEvent *event )
{
  int Id = event->timerId();

  if ( Id == WatchCurPosID ) { /* ���I�Ȋp�x�Ď� */ 
                               /* �{���ɊĎ����Â���̂��ǂ����ǂ����͂܂��� */
    WatchPos();
  }
  if ( Id == MoveID ) { /* ���ړ����̎��̒l�̊Ď� */
    MotorMove();
  }
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
       0: ����J�n
       1: ����̊J�n�_�Ɉړ�
       2: �ړI�̊p�x�ɂȂ�����v��
       3: �v�����I�������玟�̊p�x�Ɉړ�
       --- 2 �� 3 ���J��Ԃ� ---
       4: �ŏI�u���b�N�̍ŏI�_�܂ŗ����烊�s�[�g����(1�֖߂�)
       ���s�[�g���I��������n�������ďI��
       5: pause �̎��p�̃X�e�[�W
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
    if ( isFinishedCurMove() == 1 ) {  // �ړI�̊p�x�ɂȂ�����
      inMove = 0;
      // ���̖߂�l�� Busy=0, Ready=1, Error=2
      // �Ȃ̂� Error �̎��͏�ԑJ�ڂ��i�܂Ȃ��Ȃ�
      switch( (int)SMeasMode ) {
      case TRANS:
	MeasureICH( 1, SBlockDwell[ MeasB ] );   // �f�o�C�X�^�C�v���ƂɈႤ
	break;                                    // �C���^�[�t�F�C�X�֐��ɂ�������
      case FLUO:                                  // �������p�I�B
	MeasureSSD( SBlockDwell[ MeasB ] ); // (SSD �Ƃ����� 19�`�����l��������
	//  �v�����N�G�X�g��������Ƃ��A
	//  �ʂ̃f�o�C�X�̎�����l���ł���)
	break;
      case AUX:
	MeasureAUX( 0, SBlockDwell[ MeasB ] );
	break;
      }
      MeasureICH( 0, SBlockDwell[ MeasB ] ); // ���[�h�Ɋւ�炸 I0 �͕K���v������
      MeasStage = 3;
    }
    break;
  case 3:
    if ( isFinishedMeasICH( IONCH0 ) == 1 ) {  // I0 �`�����o�̌v�����Ō��
      // ���ꂪ�I��������S���I���̂͂�
      switch( (int)SMeasMode ) {
      case TRANS:
	ReadOutTransData( GoToKeV );                        // �ʓ|�Ȃ̂ō��͂܂�
	break;                                    // �쑺�t�@�C���t�H�[�}�b�g�ɂ͂��Ȃ�
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
}

void MainWindow::MonSequence( void )
{
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
}
