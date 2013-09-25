
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
  S2Dps << 0 << 0 << 0;
  S2Dnow << 0 << 0 << 0;
  S2Dsx << 0 << 0 << 0;
  S2Dex << 0 << 0 << 0;
  S2Ddx << 0 << 0 << 0;
  S2Di << 0 << 0 << 0;

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

  for ( int i = 0; i < Changers.count(); i++ ) {
    // Changers[] �� MwChanger (MwSetup) �ȶ�ͭ
    S2DChangerSelect->addItem( Changers[i]->name() );
  }
  if ( Changers.count() > 0 ) {
    S2DUseChanger->setChecked( true );
    S2DUseChanger->setEnabled( true );
    S2DNewChangerSelected( S2DChangerSelect->currentIndex() );
    connect( S2DChangerSelect, SIGNAL( currentIndexChanged( int ) ),
	     this, SLOT( S2DNewChangerSelected( int ) ) );
    S2DUse3rdAx->setChecked( false );
    // S2DUse3rdAx->setEnabled( false );
  } else {
    S2DUseChanger->setChecked( false );
    S2DUseChanger->setEnabled( false );
    // S2DUse3rdAx->setEnabled( true );
  }

  inS2D = false;
  S2DTimer = new QTimer;
  isS2DSFluo = false;

  connect( S2DStart, SIGNAL( clicked() ), this, SLOT( S2DScanStart() ) );
  connect( S2DTimer, SIGNAL( timeout() ), this, SLOT( S2DScanSequence() ) );
  connect( S2DFileSelect, SIGNAL( clicked() ), S2DFileSel, SLOT( show() ) );
  connect( S2DFileSel, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( newS2DFileSelected( const QString & ) ) );
  for ( int i = 0; i < S2DStarts.count(); i++ ) {
    connect( S2DStarts[i], SIGNAL( editingFinished() ), this, SLOT( newS2DSteps() ) );
    connect( S2DEnds[i], SIGNAL( editingFinished() ), this, SLOT( newS2DSteps() ) );
    connect( S2DSteps[i], SIGNAL( editingFinished() ), this, SLOT( newS2DPoints() ) );
    connect( S2DPoints[i], SIGNAL( editingFinished() ), this, SLOT( newS2DSteps() ) );
  }
  connect( S2DUseChanger, SIGNAL( toggled(bool) ), this, SLOT( S2DSetUseChangers(bool) ) );
}

void MainWindow::S2DSetUseChangers( bool f )
{
  if ( f ) {
    S2DUse3rdAx->setChecked( false );
    // S2DUse3rdAx->setEnabled( false );
    S2DNewChangerSelected( S2DChangerSelect->currentIndex() );
  } else {
    // S2DUse3rdAx->setEnabled( true );
  }
}

void MainWindow::S2DNewChangerSelected( int i )
{
  if ( Changers.count() <= 0 )
    return;

#if 0
  disconnect( SIGNAL( newValue( QString ) ),
	      this, SLOT( ShowS2DChangerPosition( QString ) ) );
  connect( Changers[i]->unit1(), SIGNAL( newValue( QString ) ),
	   this, SLOT( ShowS2DChangerPosition( QString ) ) );
  connect( Changers[i]->unit2(), SIGNAL( newValue( QString ) ),
	   this, SLOT( ShowS2DChangerPosition( QString ) ) );

  Changers[i]->unit1()->GetValue();
  Changers[i]->unit2()->GetValue();
#endif

  for ( int j = 0; j < S2DOkMotors.count(); j++ ) {
    if ( Changers[i]->unit1() == S2DOkMotors[j] ) {
      S2DAx1->setCurrentIndex( j );
    }
    if ( Changers[i]->unit2() == S2DOkMotors[j] ) {
      S2DAx2->setCurrentIndex( j );
    }
  }
}

