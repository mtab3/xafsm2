
#include "MainWindow.h"

#include "SMsg.h"

void MainWindow::setupScan2DArea( void )
{
  S2DFileSel = new QFileDialog;
  S2DFileSel->setAcceptMode( QFileDialog::AcceptSave );
  S2DFileSel->setDirectory( QDir::currentPath() );
  S2DFileSel->setFilter( "*.dat" );

  S2DAxis << S2DAx1 << S2DAx2 << S2DAx3;
  S2DCurPos << S2DCurPos1 << S2DCurPos2 << S2DCurPos3;
  S2DUnits << S2DUnit1 << S2DUnit2 << S2DUnit3;
  S2DStarts << S2DStartP1 << S2DStartP2 << S2DStartP3;
  S2DEnds << S2DEndP1 << S2DEndP2 << S2DEndP3;
  S2DSteps << S2DStep1 << S2DStep2 << S2DStep3;
  S2DPoints << S2DPoints1 << S2DPoints2 << S2DPoints3;
  S2DRelAbs << S2DRelAbs1 << S2DRelAbs2 << S2DRelAbs3;
  S2DSelectedMotors << NULL << NULL << NULL;

  S2DI.motors = 3;
  S2DI.ScanBothDir = false;
  S2DI.Use3rdAx = false;
  S2DI.unit << NULL << NULL << NULL;
  S2DI.used << true << true << false;
  S2DI.ps << 0 << 0 << 0;
  S2DI.now << 0 << 0 << 0;
  S2DI.sx << 0 << 0 << 0;
  S2DI.ex << 0 << 0 << 0;
  S2DI.dx << 0 << 0 << 0;
  S2DI.i << 0 << 0 << 0;
  S2DI.ScanMode = STEP;

  S2DInfoIsValid = false;

  S2DV->setParent( this );

  for ( int i = 0; i < ASensors.count(); i++ ) {
    S2DOkSensors << ASensors[i];
    SelectS2DSensor->addItem( ASensors[i]->getName() );
  }

  for ( int i = 0; i < AMotors.count(); i++ ) {
    // SetSpeed, SetMaxSpeed 等が使えないとダメ。現状 PM のみ
    if ( AMotors[i]->getType() == "PM" ) {
      S2DOkMotors << AMotors[i];
      for ( int j = 0; j < S2DAxis.count(); j++ ) {
	S2DAxis[j]->addItem( AMotors[i]->getName() );
      }
    }
  }
  for ( int i = 0; i < S2DAxis.count(); i++ ) {
    newAx0( i, S2DAxis[i]->currentIndex() );
    connect( S2DAxis[i], SIGNAL( currentIndexChanged( int ) ),
	     this, SLOT( newAx( int ) ),
	     Qt::UniqueConnection );
  }

  for ( int i = 0; i < Changers.count(); i++ ) {
    // Changers[] は MwChanger (MwSetup) と共有
    S2DChangerSelect->addItem( Changers[i]->name() );
  }
  if ( Changers.count() > 0 ) {
    S2DUseChanger->setChecked( true );
    S2DUseChanger->setEnabled( true );
    S2DNewChangerSelected( S2DChangerSelect->currentIndex() );
    connect( S2DChangerSelect, SIGNAL( currentIndexChanged( int ) ),
	     this, SLOT( S2DNewChangerSelected( int ) ),
	     Qt::UniqueConnection );
    S2DUse3rdAxF->setChecked( false );
    // S2DUse3rdAx->setEnabled( false );
  } else {
    S2DUseChanger->setChecked( false );
    S2DUseChanger->setEnabled( false );
    // S2DUse3rdAx->setEnabled( true );
  }

  inS2D = false;
  S2DTimer = new QTimer;
  S2DTimer2 = new QTimer;
  isS2DSFluo = false;

  connect( S2DStart, SIGNAL( clicked() ), this, SLOT( S2DScanStart() ),
	   Qt::UniqueConnection );
  connect( S2DTimer, SIGNAL( timeout() ), this, SLOT( S2DStepScanSequence() ),
	   Qt::UniqueConnection );
  connect( S2DFileSelect, SIGNAL( clicked() ), S2DFileSel, SLOT( show() ),
	   Qt::UniqueConnection );
  connect( S2DFileSel, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( newS2DFileSelected( const QString & ) ),
	   Qt::UniqueConnection );
  for ( int i = 0; i < S2DStarts.count(); i++ ) {
    connect( S2DStarts[i], SIGNAL( editingFinished() ), this, SLOT( newS2DSteps() ),
	     Qt::UniqueConnection );
    connect( S2DEnds[i], SIGNAL( editingFinished() ), this, SLOT( newS2DSteps() ),
	     Qt::UniqueConnection );
    connect( S2DSteps[i], SIGNAL( editingFinished() ), this, SLOT( newS2DPoints() ),
	     Qt::UniqueConnection );
    connect( S2DPoints[i], SIGNAL( editingFinished() ), this, SLOT( newS2DSteps() ),
	     Qt::UniqueConnection );
  }
  connect( S2DUseChanger, SIGNAL( toggled(bool) ),
	   this, SLOT( S2DSetUseChangers(bool) ),
	   Qt::UniqueConnection );
  connect( S2DTimer2, SIGNAL( timeout() ), this, SLOT( S2DRContScanMeas() ),
	   Qt::UniqueConnection );
}

