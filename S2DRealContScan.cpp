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
  for ( int i = 0; i < S2DI.motors; i++ ) {
    if ( S2DI.used[i] && S2DI.unit[i]->isBusy0() )
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
    for ( int i = 0; i < S2DI.motors; i++ ) {
      if ( S2DI.used[i] ) {
	S2DI.unit[i]->SetHighSpeed( S2DI.unit[i]->highestSpeed() ); // ���ԡ��ɥޥå���
	S2DI.unit[i]->SetSpeed( HIGH );                          // ���ԡ��ɥޥå���
	S2DI.unit[i]->SetValue( S2DI.unit[i]->u2p( S2DI.sx[i] ) ); // �����˰�ư
	S2DI.i[i] = 0; // ���ƥåץ���ȥ����ѿ������
      }
    }
    S2DStage++;
    break;
  case 3:
    // 1st Ax �Τߡ���������ѤΥ��ԡ��ɤ˥��å�
    pps = (int)fabs( (double)S2DI.dx[0] * S2DI.ps[0]
		     / S2DI.unit[0]->getUPP()
		     / S2DI.Dwell );
    if ( pps == 0 ) pps = 1;
    if ( pps > S2DI.unit[0]->highestSpeed() ) {
      msg = tr( "The scan speed %1 was limited to %2" )
	.arg( pps ).arg( S2DI.unit[0]->highestSpeed() );
      qDebug() << msg;
      statusbar->showMessage( msg, 2000 );
      pps = S2DI.unit[0]->highestSpeed();
    }
    S2DI.unit[0]->SetHighSpeed( pps );
    S2DStage++;
    // break ���ʤ�
  case 4:     // ��ԡ��ȥݥ����
    // ��¬���Ͻ���
    S2DI.i[0] = -1;
    connect( mUnits.at(0), SIGNAL( newValue( QString ) ),
	     this, SLOT( S2DNewScanValue( QString ) ),
	     Qt::UniqueConnection );
    S2DTimer2->start( S2DI.Dwell * 1000 / S2DI.ps[0] );
    mUnits.getValue();
    // Ʊ���ˡֽ����פ˰�ư����
    if ( S2DScanDir == FORWARD ) {
      S2DI.unit[0]->SetValue( S2DI.unit[0]->u2p( S2DI.ex[0] ) );
    } else {
      S2DI.unit[0]->SetValue( S2DI.unit[0]->u2p( S2DI.sx[0] ) );
    }
    S2DStage++;
    break;
  case 5:
    S2DTimer2->stop();
   // 1st ax ��ü������ã�����Τ�
    S2DWriteBlankLine();
    S2DI.i[1]++;
    if ( S2DI.i[1] <= S2DI.ps[1] ) {       // 2nd ax ��ü���Ǥʤ����
      S2DI.unit[1]->SetValue( S2DI.unit[1]->u2p( S2DI.sx[1] + S2DI.i[1] * S2DI.dx[1] ) );
      // 2nd ax �ϼ������˰�ư
      if ( ! S2DI.ScanBothDir ) { 
	S2DI.unit[0]->SetHighSpeed( S2DI.unit[0]->highSpeed() ); // 1st ax ���®�ˤ���
	S2DI.unit[0]->SetValue( S2DI.unit[0]->u2p( S2DI.sx[0] ) ); // 1st ax �ϸ������ᤷ
	S2DStage = 3;    // 1st ax ��®�٤򥹥ƥå��Ѥ�®�٤��᤹
      } else {
	S2DScanDir = ReversedDir( S2DScanDir );
	S2DStage = 4;    // ®���ѹ�������
      }
      break;
    }
    // 2nd ax ��ü����ã���Ƥ�����
    if ( S2DI.Use3rdAx ) {  // 3��������󤹤���
      S2DI.i[1] = 0;
      S2DI.i[2]++;
      if ( S2DI.i[2] <= S2DI.ps[2] ) {  // 3rd ax ��ü���Ǥʤ����
	S2DI.unit[1]->SetValue( S2DI.unit[1]->u2p( S2DI.sx[1] ) ); // 2nd ax �ϸ������ᤷ
	S2DI.unit[2]->SetValue( S2DI.unit[2]->u2p( S2DI.sx[2] + S2DI.i[2] * S2DI.dx[2] ) );
	// 3rd ax �ϼ������˰�ư
	if ( ! S2DI.ScanBothDir ) { 
	  S2DI.unit[0]->SetHighSpeed( S2DI.unit[0]->highSpeed() ); // 1st ax ���®�ˤ���
	  S2DI.unit[0]->SetValue( S2DI.unit[0]->u2p( S2DI.sx[0] ) ); // 1st ax �ϸ������ᤷ
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
    S2DI.unit[0]->SetHighSpeed( S2DI.unit[0]->highSpeed() ); // 1st ax ���®�ˤ���
    for ( int i = 0; i < S2DI.motors; i++ ) {          // �����������᤹
      if ( S2DI.used[i] )
	S2DI.unit[i]->SetValue( S2DI.now[i] );
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
    disconnect( mUnits.at(0), SIGNAL( newValue( QString ) ),
		this, SLOT( S2DNewScanValue( QString ) ) );
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
  double V = v.toDouble();
  
  if ( S2DScanDir == FORWARD ) 
    S2DWriteBody2( S2DI.i[0]+1, S2DI.i[1] );  // SSD �ε�Ͽ�Ϥ���
  else
    S2DWriteBody2( S2DI.ps[0]-S2DI.i[0]-1, S2DI.i[1] );  // SSD �ε�Ͽ�Ϥ���

  if (( S2DI.i[0] >= 0 ) && ( S2DI.i[0] <= S2DI.ps[0] )) {
    // �ե����뵭Ͽ
    S2DWriteBody( V - S2DLastV );
    // ����
    if ( S2DScanDir == FORWARD ) {
      S2DV->setData( S2DI.i[0], S2DI.i[1], V - S2DLastV );
    } else {
      S2DV->setData( S2DI.ps[0] - S2DI.i[0] - 1, S2DI.i[1], V - S2DLastV );
    }
  }
  S2DLastV = V;
  S2DI.i[0]++;
}