void MainWindow::newS2DPoints( void )
{
  int i;
  for ( i = 0; i < S2DSteps.count(); i++ ) {
    if ( S2DSteps[i] == sender() )
      break;
  }
  if ( i < S2DSteps.count() ) {
    double d = fabs( S2DSteps[i]->text().toDouble() );
    double s = S2DStarts[i]->text().toDouble();
    double e = S2DEnds[i]->text().toDouble();
    if ( d > 0 ) {
      int p = fabs( ( e - s ) / d );
      if ( p > 0 ) {
	S2DPoints[i]->setText( QString::number( p ) );
      } else {
	S2DPoints[i]->setText( "1" );
      }
    } else {
      S2DPoints[i]->setText( "1" );
    }
  }
}

void MainWindow::newS2DSteps( void )
{
  int i;
  for ( i = 0; i < S2DStarts.count(); i++ ) {
    if ( ( S2DStarts[i] == sender() )
	 ||( S2DEnds[i] == sender() )
	 ||( S2DPoints[i] == sender() ) )
      break;
  }
  if ( i < S2DStarts.count() ) {
    int p = abs( S2DPoints[i]->text().toInt() );
    if ( p > 0 )
      S2DSteps[i]
	->setText( QString::number( ( S2DEnds[i]->text().toDouble()
				      - S2DStarts[i]->text().toDouble() ) / p ) );
    else 
      S2DSteps[i]
	->setText( QString::number( ( S2DEnds[i]->text().toDouble()
				      - S2DStarts[i]->text().toDouble() ) / 1 ) );
  }
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

    QString User;
    for ( int i = 0; i < S2DMotors.count(); i++ ) {
      if ( S2DMotorUse[i] ) {
	// ��������ϰϤ��������뤫������å�
	if ( S2DStarts[i]->text().toDouble() == S2DEnds[i]->text().toDouble() ) {
	  statusbar->showMessage( tr( "2D Scan Range Error." ), 2000 );
	  return;
	}
	// ¬�������� 1�ʾ夢�뤳�Ȥ��ǧ
	if ( S2DPoints[i]->text().toInt() <= 0 ) {
	  statusbar->showMessage( tr( "2D Scan Points Error." ), 2000 );
	  return;
	}
	// �������֤äƤʤ��������å�
	for ( int j = i+1; j < S2DMotors.count(); j++ ) {
	  if ( S2DMotorUse[j] ) {
	    if ( S2DMotors[i] == S2DMotors[j] ) {
	      statusbar
		->showMessage( tr( "The same motor was selected for different axis." ),
			       2000 );
	      return;
	    }
	  }
	}

	if ( ( User = UUnits.isTheUnitInUse( S2DMotors[i] ) ) != "" ) {
	  // �⡼������¾�Τ��Ȥ˻Ȥ��Ƥ��������
	  statusbar->showMessage( tr( "The Motor [%1] is used by the process %2!" )
				  .arg( S2DMotors[i]->getName() ).arg( User ), 2000 );
	  return;
	}
      }
    }

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

      if ( ( User = UUnits.isTheUnitInUse( mUnits.at(i) ) ) != "" ) {
	// ���д郎¾�Τ��Ȥ˻Ȥ�줿�����
	statusbar->showMessage( tr( "The Sensor [%1] is used by the process %2!" )
				.arg( mUnits.at(i)->getName() ).arg( User ), 2000 );
	return;
      }
    }

    isS2DSFluo = false;
    if ( as == SFluo ) {
      isS2DSFluo = true;
    }
    if ( S2DStepScan->isChecked() ) {  // ���ƥåץ������
      S2DStepF = true;
    } else {
      // Ϣ³�������ϸ��ߥ��ݡ��Ȥ��Ƥ��ʤ�
      if ( ( SFluo == NULL )
	   ||( S2DOkSensors.value( SelectS2DSensor->currentIndex() ) != SFluo ) ) {
	NewLogMsg( tr( "Continuous scan is not available now." ) );
	return;
      }
      S2DStepF = false;
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

    SetupS2DParams();   // �������ѥ�᡼���� GUI ���������ѿ��˥��ԡ�
    // 1st �� 2nd �μ���ñ�̤�Ʊ���ʤ顢ɽ���νĲ���򥹥�����ϰϤ�
    // �Ĳ���˹�碌��褦�����Ϥ��롣
    // �����Ǥʤ���С����̤��äѤ���Ȥ���
    if ( S2DMotors[0]->getUnit() == S2DMotors[1]->getUnit() ) {
      S2DV->setRatioType( REAL_RATIO );
    } else {
      S2DV->setRatioType( AS_SCREEN );
    }
    S2DV->setRange( S2Dsx[0], S2Dsx[1],
		    S2Ddx[0], S2Ddx[1],
		    S2Dps[0], S2Dps[1]+1 );

    for ( int i = 0; i < S2DMotors.count(); i++ ) {
      if ( S2DMotorUse[i] ) {
	UUnits.addUnit( S2D_ID, S2DMotors[i] );
      }
    }
    for ( int i = 0; i < mUnits.count(); i++ ) {
      UUnits.addUnit( S2D_ID, mUnits.at(i) );
    }

    S2DLastV = 0;
    S2DFile = S2DFileName->text();
    S2DWriteHead();
    mUnits.clearStage();
    S2DStage = 0;
    S2DTimer->start( 10 );
  } else {
    S2DStop0();
  }
}