void MainWindow::S2DSetUseChangers( bool f )
{
  if ( f ) {
    S2DUse3rdAxF->setChecked( false );
    // S2DUse3rdAx->setEnabled( false );
    S2DNewChangerSelected( S2DChangerSelect->currentIndex() );
  } else {
    // S2DUse3rdAx->setEnabled( true );
  }
}

void MainWindow::S2DNewChangerSelected( int i )
{
  if ( Changers.count() <= 0 )
    return;

#if 0
  disconnect( SIGNAL( newValue( QString ) ),
	      this, SLOT( ShowS2DChangerPosition( QString ) ) );
  connect( Changers[i]->unit1(), SIGNAL( newValue( QString ) ),
	   this, SLOT( ShowS2DChangerPosition( QString ) ),
	   Qt::UniqueConnection );
  connect( Changers[i]->unit2(), SIGNAL( newValue( QString ) ),
	   this, SLOT( ShowS2DChangerPosition( QString ) ),
	   Qt::UniqueConnection );

  Changers[i]->unit1()->GetValue();
  Changers[i]->unit2()->GetValue();
#endif

  for ( int j = 0; j < S2DOkMotors.count(); j++ ) {
    if ( Changers[i]->unit1() == S2DOkMotors[j] ) {
      S2DAx1->setCurrentIndex( j );
    }
    if ( Changers[i]->unit2() == S2DOkMotors[j] ) {
      S2DAx2->setCurrentIndex( j );
    }
  }
}

void MainWindow::newS2DPoints( void )
{
  int i;
  for ( i = 0; i < S2DSteps.count(); i++ ) {
    if ( S2DSteps[i] == sender() )
      break;
  }
  if ( i < S2DSteps.count() ) {
    double d = fabs( S2DSteps[i]->text().toDouble() );
    double s = S2DStarts[i]->text().toDouble();
    double e = S2DEnds[i]->text().toDouble();
    if ( d > 0 ) {
      int p = fabs( ( e - s ) / d );
      if ( p > 0 ) {
	S2DPoints[i]->setText( QString::number( p ) );
      } else {
	S2DPoints[i]->setText( "1" );
      }
    } else {
      S2DPoints[i]->setText( "1" );
    }
  }
}

