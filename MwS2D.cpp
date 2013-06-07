
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
  S2Dsx << 0 << 0 << 0;
  S2Dex << 0 << 0 << 0;
  S2Ddx << 0 << 0 << 0;
  S2Dsign << 1 << 1 << 1;

  for ( int i = 0; i < ASensors.count(); i++ ) {
    S2DOkSensors << ASensors[i];
    SelectS2DSensor->addItem( ASensors[i]->getName() );
  }
  for ( int i = 0; i < AMotors.count(); i++ ) {
    S2DOkMotors << AMotors[i];
    for ( int j = 0; j < S2DAxis.count(); j++ ) {
      S2DAxis[j]->addItem( AMotors[i]->getName() );
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
  S2DMotors[ax] = S2DOkMotors[motor];   // その軸に選ばれたモータを覚えておく
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

    if ( S2DStepScan->isChecked() ) {  // ステップスキャン
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
      NewLogMsg( tr( "Continuous scan is not available now." ) );
      return;
    }

    inS2D = true;
    NewLogMsg( QString( tr( "2D Scan Start (%1 %2 %3 (%4))" ) )
	       .arg( as->getName() )
	       .arg( S2DMotors[0]->getName() )
	       .arg( S2DMotors[1]->getName() )
	       .arg( S2DMotors[2]->getName() ) );
    
    // MovingS = SPSMotorS->currentIndex();  // モータの速度はセットしない
    // am->SetSpeed( MSpeeds[ MovingS ].MSid );
    
    S2DStart->setText( tr( "Stop" ) );
    S2DStart->setStyleSheet( InActive );
    
    S2DDataStat = NEW;
    S2DFileName->setStyleSheet( FSTATCOLORS[ ScanDataStat ][ ScanNameStat ] );
    S2DFileName->setToolTip( FSTATMsgs[ ScanDataStat ][ ScanNameStat ] );

    SetupS2DParams();
    S2DStage = 0;
    S2DTimer->start( 100 );
  } else {
    S2DStop0();
  }
}

void MainWindow::SetupS2DParams( void )
{
  for ( int i = 0; i < S2DAxis.count(); i++ ) {
    if ( S2DRelAbs[i]->stat() == ABS ) {
      S2Dsx[i] = S2DMotors[i]->u2p( S2DStars[i]->text().toDouble() );
      S2Dex[i] = S2DMotors[i]->u2p( S2DEnds[i]->text().toDouble() );
    }
    S2Ddx[i] = ( S2Dex[i] - S2Dsx[i] ) / S2DPoints[i]->text().toDouble();
    S2DSign[i] = ( S2Dex[i] > S2Dsx[i] ) ? 1 : -1;
  }
}

void MainWindow::S2DScanSequence( void )
{
  // モータ駆動中は入ってこない (とりあえずステップのことだけ考える)
  for ( int i = 0; i < S2DMotors.count(); i++ ) {
    if ( S2DMotorUse[i] && S2DMotors[i]->isBusy0() )
      return;
  }
  // センサー busy でも入ってこない
  if ( mUnits.isBusy() )
    return;

  switch( S2DStage ) {
  case 0:       // 始点に移動
    
    break;
  }
}

void MainWindow::S2DStop0( void )
{
  S2DStage = 0;
}
