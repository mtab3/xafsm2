
#include "MainWindow.h"

#include <QIODevice>

/**********************************************************************/

#define DTAUTOSEQFNAME  "DTAutoCalib.seq"  

void MainWindow::DTAutoCalibStart( void )
{
  if ( ! inDTAutoCalib ) {
    if ( inAutoSeq ) {
      QString msg = tr( "Some Sequence is running" );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
      return;
    }
    QString fname = DTAUTOSEQFNAME;
    
    QFileInfo F( fname );
    if ( ! F.exists() ) {
      fname = ":" + fname;
    }
    
    inDTAutoCalib = true;
    inAutoSeq = true;
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
  inDTAutoCalib = false;
}

/**********************************************************************/

#define SSDENGAUTOSEQFNAME  "SSDEngAutoCalib.seq"

void MainWindow::SSDEngAutoCalibStart( void )
{
  if ( ! inSSDEngAutoCalib ) {
    if ( inAutoSeq ) {
      QString msg = tr( "Some Sequence is running" );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
      return;
    }
    QString fname = SSDENGAUTOSEQFNAME;
    
    QFileInfo F( fname );
    if ( ! F.exists() ) {
      fname = ":" + fname;
    }

    inSSDEngAutoCalib = true;
    inAutoSeq = true;
    SSDEngAutoCalib->setText( tr( "Stop" ) );
    SSDEngAutoCalib->setStyleSheet( InActive );

    connect( this, SIGNAL( AutoSequenceFinished() ),
	     this, SLOT( SSDEngAutoCalibFinished() ),
	     Qt::UniqueConnection );
    AutoSequence( fname );
  } else {
    AutoSequenceEnd();
  }
}

void MainWindow::SSDEngAutoCalibFinished( void )
{
  disconnect( this, SIGNAL( AutoSequenceFinished() ),
	      this, SLOT( DTAutoCalibFinished() ) );
  SSDEngAutoCalib->setText( tr( "Auto Calib." ) );
  SSDEngAutoCalib->setStyleSheet( NormalB );
  inSSDEngAutoCalib = false;
}

/*********************************************************************/

void MainWindow::setupAutoSequence( void )
{
  inAutoSeq = false;
  inDTAutoCalib = false;
  inSSDEngAutoCalib = false;
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
  ASVals.clear();

  ASLabels.clear();
  for ( int i = 0; i < ASCMDs.count(); i++ ) {
    if ( ASCMDs[i][0] == "LABEL" )
      ASLabels[ ASCMDs[i][1] ] = i;
  }

  ASCMDi = 0;
  ASCMDii = 0;
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
  QStringList LINE;
  bool progress = true;

  if ( AutoSequenceShouldBeLocked() )
    return;

  while ( ASCMDi < ASCMDs.count() ) {
    LINE = ASCMDs[ ASCMDi ];
    // 後ろから前へ置換する
    // 2つ目以降は $aa:$bb の aa も bb もそれ以降も全部置換する
    for ( int i = LINE.count()-1; i > 0; i-- ) {
      int p;
      while( ( p = LINE[i].lastIndexOf( '$', -1 ) ) >= 0 ) {
	QString theVal = LINE[i].mid( p + 1 );
	LINE[i] = LINE[i].left( p ) + ASVals[ theVal ];
      }
    }
    // 変数置換
    if ( LINE[0][0] == '$' ) {  // 最初の項目は $aa:$bb の bb は置換するけど aa はしない
      int p;
      while( ( p = LINE[0].lastIndexOf( '$', -1 ) ) > 0 ) {
	QString theVal = LINE[0].mid( p + 1 );
	LINE[0] = LINE[0].left( p ) + ASVals[ theVal ];
      }
    }
    // 
    CMD = FUNC = VAL = VAL2 = "";
    if ( LINE.count() > 0 ) CMD = LINE[0];
    if ( LINE.count() > 1 ) FUNC = LINE[1];
    if ( LINE.count() > 2 ) VAL = LINE[2];
    if ( LINE.count() > 3 ) VAL2 = LINE[3];

    if ( CMD[0] == '$' ) {
      ASVals[ CMD.mid(1) ] = FUNC;
    } else if ( CMD == "ARRAY" ) {
      for ( int i = 2; i < LINE.count(); i++ ) {
	ASVals[ FUNC+":"+QString::number(i-2) ] = LINE[i];
      }
    } else if ( CMD == "INC" ) {
      ASVals[ FUNC ] = QString::number( ASVals[ FUNC ].toInt() + 1 );
    } else if ( CMD == "ADD" ) {
      ASVals[ FUNC ] = QString::number( ASVals[ FUNC ].toInt() + VAL.toInt() );
    } else if ( CMD == "SADD" ) {
      ASVals[ FUNC ] = ASVals[ FUNC ] + VAL;
    } else if ( CMD == "SHOW" ) {
      qDebug() << "Show " << LINE;
    } else if ( CMD == "MONOCH" ) {
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
    } else if ( CMD == "MEASURE" ) {
      if ( FUNC == "CLEAR" ) {
	ASMUnits.clearUnits();
      } else if ( FUNC == "SENS" ) {
	int i;
	for ( i = 0; i < ASensors.count(); i++ ) {
	  if ( ASensors[i]->uid() == VAL ) {
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
      } else if ( FUNC == "MEASURE" ) {
	progress = false;
	switch( ASCMDii ) {
	case 0:
	    ASMUnits.clearStage();
	    ASCMDii++;
	    break;
	case 1:
	  if ( !ASMUnits.init() ) {
	    ASMUnits.clearStage();
	    ASCMDii++;
	  }
	  break;
	case 2:
	  if (( !ASMUnits.isParent() )||( ! ASMUnits.getValue0() )) {
	    ASMUnits.clearStage();
	    ASCMDii++;
	  }
	  break;
	case 3:
	  if ( ! ASMUnits.getValue() ) {
	    ASCMDii = 0;     // ASCMDii で回るサブループの最後では必ず ASCMDii = 0;
	    ASMUnits.clearStage(); // これは不要のはずだけど念の為
	    progress = true;
	  }
	  break;
	}
      } else if ( FUNC == "READ" ) {
	ASMUnits.readValue( ASMeasVals, ASMeasCPSs, false );  // true : correct dark
      }
    } else if ( CMD == "FILE" ) {
      VAL.remove( QChar( '"' ) );
      QIODevice::OpenMode mode;
      QString Item;
      QFile f( VAL );
      if ( ( FUNC == "WRITE" )||( FUNC == "APPEND" ) ) {
	if ( FUNC == "WRITE" ) {
	  mode = QIODevice::WriteOnly | QIODevice::Text;
	} else {
	  mode = QIODevice::Append | QIODevice::Text;
	}
	if ( f.open( mode ) ) {
	  QTextStream out( &f );
	  for ( int i = 3; i < LINE.count(); i++ ) {
	    Item = LINE[i].replace( QChar( '~' ), QChar( ' ' ) );
	    Item.remove( QChar( '"' ) );
	    if ( Item == "@DATE" ) {
	      out << QDate::currentDate().toString( "yyyy/MM/dd" );
	    } else if ( Item == "@TIME" ) {
	      out << QTime::currentTime().toString( "hh:mm:ss" );
	    } else if ( Item == "@RING" ) {
	      if ( SLS != NULL ) {
		out << SLS->value();
	      }
	    } else if ( Item == "@MEASURED" ) {
	      for ( int i = 0; i < ASMUnits.count(); i++ ) {
		out << ASMeasVals[i] << "\t";
	      }
	    } else {
	      out << Item;
	    }
	  }
	  out << endl;
	  f.close();
	} else {
	  qDebug() << "Cannot open file " << VAL << FUNC;
	}
      } else {
	qDebug() << "Wrong file mode " << VAL << FUNC;
      }
    } else if ( CMD == "MEASSSD" ) {
      if ( ( SFluo != NULL )||( ( VAL != "REAL" )&&( VAL != "LIVE" ) ) ) {
	if ( FUNC == "MEAS" ) {
	  progress = false;
	  switch( ASCMDii ) {
	  case 0:
	    ASMUnits.clearUnits();
	    ASMUnits.addUnit( SFluo );
	    SFluo->initLocalStage();
	    ASCMDii++;
	    break;
	  case 1:
	    qDebug() << "set preset type";
	    SFluo->setSSDPresetType( VAL );
	    ASCMDii++;
	    break;
	  case 2:
	    SFluo->SetTime( VAL2.toDouble() );
	    ASCMDii++;
	    break;
	  case 3:
	    qDebug() << "try to init sensor and go";
	    if ( SFluo->InitSensor() == false ) {  // true :: initializing
	      SFluo->setIsBusy( true );
	      SFluo->RunStart();
	      ASCMDii++;
	    }
	    break;
	  case 4:
	    SFluo->GetMCAs();
	    ASCMDii = 0;
	    progress = true;
	    break;
	  }
	} else if ( FUNC == "MODE" ) {
	  SFluo->setSSDPresetType( VAL );
	  CheckMUnits.add( SFluo, true, true );
	} else if ( FUNC == "RECORD" ) {
	  VAL.remove( QChar( '"' ) );
	  aMCASet *set = new aMCASet;
	  SaveMCADataOnMem( set );
	  set->save( VAL, "measured by auto Sequence" );
	  //	  saveMCAData0( VAL, set );
	  delete set;
	}
      } else {
	if ( SFluo == NULL ) qDebug() << "MEASSSD: SSD is not available";
	if (( VAL != "REAL" )&&( VAL != "LIVE" ))
	  qDebug() << "MEASSSD: only REAL or LIVE is availabe";
      }
    } else if ( CMD == "LABEL" ) {
      // ラベルの処理はここではしない
    } else if ( CMD == "GOTO" ) {
      ASCMDi = ASLabels[ FUNC ];
    } else if ( CMD == "IFEQ" ) {
      if ( FUNC != VAL )
	ASCMDi++;
    } else if ( CMD == "IFNEQ" ) {
      if ( FUNC == VAL )
	ASCMDi++;
    } else if ( CMD == "END" ) {
      break;                        // goto AutoSequenceStop
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

  inAutoSeq = false;
  emit AutoSequenceFinished();
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
  int i;
  QString line;

  while( ! ASin.atEnd() ) {
    bool inQ = false;
    line = ASin.readLine();
    for ( i = 0; i < line.length(); i++ ) {
      if ( line[i] == '"' ) inQ = ! inQ;
      if (( line[i] == ' ' )&&( inQ )) line[i] = '~';
      if ( line[i] == '#' ) break;
    }
    if ( i < line.length() ) {
      line = line.left( i );
    }
    items = line.simplified().split( QRegExp( "\\s+" ) );
    for ( int i = 0; i < items.count(); i++ ) {
      items[i].remove( QChar( '"' ) );
    }
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