void MainWindow::SetupS2DParams( void )
{
  for ( int i = 0; i < S2DAxis.count(); i++ ) {
    S2Dnow[i] = S2DMotors[i]->value().toInt();
    if ( S2DRelAbs[i]->stat() == ABS ) {
      S2Dsx[i] = S2DStarts[i]->text().toDouble();
      S2Dex[i] = S2DEnds[i]->text().toDouble();
    } else {
      S2Dsx[i] = S2DCurPos[i]->text().toDouble() + S2DStarts[i]->text().toDouble();
      S2Dex[i] = S2DCurPos[i]->text().toDouble() + S2DEnds[i]->text().toDouble();
    }
    S2Dps[i] = abs( S2DPoints[i]->text().toInt() );
    if ( S2Dps[i] == 0 )
      S2Dps[i] = 1;
    S2Ddx[i] = ( S2Dex[i] - S2Dsx[i] ) / S2Dps[i];
    if ( S2Ddx[i] == 0 ) {
      //      S2Ddx[i] = S2Dsign[i];
    }
  }
}

#define S2D_END_STAGE    ( 99 )

void MainWindow::S2DStepScanSequence( void )
{
  // 1���ܤμ��ȡ�2, 3���ܤμ��ϡ����ƥåפιͤ������㤦��
  // 1���ܤμ��ϡ�¬����˼������˰�ư����褦�����椷�Ƥ���Τ�
  // s: -10, e: -10, periods 10 (step 2) �Ȥ���ȡ�
  //    -10��-8, -8��-6, ... 8��10 �� 10��¬�� + ��ư���롣
  // 2���ܤμ��ϡ�Ʊ����
  // s: -10, e: -10, periods 10 (step 2) �Ȥ���ȡ�
  //    -10, -8, -6, ... 8, 10 �� 10���ư����11��¬�ꤹ�롣
  // ���Τ��ᡢ1���ܤμ��ȡ�2���ܤμ���Ʊ���ˤ��Ƥ⡢��������¬��ˤʤ�ʤ���
  // �⤷Ʊ���ˤ��������
  // 1���ܤμ�:: s: -11, e: -11, periods 11 (step 2) 
  // 2���ܤμ�:: s: -10, e: -10, periods 10 (step 2) 
  // ���ͤˡ�1���ܤμ��λ����2���ܤμ��λ����Ⱦ���ƥåפ��餹ɬ�פ����롣

  // �⡼����ư������äƤ��ʤ� (�Ȥꤢ�������ƥåפΤ��Ȥ����ͤ���)

  for ( int i = 0; i < S2DMotors.count(); i++ ) {
    if ( S2DMotorUse[i] && S2DMotors[i]->isBusy0() )
      return;
  }
  // ���󥵡� busy �Ǥ����äƤ��ʤ�
  if ( S2DStepF ) {
    if ( mUnits.isBusy() ) {
      return;
    }
  } else {
    if ( isS2DSFluo ) {
      if ( S2DStage < 3 ) {
	if ( mUnits.isBusy() )
	  return;
      } else {
	if ( SFluo->isBusy2() ) {
	  return;
	}
      }
    } else {
      if ( mUnits.isBusy() )
	return;
    }
  }

  int Limit0;
  int pps;

  switch( S2DStage ) {
  case 0:
    // ���д�����
    if ( mUnits.init() == false ) {  // true :: initializing
      mUnits.clearStage();
      if ( S2DStepF )
	S2DStage = 2;
      else
	SFluo->setSSDPresetType( "NONE" );
	S2DStage = 1;
    }
    break;
  case 1:
    if ( SFluo->InitSensor() == false ) {  // true :: initializing
      SFluo->RunStart();
      SFluo->InitLocalStage();
      S2DStage = 3;
    }
    break;
  case 2:
    // ���д�η�¬���֥��å�
    mUnits.setDwellTimes( S2DTime1->text().toDouble() );
    mUnits.setDwellTime();
    S2DStage++;
    break;
  case 3:
    // �������Ф���
    for ( int i = 0; i < S2DMotors.count(); i++ ) {
      S2DMotors[i]->SetSpeed( HIGH );                          // ���ԡ��ɥޥå���
      S2DMotors[i]->SetValue( S2DMotors[i]->u2p( S2Dsx[i] ) ); // �����˰�ư
      S2Di[i] = 0; // ���ƥåץ���ȥ����ѿ������
    }
    S2DStage++;
    break;
  case 4:
    // 1st Ax �Τߡ���������ѤΥ��ԡ��ɤ˥��å�
    pps = (int)fabs( (double)S2Ddx[0]
		     / S2DMotors[0]->getUPP()
		     / S2DTime1->text().toDouble() );
    if ( pps == 0 ) pps = 1;
    if ( pps > S2DMotors[0]->highSpeed() )
      pps = S2DMotors[0]->highSpeed();
    S2DMotors[0]->SetHighSpeed( pps );
    S2DStage++;
    // break ���ʤ�
  case 5:     // ��ԡ��ȥݥ����
    // ��¬���Ͻ���
    if ( S2DStepF ) {
      mUnits.clearStage();
      if ( ! mUnits.isParent() )  // �Ƥ����ʤ���м��Υ��ƥåפϤȤФ�
	S2DStage++;
      S2DStage++;
    } else {                      // SSD ��Ϣ³¬��
      SFluo->GetMCAs();
      qDebug() << "get mca";
      // Ʊ���˼������˰�ư����
      S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] + (S2Di[0]+1)*S2Ddx[0] ) );
      S2DStage = 8;
    }
    break;
  case 6:
    if ( mUnits.getValue0() == false ) { // �ƥ�˥åȤν���
      mUnits.clearStage();
      S2DStage++;
    }
    break;
  case 7:
    if ( mUnits.getValue() == false ) {  // ��¬����
      mUnits.clearStage();
      // Ʊ���˼������˰�ư����
      S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] + (S2Di[0]+1)*S2Ddx[0] ) );
      S2DStage++;
    }
    break;
  case 8:
    // ��¬���ɤ߼��
    mUnits.readValue( S2DVals, S2DCPSs, false );  // false : ���������������ʤ�
    // �ե����뵭Ͽ
    S2DWriteBody( S2DVals[0] - S2DLastV );
    if ( !isS2DSFluo || S2DStepF || S2Di[0] > 0 ) { // �ָ�Ϣ³¬��ʤ�
      // ����
      S2DV->setData( S2Di[0] - ( (isS2DSFluo && !S2DStepF) ? 1 : 0 ),
		     S2Di[1], S2DVals[0] - S2DLastV );
    }
    S2DLastV = S2DVals[0];
    // ���ƥåץ���ȥ����ѿ�����
    S2Di[0]++;
    Limit0 = S2Dps[0];
    if ( isS2DSFluo && !S2DStepF ) // �ָ�Ϣ³¬��ʤ�
      Limit0 += 1;
    if ( S2Di[0] < Limit0 ) { // 1st ax ��ü���Ǥʤ����
      S2DStage = 5;
      break;
    }
    // 1st ax ��ü������ã���Ƥ�����
    S2DWriteBlankLine();
    S2Di[0] = 0;
    S2Di[1]++;
    if ( S2Di[1] <= S2Dps[1] ) {       // 2nd ax ��ü���Ǥʤ����
      S2DMotors[0]->SetHighSpeed( S2DMotors[0]->highSpeed() ); // 1st ax ���®�ˤ���
      S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] ) ); // 1st ax �ϸ������ᤷ
      S2DMotors[1]->SetValue( S2DMotors[1]->u2p( S2Dsx[1] + S2Di[1] * S2Ddx[1] ) );
                                                               // 2nd ax �ϼ������˰�ư
      S2DStage = 4;    // 1st ax ��®�٤򥹥ƥå��Ѥ�®�٤��᤹
      break;
    }
    // 2nd ax ��ü����ã���Ƥ�����
    if ( S2DUse3rdAx->isChecked() ) {  // 3��������󤹤���
      S2Di[1] = 0;
      S2Di[2]++;
      if ( S2Di[2] <= S2Dps[2] ) {  // 3rd ax ��ü���Ǥʤ����
	S2DMotors[0]->SetHighSpeed( S2DMotors[0]->highSpeed() ); // 1st ax ���®�ˤ���
	S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] ) ); // 1st ax �ϸ������ᤷ
	S2DMotors[1]->SetValue( S2DMotors[1]->u2p( S2Dsx[1] ) ); // 2nd ax �⸶�����ᤷ
	S2DMotors[2]->SetValue( S2DMotors[2]->u2p( S2Dsx[2] + S2Di[2] * S2Ddx[2] ) );
	                                                         // 3rd ax �ϼ������˰�ư
	S2DStage = 4;    // 1st ax ��®�٤򥹥ƥå��Ѥ�®�٤��᤹
	break;
      }
    }
    // ��λ
    S2DStage = S2D_END_STAGE;
    break;

  case S2D_END_STAGE:
    S2DWriteTail();
    S2DMotors[0]->SetHighSpeed( S2DMotors[0]->highSpeed() ); // 1st ax ���®�ˤ���
    for ( int i = 0; i < S2DMotors.count(); i++ ) {          // �����������᤹
      S2DMotors[i]->SetValue( S2Dnow[i] );
    }
    // �Ȥꤢ�������ԡ��ɤϡ�High������ΥޥޤۤäȤ���
    S2DStage++;
    break;
  case S2D_END_STAGE+1:
    inS2D = false;
    UUnits.clear( S2D_ID );
    NewLogMsg( QString( tr( "2D Scan Finished." ) ) );
    statusbar->showMessage( QString( tr( "2D Scan Finished." ) ), 2000 );
    S2DStart->setText( tr( "Start" ) );
    S2DStart->setStyleSheet( NormalB );
    S2DTimer->stop();
    break;
  }
}