void MainWindow::newS2DSteps( void )
{
  int i;
  for ( i = 0; i < S2DStarts.count(); i++ ) {
    if ( ( S2DStarts[i] == sender() )
	 ||( S2DEnds[i] == sender() )
	 ||( S2DPoints[i] == sender() ) )
      break;
  }
  if ( i < S2DStarts.count() ) {
    int p = abs( S2DPoints[i]->text().toInt() );
    if ( p > 0 )
      S2DSteps[i]
	->setText( QString::number( ( S2DEnds[i]->text().toDouble()
				      - S2DStarts[i]->text().toDouble() ) / p ) );
    else 
      S2DSteps[i]
	->setText( QString::number( ( S2DEnds[i]->text().toDouble()
				      - S2DStarts[i]->text().toDouble() ) / 1 ) );
  }
}


void MainWindow::newAx0( int ax, int motor )
{
  S2DUnits[ax]->setText( S2DOkMotors[ motor ]->getUnit() );
  S2DCurPos[ax]->setText( QString::number( S2DOkMotors[ motor ]->metricValue() ) );
  if ( S2DSelectedMotors[ax] != NULL ) {
    bool Uniq = true;
    for ( int i = 0; i < S2DSelectedMotors.count(); i++ ) {
      if ( ( i != ax )&&( S2DSelectedMotors[i] == S2DSelectedMotors[ax] ) ) {
	Uniq = false;
	break;
      }
    }
    if ( Uniq )
      disconnect( S2DSelectedMotors[ax], SIGNAL( newValue( QString ) ),
		  this, SLOT( showS2DNewAxValue( QString ) ) );
  }
  S2DSelectedMotors[ax] = S2DOkMotors[motor];   // その軸に選ばれたモータを覚えておく
  connect( S2DSelectedMotors[ax], SIGNAL( newValue( QString ) ),
	   this, SLOT( showS2DNewAxValue( QString ) ),
	   Qt::UniqueConnection );
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
    if ( S2DSelectedMotors[i] == sender() ) {
      S2DCurPos[i]->setText( QString::number( S2DSelectedMotors[i]->metricValue() ) );
    }
  }
}

