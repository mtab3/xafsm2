
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
  S2DTimes << S2DTime1 << S2DTime2 << S2DTime3;
  S2DRelAbs << S2DRelAbs1 << S2DRelAbs2 << S2DRelAbs3;

  for ( int i = 0; i < ASensors.count(); i++ ) {
    SelectS2DSensor->addItem( ASensors[i]->getName() );
  }
  for ( int i = 0; i < AMotors.count(); i++ ) {
    for ( int j = 0; j < S2DAxis.count(); j++ ) {
      S2DAxis[j]->addItem( AMotors[i]->getName() );
    }
    connect( s, SIGNAL( AnsGetValue( SMsg ) ), this, SLOT( ShowS2DCurMotorPos( SMsg ) ) );
    connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( ShowS2DCurMotorPos( SMsg ) ) );
  }
  for ( int i = 0; i < S2DAxis.count(); i++ ) {
    S2DUnits[i]->setText( AMotors[ S2DAxis[i]->currentIndex() ]->getUnit() );
    connect( S2DAxis[i], SIGNAL( currentIndexChanged( int ) ),
	     this, SLOT( newAx( int ) ) );
  }

  connect( S2DFileSelect, SIGNAL( clicked() ), S2DFileSel, SLOT( show() ) );
  connect( S2DFileSel, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( newS2DFileSelected( const QString & ) ) );
}

void MainWindow::newAx( int i )
{
  qDebug() << "newax " << i;
  for ( int j = 0; j < S2DAxis.count(); j++ ) {
    if ( sender() == S2DAxis[j] ) {
      S2DUnits[j]->setText( AMotors[ i ]->getUnit() );
      S2DCurPos[j]->setText( AMotors[ i ]->value() );
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

void MainWindow::ShowS2DCurMotorPos( SMsg msg )
{
  for ( int i = 0; i < S2DAxis.count(); i++ ) {
    AUnit *am = AMotors[ S2DAxis[i].currentIndex() ];
    
    if ( ( msg.From() == am->getDevCh() )
	 && ( ( msg.Msgt() == GETVALUE ) || ( msg.Msgt() == EvCHANGEDVALUE ) ) ) {
      if ( ( am->getType() == "SC" ) && ( msg.Msgt() == GETVALUE ) ) {
	val0 = msg.Vals().at(1);
      } else {
	val0 = msg.Val();
      }
#if 0                    // working !!!!!
      S2DCurPos->setText( val0 );
      val = QString::number
	( ( val0.toDouble() - am->getCenter() ) * am->getUPP() );
      MCurPosUnit->setText( val );
      if ( setupMDispFirstTime == true ) {  // 最初の一回だけ
	if ( MMRelAbs->stat() == ABS ) {
	  GoMotorPosPuls->setText( val0 );
	  GoMotorPosUnit->setText( val );
	  setupMDispFirstTime = false;
	} else {
	  GoMotorPosPuls->setText( 0 );
	  GoMotorPosUnit->setText( 0 );
	  setupMDispFirstTime = false;
	}
      }
      if ( am->checkNewVal() ) {
	NewLogMsg( tr( "Current Position of [%1] : [%2] %3" )
		   .arg( am->getName() )
		   .arg( val )
		   .arg( am->getUnit() ) );
      }
    }
  }
#endif
}
