#include "MainWindow.h"

// SSD ��Ȥä�Ϣ³�������˸¤�
// nct08 ��Ʊ�����Ȥ��Ǥ���Ϥ�
void MainWindow::S2DQuasiContinuousScanSequence( void )
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
  // â������ö¬���Ϥ�Ƥ��ޤ��ȸ��д�Ϥ��ä� busy �ʤΤǡ������� busy2 ���������å�
  if ( S2DStage < 2 ) {
    if ( mUnits.isBusy() )
      return;
  } else {
    if ( mUnits.isBusy2() )
      return;
  }

  qDebug() << "Stage " << S2DStage
	   << QDateTime::currentDateTime().toString("yy.MM.dd hh:mm:ss.zzz");

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
		     / S2DTime1->text().toDouble() );
    if ( pps == 0 ) pps = 1;
    if ( pps > S2DMotors[0]->highSpeed() )
      pps = S2DMotors[0]->highSpeed();
    S2DMotors[0]->SetHighSpeed( pps );
    S2DStage++;
    // break ���ʤ�
  case 4:     // ��ԡ��ȥݥ����
    // ��¬���Ͻ���
    mUnits.getValue();
    // Ʊ���˼������˰�ư����
    S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] + (S2Di[0]+1)*S2Ddx[0] ) );
    S2DStage = 5;
    break;
  case 5:
    // ��¬���ɤ߼��
    // Ϣ³¬��ʤΤǡ��ƥ饤��κǽ���ͤϡ�����ޤǤ���ʬ�ͤˤʤ롣
    // di0 = 0, 1, 2, ... 10 �� 11 ���η�¬��Ԥ��Ȥ���
    // di0 = 0 �����ϡ������ޤǤΥ⡼���ΰ�ư��������ޤ����ʬ��
    // V(1)-V(0), V(2)-V(1), V(3)-V(2),... V(10)-V(9) �� 10 ���ͤ���
    // ɽ������Ͽ���о�
    mUnits.readValue( S2DVals, S2DCPSs, false );  // false : ���������������ʤ�
    if ( S2Di[0] > 0 ) {
      // �ե����뵭Ͽ
      S2DWriteBody( S2DVals[0] - S2DLastV );
      // ����
      S2DV->setData( S2Di[0] - 1,
		     S2Di[1], S2DVals[0] - S2DLastV );
    }
    S2DLastV = S2DVals[0];
    // ���ƥåץ���ȥ����ѿ�����
    S2Di[0]++;
    // ��˽񤤤�����ǡ��տޤ������������ʤ���¬�ˤʤ�ʬ�������������䤷�Ƥ���
    if ( S2Di[0] < S2Dps[0]+1 ) { // 1st ax ��ü���Ǥʤ����
      S2DStage = 4;
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
      S2DStage = 3;    // 1st ax ��®�٤򥹥ƥå��Ѥ�®�٤��᤹
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
	S2DStage = 3;    // 1st ax ��®�٤򥹥ƥå��Ѥ�®�٤��᤹
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
    qDebug() << "before";
    if ( mUnits.Close() )
      break;
    qDebug() << "after";
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
    break;
  }
}