void MainWindow::S2DScanStart( void )
{
  AUnit *as = NULL;

  if ( !inS2D ) {
    S2DInfo oldInfo = S2DI;
    SetupS2DParams();   // スキャンパラメータを GUI から内部変数にコピー

    if ( inMeas || inSPSing || inMonitor || inMMove || inMCAMeas ) {
      statusbar
	->showMessage( tr( "Can't start 2D Scan. Othre Process is going on." ), 2000 );
      S2DI = oldInfo;
      return;
    }
    for ( int i = 0; i < S2DI.motors; i++ ) {
      if ( S2DI.used[i] && ( ! S2DI.unit[i]->isEnable() ) ) {
	QString msg = tr( "2D Scan cannot Start : (%1) is disabled" )
	  .arg( S2DI.unit[i]->getName() );
	statusbar->showMessage( msg, 2000 );
	S2DI = oldInfo;
	return;
      }
    }
    // 計測時間が 0 になってないかチェック
    if ( S2DI.Dwell <= 0 ) {
      statusbar->showMessage( tr( "Meas Time is 0 or less." ), 2000 );
      S2DI = oldInfo;
      return;
    }

    QString User;
    for ( int i = 0; i < S2DI.motors; i++ ) {
      if ( S2DI.used[i] ) {
	// スキャン範囲に幅があるかをチェック
	if ( S2DI.sx[i] == S2DI.ex[i] ) {
	  statusbar->showMessage( tr( "2D Scan Range Error." ), 2000 );
	  S2DI = oldInfo;
	  return;
	}
	// 測定点数が 1以上あることを確認
	if ( S2DI.ps[i] <= 0 ) {
	  statusbar->showMessage( tr( "2D Scan Points Error." ), 2000 );
	  S2DI = oldInfo;
	  return;
	}
	// 軸がダブってないかチェック
	for ( int j = i+1; j < S2DI.motors; j++ ) {
	  if ( S2DI.used[j] ) {
	    if ( S2DI.unit[i] == S2DI.unit[j] ) {
	      statusbar
		->showMessage( tr( "The same motor was selected for different axis." ),
			       2000 );
	      S2DI = oldInfo;
	      return;
	    }
	  }
	}

	if ( ( User = UUnits.isTheUnitInUse( S2DI.unit[i] ) ) != "" ) {
	  // モーターが他のことに使われていたらダメ
	  statusbar->showMessage( tr( "The Motor [%1] is used by the process %2!" )
				  .arg( S2DI.unit[i]->getName() ).arg( User ), 2000 );
	  S2DI = oldInfo;
	  return;
	}
      }
    }

    mUnits.clearUnits();
    mUnits.addUnit( as = S2DOkSensors.value( SelectS2DSensor->currentIndex() ) );
    mUnits.setDwellTimes( S2DI.Dwell );
    mUnits.setDwellTime();
    for ( int i = 0; i < mUnits.count(); i++ ) {
      if ( ! mUnits.at(i)->isEnable() ) {
	QString msg = QString( tr( "2D Scan cannot Start : (%1) is disabled" ) )
	  .arg( mUnits.at(i)->getName() );
	statusbar->showMessage( msg, 2000 );
	NewLogMsg( msg );
	S2DI = oldInfo;
	return;
      }

      if ( ( User = UUnits.isTheUnitInUse( mUnits.at(i) ) ) != "" ) {
	// 検出器が他のことに使われたらダメ
	statusbar->showMessage( tr( "The Sensor [%1] is used by the process %2!" )
				.arg( mUnits.at(i)->getName() ).arg( User ), 2000 );
	S2DI = oldInfo;
	return;
      }
    }

    isS2DSFluo = false;
    if ( as == SFluo ) {
      isS2DSFluo = true;
      getNewMCAView();
    }
    if ( S2DStepScan->isChecked() )
      S2DI.ScanMode = STEP;
    if ( S2DQuasiContScan->isChecked() ) {
      if ( ! CheckOkList( as, CScanOk ) ) {
	NewLogMsg( tr( "Continuous scan is not available now." ) );
	S2DI = oldInfo;
	return;
      }
      S2DI.ScanMode = QCONT;
    }
    if ( S2DRealContScan->isChecked() ) {
      if ( ! CheckOkList( as, CScanOk ) ) {
	NewLogMsg( tr( "Continuous scan is not available now." ) );
	S2DI = oldInfo;
	return;
      }
      S2DI.ScanMode = RCONT;
    }

    inS2D = true;
    if ( S2DI.Use3rdAx ) {
      NewLogMsg( QString( tr( "2D Scan Start (%1 %2 %3 (%4))" ) )
		 .arg( as->getName() )
		 .arg( S2DI.unit[0]->getName() )
		 .arg( S2DI.unit[1]->getName() )
		 .arg( S2DI.unit[2]->getName() ) );
    } else {
      NewLogMsg( QString( tr( "2D Scan Start (%1 %2 %3)" ) )
		 .arg( as->getName() )
		 .arg( S2DI.unit[0]->getName() )
		 .arg( S2DI.unit[1]->getName() ) );
    }

    S2DStart->setText( tr( "Stop" ) );
    S2DStart->setStyleSheet( InActive );
    
    S2DDataStat = NEW;
    S2DFileName->setStyleSheet( FSTATCOLORS[ ScanDataStat ][ ScanNameStat ] );
    S2DFileName->setToolTip( FSTATMsgs[ ScanDataStat ][ ScanNameStat ] );

    // 1st と 2nd の軸の単位が同じなら、表示の縦横比をスキャン範囲の
    // 縦横比に合わせるように努力する。
    // そうでなければ、画面いっぱいを使う。
    if ( S2DI.unit[0]->getUnit() == S2DI.unit[1]->getUnit() ) {
      S2DV->setRatioType( REAL_RATIO );
    } else {
      S2DV->setRatioType( AS_SCREEN );
    }
    S2DV->setRange( S2DI.sx[0], S2DI.sx[1],
		    S2DI.dx[0], S2DI.dx[1],
		    S2DI.ps[0], S2DI.ps[1]+1 );

    for ( int i = 0; i < S2DI.motors; i++ ) {
      if ( S2DI.used[i] ) {
	UUnits.addUnit( S2D_ID, S2DI.unit[i] );
      }
    }
    for ( int i = 0; i < mUnits.count(); i++ ) {
      UUnits.addUnit( S2D_ID, mUnits.at(i) );
    }

    S2DLastV = 0;
    S2DI.MCAFile = S2DFile = S2DFileName->text();
    if ( S2DI.MCAFile.isEmpty() )
      S2DI.MCAFile = QString( "S2DMCA0000.dat" );
    S2DWriteHead();
    mUnits.clearStage();
    S2DStage = 0;
    S2DTimer->disconnect();
    S2DScanDir = FORWARD;
    switch ( S2DI.ScanMode ) {
    case STEP:
      connect( S2DTimer, SIGNAL( timeout() ), this, SLOT( S2DStepScanSequence() ),
	       Qt::UniqueConnection );
      break;
    case QCONT:
      connect( S2DTimer, SIGNAL( timeout() ),
	       this, SLOT( S2DQuasiContinuousScanSequence() ),
	       Qt::UniqueConnection );
      break;
    case RCONT:
      connect( S2DTimer, SIGNAL( timeout() ),
	       this, SLOT( S2DRealContinuousScanSequence() ),
	       Qt::UniqueConnection );
      break;
    default:
      qDebug() << "non-defined scan mode !";
      return;
    }
    S2DInfoIsValid = true;
    S2DTimer->start( 10 );
  } else {
    S2DStop0();
  }
}

