
#include "MainWindow.h"

#include "SMsg.h"

void MainWindow::setupScan2DArea( void )
{
  S2DFileSel = new QFileDialog;
  S2DFileSel->setAcceptMode( QFileDialog::AcceptSave );
  S2DFileSel->setDirectory( QDir::currentPath() );
  S2DFileSel->setFilter( "*.dat" );

  S2DAxis << S2DAx1 << S2DAx2 <<S2DAx3;
  S2DCurPos << S2DCurPos1 << S2DCurPos2 << S2DCurPos3;
  S2DUnits << S2DUnit1 << S2DUnit2 << S2DUnit3;
  S2DStarts << S2DStartP1 << S2DStartP2 << S2DStartP3;
  S2DEnds << S2DEndP1 << S2DEndP2 << S2DEndP3;
  S2DSteps << S2DStep1 << S2DStep2 << S2DStep3;
  S2DPoints << S2DPoints1 << S2DPoints2 << S2DPoints3;
  S2DRelAbs << S2DRelAbs1 << S2DRelAbs2 << S2DRelAbs3;
  S2DMotors << NULL << NULL << NULL;
  S2DMotorUse << true << true << false;
  S2Dnow << 0 << 0 << 0;
  S2Dsx << 0 << 0 << 0;
  S2Dex << 0 << 0 << 0;
  S2Ddx << 0 << 0 << 0;
  S2Dx  << 0 << 0 << 0;
  S2Dsign << 1 << 1 << 1;

  for ( int i = 0; i < ASensors.count(); i++ ) {
    S2DOkSensors << ASensors[i];
    SelectS2DSensor->addItem( ASensors[i]->getName() );
  }
  for ( int i = 0; i < AMotors.count(); i++ ) {
    // SetSpeed, SetMaxSpeed �����Ȥ��ʤ��ȥ��ᡣ���� PM �Τ�
    if ( AMotors[i]->getType() == "PM" ) {
      S2DOkMotors << AMotors[i];
      for ( int j = 0; j < S2DAxis.count(); j++ ) {
	S2DAxis[j]->addItem( AMotors[i]->getName() );
      }
    }
  }
  for ( int i = 0; i < S2DAxis.count(); i++ ) {
    newAx0( i, S2DAxis[i]->currentIndex() );
    connect( S2DAxis[i], SIGNAL( currentIndexChanged( int ) ),
	     this, SLOT( newAx( int ) ) );
  }

  inS2D = false;
  S2DTimer = new QTimer;

  connect( S2DStart, SIGNAL( clicked() ), this, SLOT( S2DScanStart() ) );
  connect( S2DTimer, SIGNAL( timeout() ), this, SLOT( S2DScanSequence() ) );
  connect( S2DFileSelect, SIGNAL( clicked() ), S2DFileSel, SLOT( show() ) );
  connect( S2DFileSel, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( newS2DFileSelected( const QString & ) ) );
}

void MainWindow::newAx0( int ax, int motor )
{
  S2DUnits[ax]->setText( S2DOkMotors[ motor ]->getUnit() );
  S2DCurPos[ax]->setText( QString::number( S2DOkMotors[ motor ]->metricValue() ) );
  if ( S2DMotors[ax] != NULL ) {
    bool Uniq = true;
    for ( int i = 0; i < S2DMotors.count(); i++ ) {
      if ( ( i != ax )&&( S2DMotors[i] == S2DMotors[ax] ) ) {
	Uniq = false;
	break;
      }
    }
    if ( Uniq )
      disconnect( S2DMotors[ax], SIGNAL( newValue( QString ) ),
		  this, SLOT( showS2DNewAxValue( QString ) ) );
  }
  S2DMotors[ax] = S2DOkMotors[motor];   // ���μ������Ф줿�⡼����Ф��Ƥ���
  connect( S2DMotors[ax], SIGNAL( newValue( QString ) ),
	   this, SLOT( showS2DNewAxValue( QString ) ) );
}

void MainWindow::newAx( int m )
{
  for ( int ax = 0; ax < S2DAxis.count(); ax++ ) {
    if ( sender() == S2DAxis[ax] ) {
      newAx0( ax, m );
    }
  }
}

