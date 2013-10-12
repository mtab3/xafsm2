#include "MainWindow.h"

// Ϣ³�������Υ����ɤϾä��ơ����ƥåץ������������ò�����
// S2DStepF �Ȥ��Υ����å��Ϥ���γ��ǽ���äƤ��ΤȤ���

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

  QString msg;

  // �⡼����ư������äƤ��ʤ� (�Ȥꤢ�������ƥåפΤ��Ȥ����ͤ���)

  for ( int i = 0; i < S2DI.motors; i++ ) {
    if ( S2DI.used[i] && S2DI.unit[i]->isBusy0() )
      return;
  }

#if 0
  // ���󥵡� busy �Ǥ����äƤ��ʤ�
  if (( isS2DSFluo )&&( S2DStage >= 3 )) {
    if ( SFluo->isBusy2() )
      return;
  } else {
#endif
  if ( mUnits.isBusy() ) {
    return;
  }
#if 0
  }
#endif
  
  int pps;
  switch( S2DStage ) {
  case 0:
    // ���д�����
    if ( mUnits.init() == false ) {  // true :: initializing
      mUnits.clearStage();
      S2DStage++;
    }
    break;
  case 1:
    // ���д�η�¬���֥��å�
    mUnits.setDwellTimes( S2DI.Dwell );
    mUnits.setDwellTime();
    S2DStage++;
    break;
  case 2:
    // �������Ф���
    for ( int i = 0; i < S2DI.motors; i++ ) {
      if ( S2DI.used[i] ) {
	S2DI.unit[i]->SetHighSpeed( S2DI.unit[i]->highestSpeed() ); // ���ԡ��ɥޥå���
	S2DI.unit[i]->SetSpeed( HIGH );                          // ���ԡ��ɥޥå���
	S2DI.unit[i]->SetValue( S2DI.unit[i]->u2p( S2DI.sx[i] ) ); // �����˰�ư
	S2DI.i[i] = 0; // ���ƥåץ���ȥ������ѿ������
      }
    }
    S2DStage++;
    break;
  case 3:     // �⤦�ҤȤĤΥ�ԡ��ȥݥ����
    // 1st Ax �Τߡ���������ѤΥ��ԡ��ɤ˥��å�
    pps = (int)fabs( (double)S2DI.dx[0]
		     / S2DI.unit[0]->getUPP()
		     / S2DTime1->text().toDouble() );
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
    mUnits.clearStage();
    if ( ! mUnits.isParent() )  // �Ƥ����ʤ���м��Υ��ƥåפϤȤФ�
      S2DStage++;
    S2DStage++;
    break;
  case 5:
    if ( mUnits.getValue0() == false ) { // �ƥ�˥åȤν���
      mUnits.clearStage();
      S2DStage++;
    }
    break;
  case 6:
    if ( mUnits.getValue() == false ) {  // ��¬����
      mUnits.clearStage();
      // Ʊ���˼������˰�ư����
      S2DI.unit[0]->SetValue( S2DI.unit[0]
			      ->u2p( S2DI.sx[0] + (S2DI.i[0]+1)*S2DI.dx[0] ) );
      S2DStage++;
    }
    break;
  case 7:
    // ��¬���ɤ߼��
    mUnits.readValue( S2DVals, S2DCPSs, false );  // false : ���������������ʤ�
    // �ե����뵭Ͽ
    S2DWriteBody( S2DVals[0] );
    S2DWriteBody2( S2DI.i[0], S2DI.i[1] );
    S2DV->setData( S2DI.i[0], S2DI.i[1], S2DVals[0] ); // ����
    // ���ƥåץ���ȥ������ѿ�����
    S2DI.i[0]++;
    if ( S2DI.i[0] < S2DI.ps[0] ) { // 1st ax ��ü���Ǥʤ����
      S2DStage = 4;
      break;
    }
    // 1st ax ��ü������ã���Ƥ�����
    S2DWriteBlankLine();
    S2DI.i[0] = 0;
    S2DI.i[1]++;
    if ( S2DI.i[1] <= S2DI.ps[1] ) {       // 2nd ax ��ü���Ǥʤ����
      S2DI.unit[0]->SetHighSpeed( S2DI.unit[0]->highSpeed() ); // 1st ax ���®�ˤ���
      S2DI.unit[0]->SetValue( S2DI.unit[0]->u2p( S2DI.sx[0] ) ); // 1st ax �ϸ������ᤷ
      S2DI.unit[1]->SetValue( S2DI.unit[1]->u2p( S2DI.sx[1] + S2DI.i[1] * S2DI.dx[1] ) );
                                                               // 2nd ax �ϼ������˰�ư
      S2DStage = 3;    // 1st ax ��®�٤򥹥ƥå��Ѥ�®�٤��᤹
      break;
    }
    // 2nd ax ��ü����ã���Ƥ�����
    if ( S2DI.Use3rdAx ) {  // 3��������󤹤���
      S2DI.i[1] = 0;
      S2DI.i[2]++;
      if ( S2DI.i[2] <= S2DI.ps[2] ) {  // 3rd ax ��ü���Ǥʤ����
	S2DI.unit[0]->SetHighSpeed( S2DI.unit[0]->highSpeed() ); // 1st ax ���®�ˤ���
	S2DI.unit[0]->SetValue( S2DI.unit[0]->u2p( S2DI.sx[0] ) ); // 1st ax �ϸ������ᤷ
	S2DI.unit[1]->SetValue( S2DI.unit[1]->u2p( S2DI.sx[1] ) ); // 2nd ax �⸶�����ᤷ
	S2DI.unit[2]->SetValue( S2DI.unit[2]->u2p( S2DI.sx[2] + S2DI.i[2] * S2DI.dx[2] ) );
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
