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
  if ( Changers.count() > 0 ) {
    AutoModeButton->setEnabled( true );
    ChangerGo->setEnabled( true );
    SetChangerCenter->setEnabled( true );
    DatumChanger->setEnabled( true );
    NewChangerSelected( ChangerSelect->currentIndex() );
  } else {
    AutoModeButton->setEnabled( false );
    ChangerGo->setEnabled( false );
    SetChangerCenter->setEnabled( false );
    DatumChanger->setEnabled( false );
  }
  connect( ChangerSelect, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( NewChangerSelected( int ) ),
	   Qt::UniqueConnection );
  connect( SetChangerCenter, SIGNAL( clicked() ), this, SLOT( SetNewChangerCenter() ),
	   Qt::UniqueConnection );
  connect( ChangerGo, SIGNAL( clicked() ), this, SLOT( ChangerGoToNewPosition() ),
	   Qt::UniqueConnection );
  connect( DatumChanger, SIGNAL( clicked() ), this, SLOT( StartDatumChanger() ),
	   Qt::UniqueConnection );
  connect( this, SIGNAL( ChangerNext() ), this, SLOT( AutoXAFSSequence() ),
	   Qt::UniqueConnection );
  
  movingC1 = movingC2 = NULL;
}

void MainWindow::StartDatumChanger( void )
{
}

void MainWindow::AutoMeasurement( void )
{
  MeasA = 0;
  AutoModeFirst = true;   // 通常/AutoMode にかかわらず 1 回目の測定フラグは立てておく
                          // ファイルの上書きチェックを制御するため
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
    moveToTarget( Changers[ ChangerSelect->currentIndex() ],
		  AutoModeParams[ MeasA ].num,
                  AutoModeParams[ MeasA ].dx,
                  AutoModeParams[ MeasA ].dz );
  }
  StartMeasurement();
}

void MainWindow::AutoXAFSSequence( void )
{
  if ( Changers.count() <= 0 )
    return;

  AutoModeFirst = false;

  //  qDebug() << "AutoXAFSSequence";
  //  qDebug() << "MeasA" << MeasA
  //           << "AutoModeParams.count()" << AutoModeParams.count();

  if ( ( MeasA + 1 ) >=  AutoModeParams.count() ) {
    Changers[ ChangerSelect->currentIndex() ]->unit1()->SetValue( 0 );
    Changers[ ChangerSelect->currentIndex() ]->unit2()->SetValue( 0 );
    disconnect( MMainTh, SIGNAL( ChangedIsBusy1( QString ) ),
                this, SLOT( AutoXAFSSequence() ) );
  } else if ( !(MMainTh->isBusy()) ) {
    MeasA++;
    disconnect( MMainTh, SIGNAL( ChangedIsBusy1( QString ) ),
                this, SLOT( AutoXAFSSequence() ) );
    moveToTarget( Changers[ ChangerSelect->currentIndex() ],
		  AutoModeParams[ MeasA ].num,
                  AutoModeParams[ MeasA].dx,
                  AutoModeParams[ MeasA ].dz );
    StartMeasurement();
  }
}

void MainWindow::SetNewChangerCenter( void )
{
  if ( Changers.count() <= 0 )
    return;

  Changer *changer = Changers[ ChangerSelect->currentIndex() ];
  AUnit *c1 = changer->unit1();
  AUnit *c2 = changer->unit2();

  int target = ChangerCurrentHolder->value();       // Target は 1 始まりの整数
  double ix = ( target - 1 ) % changer->holders1() - changer->center1();    // -1, 0, 1
  double iz = ( target - 1 ) / changer->holders1() - changer->center2();    // -1, 0, 1
  int nowx = c1->value().toInt();
  int nowz = c2->value().toInt();
  c1->setCenter( nowx - ix * changer->spacing1() / c1->getUPP() * changer->dir1() );
  c2->setCenter( nowz - iz * changer->spacing2() / c2->getUPP() * changer->dir2() );
}

void MainWindow::moveToTarget( Changer *changer, int target, double dx, double dz )
{
#if 0
  if ( Changers.count() <= 0 )
    return;
#endif

  //  Changer *changer = Changers[ ChangerSelect->currentIndex() ];
  AUnit *c1 = changer->unit1();
  AUnit *c2 = changer->unit2();

  MovingToNewSamplePosition = true;   // このフラグで移動中の測定をブロックする
  double ix = ( target - 1 ) % changer->holders1() - changer->center1();    // -1, 0, 1
  double iz = ( target - 1 ) / changer->holders1() - changer->center2();    // -1, 0, 1
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
  if ( Changers.count() <= 0 )
    return;

  Changer *changer = Changers[ ChangerSelect->currentIndex() ];
  movingC1 = changer->unit1();
  movingC2 = changer->unit2();
  connect( movingC1, SIGNAL( ChangedIsBusy1( QString ) ),
	   this, SLOT( ChangerReached( QString ) ),
	   Qt::UniqueConnection );
  connect( movingC2, SIGNAL( ChangedIsBusy1( QString ) ),
	   this, SLOT( ChangerReached( QString ) ),
	   Qt::UniqueConnection );

  moveToTarget( Changers[ ChangerSelect->currentIndex() ],
		ChangerToGoHolderSelect->value(),
		ChangerToGoFinePosition1->text().toDouble(),
		ChangerToGoFinePosition2->text().toDouble() );

  ChangerGo->setText( tr( "Moving" ) );
  ChangerGo->setStyleSheet( InActive );
  ChangerGo->setEnabled( false );
}