// ScanSequence �Υ��ԡ�
// ���ƥåץ�����󤬴������Ƥ��ơ�Ϣ³�������������줫������ַ�
// �������顢Ϣ³������������Ƥ⤦����ñ�㲽������Τ������ߤ�
// StepScanSequence
#if 0         

void MainWindow::S2DScanSequence( void )
{
  // 1���ܤμ��ȡ�2, 3���ܤμ��ϡ����ƥåפιͤ������㤦��
  // 1���ܤμ��ϡ�¬����˼������˰�ư����褦�����椷�Ƥ���Τ�
  // s: -10, e: -10, periods 10 (step 2) �Ȥ���ȡ�
  //    -10��-8, -8��-6, ... 8��10 �� 10��¬�� + ��ư���롣
  // 2���ܤμ��ϡ�Ʊ����
  // s: -10, e: -10, periods 10 (step 2) �Ȥ���ȡ�
  //    -10, -8, -6, ... 8, 10 �� 10���ư����11��¬�ꤹ�롣
  // ���Τ��ᡢ1���ܤμ��ȡ�2���ܤμ���Ʊ���ˤ��Ƥ⡢��������¬��ˤʤ�ʤ���
  // �⤷Ʊ���ˤ��������
  // 1���ܤμ�:: s: -11, e: -11, periods 11 (step 2) 
  // 2���ܤμ�:: s: -10, e: -10, periods 10 (step 2) 
  // ���ͤˡ�1���ܤμ��λ����2���ܤμ��λ����Ⱦ���ƥåפ��餹ɬ�פ����롣

  // �⡼����ư������äƤ��ʤ� (�Ȥꤢ�������ƥåפΤ��Ȥ����ͤ���)

  for ( int i = 0; i < S2DMotors.count(); i++ ) {
    if ( S2DMotorUse[i] && S2DMotors[i]->isBusy0() )
      return;
  }
  // ���󥵡� busy �Ǥ����äƤ��ʤ�
  if ( S2DStepF ) {
    if ( mUnits.isBusy() ) {
      return;
    }
  } else {
    if ( isS2DSFluo ) {
      if ( S2DStage < 3 ) {
	if ( mUnits.isBusy() )
	  return;
      } else {
	if ( SFluo->isBusy2() ) {
	  return;
	}
      }
    } else {
      if ( mUnits.isBusy() )
	return;
    }
  }

  int Limit0;
  int pps;

  switch( S2DStage ) {
  case 0:
    // ���д�����
    if ( mUnits.init() == false ) {  // true :: initializing
      mUnits.clearStage();
      if ( S2DStepF )
	S2DStage = 2;
      else
	SFluo->setSSDPresetType( "NONE" );
	S2DStage = 1;
    }
    break;
  case 1:
    if ( SFluo->InitSensor() == false ) {  // true :: initializing
      SFluo->RunStart();
      SFluo->InitLocalStage();
      S2DStage = 3;
    }
    break;
  case 2:
    // ���д�η�¬���֥��å�
    mUnits.setDwellTimes( S2DTime1->text().toDouble() );
    mUnits.setDwellTime();
    S2DStage++;
    break;
  case 3:
    // �������Ф���
    for ( int i = 0; i < S2DMotors.count(); i++ ) {
      S2DMotors[i]->SetSpeed( HIGH );                          // ���ԡ��ɥޥå���
      S2DMotors[i]->SetValue( S2DMotors[i]->u2p( S2Dsx[i] ) ); // �����˰�ư
      S2Di[i] = 0; // ���ƥåץ���ȥ����ѿ������
    }
    S2DStage++;
    break;
  case 4:
    // 1st Ax �Τߡ���������ѤΥ��ԡ��ɤ˥��å�
    pps = (int)fabs( (double)S2Ddx[0]
		     / S2DMotors[0]->getUPP()
		     / S2DTime1->text().toDouble() );
    if ( pps == 0 ) pps = 1;
    if ( pps > S2DMotors[0]->highSpeed() )
      pps = S2DMotors[0]->highSpeed();
    S2DMotors[0]->SetHighSpeed( pps );
    S2DStage++;
    // break ���ʤ�
  case 5:     // ��ԡ��ȥݥ����
    // ��¬���Ͻ���
    if ( S2DStepF ) {
      mUnits.clearStage();
      if ( ! mUnits.isParent() )  // �Ƥ����ʤ���м��Υ��ƥåפϤȤФ�
	S2DStage++;
      S2DStage++;
    } else {                      // SSD ��Ϣ³¬��
      SFluo->GetMCAs();
      qDebug() << "get mca";
      // Ʊ���˼������˰�ư����
      S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] + (S2Di[0]+1)*S2Ddx[0] ) );
      S2DStage = 8;
    }
    break;
  case 6:
    if ( mUnits.getValue0() == false ) { // �ƥ�˥åȤν���
      mUnits.clearStage();
      S2DStage++;
    }
    break;
  case 7:
    if ( mUnits.getValue() == false ) {  // ��¬����
      mUnits.clearStage();
      // Ʊ���˼������˰�ư����
      S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] + (S2Di[0]+1)*S2Ddx[0] ) );
      S2DStage++;
    }
    break;
  case 8:
    // ��¬���ɤ߼��
    mUnits.readValue( S2DVals, S2DCPSs, false );  // false : ���������������ʤ�
    // �ե����뵭Ͽ
    S2DWriteBody( S2DVals[0] - S2DLastV );
    if ( !isS2DSFluo || S2DStepF || S2Di[0] > 0 ) { // �ָ�Ϣ³¬��ʤ�
      // ����
      S2DV->setData( S2Di[0] - ( (isS2DSFluo && !S2DStepF) ? 1 : 0 ),
		     S2Di[1], S2DVals[0] - S2DLastV );
    }
    S2DLastV = S2DVals[0];
    // ���ƥåץ���ȥ����ѿ�����
    S2Di[0]++;
    Limit0 = S2Dps[0];
    if ( isS2DSFluo && !S2DStepF ) // �ָ�Ϣ³¬��ʤ�
      Limit0 += 1;
    if ( S2Di[0] < Limit0 ) { // 1st ax ��ü���Ǥʤ����
      S2DStage = 5;
      break;
    }
    // 1st ax ��ü������ã���Ƥ�����
    S2DWriteBlankLine();
    S2Di[0] = 0;
    S2Di[1]++;
    if ( S2Di[1] <= S2Dps[1] ) {       // 2nd ax ��ü���Ǥʤ����
      S2DMotors[0]->SetHighSpeed( S2DMotors[0]->highSpeed() ); // 1st ax ���®�ˤ���
      S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] ) ); // 1st ax �ϸ������ᤷ
      S2DMotors[1]->SetValue( S2DMotors[1]->u2p( S2Dsx[1] + S2Di[1] * S2Ddx[1] ) );
                                                               // 2nd ax �ϼ������˰�ư
      S2DStage = 4;    // 1st ax ��®�٤򥹥ƥå��Ѥ�®�٤��᤹
      break;
    }
    // 2nd ax ��ü����ã���Ƥ�����
    if ( S2DUse3rdAx->isChecked() ) {  // 3��������󤹤���
      S2Di[1] = 0;
      S2Di[2]++;
      if ( S2Di[2] <= S2Dps[2] ) {  // 3rd ax ��ü���Ǥʤ����
	S2DMotors[0]->SetHighSpeed( S2DMotors[0]->highSpeed() ); // 1st ax ���®�ˤ���
	S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] ) ); // 1st ax �ϸ������ᤷ
	S2DMotors[1]->SetValue( S2DMotors[1]->u2p( S2Dsx[1] ) ); // 2nd ax �⸶�����ᤷ
	S2DMotors[2]->SetValue( S2DMotors[2]->u2p( S2Dsx[2] + S2Di[2] * S2Ddx[2] ) );
	                                                         // 3rd ax �ϼ������˰�ư
	S2DStage = 4;    // 1st ax ��®�٤򥹥ƥå��Ѥ�®�٤��᤹
	break;
      }
    }
    // ��λ
    S2DStage = S2D_END_STAGE;
    break;

  case S2D_END_STAGE:
    S2DWriteTail();
    S2DMotors[0]->SetHighSpeed( S2DMotors[0]->highSpeed() ); // 1st ax ���®�ˤ���
    for ( int i = 0; i < S2DMotors.count(); i++ ) {          // �����������᤹
      S2DMotors[i]->SetValue( S2Dnow[i] );
    }
    // �Ȥꤢ�������ԡ��ɤϡ�High������ΥޥޤۤäȤ���
    S2DStage++;
    break;
  case S2D_END_STAGE+1:
    inS2D = false;
    UUnits.clear( S2D_ID );
    NewLogMsg( QString( tr( "2D Scan Finished." ) ) );
    statusbar->showMessage( QString( tr( "2D Scan Finished." ) ), 2000 );
    S2DStart->setText( tr( "Start" ) );
    S2DStart->setStyleSheet( NormalB );
    S2DTimer->stop();
    break;
  }
}