void MainWindow::newS2DFileSelected( const QString &fname )
{
  S2DFileName->setText( fname );
  S2DNameStat = NEW;
  S2DFileName->setStyleSheet( FSTATCOLORS[ S2DDataStat ][ S2DNameStat ] );
  S2DFileName->setToolTip( FSTATMsgs[ S2DDataStat ][ S2DNameStat ] );
}

void MainWindow::showS2DNewAxValue( QString )
{
  for ( int i = 0; i < S2DAxis.count(); i++ ) {
    if ( S2DMotors[i] == sender() ) {
      S2DCurPos[i]->setText( QString::number( S2DMotors[i]->metricValue() ) );
    }
  }
}

void MainWindow::S2DScanStart( void )
{
  AUnit *as = NULL;

  if ( !inS2D ) {
    if ( inMeas || inSPSing || inMonitor || inMMove || inMCAMeas ) {
      statusbar
	->showMessage( tr( "Can't start 2D Scan. Othre Process is going on." ), 2000 );
      return;
    }
    S2DMotorUse[ S2DMotorUse.count() - 1 ] = S2DUse3rdAx->isChecked();
    for ( int i = 0; i < S2DMotors.count(); i++ ) {
      if ( S2DMotorUse[i] && ( ! S2DMotors[i]->isEnable() ) ) {
	QString msg = tr( "2D Scan cannot Start : (%1) is disabled" )
	  .arg( S2DMotors[i]->getName() );
	statusbar->showMessage( msg, 2000 );
	return;
      }
    }
    // ��¬���֤� 0 �ˤʤäƤʤ��������å�
    if ( S2DTime1->text().toDouble() <= 0 ) {
      statusbar->showMessage( tr( "Meas Time is 0 or less." ), 2000 );
    }
    for ( int i = 0; i < S2DMotors.count(); i++ ) {
      if ( S2DMotorUse[i] ) {
	// ��������ϰϤ��������뤫������å�
	if ( S2DStarts[i]->text().toDouble() == S2DEnds[i]->text().toDouble() ) {
	      statusbar->showMessage( tr( "2D Scan Range Error." ), 2000 );
	}
	// ¬�������� 1�ʾ夢�뤳�Ȥ��ǧ
	if ( S2DPoints[i]->text().toInt() > 0 ) {
	      statusbar->showMessage( tr( "2D Scan Points Error." ), 2000 );
	}
	// �������֤äƤʤ��������å�
	for ( int j = i+1; j < S2DMotors.count(); j++ ) {
	  if ( S2DMotorUse[j] ) {
	    if ( S2DMotors[i] == S2DMotors[j] ) {
	      statusbar
		->showMessage( tr( "The same motor was selected for different axis." ),
			       2000 );
	    }
	  }
	}
      }
    }
    if ( S2DStepScan->isChecked() ) {  // ���ƥåץ������
      mUnits.clearUnits();
      mUnits.addUnit( as = S2DOkSensors.value( SelectS2DSensor->currentIndex() ) );
      mUnits.setDwellTimes( S2DTime1->text().toDouble() );
      mUnits.setDwellTime();

      for ( int i = 0; i < mUnits.count(); i++ ) {
	if ( ! mUnits.at(i)->isEnable() ) {
	  QString msg = QString( tr( "2D Scan cannot Start : (%1) is disabled" ) )
	    .arg( mUnits.at(i)->getName() );
	  statusbar->showMessage( msg, 2000 );
	  NewLogMsg( msg );
	  return;
	}
      }
    } else {
      // Ϣ³�������ϸ��ߥ��ݡ��Ȥ��Ƥ��ʤ�
      NewLogMsg( tr( "Continuous scan is not available now." ) );
      return;
    }
    inS2D = true;
    NewLogMsg( QString( tr( "2D Scan Start (%1 %2 %3 (%4))" ) )
	       .arg( as->getName() )
	       .arg( S2DMotors[0]->getName() )
	       .arg( S2DMotors[1]->getName() )
	       .arg( S2DMotors[2]->getName() ) );
    
    S2DStart->setText( tr( "Stop" ) );
    S2DStart->setStyleSheet( InActive );
    
    S2DDataStat = NEW;
    S2DFileName->setStyleSheet( FSTATCOLORS[ ScanDataStat ][ ScanNameStat ] );
    S2DFileName->setToolTip( FSTATMsgs[ ScanDataStat ][ ScanNameStat ] );

    SetupS2DParams();
    // 1st �� 2nd �μ���ñ�̤�Ʊ���ʤ顢ɽ���νĲ���򥹥�����ϰϤ�
    // �Ĳ���˹�碌��褦�����Ϥ��롣
    // �����Ǥʤ���С����̤��äѤ���Ȥ���
    if ( S2DMotors[0]->getUnit() == S2DMotors[1]->getUnit() ) {
      S2DV->setRatioType( REAL_RATIO );
    } else {
      S2DV->setRatioType( AS_SCREEN );
    }
    S2DV->setRange( S2DStarts[0]->text().toDouble(), S2DEnds[0]->text().toDouble(),
		    S2DStarts[1]->text().toDouble(), S2DEnds[1]->text().toDouble() );

    S2DStage = 0;
    S2DTimer->start( 100 );
  } else {
    S2DStop0();
  }
}