void MainWindow::ChangerReached( QString )
{
  if ( movingC1->isBusy() || movingC2->isBusy() ) 
    return;

  disconnect( movingC1, SIGNAL( ChangedIsBusy1( QString ) ),
	   this, SLOT( ChangerReached( QString ) ) );
  disconnect( movingC2, SIGNAL( ChangedIsBusy1( QString ) ),
	   this, SLOT( ChangerReached( QString ) ) );

  ChangerGo->setText( tr( "MoveTo" ) );
  ChangerGo->setStyleSheet( NormalB );
  ChangerGo->setEnabled( true );
}

void MainWindow::SChangerReached( QString )
{
  if ( movingSC1->isBusy() || movingSC2->isBusy() ) 
    return;

  disconnect( movingSC1, SIGNAL( ChangedIsBusy1( QString ) ),
	   this, SLOT( ChangerReached( QString ) ) );
  disconnect( movingSC2, SIGNAL( ChangedIsBusy1( QString ) ),
	   this, SLOT( ChangerReached( QString ) ) );

  AttenMoveTo->setText( tr( "GoThere" ) );
  AttenMoveTo->setStyleSheet( NormalB );
  AttenMoveTo->setEnabled( true );
}

void MainWindow::NewChangerSelected( int i )
{
  if ( Changers.count() <= 0 )
    return;

  ChangerToGoHolderSelect->setMinimum( 1 );
  ChangerToGoHolderSelect->setMaximum( Changers[ i ]->holders() );
  ChangerCurrentHolder->setMinimum( 1 );
  ChangerCurrentHolder->setMaximum( Changers[ i ]->holders() );
  ChangerUnit1->setText( Changers[i]->unit1()->getUnit() );
  ChangerUnit2->setText( Changers[i]->unit2()->getUnit() );

  disconnect( SIGNAL( newValue( QString ) ),
	      this, SLOT( ShowChangerPosition( QString ) ) );
  connect( Changers[i]->unit1(), SIGNAL( newValue( QString ) ),
	   this, SLOT( ShowChangerPosition( QString ) ),
	   Qt::UniqueConnection );
  connect( Changers[i]->unit2(), SIGNAL( newValue( QString ) ),
	   this, SLOT( ShowChangerPosition( QString ) ),
	   Qt::UniqueConnection );
  Changers[i]->unit1()->GetValue();
  Changers[i]->unit2()->GetValue();
}

void MainWindow::ShowChangerPosition( QString )
{
  if ( Changers.count() <= 0 )
    return;

  Changer *changer = Changers[ ChangerSelect->currentIndex() ];
  AUnit *c1 = changer->unit1();
  AUnit *c2 = changer->unit2();
  double p1 = c1->value().toDouble();
  double p2 = c2->value().toDouble();

  double x = ( p1 - c1->getCenter() )
    * c1->getUPP() * changer->dir1() / changer->spacing1() + changer->center1();
  double y = ( p2 - c2->getCenter() )
    * c2->getUPP() * changer->dir2() / changer->spacing2() + changer->center2();

  int ix, iy;
  if ( x > 0 ) { ix = (int)( x + 0.5 ); } else { ix = -(int)( -x + 0.5 ); }
  if ( y > 0 ) { iy = (int)( y + 0.5 ); } else { iy = -(int)( -y + 0.5 ); }
  int num = changer->holders1() * iy + ix + 1;

  double dx = ( p1 - c1->getCenter() ) * c1->getUPP() * changer->dir1()
    - ( ix - changer->center1() ) * changer->spacing1();
  double dy = ( p2 - c2->getCenter() ) * c2->getUPP() * changer->dir2()
    - ( iy - changer->center2() ) * changer->spacing2();

  ChangerToGoHolderSelect->setValue( num );
  ChangerCurrentHolder->setValue( num );
  ChangerToGoFinePosition1->setText( QString::number( dx ) );
  ChangerToGoFinePosition2->setText( QString::number( dy ) );
}