void MainWindow::SetupS2DParams( void )
{
  S2DI.ScanBothDir = S2DScanBothDir->isChecked();
  S2DI.Use3rdAx = S2DUse3rdAxF->isChecked();
  S2DI.Dwell = S2DTime1->text().toDouble();
  S2DI.used[0] = true;
  S2DI.used[1] = true;
  S2DI.used[2] = S2DI.Use3rdAx;
  for ( int i = 0; i < S2DAxis.count(); i++ ) {
    S2DI.unit[i] = S2DSelectedMotors[i];
    S2DI.now[i] = S2DI.unit[i]->value().toInt();
    if ( S2DRelAbs[i]->stat() == ABS ) {
      S2DI.sx[i] = S2DStarts[i]->text().toDouble();
      S2DI.ex[i] = S2DEnds[i]->text().toDouble();
    } else {
      S2DI.sx[i] = S2DCurPos[i]->text().toDouble() + S2DStarts[i]->text().toDouble();
      S2DI.ex[i] = S2DCurPos[i]->text().toDouble() + S2DEnds[i]->text().toDouble();
    }
    S2DI.ps[i] = abs( S2DPoints[i]->text().toInt() );
    if ( S2DI.ps[i] == 0 )
      S2DI.ps[i] = 1;
    S2DI.dx[i] = ( S2DI.ex[i] - S2DI.sx[i] ) / S2DI.ps[i];
    if ( S2DI.dx[i] == 0 ) {
      //      S2Ddx[i] = S2Dsign[i];
    }
  }
}

void MainWindow::S2DStop0( void )
{
  S2DStart->setText( tr( "Start" ) );
  S2DStage = S2D_END_STAGE;
}

