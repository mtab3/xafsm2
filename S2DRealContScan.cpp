#include "MainWindow.h"

// SSD ��Ȥä�Ϣ³�������˸¤�
// nct08 ��Ʊ�����Ȥ��Ǥ���Ϥ�
void MainWindow::S2DRealContinuousScanSequence( void )
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

  QString msg;

  // �⡼����ư������äƤ��ʤ�
  // ��¬���̤Υ����ޡ��ǹԤ���
  for ( int i = 0; i < S2DMotors.count(); i++ ) {
    if ( S2DMotorUse[i] && S2DMotors[i]->isBusy0() )
      return;
  }
  // ���󥵡� busy �Ǥ����äƤ��ʤ� 
  // â������ö¬���Ϥ�Ƥ��ޤ��ȸ��д�Ϥ��ä� busy �ʤΤǡ������� busy2 ���������å�
  if ( S2DStage < 2 ) {
    if ( mUnits.isBusy() )
      return;
  } else {
    if ( mUnits.isBusy2() )
      return;
  }

  int pps;
  switch( S2DStage ) {
  case 0:
    // ���д�����
    if ( mUnits.init() ) // true :: initializing
      break;
    mUnits.clearStage();
    S2DStage++;
    break;
  case 1:
    if ( mUnits.getValue02() )
      break;
    mUnits.clearStage();
    S2DStage++;
    break;
  case 2:
    // �������Ф���
    for ( int i = 0; i < S2DMotors.count(); i++ ) {
      S2DMotors[i]->SetSpeed( HIGH );                          // ���ԡ��ɥޥå���
      S2DMotors[i]->SetValue( S2DMotors[i]->u2p( S2Dsx[i] ) ); // �����˰�ư
      S2Di[i] = 0; // ���ƥåץ���ȥ����ѿ������
    }
    S2DStage++;
    break;
  case 3:
    // 1st Ax �Τߡ���������ѤΥ��ԡ��ɤ˥��å�
    pps = (int)fabs( (double)S2Ddx[0]
		     / S2DMotors[0]->getUPP()
		     / S2DDwell );
    if ( pps == 0 ) pps = 1;
    if ( pps > S2DMotors[0]->highestSpeed() ) {
      msg = tr( "The scan speed %1 was limited to %2" )
	.arg( pps ).arg( S2DMotors[0]->highestSpeed() );
      qDebug() << msg;
      statusbar->showMessage( msg, 2000 );
      pps = S2DMotors[0]->highestSpeed();
    }
    S2DMotors[0]->SetHighSpeed( pps );
    S2DStage++;
    // break ���ʤ�
  case 4:     // ��ԡ��ȥݥ����
    // ��¬���Ͻ���
    S2Di[0] = -1;
    connect( mUnits.at(0), SIGNAL( newValue( QString ) ),
	     this, SLOT( S2DNewScanValue( QString ) ),
	     Qt::UniqueConnection );
    S2DTimer2->start( S2DDwell * 1000 );
    mUnits.getValue();
    // Ʊ���ˡֽ����פ˰�ư����
    if ( S2DScanDir == FORWARD ) {
      S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dex[0] ) );
      qDebug() << "SetV " << S2Dex[0];
    } else {
      S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] ) );
      qDebug() << "SetV " << S2Dsx[0];
    }
    S2DStage++;
    break;
  case 5:
    S2DTimer2->stop();
   // 1st ax ��ü������ã�����Τ�
    S2DWriteBlankLine();
    S2Di[1]++;
    if ( S2Di[1] <= S2Dps[1] ) {       // 2nd ax ��ü���Ǥʤ����
      S2DMotors[1]->SetValue( S2DMotors[1]->u2p( S2Dsx[1] + S2Di[1] * S2Ddx[1] ) );
      // 2nd ax �ϼ������˰�ư
      if ( ! S2DContScanBothDir->isChecked() ) { 
	S2DMotors[0]->SetHighSpeed( S2DMotors[0]->highSpeed() ); // 1st ax ���®�ˤ���
	S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] ) ); // 1st ax �ϸ������ᤷ
	S2DStage = 3;    // 1st ax ��®�٤򥹥ƥå��Ѥ�®�٤��᤹
      } else {
	S2DScanDir = ReversedDir( S2DScanDir );
	S2DStage = 4;    // ®���ѹ�������
      }
      break;
    }
    // 2nd ax ��ü����ã���Ƥ�����
    if ( S2DUse3rdAx->isChecked() ) {  // 3��������󤹤���
      S2Di[1] = 0;
      S2Di[2]++;
      if ( S2Di[2] <= S2Dps[2] ) {  // 3rd ax ��ü���Ǥʤ����
	S2DMotors[1]->SetValue( S2DMotors[1]->u2p( S2Dsx[1] ) ); // 2nd ax �ϸ������ᤷ
	S2DMotors[2]->SetValue( S2DMotors[2]->u2p( S2Dsx[2] + S2Di[2] * S2Ddx[2] ) );
	// 3rd ax �ϼ������˰�ư
	if ( ! S2DContScanBothDir->isChecked() ) { 
	  S2DMotors[0]->SetHighSpeed( S2DMotors[0]->highSpeed() ); // 1st ax ���®�ˤ���
	  S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] ) ); // 1st ax �ϸ������ᤷ
	  S2DStage = 3;    // 1st ax ��®�٤򥹥ƥå��Ѥ�®�٤��᤹
	} else {
	  S2DScanDir = ReversedDir( S2DScanDir );
	  S2DStage = 4;    // ®���ѹ�������
	}
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
    mUnits.clearStage();
    S2DStage++;
    break;
  case S2D_END_STAGE+1:
    if ( mUnits.Close() )
      break;
    S2DStage++;
    break;
  case S2D_END_STAGE+2:
    inS2D = false;
    UUnits.clear( S2D_ID );
    NewLogMsg( QString( tr( "2D Scan Finished." ) ) );
    statusbar->showMessage( QString( tr( "2D Scan Finished." ) ), 2000 );
    S2DStart->setText( tr( "Start" ) );
    S2DStart->setStyleSheet( NormalB );
    S2DTimer->stop();
    S2DTimer2->stop();
    break;
  }
}

void MainWindow::S2DRContScanMeas( void )
{
  mUnits.getValue();
}

void MainWindow::S2DNewScanValue( QString v )
{
  qDebug() << "newvalue " << v;
  double V = v.toDouble();
  if (( S2Di[0] >= 0 ) && ( S2Di[0] <= S2Dps[0] )) {
    // �ե����뵭Ͽ
    S2DWriteBody( V - S2DLastV );
    // ����
    if ( S2DScanDir == FORWARD ) {
      S2DV->setData( S2Di[0], S2Di[1], V - S2DLastV );
    } else {
      S2DV->setData( S2Dps[0] - S2Di[0] - 1, S2Di[1], V - S2DLastV );
    }
  }
  S2DLastV = V;
  S2Di[0]++;
}