#endif












void MainWindow::S2DStop0( void )
{
  S2DStart->setText( tr( "Start" ) );
  S2DStage = S2D_END_STAGE;
}

void MainWindow::S2DWriteHead( void )
{
  if ( S2DFile.simplified().isEmpty() )
    return;

  QFile f( S2DFile );

  if ( !f.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;

  // Writing fixed headers
  QTextStream out(&f);

  // "#" �� ":" �δ֤� " " ���� 11��

  out << "# 1306 Aichi SR 2D Scan" << endl;
  out << "#" << " Date      : " << QDateTime::currentDateTime()
                                     .toString("yy.MM.dd hh:mm:ss.zzz") << endl;
  if ( SLS != NULL ) 
    out << "#" << " Ring Cur. : " << SLS->value().toDouble() << "[mA]" << endl;

  for ( int i = 0; i < S2DMotors.count(); i++ ) {
    if ( S2DMotorUse[i] ) {
      out << "#" << QString( " Axis %1    : " ).arg( i, 1 )
	  << QString( " %1" ).arg( S2Dsx[i], 10 )
	  << QString( " %1" ).arg( S2Dex[i], 10 )
	  << QString( " %1" ).arg( S2Ddx[i], 10 )
	  << QString( " %1" ).arg( S2Dps[i], 10 )
	  << " : " << S2DMotors[i]->getName() << endl;
    }
  }
  out << "#" << endl;

  f.close();
}

void MainWindow::S2DWriteBody( double v )
{
  QFile f( S2DFile );

  if ( !f.open( QIODevice::Append | QIODevice::Text ) )
    return;

  QTextStream out(&f);

  for ( int i = 0; i < S2DMotors.count(); i++ ) {
    if ( S2DMotorUse[i] ) {
      out << QString( " %1" ).arg( S2Dsx[i] + S2Di[i] * S2Ddx[i], 10 );
    }
  }
  out <<  QString( " %1" ).arg( v, 10 ) << endl;

  f.close();

  S2DWriteBody2();
}

void MainWindow::S2DWriteBody2( void )
{
  if ( isS2DSFluo && S2DRecordMCA->isChecked() ) {
    QFileInfo BaseFile( S2DFile );
    QFileInfo mcaFile;
    if ( S2DUse3rdAx->isChecked() ) {
      mcaFile = QFileInfo( mcaDir,
			   QString( "%1-%2-%3-%4.dat" )
			   .arg( BaseFile.baseName() )
			   .arg( S2Di[0], 4, 10, QChar( '0' ) )
			   .arg( S2Di[1], 4, 10, QChar( '0' ) )
			   .arg( S2Di[2], 4, 10, QChar( '0' ) ) );
    } else {
      mcaFile = QFileInfo( mcaDir,
			   QString( "%1-%2-%3.dat" )
			   .arg( BaseFile.baseName() )
			   .arg( S2Di[0], 4, 10, QChar( '0' ) )
			   .arg( S2Di[1], 4, 10, QChar( '0' ) ) );
    }
    saveMCAData0( mcaFile.canonicalFilePath() );
  }
}

void MainWindow::S2DWriteBlankLine( void )
{
  QFile f( S2DFile );

  if ( !f.open( QIODevice::Append | QIODevice::Text ) )
    return;

  QTextStream out(&f);
  //  out << "#" << endl;
  out << "" << endl;         // gnuplot �� # ���ȡ��ǡ�������ڤ��ܤ��ȻפäƤ���ʤ�

  f.close();
}

void MainWindow::S2DWriteTail( void )  // ��λ���λ��֤� I0 ������Ͽ (�ե���������)
{
  QFile f( S2DFile );

  if ( !f.open( QIODevice::Append | QIODevice::Text ) )
    return;

  QTextStream out(&f);

  out << "#" << " Date      : " << QDateTime::currentDateTime()
                                     .toString("yy.MM.dd hh:mm:ss.zzz") << endl;
  if ( SLS != NULL ) 
    out << "#" << " Ring Cur. : " << SLS->value().toDouble() << "[mA]" << endl;

  f.close();
}