void MainWindow::S2DWriteHead( void )
{
  if ( S2DFile.simplified().isEmpty() )
    return;

  QFile f( S2DFile );

  if ( !f.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;

  // Writing fixed headers
  QTextStream out(&f);

  // "#" と ":" の間は " " 込で 11桁

  out << "# 1306 Aichi SR 2D Scan" << endl;
  out << "#" << " Date      : " << QDateTime::currentDateTime()
                                     .toString("yy.MM.dd hh:mm:ss.zzz") << endl;
  if ( SLS != NULL ) 
    out << "#" << " Ring Cur. : " << SLS->value().toDouble() << "[mA]" << endl;

  out << "# Scan Mode : ";
  switch( S2DI.ScanMode ) {
  case STEP: out << "Step Scan" << endl; break;
  case QCONT: out << "Quasi Continuous Scan" << endl; break;
  case RCONT: out << "Real Continuous Scan" << endl; break;
  }
  if ( S2DI.ScanBothDir ) {
    out << "# Scan dir : Both" << endl;
  } else {
    out << "# Scan dir : Single" << endl;
  }

  for ( int i = 0; i < S2DI.motors; i++ ) {
    if ( S2DI.used[i] ) {
      out << "#" << QString( " Axis %1    : " ).arg( i, 1 )
	  << QString( " %1" ).arg( S2DI.sx[i], 10 )
	  << QString( " %1" ).arg( S2DI.ex[i], 10 )
	  << QString( " %1" ).arg( S2DI.dx[i], 10 )
	  << QString( " %1" ).arg( S2DI.ps[i], 10 )
	  << " : " << S2DI.unit[i]->getName() << endl;
    }
  }

  out << "#" << QString( " Dwell Time : %1" ).arg( S2DI.Dwell ) << endl;

  out << "#" << endl;

  f.close();
}

void MainWindow::S2DWriteBody( double v )
{
  if ( ! S2DFile.simplified().isEmpty() ) {
    QFile f( S2DFile );
    
    if ( !f.open( QIODevice::Append | QIODevice::Text ) )
      return;
    
    QTextStream out(&f);
    
    for ( int i = 0; i < S2DI.motors; i++ ) {
      if ( S2DI.used[i] ) {
	out << QString( " %1" )
	  .arg( S2DI.sx[i] + ( S2DI.i[i] + ((i==0)? 0.5 : 0) ) * S2DI.dx[i], 10 );
      }
    }
    out <<  QString( " %1" ).arg( v, 10 ) << endl;
    
    f.close();
  }
}

void MainWindow::S2DWriteBody2( int ix, int iy )
{
  if ( isS2DSFluo ) {
    QFileInfo mcaFile = S2DGenerateMCAFileName( ix, iy, S2DI.i[2] );
    saveMCAData0( mcaFile.canonicalFilePath() );
  }
}


QFileInfo MainWindow::S2DGenerateMCAFileName( int i1, int i2, int i3 )
{
  QFileInfo BaseFile( S2DI.MCAFile );
  QFileInfo mcaFile;

  if ( S2DI.Use3rdAx ) {
    mcaFile = QFileInfo( mcaDir,
			 QString( "%1-%2-%3-%4.dat" )
			 .arg( BaseFile.baseName() )
			 .arg( i1, 4, 10, QChar( '0' ) )
			 .arg( i2, 4, 10, QChar( '0' ) )
			 .arg( i3, 4, 10, QChar( '0' ) ) );
  } else {
    mcaFile = QFileInfo( mcaDir,
			 QString( "%1-%2-%3.dat" )
			 .arg( BaseFile.baseName() )
			 .arg( i1, 4, 10, QChar( '0' ) )
			 .arg( i2, 4, 10, QChar( '0' ) ) );
  }

  return mcaFile;
}

void MainWindow::S2DWriteBlankLine( void )
{
  if ( S2DFile.simplified().isEmpty() )
    return;

  QFile f( S2DFile );

  if ( !f.open( QIODevice::Append | QIODevice::Text ) )
    return;

  QTextStream out(&f);
  //  out << "#" << endl;
  out << "" << endl;         // gnuplot は # だと、データ列の切れ目だと思ってくれない

  f.close();
}

void MainWindow::S2DWriteTail( void )  // 終了時の時間と I0 だけ記録 (ファイル末尾)
{
  if ( S2DFile.simplified().isEmpty() )
    return;

  QFile f( S2DFile );

  if ( !f.open( QIODevice::Append | QIODevice::Text ) )
    return;

  QTextStream out(&f);

  out << "#" << " Date      : " << QDateTime::currentDateTime()
                                     .toString("yy.MM.dd hh:mm:ss.zzz") << endl;
  if ( SLS != NULL ) 
    out << "#" << " Ring Cur. : " << SLS->value().toDouble() << "[mA]" << endl;

  f.close();
}
 
 void MainWindow::S2DMoveToPointedPosition( int ix, int iy )
 {
   if (( ! S2DInfoIsValid )||( inS2D ))
     return;

   double x = S2DI.sx[0] + S2DI.dx[0] * ( ix + 0.5 );
   double y = S2DI.sx[1] + S2DI.dx[1] * iy;

   S2DI.unit[0]->SetHighSpeed( S2DI.unit[0]->highestSpeed() );
   S2DI.unit[0]->SetSpeed( HIGH );
   S2DI.unit[1]->SetHighSpeed( S2DI.unit[1]->highestSpeed() );
   S2DI.unit[1]->SetSpeed( HIGH );

   S2DI.unit[0]->SetValue( S2DI.unit[0]->u2p( x ) );
   S2DI.unit[1]->SetValue( S2DI.unit[1]->u2p( y ) );
 }


void MainWindow::S2DReCalcMap( double s, double e )
{
  setAllROIs();

  if ( ! S2DInfoIsValid )
    return;

  QFileInfo mcaFile;
  double sum = 0;
  double lastsum = 0;

  if ( S2DI.ScanMode == STEP ) {
    for ( int i = 0; i <= S2DI.ps[1]; i++ ) {
      for ( int j = 0; j < S2DI.ps[0]; j++ ) {
	mcaFile = S2DGenerateMCAFileName( j, i, S2DI.ps[2] );
	if ( ( sum = S2DReCalcAMapPoint( mcaFile.canonicalFilePath(), s, e ) ) > 0 ) {
	  S2DV->setData( j, i, sum );
	} else {
	  return;
	}
	lastsum = sum;
      }
    }
  } else {
    for ( int i = 0; i <= S2DI.ps[1]; i++ ) {
      if (( S2DI.ScanBothDir ) && (( i % 2 ) == 1 )) {
	for ( int j = S2DI.ps[0]; j >= 0; j-- ) {
	  mcaFile = S2DGenerateMCAFileName( j, i, S2DI.ps[2] );
	  if ( ( ( sum = S2DReCalcAMapPoint( mcaFile.canonicalFilePath(), s, e ) ) > 0 )
	       && ( j < S2DI.ps[0] ) ) {
	    S2DV->setData( j, i, sum - lastsum );
	  }
	  if ( sum < 0 )
	    return;
	  lastsum = sum;
	}
      } else {
	for ( int j = 0; j <= S2DI.ps[0]; j++ ) {
	  mcaFile = S2DGenerateMCAFileName( j, i, S2DI.ps[2] );
	  if ( ( ( sum = S2DReCalcAMapPoint( mcaFile.canonicalFilePath(), s, e ) ) > 0 )
	       && ( j > 0 ) ) {
	    S2DV->setData( j - 1, i, sum - lastsum );
	  }
	  if ( sum < 0 )
	    return;
	  lastsum = sum;
	}
      }
    }
  }
}

double MainWindow::S2DReCalcAMapPoint( QString fname, double s, double e )
{
  QStringList vals;
  QFile f( fname );
  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return -1.;

  QTextStream in( &f );
  double eng;
  double cnt, sum = 0;
  while( !in.atEnd() ) {
    vals = in.readLine().simplified().split( QRegExp( "\\s+" ) );
    if (( vals[0] != "#" )&&( vals.count() >= 36 )) {
      for ( int i = 0; i < 19; i++ ) {
	if ( SSDbs2[i]->isChecked() == PBTrue ) {
	  eng = vals[i*2+1].toDouble();
	  cnt = vals[i*2+2].toDouble();
	  if (( eng >= s )&&( eng <= e )) {
	    sum += cnt;
	  }
	}
      }
    }
  }
  f.close();
  return sum;
}
