#include "MainWindow.h"

void MainWindow::setupChangerArea( void )
{
  for ( int i = 0; i < Changers.count(); i++ ) {
    for ( int j = 0; j < AMotors.count(); j++ ) {
      if ( Changers[i]->unitID1() == AMotors[j]->getUid() ) {
	Changers[i]->setUnit1( AMotors[j] );
	//	qDebug() << "Changer " << i << "unit1 " << AMotors[j]->getName();
      }
      if ( Changers[i]->unitID2() == AMotors[j]->getUid() ) {
	Changers[i]->setUnit2( AMotors[j] );
	//	qDebug() << "Changer " << i << "unit2 " << AMotors[j]->getName();
      }
    }
    ChangerSelect->addItem( Changers[i]->name() );
  }
  NewChangerSelected( ChangerSelect->currentIndex() );
  connect( ChangerSelect, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( NewChangerSelected( int ) ) );
  connect( SetChangerCenter, SIGNAL( clicked() ), this, SLOT( SetNewChangerCenter() ) );
  connect( ChangerGo, SIGNAL( clicked() ), this, SLOT( ChangerGoToNewPosition() ) );
  connect( DatumChanger, SIGNAL( clicked() ), this, SLOT( StartDatumChanger() ) );
  connect( this, SIGNAL( ChangerNext() ), this, SLOT( AutoSequence() ) );
}

void MainWindow::StartDatumChanger( void )
{
}

void MainWindow::AutoMeasurement( void )
{
  MeasA = 0;
  AutoModeFirst = true;   // �̾�/AutoMode �ˤ�����餺 1 ���ܤ�¬��ե饰��Ω�ƤƤ���
                          // �ե�����ξ�񤭥����å������椹�뤿��
  if ( AutoModeButton->isChecked() ) {
    if ( ! ParseAutoMode() ) {
      statusbar->showMessage( tr( "Auto mode parameters are illigal."),
                              2000 );
      return;
    }
    if ( AutoModeParams.count() == 0 ) {
      statusbar->showMessage( tr( "Auto mode parameters are not set."),
                              2000 );
      return;
    }
    // In automode, a user should choose "Use Measured" to prevent stopping.
    if ( MeasBackBeforeMeas->isChecked() ) {
      statusbar->showMessage( tr( "Please choose \"Use Measured\""),
                              2000 );
      return;
    }
    // AutoMode->setDisabled( true );  // Now, there is no need to set it to be disabled
    MeasA = 0;
    // AutoModeParams.remove( 0 );
    moveToTarget( AutoModeParams[ MeasA ].num,
                  AutoModeParams[ MeasA ].dx,
                  AutoModeParams[ MeasA ].dz );
  }
  StartMeasurement();
}

void MainWindow::AutoSequence( void )
{
  AutoModeFirst = false;

  //  qDebug() << "AutoSequence";
  //  qDebug() << "MeasA" << MeasA
  //           << "AutoModeParams.count()" << AutoModeParams.count();

  if ( ( MeasA + 1 ) >=  AutoModeParams.count() ) {
    Changers[ ChangerSelect->currentIndex() ]->unit1()->SetValue( 0 );
    Changers[ ChangerSelect->currentIndex() ]->unit2()->SetValue( 0 );
    disconnect( MMainTh, SIGNAL( ChangedIsBusy1( QString ) ),
                this, SLOT( AutoSequence() ) );
  } else if ( !(MMainTh->isBusy()) ) {
    MeasA++;
    disconnect( MMainTh, SIGNAL( ChangedIsBusy1( QString ) ),
                this, SLOT( AutoSequence() ) );
    moveToTarget( AutoModeParams[ MeasA ].num,
                  AutoModeParams[ MeasA].dx,
                  AutoModeParams[ MeasA ].dz );
    StartMeasurement();
  }
}

void MainWindow::SetNewChangerCenter( void )
{
  Changer *changer = Changers[ ChangerSelect->currentIndex() ];
  AUnit *c1 = changer->unit1();
  AUnit *c2 = changer->unit2();

  int target = ChangerCurrentHolder->value();       // Target �� 1 �Ϥޤ������
  int ix = ( target - 1 ) % changer->holders1() - changer->center1();    // -1, 0, 1
  int iz = ( target - 1 ) / changer->holders2() - changer->center2();    // -1, 0, 1
  int nowx = c1->value().toInt();
  int nowz = c2->value().toInt();
  c1->setCenter( nowx + ix * changer->spacing1() / c1->getUPP() * changer->dir1() );
  c2->setCenter( nowz + iz * changer->spacing2() / c2->getUPP() * changer->dir2() );
}

void MainWindow::moveToTarget( int target, double dx, double dz )
{
  Changer *changer = Changers[ ChangerSelect->currentIndex() ];
  AUnit *c1 = changer->unit1();
  AUnit *c2 = changer->unit2();

  MovingToNewSamplePosition = true;   // ���Υե饰�ǰ�ư���¬���֥�å�����
  int ix = ( target - 1 ) % changer->holders1() - changer->center1();    // -1, 0, 1
  int iz = ( target - 1 ) / changer->holders2() - changer->center2();    // -1, 0, 1
  int targetx = c1->u2p( changer->spacing1() * ix * changer->dir1() )
                   + dx / c1->getUPP() * changer->dir1();
  int targetz = c2->u2p( changer->spacing2() * iz * changer->dir2() )
                   + dz / c2->getUPP() * changer->dir2();
  DFName00 = QString("_%1").arg( (int)MeasA, 4, 10, QChar( '0' ) );

  AutoModeComment = QString( "Sample No. %1 +%2[mm] +%3[mm]" )
    .arg( target ).arg( dx ).arg( dz );
  c1->SetValue( targetx );
  c2->SetValue( targetz );
}

void MainWindow::ChangerGoToNewPosition( void )
{
  moveToTarget( ChangerToGoHolderSelect->value(),
		ChangerToGoFinePosition1->text().toDouble(),
		ChangerToGoFinePosition2->text().toDouble() );

  ChangerGo->setText( tr( "Moving" ) );
  ChangerGo->setStyleSheet( InActive );
}

void MainWindow::NewChangerSelected( int i )
{
  ChangerToGoHolderSelect->setMinimum( 1 );
  ChangerToGoHolderSelect->setMaximum( Changers[ i ]->holders() );
  ChangerCurrentHolder->setMinimum( 1 );
  ChangerCurrentHolder->setMaximum( Changers[ i ]->holders() );
  ChangerUnit1->setText( Changers[i]->unit1()->getUnit() );
  ChangerUnit2->setText( Changers[i]->unit2()->getUnit() );
}