void MainWindow::SetupS2DParams( void )
{
  for ( int i = 0; i < S2DAxis.count(); i++ ) {
    S2Dnow[i] = S2DMotors[i]->value().toInt();
    if ( S2DRelAbs[i]->stat() == ABS ) {
      S2Dsx[i] = S2DMotors[i]->u2p( S2DStarts[i]->text().toDouble() );
      S2Dex[i] = S2DMotors[i]->u2p( S2DEnds[i]->text().toDouble() );
    } else {
      S2Dsx[i] = S2DMotors[i]->u2p( S2DCurPos[i]->text().toDouble()
				    + S2DStarts[i]->text().toDouble() );
      S2Dex[i] = S2DMotors[i]->u2p( S2DCurPos[i]->text().toDouble()
				    + S2DEnds[i]->text().toDouble() );
    }
    S2Ddx[i] = ( S2Dex[i] - S2Dsx[i] ) / S2DPoints[i]->text().toDouble();
    S2Dsign[i] = ( S2Dex[i] > S2Dsx[i] ) ? 1 : -1;
    if ( S2Ddx[i] == 0 ) 
      S2Ddx[i] = S2Dsign[i];
  }
}

#define S2D_END_STAGE    ( 99 )

void MainWindow::S2DScanSequence( void )
{
  // �⡼����ư������äƤ��ʤ� (�Ȥꤢ�������ƥåפΤ��Ȥ����ͤ���)
  for ( int i = 0; i < S2DMotors.count(); i++ ) {
    if ( S2DMotorUse[i] && S2DMotors[i]->isBusy0() )
      return;
  }
  // ���󥵡� busy �Ǥ����äƤ��ʤ�
  if ( mUnits.isBusy() )
    return;

  int pps;

  switch( S2DStage ) {
  case 0:
    // �������Ф���
    for ( int i = 0; i < S2DMotors.count(); i++ ) {
      S2DMotors[i]->SetSpeed( HIGH );       // ���ԡ��ɥޥå���
      S2DMotors[i]->SetValue( S2Dsx[i] );   // �����˰�ư
      S2Dx[i] = S2Dsx[i];                   // ���ƥåץ���ȥ����ѿ������
    }
    // ���д�η�¬���֥��å�
    mUnits.setDwellTimes( S2DTime1->text().toDouble() );
    mUnits.setDwellTime();
    S2DStage++;
    break;
  case 1:
    // 1st Ax �Τߡ���������ѤΥ��ԡ��ɤ˥��å�
    pps = (int)fabs( (double)S2Ddx[0] / S2DTime1->text().toDouble() );
    if ( pps == 0 ) pps = 1;
    if ( pps > S2DMotors[0]->highSpeed() )
      pps = S2DMotors[0]->highSpeed();
    S2DMotors[0]->SetHighSpeed( pps );
    S2DStage++;
    // break ���ʤ�
  case 2:     // ��ԡ��ȥݥ����
    // ��¬���Ͻ���
    mUnits.clearStage();
    if ( ! mUnits.isParent() )  // �Ƥ����ʤ���м��Υ��ƥåפϤȤФ�
      S2DStage++;
    S2DStage++;
    break;
  case 3:
    if ( mUnits.getValue0() == false ) { // �ƥ�˥åȤν���
      mUnits.clearStage();
      S2DStage++;
    }
    break;
  case 4:
    if ( mUnits.getValue() == false ) {  // ��¬����
      mUnits.clearStage();
      S2DMotors[0]->SetValue( S2Dx[0] + S2Ddx[0] );   // Ʊ���˼������˰�ư����
      S2DStage++;
    }
    break;
  case 5:
    // ��¬���ɤ߼��
    mUnits.readValue( S2DVals, S2DCPSs, false );  // false : ���������������ʤ�
    // ����
    
    // ���ƥåץ���ȥ����ѿ�����
    S2Dx[0] += S2Ddx[0];
    if ( (( S2Dsign[0] > 0 ) && ( S2Dx[0] < S2Dex[0] ))  // 1st ax ��ü���Ǥʤ����
	 ||(( S2Dsign[0] < 0 ) && ( S2Dx[0] > S2Dex[0] )) ) {
      S2DMotors[0]->SetValue( S2Dx[0] );   // 1st ax �򼡤����˰�ư
      S2DStage = 2;
      break;
    }
    // 1st ax ��ü������ã���Ƥ�����
    S2Dx[0] = S2Dsx[0];
    S2Dx[1] += S2Ddx[1];
    if ( (( S2Dsign[1] > 0 ) && ( S2Dx[1] < S2Dex[1] ))       // 2nd ax ��ü���Ǥʤ����
	 ||(( S2Dsign[1] < 0 ) && ( S2Dx[1] > S2Dex[1] )) ) {
      S2DMotors[0]->SetHighSpeed( S2DMotors[0]->highSpeed() ); // 1st ax ���®�ˤ���
      S2DMotors[0]->SetValue( S2Dx[0] );   // 1st ax �ϸ������ᤷ
      S2DMotors[1]->SetValue( S2Dx[1] );   // 2nd ax �ϼ������˰�ư
      S2DStage = 1;    // 1st ax ��®�٤򥹥ƥå��Ѥ�®�٤��᤹
      break;
    }
    // 2nd ax ��ü����ã���Ƥ�����
    if ( S2DUse3rdAx->isChecked() ) {  // 3��������󤹤���
      S2Dx[1] = S2Dsx[1];
      S2Dx[2] += S2Ddx[2];
      if ( (( S2Dsign[2] > 0 ) && ( S2Dx[2] < S2Dex[2] ))   // 3rd ax ��ü���Ǥʤ����
	   ||(( S2Dsign[2] < 0 ) && ( S2Dx[2] > S2Dex[2] )) ) {
	S2DMotors[0]->SetHighSpeed( S2DMotors[0]->highSpeed() ); // 1st ax ���®�ˤ���
	S2DMotors[0]->SetValue( S2Dx[0] );   // 1st ax �ϸ������ᤷ
	S2DMotors[1]->SetValue( S2Dx[1] );   // 2nd ax �⸶�����ᤷ
	S2DMotors[2]->SetValue( S2Dx[2] );   // 3rd ax �ϼ������˰�ư
	S2DStage = 1;    // 1st ax ��®�٤򥹥ƥå��Ѥ�®�٤��᤹
	break;
      }
    }
    // ��λ
    S2DStage = S2D_END_STAGE;
    break;

  case S2D_END_STAGE:
    S2DMotors[0]->SetHighSpeed( S2DMotors[0]->highSpeed() ); // 1st ax ���®�ˤ���
    for ( int i = 0; i < S2DMotors.count(); i++ ) {          // �����������᤹
      S2DMotors[i]->SetValue( S2Dnow[i] );
    }
    // �Ȥꤢ�������ԡ��ɤϡ�High������ΥޥޤۤäȤ���
    S2DStage++;
    break;
  case S2D_END_STAGE+1:
    inS2D = false;
    NewLogMsg( QString( tr( "2D Scan Finished." ) ) );
    statusbar->showMessage( QString( tr( "2D Scan Finished." ) ), 2000 );
    S2DStart->setText( tr( "Start" ) );
    S2DStart->setStyleSheet( NormalB );
    S2DTimer->stop();
    break;
  }
}

void MainWindow::S2DStop0( void )
{
  S2DStart->setText( tr( "Start" ) );
  S2DStage = S2D_END_STAGE;
}
