
#include "MainWindow.h"

/**********************************************************************/

#define DTAUTOSEQFNAME  "DTAutoCalib.seq"  

void MainWindow::DTAutoCalibStart( void )
{
  if ( ! inDTAutoCalib ) {
    QString fname = DTAUTOSEQFNAME;
    
    QFileInfo F( fname );
    if ( ! F.exists() ) {
      fname = ":" DTAUTOSEQFNAME;
    }

    inDTAutoCalib = true;
    DTAutoCalib->setText( tr( "Stop" ) );
    DTAutoCalib->setStyleSheet( InActive );

    connect( this, SIGNAL( AutoSequenceFinished() ), this, SLOT( DTAutoCalibFinished() ),
	     Qt::UniqueConnection );
    AutoSequence( fname );
  } else {
    AutoSequenceEnd();
  }
}

void MainWindow::DTAutoCalibFinished( void )
{
  disconnect( this, SIGNAL( AutoSequenceFinished() ),
	      this, SLOT( DTAutoCalibFinished() ) );
  DTAutoCalib->setText( tr( "Auto Calib." ) );
  DTAutoCalib->setStyleSheet( NormalB );
}

/*********************************************************************/

void MainWindow::setupAutoSequence( void )
{
  inDTAutoCalib = false;
  ASSCDx = ASSCDy = 0;
  ASTimer = new QTimer;
  connect( ASTimer, SIGNAL( timeout() ), this, SLOT( AutoSequence0() ),
	   Qt::UniqueConnection );
}

void MainWindow::AutoSequence( QString fname )
{
  ASReadSEQFile( fname );

  CheckMUnits.clear();
  CheckSUnits.clear();
  ASCMDi = 0;
  ASTimer->start( 10 );
}

bool MainWindow::AutoSequenceShouldBeLocked( void )
{
  if (( CheckMUnits.isBusy() )||( CheckSUnits.isBusy() ))
    return true;

  if (( ASMUnits.isBusy() )||( ASMUnits.isBusy2() ))
    return true;

  return false;
}

void MainWindow::AutoSequence0( void )
{
  QString CMD;
  QString FUNC;
  QString VAL, VAL2;
  bool progress = true;

  if ( AutoSequenceShouldBeLocked() )
    return;

  while ( ASCMDi < ASCMDs.count() ) {
    CMD = FUNC = VAL = VAL2 = "";
    if ( ASCMDs[ ASCMDi ].count() > 0 ) CMD = ASCMDs[ ASCMDi ][0];
    if ( ASCMDs[ ASCMDi ].count() > 1 ) FUNC = ASCMDs[ ASCMDi ][1];
    if ( ASCMDs[ ASCMDi ].count() > 2 ) VAL = ASCMDs[ ASCMDi ][2];
    if ( ASCMDs[ ASCMDi ].count() > 3 ) VAL2 = ASCMDs[ ASCMDi ][3];
    qDebug() << CMD << FUNC << VAL;

    if ( CMD == "MONOCH" ) {
      if ( FUNC == "ENERGY" ) {
	CheckMUnits.add( MMainTh, true, true );
	MoveCurThPosKeV( VAL.toDouble() );
      }
    } else if ( CMD == "SCHANGER" ) {
      if ( FUNC == "SELECT" ) {               // SCHANGER SELECT
	int i;
	for ( i = 0; i < SChangers.count(); i++ ) {
	  if ( SChangers[i]->id() == VAL ) {
	    ASSChanger = SChangers[i];
	    break;
	  }
	}
	if ( i >= SChangers.count() ) {
	  qDebug() << "Can not find the selected Special Changer " << VAL;
	  AutoSequenceEnd();
	}
      } else if ( FUNC == "SETDXDY" ) {
	if ( VAL == "ASIS" ) {
	  ASSCDx = AttDx->text().toDouble();
	  ASSCDy = AttDy->text().toDouble();
	} else {
	  ASSCDx = VAL.toDouble();
	  ASSCDy = VAL2.toDouble();
	}
      } else if ( FUNC == "MOVE" ) {          // SCHANGER MOVE
	int i;
	for ( i = 0; i < ASSChanger->specNames(); i++ ) {
	  if ( ASSChanger->specName( i )->attrib( 0 ) == VAL ) {
	    CheckMUnits.add( ASSChanger->baseChangerP()->unit1(), true, true );
	    CheckMUnits.add( ASSChanger->baseChangerP()->unit2(), true, true );
	    moveToTarget( ASSChanger->baseChangerP(),
			  ASSChanger->specName( i )->position(),
			  ASSCDx, ASSCDy );
	    break;
	  }
	}
	if ( i >= ASSChanger->specNames() ) {
	  qDebug() << "Can not find the selected Special Changer position" << VAL;
	}
      }
    } else if ( CMD == "MONITOR" ) {
      if ( FUNC == "CLEAR" ) {
	ASMUnits.clearUnits();
      } else if ( FUNC == "SENS" ) {
	int i;
	for ( i = 0; i < ASensors.count(); i++ ) {
	  if ( ASensors[i]->getUid() == VAL ) {
	    ASMUnits.addUnit( ASensors[i] );
	    break;
	  }
	}
	if ( i >= ASensors.count() ) {
	  qDebug() << "Can not find the selected Sensor" << VAL;
	}
      } else if ( FUNC == "DTIME" ) {
	ASMUnits.setDwellTimes( VAL.toDouble() );
	ASMUnits.setDwellTime();
      } else if ( FUNC == "CLEAR_LOCAL_STAGE" ) {
	ASMUnits.clearStage();
	ASCMDii = 0;
      } else if ( FUNC == "MEASURE" ) {
	progress = false;
	switch( ASCMDii ) {
	case 0:
	  if (( !ASMUnits.isParent() )||( ! ASMUnits.getValue0() )) {
	    ASMUnits.clearStage();
	    ASCMDii++;
	  }
	  break;
	case 1:
	  if ( ! ASMUnits.getValue() ) {
	    ASMUnits.clearStage();
	    progress = true;
	  }
	  break;
	}
      } else if ( FUNC == "READ" ) {
	ASMUnits.readValue( ASMeasVals, ASMeasCPSs, false );  // true : correct dark
	qDebug() << "units " << ASMUnits.count();
	for ( int i = 0; i < ASMUnits.count(); i++ ) {
	  qDebug() << "read vals" << i << ASMeasVals[i];
	  qDebug() << "read cpss" << i << ASMeasCPSs[i];
	}
      }
    } else {
      qDebug() << "No CMD [" << CMD << "]";
    }

    if ( progress )
      ASCMDi++;
    return;
  }

  AutoSequenceEnd();
}

void MainWindow::AutoSequenceEnd( void )
{
  ASTimer->stop();

  emit AutoSequenceFinished();

  inDTAutoCalib = false;
}

void MainWindow::ASReadSEQFile( QString fname )
{
  ASFile.setFileName( fname );
  if ( ! ASFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return;

  ASin.setDevice( &ASFile );
  ASCMDs.clear();
  
  QStringList items;
  while( ASReadNextLine( items ) ) {
    ASCMDs << items;
  }

  ASFile.close();
}

bool MainWindow::ASReadNextLine( QStringList &items )
{
  while( ! ASin.atEnd() ) {
    items = ASin.readLine().simplified().split( QRegExp( "\\s+" ) );
    if ( items.count() <= 0 )
      continue;
    if ( items[0] == "" )
      continue;
    if ( items[0][0] == '#' )
      continue;
    return true;
  }

  return false;
}
