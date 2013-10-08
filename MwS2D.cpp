
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
  S2Dps << 0 << 0 << 0;
  S2Dnow << 0 << 0 << 0;
  S2Dsx << 0 << 0 << 0;
  S2Dex << 0 << 0 << 0;
  S2Ddx << 0 << 0 << 0;
  S2Di << 0 << 0 << 0;
  S2DScanMode = STEP;

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
    S2DUse3rdAx->setChecked( false );
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
    S2DUse3rdAx->setChecked( false );
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
    if ( S2DMotors[i] == sender() ) {
      S2DCurPos[i]->setText( QString::number( S2DMotors[i]->metricValue() ) );
    }
  }
}

void MainWindow::S2DScanStart( void )
{
  AUnit *as = NULL;

  if ( !inS2D ) {
    SetupS2DParams();   // スキャンパラメータを GUI から内部変数にコピー

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
    // 計測時間が 0 になってないかチェック
    if ( S2DDwell <= 0 ) {
      statusbar->showMessage( tr( "Meas Time is 0 or less." ), 2000 );
    }

    QString User;
    for ( int i = 0; i < S2DMotors.count(); i++ ) {
      if ( S2DMotorUse[i] ) {
	// スキャン範囲に幅があるかをチェック
	if ( S2DStarts[i]->text().toDouble() == S2DEnds[i]->text().toDouble() ) {
	  statusbar->showMessage( tr( "2D Scan Range Error." ), 2000 );
	  return;
	}
	// 測定点数が 1以上あることを確認
	if ( S2DPoints[i]->text().toInt() <= 0 ) {
	  statusbar->showMessage( tr( "2D Scan Points Error." ), 2000 );
	  return;
	}
	// 軸がダブってないかチェック
	for ( int j = i+1; j < S2DMotors.count(); j++ ) {
	  if ( S2DMotorUse[j] ) {
	    if ( S2DMotors[i] == S2DMotors[j] ) {
	      statusbar
		->showMessage( tr( "The same motor was selected for different axis." ),
			       2000 );
	      return;
	    }
	  }
	}

	if ( ( User = UUnits.isTheUnitInUse( S2DMotors[i] ) ) != "" ) {
	  // モーターが他のことに使われていたらダメ
	  statusbar->showMessage( tr( "The Motor [%1] is used by the process %2!" )
				  .arg( S2DMotors[i]->getName() ).arg( User ), 2000 );
	  return;
	}
      }
    }

    mUnits.clearUnits();
    mUnits.addUnit( as = S2DOkSensors.value( SelectS2DSensor->currentIndex() ) );
    mUnits.setDwellTimes( S2DDwell );
    mUnits.setDwellTime();
    for ( int i = 0; i < mUnits.count(); i++ ) {
      if ( ! mUnits.at(i)->isEnable() ) {
	QString msg = QString( tr( "2D Scan cannot Start : (%1) is disabled" ) )
	  .arg( mUnits.at(i)->getName() );
	statusbar->showMessage( msg, 2000 );
	NewLogMsg( msg );
	return;
      }

      if ( ( User = UUnits.isTheUnitInUse( mUnits.at(i) ) ) != "" ) {
	// 検出器が他のことに使われたらダメ
	statusbar->showMessage( tr( "The Sensor [%1] is used by the process %2!" )
				.arg( mUnits.at(i)->getName() ).arg( User ), 2000 );
	return;
      }
    }

    isS2DSFluo = false;
    if ( as == SFluo ) {
      isS2DSFluo = true;
    }
    if ( S2DStepScan->isChecked() )
      S2DScanMode = STEP;
    if ( S2DQuasiContScan->isChecked() ) {
      if ( ! CheckOkList( as, CScanOk ) ) {
	NewLogMsg( tr( "Continuous scan is not available now." ) );
	return;
      }
      S2DScanMode = QCONT;
    }
    if ( S2DRealContScan->isChecked() ) {
      if ( ! CheckOkList( as, CScanOk ) ) {
	NewLogMsg( tr( "Continuous scan is not available now." ) );
	return;
      }
      S2DScanMode = RCONT;
    }

    inS2D = true;
    NewLogMsg( QString( tr( "2D Scan Start (%1 %2 %3 (%4))" ) )
	       .arg( as->getName() )
	       .arg( S2DMotors[0]->getName() )
	       .arg( S2DMotors[1]->getName() )
	       .arg( S2DMotors[2]->getName() ) );

    S2DStart->setText( tr( "Stop" ) );
    S2DStart->setStyleSheet( InActive );
    
    S2DDataStat = NEW;
    S2DFileName->setStyleSheet( FSTATCOLORS[ ScanDataStat ][ ScanNameStat ] );
    S2DFileName->setToolTip( FSTATMsgs[ ScanDataStat ][ ScanNameStat ] );

    // 1st と 2nd の軸の単位が同じなら、表示の縦横比をスキャン範囲の
    // 縦横比に合わせるように努力する。
    // そうでなければ、画面いっぱいを使う。
    if ( S2DMotors[0]->getUnit() == S2DMotors[1]->getUnit() ) {
      S2DV->setRatioType( REAL_RATIO );
    } else {
      S2DV->setRatioType( AS_SCREEN );
    }
    S2DV->setRange( S2Dsx[0], S2Dsx[1],
		    S2Ddx[0], S2Ddx[1],
		    S2Dps[0], S2Dps[1]+1 );

    for ( int i = 0; i < S2DMotors.count(); i++ ) {
      if ( S2DMotorUse[i] ) {
	UUnits.addUnit( S2D_ID, S2DMotors[i] );
      }
    }
    for ( int i = 0; i < mUnits.count(); i++ ) {
      UUnits.addUnit( S2D_ID, mUnits.at(i) );
    }

    S2DLastV = 0;
    S2DFile = S2DFileName->text();
    S2DWriteHead();
    mUnits.clearStage();
    S2DStage = 0;
    S2DTimer->disconnect();
    S2DScanDir = FORWARD;
    switch ( S2DScanMode ) {
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
    S2DTimer->start( 10 );
  } else {
    S2DStop0();
  }
}

void MainWindow::SetupS2DParams( void )
{
  S2DDwell = S2DTime1->text().toDouble();
  for ( int i = 0; i < S2DAxis.count(); i++ ) {
    S2Dnow[i] = S2DMotors[i]->value().toInt();
    if ( S2DRelAbs[i]->stat() == ABS ) {
      S2Dsx[i] = S2DStarts[i]->text().toDouble();
      S2Dex[i] = S2DEnds[i]->text().toDouble();
    } else {
      S2Dsx[i] = S2DCurPos[i]->text().toDouble() + S2DStarts[i]->text().toDouble();
      S2Dex[i] = S2DCurPos[i]->text().toDouble() + S2DEnds[i]->text().toDouble();
    }
    S2Dps[i] = abs( S2DPoints[i]->text().toInt() );
    if ( S2Dps[i] == 0 )
      S2Dps[i] = 1;
    S2Ddx[i] = ( S2Dex[i] - S2Dsx[i] ) / S2Dps[i];
    if ( S2Ddx[i] == 0 ) {
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
  if ( S2DStepScan->isChecked() ) {
    out << "Step Scan" << endl;
  } else if ( S2DQuasiContScan->isChecked() ) {
    if ( S2DContScanBothDir->isChecked() ) {
      out << "Quasi Cont. Scan in Both Dir" << endl;
    } else {
      out << "Quasi Cont. Scan in Single Dir" << endl;
    }
  } else if ( S2DRealContScan->isChecked() ) {
    if ( S2DContScanBothDir->isChecked() ) {
      out << "Real Cont. Scan in Both Dir" << endl;
    } else {
      out << "Real Cont. Scan in Single Dir" << endl;
    }
  } else {
    qDebug() << "No Scan mode is selected !!";
    f.close();
    return;
  }

  for ( int i = 0; i < S2DMotors.count(); i++ ) {
    if ( S2DMotorUse[i] ) {
      out << "#" << QString( " Axis %1    : " ).arg( i, 1 )
	  << QString( " %1" ).arg( S2Dsx[i], 10 )
	  << QString( " %1" ).arg( S2Dex[i], 10 )
	  << QString( " %1" ).arg( S2Ddx[i], 10 )
	  << QString( " %1" ).arg( S2Dps[i], 10 )
	  << " : " << S2DMotors[i]->getName() << endl;
    }
  }

  out << "#" << QString( " Dwell Time : %1" ).arg( S2DDwell ) << endl;

  out << "#" << endl;

  f.close();
}

void MainWindow::S2DWriteBody( double v )
{
  if ( S2DFile.simplified().isEmpty() )
    return;

  QFile f( S2DFile );

  if ( !f.open( QIODevice::Append | QIODevice::Text ) )
    return;

  QTextStream out(&f);

  for ( int i = 0; i < S2DMotors.count(); i++ ) {
    if ( S2DMotorUse[i] ) {
      out << QString( " %1" ).arg( S2Dsx[i] + S2Di[i] * S2Ddx[i], 10 );
    }
  }
  out <<  QString( " %1" ).arg( v, 10 ) << endl;

  f.close();

  S2DWriteBody2();
}

void MainWindow::S2DWriteBody2( void )
{
  if ( isS2DSFluo && S2DRecordMCA->isChecked() ) {
    QFileInfo BaseFile( S2DFile );
    QFileInfo mcaFile;
    if ( S2DUse3rdAx->isChecked() ) {
      mcaFile = QFileInfo( mcaDir,
			   QString( "%1-%2-%3-%4.dat" )
			   .arg( BaseFile.baseName() )
			   .arg( S2Di[0], 4, 10, QChar( '0' ) )
			   .arg( S2Di[1], 4, 10, QChar( '0' ) )
			   .arg( S2Di[2], 4, 10, QChar( '0' ) ) );
    } else {
      mcaFile = QFileInfo( mcaDir,
			   QString( "%1-%2-%3.dat" )
			   .arg( BaseFile.baseName() )
			   .arg( S2Di[0], 4, 10, QChar( '0' ) )
			   .arg( S2Di[1], 4, 10, QChar( '0' ) ) );
    }
    saveMCAData0( mcaFile.canonicalFilePath() );
  }
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
