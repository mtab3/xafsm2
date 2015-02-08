
#include "MainWindow.h"

#include "global.h"
#include "XafsM.h"
#include "SMsg.h"

#define LOWER_LIMIT_OF_DWELL_TIME_IN_REAL_CONT_SCAN ( 0.25 )

void MainWindow::setupScan2DArea( void )
{
  S2Dview = S2DBase->getView();
  S2DBase->setLoadBHidden( true );
  S2DBase->setParent( this );
  S2DBase->setLayout( S2DVFrame->layout() );

  S2DStart->setStyleSheet( NormalEXECB );
  
  S2DFileSel = new QFileDialog;
  S2DFileSel->setAcceptMode( QFileDialog::AcceptSave );
  S2DFileSel->setDirectory( QDir::currentPath() );
  S2DFileSel->setNameFilter( "*.dat" );
  S2DFileSel->setConfirmOverwrite( false );
  
  QPushButton *tmpB;
  S2DAskOverWrite = new QMessageBox;
  tmpB = S2DAskOverWrite->addButton( tr( "Cancel" ), QMessageBox::RejectRole );
  S2DAskOverWrite->addButton( tr( "OK" ), QMessageBox::AcceptRole );
  S2DAskOverWrite->setWindowTitle( tr( "Over Write ?" ) );
  S2DAskOverWrite->setDefaultButton( tmpB );
  connect( S2DAskOverWrite, SIGNAL( accepted() ), this, SLOT( S2DOkOverWrite() ),
	   Qt::UniqueConnection );
  connect( S2DAskOverWrite, SIGNAL( rejected() ), this, SLOT( S2DScanNotStart() ),
	   Qt::UniqueConnection );

  S2DAskOverWrite2 = new QMessageBox;
  tmpB = S2DAskOverWrite2->addButton( tr( "Cancel" ), QMessageBox::RejectRole );
  S2DAskOverWrite2->addButton( tr( "OK" ), QMessageBox::AcceptRole );
  S2DAskOverWrite2->setWindowTitle( tr( "Over Write ?" ) );
  S2DAskOverWrite2->setDefaultButton( tmpB );
  connect( S2DAskOverWrite2, SIGNAL( accepted() ), this, SLOT( S2DOkOverWrite2() ),
	   Qt::UniqueConnection );

  S2DAxis << S2DAx1 << S2DAx2 << S2DAx3;
  S2DCurPos << S2DCurPos1 << S2DCurPos2 << S2DCurPos3;
  S2DUnits << S2DUnit1 << S2DUnit2 << S2DUnit3;
  S2DStarts << S2DStartP1 << S2DStartP2 << S2DStartP3;
  S2DEnds << S2DEndP1 << S2DEndP2 << S2DEndP3;
  S2DSteps << S2DStep1 << S2DStep2 << S2DStep3;
  S2DPoints << S2DPoints1 << S2DPoints2 << S2DPoints3;
  S2DRelAbs << S2DRelAbs1 << S2DRelAbs2 << S2DRelAbs3;
  S2DSelectedMotors << NULL << NULL << NULL;

  //  S2Dview->setParent( this );
  S2DFileCheckIsReady = false;

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

  CheckS2DDwellTime();

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

  connect( S2DStepScan, SIGNAL( clicked() ), this, SLOT( CheckS2DDwellTime() ),
	   Qt::UniqueConnection );
  connect( S2DQuasiContScan, SIGNAL( clicked() ), this, SLOT( CheckS2DDwellTime() ),
	   Qt::UniqueConnection );
  connect( S2DRealContScan, SIGNAL( clicked() ), this, SLOT( CheckS2DDwellTime() ),
	   Qt::UniqueConnection );
  connect( S2DTime1, SIGNAL( editingFinished() ), this, SLOT( CheckS2DDwellTime() ),
	   Qt::UniqueConnection );

  connect( S2DFileSave, SIGNAL( clicked() ), this, SLOT( SaveS2DResult0() ),
	   Qt::UniqueConnection );
  connect( S2DSaveMCAs, SIGNAL( clicked() ), this, SLOT( SaveS2DMCAs() ),
	   Qt::UniqueConnection );

  S2DPrintD = new QPrintDialog;
#if 0
  connect( S2DBase, SIGNAL( print() ), S2DPrintD, SLOT( show() ),
	   Qt::UniqueConnection );
#endif
  connect( S2DPrintB, SIGNAL( clicked() ), S2DPrintD, SLOT( show() ),
	   Qt::UniqueConnection );
#if 0
  connect( S2DPrintD, SIGNAL( accepted( QPrinter * ) ),
	   S2Dview, SLOT( print( QPrinter * ) ), Qt::UniqueConnection );
#endif
  connect( S2DPrintD, SIGNAL( accepted( QPrinter * ) ),
	   S2DBase, SLOT( print( QPrinter * ) ), Qt::UniqueConnection );
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
  CheckS2DDwellTime();
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
    if ( p > 0 ) {
      S2DSteps[i]
	->setText( QString::number( ( S2DEnds[i]->text().toDouble()
				      - S2DStarts[i]->text().toDouble() ) / p ) );
    } else {
      S2DSteps[i]
	->setText( QString::number( ( S2DEnds[i]->text().toDouble()
				      - S2DStarts[i]->text().toDouble() ) / 1 ) );
    }
  }
  CheckS2DDwellTime();
}


void MainWindow::newAx0( int ax, int motor )
{
  double pos;
  S2DUnits[ax]->setText( S2DOkMotors[ motor ]->getUnit() );
  S2DCurPos[ax]->setText( QString::number( pos = S2DOkMotors[ motor ]->metricValue() ) );
  if ( S2Dview != NULL )
    S2Dview->setNowPosition( ax, pos );

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

void MainWindow::SaveS2DResult0( void )
{
  QFileInfo f( S2DFileName0->text() );

  if ( f.exists() ) {
    S2DAskOverWrite2
      ->setText( tr( "File [%1] Over Write ?" ).arg( f.completeBaseName() ) );
    S2DAskOverWrite2->show();
  } else {
    S2DI.SaveFile = S2DFileName0->text();
    SaveS2DResult();
  }
}

void MainWindow::S2DOkOverWrite2( void )
{
  S2DI.SaveFile = S2DFileName0->text();
  SaveS2DResult();
}

void MainWindow::newS2DFileSelected( const QString &fname )
{
  QString Fname = CheckFNameExt( fname, "dat" );
  S2DFileName0->setText( Fname );
  QFileInfo f( Fname );

  if ( f.exists() ) {
    S2DNameStat = OLD;
    S2DDataStat = NEW;
    // 本当はファイルは New ではないが、ファイルの中身と(おそらく)違うので、
    // 書き込んではいけないという意味で、{ Name, Data } = { OLD, NEW } としておく。
  } else {
    S2DNameStat = NEW;
  }
  S2DFileName0->setStyleSheet( FSTATCOLORS[ S2DDataStat ][ S2DNameStat ] );
  S2DFileName0->setToolTip( FSTATMsgs[ S2DDataStat ][ S2DNameStat ] );
}

void MainWindow::showS2DNewAxValue( QString )
{
  double pos;

  for ( int i = 0; i < S2DAxis.count(); i++ ) {
    if ( S2DSelectedMotors[i] == sender() ) {
      S2DCurPos[i]->setText( QString::number( pos = S2DSelectedMotors[i]->metricValue() ) );
      if ( S2Dview != NULL )
	S2Dview->setNowPosition( i, pos );
    }
  }
}

void MainWindow::S2DScanStart( void )
{
  AUnit *as = NULL;

  if ( !inS2D ) {
    S2DInfo oldInfo = S2DI;
    CheckS2DDwellTime();
    SetupS2DParams();   // スキャンパラメータを GUI から内部変数にコピー

    // inMMove のチェック不要(後で UUnits のチェックがあるので...)
    // 他のチェックもいらないかも
    // 自分自信が動いてるかどうかのチェックだけは必要だが
    // 使用するUnitがダブっていなければ他の動作が並列で走っても構わないはず
    if ( inMeas || inSPSing || inMonitor || inMCAMeas ) {
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

	if ( ( User = UUnits.user( S2DI.unit[i] ) ) != "" ) {
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

      if ( ( User = UUnits.user( mUnits.at(i) ) ) != "" ) {
	// 検出器が他のことに使われたらダメ
	statusbar->showMessage( tr( "The Sensor [%1] is used by the process %2!" )
				.arg( mUnits.at(i)->getName() ).arg( User ), 2000 );
	S2DI = oldInfo;
	return;
      }
    }

    S2DI.isSFluo = false;
    if ( as == SFluo ) {
      S2DI.isSFluo = true;
      //      if ( cMCAView == NULL ) {
      getNewMCAView();   // ここで確実に MCAData が有効になる
      cMCAViewC->setDeletable( false );
    }
    switch( S2DI.ScanMode ) {
    case STEP: break;
    case QCONT:
      if ( ! CheckOkList( as, CScanOk ) ) {
	NewLogMsg( tr( "Continuous scan is not available now." ) );
	S2DI = oldInfo;
	return;
      }
      break;
    case RCONT:
      if ( ! CheckOkList( as, CScanOk ) ) {
	NewLogMsg( tr( "Continuous scan is not available now." ) );
	S2DI = oldInfo;
	return;
      }
      break;
    }

    // ここから先は return しない

    S2DBase->setS2DI( S2DI );
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
    S2DFileName0->setStyleSheet( FSTATCOLORS[ ScanDataStat ][ ScanNameStat ] );
    S2DFileName0->setToolTip( FSTATMsgs[ ScanDataStat ][ ScanNameStat ] );

    // 1st と 2nd の軸の単位が同じなら、表示の縦横比をスキャン範囲の
    // 縦横比に合わせるように努力する。
    // そうでなければ、画面いっぱいを使う。
    if ( S2DI.unit[0]->getUnit() == S2DI.unit[1]->getUnit() ) {
      S2Dview->setRatioType( REAL_RATIO );
    } else {
      S2Dview->setRatioType( AS_SCREEN );
    }
    S2Dview->setRange( S2DI.sx[0], S2DI.sx[1]-S2DI.dx[1]/2,
		    S2DI.dx[0], S2DI.dx[1],
		    S2DI.ps[0], S2DI.ps[1]+1 );

    for ( int i = 0; i < S2DI.motors; i++ ) {
      if ( S2DI.used[i] ) {
	UUnits.addAnUnit( S2D_ID, S2DI.unit[i] );
      }
    }
    for ( int i = 0; i < mUnits.count(); i++ ) {
      UUnits.addAnUnit( S2D_ID, mUnits.at(i) );
    }

    S2DBase->mapNew( S2DI.ps[0]+((S2DI.ScanMode == STEP)?0:1),
		     S2DI.ps[1]+1, MCALength, SAVEMCACh );
    S2DLastV = 0;
    S2DI.MCAFile = S2DI.SaveFile;
    if ( S2DI.MCAFile.isEmpty() )
      S2DI.MCAFile = QString( "S2DMCA0000.dat" );
    //    S2DWriteHead();
    S2DFileCheck();
    mUnits.clearStage();
    S2DStage = 0;
    S2DScanDir = FORWARD;

    S2DTimer->disconnect();
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
    S2DI.valid = true;
    S2DTimer->start( 10 );
  } else {
    S2DStop0();
    cMCAViewC->setDeletable( true );
  }
}

void MainWindow::S2DFileCheck( void )
{
  QFileInfo f( S2DI.SaveFile );
  if ( f.exists() ) {
    S2DFileCheckIsReady = false;
    S2DAskOverWrite
      ->setText( tr( "File [%1] Over Write ?" ).arg( f.completeBaseName() ) );
    S2DAskOverWrite->show();
  } else {
    S2DFileCheckIsReady = true;
  }
}

void MainWindow::S2DOkOverWrite( void )
{
  S2DFileCheckIsReady = true;
}

void MainWindow::S2DScanNotStart( void )
{
  S2DStop0();
  S2DFileCheckIsReady = true;
}

void MainWindow::CheckS2DDwellTime( void )
{
  AUnit *as = S2DSelectedMotors[0];
  double sx, ex, dx, dwell;
  int pps, ps;

  dwell = S2DTime1->text().toDouble();
  if ( dwell == 0 ) dwell = 1;

  sx = S2DStarts[0]->text().toDouble();
  ex = S2DEnds[0]->text().toDouble();
  ps = S2DPoints[0]->text().toInt();
  if ( ps < 1 ) ps = 1;
  dx = ( ex - sx ) / ps;

  if ( S2DStepScan->isChecked() ) { // Step Scan
    // Step Scan の時 dwell にパルスモータの最高速に起因した制限は無いはず
  }
  else if ( S2DQuasiContScan->isChecked() ) {
    pps = (int)fabs( dx / as->getUPP() / dwell );
    if ( pps > as->highestSpeed() ) { // Quasi Cont. Scan
      pps = as->highestSpeed();
    }
    dwell = fabs( dx / as->getUPP() / pps );
  }
  else if ( S2DRealContScan->isChecked() ) { // Real Cont. Scan
    if ( dwell / ps < 0.25 ) dwell = ps * LOWER_LIMIT_OF_DWELL_TIME_IN_REAL_CONT_SCAN;
    pps = (int)fabs( dx * ps / as->getUPP() / dwell );
    if ( pps > as->highestSpeed() ) {
      pps = as->highestSpeed();
    }
    dwell = fabs( dx * ps / as->getUPP() / pps );
  }
  S2DTime1->setText( QString::number( dwell ) );
  S2DPoints[0]->setText( QString::number( ps ) );
  S2DSteps[0]->setText( QString::number( dx ) );
}

void MainWindow::SetupS2DParams( void )
{
  S2DI.SaveFile = S2DFileName0->text();
  S2DI.ScanMode = STEP;  // default
  if ( S2DQuasiContScan->isChecked() )
    S2DI.ScanMode = QCONT;
  if ( S2DRealContScan->isChecked() )
    S2DI.ScanMode = RCONT;

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
  S2DI.startDir = ( S2DI.sx[0] < S2DI.ex[0] ) ? FORWARD : BACKWARD;

  int pps = 0;
  if ( S2DI.ScanMode == RCONT ) {
    pps = (int)fabs( (double)S2DI.dx[0] * S2DI.ps[0]
			 / S2DI.unit[0]->getUPP()
			 / S2DI.Dwell );
  } else {
    pps = (int)fabs( (double)S2DI.dx[0]
		     / S2DI.unit[0]->getUPP()
		     / S2DI.Dwell );
    
    
  }
  if ( pps == 0 ) pps = 1;
  if ( pps > S2DI.unit[0]->highestSpeed() ) {
    QString msg = tr( "The scan speed %1 was limited to %2" )
      .arg( pps ).arg( S2DI.unit[0]->highestSpeed() );
    statusbar->showMessage( msg, 2000 );
    pps = S2DI.unit[0]->highestSpeed();
  }
  S2DI.pps = pps;
  S2DI.Dwell = fabs( S2DI.sx[0] - S2DI.ex[0] ) / S2DI.unit[0]->getUPP() / pps;
}

void MainWindow::S2DStop0( void )
{
  S2DStart->setText( tr( "Start" ) );
  S2DStage = S2D_END_STAGE;

  S2DDataStat = OLD;
  S2DDataStat = OLD;
  S2DFileName0->setStyleSheet( FSTATCOLORS[ ScanDataStat ][ ScanNameStat ] );
  S2DFileName0->setToolTip( FSTATMsgs[ ScanDataStat ][ ScanNameStat ] );

}

void MainWindow::S2DStop00( void )
{
  inS2D = false;
  UUnits.removeUnits( S2D_ID );
  NewLogMsg( QString( tr( "2D Scan Finished." ) ) );
  statusbar->showMessage( QString( tr( "2D Scan Finished." ) ), 2000 );
  S2DStart->setText( tr( "Start" ) );
  S2DStart->setStyleSheet( NormalEXECB );
  S2DTimer->stop();
}

void MainWindow::SaveS2DResult( void )
{
  // S2D の事後セーブ
  // 一番最近の S2D の結果だけがセーブの対象。
  // S2DI が MainWindow 側にしか無いから
  S2DView *view = S2DBase->getView();
  if ( view == NULL ) {
    statusbar->showMessage( tr( "S2D data is not valid" ), 2000 );
    return;
  }

  if ( S2DI.valid ) {
    if ( ! S2DI.SaveFile.simplified().isEmpty() ) {
      QFile f( S2DI.SaveFile );
      
      if ( !f.open( QIODevice::Append | QIODevice::Text ) )
	return;
      
      QTextStream out(&f);

      S2DWriteHead( out );
      S2DI.save( out );
      S2DWriteHead2( out );
      
      for ( int iy = 0; iy <= S2DI.ps[1]; iy++ ) {
	for ( int ix = 0; ix < S2DI.ps[0]; ix++ ) {
	  out << QString( " %1" ).arg( S2DI.sx[0] + ( ix + 0.5 ) * S2DI.dx[0], 10 )
	      << QString( " %1" ).arg( S2DI.sx[1] + ( iy ) * S2DI.dx[1], 10 )
	      << QString( " %1" ).arg( view->getData( ix, iy ), 10 )
	      << endl;
	}
	out << endl;
      }
      
      f.close();
    }
  }
  S2DNameStat = OLD;
  S2DDataStat = OLD;
  S2DFileName0->setStyleSheet( FSTATCOLORS[ S2DDataStat ][ S2DNameStat ] );
  S2DFileName0->setToolTip( FSTATMsgs[ S2DDataStat ][ S2DNameStat ] );
}

void MainWindow::S2DWriteHead0 ( void )
{
  if ( S2DI.SaveFile.simplified().isEmpty() )
    return;

  QFile f( S2DI.SaveFile );

  if ( !f.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;

  // Writing fixed headers
  QTextStream out(&f);

  S2DWriteHead( out );
  S2DI.save( out );

  f.close();
}

void MainWindow::S2DWriteHead( QTextStream &out )
{
  // "#" と ":" の間は " " 込で 11桁

  out << FileIDs[ S2DDATA ] << endl;
  out << "#" << " Date      : " << QDateTime::currentDateTime()
                                     .toString("yy.MM.dd hh:mm:ss.zzz") << endl;
  if ( SLS != NULL ) 
    out << "#" << " Ring Cur. : " << SLS->value().toDouble() << "[mA]" << endl;
}

void MainWindow::S2DWriteHead2( QTextStream &out )
{
  out << "# ***************************************************" << endl;
  out << "# ** This file was generated after the measurement **" << endl;
  out << "# ***************************************************" << endl;
  out << endl;
}

void MainWindow::S2DWriteBody( double v )
{
  if ( ! S2DI.SaveFile.simplified().isEmpty() ) {
    QFile f( S2DI.SaveFile );
    
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
  if ( S2DI.isSFluo ) {
    if (( ix < 0 )||( ix > S2DI.ps[0] )
	||( iy < 0 )||( iy > S2DI.ps[1] ))
      return;

    SaveMCADataOnMem( S2DBase->mapAPoint( ix, iy ) );        // iz は無視
  }
}

void MainWindow::SaveMCADataOnMem( aMCASet *set )
{
  if ( set == NULL )
    return;

  set->date = QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" );
  set->RINGCurrent = ( ( SLS != NULL) ? SLS->value().toDouble() : -1 );
  set->I0 = ( ( SI0 != NULL ) ? SI0->value().toDouble() : -1 );

  for ( int ch = 0; ch < SAVEMCACh; ch++ ) {
    double *E = set->Ch[ ch ].E;
    quint32 *cnt = set->Ch[ ch ].cnt;
    for ( int i = 0; i < MCALength; i++ ) {
      E[i] = kev2pix->p2E( ch, i );
      cnt[i] = SFluo->getAMCAdata( ch, i );
    }
    set->Heads[ ch ] = SFluo->getAMCAHead( ch );
    set->ROIStart[ ch ] = ROIStart[ ch ];
    set->ROIEnd[ ch ] = ROIEnd[ ch ];
  }
  if ( cMCAView != NULL ) {
    if ( ShowAlwaysSelElm->isChecked() ) {
      set->Elms = cMCAView->getSelectedElms();
    }
  }
  set->setValid( true );
}

#if 0
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
#endif

void MainWindow::S2DWriteBlankLine( void )
{
  if ( S2DI.SaveFile.simplified().isEmpty() )
    return;

  QFile f( S2DI.SaveFile );

  if ( !f.open( QIODevice::Append | QIODevice::Text ) )
    return;

  QTextStream out(&f);
  //  out << "#" << endl;
  out << "" << endl;         // gnuplot は # だと、データ列の切れ目だと思ってくれない

  f.close();
}

void MainWindow::S2DWriteTail( void )  // 終了時の時間と I0 だけ記録 (ファイル末尾)
{
  if ( S2DI.SaveFile.simplified().isEmpty() )
    return;

  QFile f( S2DI.SaveFile );

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
   if (( ! S2DI.valid )||( inS2D ))
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


void MainWindow::S2DSetROIs( void )
{
  if ( AutoROIsetAll->isChecked() )
    setAllROIs();
}

#if 0
void MainWindow::S2DReCalcMap0( void )
{
  if ( ( ! S2DI.valid ) || inMeas || inMCAMeas || inS2D ) {
    return;
  }

  QFileInfo mcaFile;
  double sum = 0;
  double lastsum = 0;

  if ( S2DI.ScanMode == STEP ) {
    for ( int i = 0; i <= S2DI.ps[1]; i++ ) {
      for ( int j = 0; j < S2DI.ps[0]; j++ ) {
	sum = S2DReCalcAMapPointOnMem( j, i, S2DBase->getMCAMap() );
	if ( sum > 0 ) {
	  S2Dview->setData( j, i, sum );
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
	  sum = S2DReCalcAMapPointOnMem( j, i, S2DBase->getMCAMap() );
	  if ( ( sum > 0 ) && ( j < S2DI.ps[0] ) ) {
	    S2Dview->setData( j, i, sum - lastsum );
	  }
	  if ( sum < 0 )
	    return;
	  lastsum = sum;
	}
      } else {
	for ( int j = 0; j <= S2DI.ps[0]; j++ ) {
	  sum = S2DReCalcAMapPointOnMem( j, i, S2DBase->getMCAMap() );
	  if ( ( sum > 0 ) && ( j > 0 ) ) {
	    S2Dview->setData( j - 1, i, sum - lastsum );
	  }
	  if ( sum < 0 )
	    return;
	  lastsum = sum;
	}
      }
    }
  }
}

double MainWindow::S2DReCalcAMapPoint( QString fname )
{
  QStringList vals;
  QFile f( fname );
  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return -1.;

  QTextStream in( &f );
  double eng;
  double cnt, sum = 0;
  QVector<double> ss;
  QVector<double> es;
  for ( int i = 0; i < MaxSSDs; i++ ) {
    ss << kev2pix->p2E( i, ROIStart[ i ].toDouble() );
    es << kev2pix->p2E( i, ROIEnd[ i ].toDouble() );
  }
  while( !in.atEnd() ) {
    vals = in.readLine().simplified().split( QRegExp( "\\s+" ) );
    if (( vals[0] != "#" )&&( vals.count() >= 36 )) {
      for ( int i = 0; i < MaxSSDs; i++ ) {
	if ( SSDbs2[i]->isChecked() == PBTrue ) {
	  eng = vals[i*2+1].toDouble();
	  cnt = vals[i*2+2].toDouble();
	  if (( eng >= ss[i] )&&( eng <= es[i] )) {
	    sum += cnt;
	  }
	}
      }
    }
  }
  f.close();
  return sum;
}

double MainWindow::S2DReCalcAMapPointOnMem( int ix, int iy, aMCAMap *map )
{
  double sum = 0;

  QVector<double> ss;
  QVector<double> es;
  for ( int i = 0; i < MaxSSDs; i++ ) {
    ss << kev2pix->p2E( i, ROIStart[ i ].toDouble() );
    es << kev2pix->p2E( i, ROIEnd[ i ].toDouble() );
  }

  aMCASet *set = map->aPoint( ix, iy );
  if ( ( set != NULL )&&( set->isValid() ) ) {
    for ( int ch = 0; ch < SAVEMCACh; ch++ ) {
      if ( SSDbs2[ ch ]->isChecked() == PBTrue ) {
	double *E = set->Ch[ ch ].E;
	quint32 *cnt = set->Ch[ ch ].cnt;
	for ( int i = 0; i < MCALength; i++ ) {
	  if (( E[i] >= ss[ch] )&&( E[i] <= es[ch] )) {
	    sum += cnt[i];
	  }
	}
      }
    }
  }

  return sum;
}
#endif

void MainWindow::ShowMCASpectrum( aMCASet *set1, aMCASet *set2 )
{
  quint32 *cnt1 = NULL;
  quint32 *cnt2 = NULL;
  if ( ( set1 != NULL )&&( set1->isValid() ) )
    cnt1 = set1->Ch[ cMCACh ].cnt;
  if ( ( set2 != NULL )&&( set2->isValid() ) )
    cnt2 = set2->Ch[ cMCACh ].cnt;

  if ( cnt1 != NULL ) {
    for ( int i = 0; i < MCALength; i++ ) {
      if ( cnt2 != NULL ) {
	MCAData[i] = abs( cnt1[i] - cnt2[i] );
      } else {
	MCAData[i] = cnt1[i];
      }
    }
    cMCAView->update();
  }
}

#if 0
void MainWindow::S2DShowInfoAtNewPosition( int ix, int iy, aMCASet *set )
{
  if (( ! S2DI.valid )||( inS2D )||( ! S2DI.isSFluo )
      ||( cMCAView == NULL )||( MCAData== NULL ))
    return;

  int dx;
  aMCASet *set1, *set2;
  quint32 *cnt1, *cnt2;

  set1 = map->aPoint( ix, iy );
  if ( ( set1 == NULL ) || ( ! set1->isValid() ) )
    return;
  cnt1 = set1->Ch[ cMCACh ].cnt;

  if ( S2DI.ScanMode == STEP ) {
    for ( int i = 0; i < MCALength; i++ ) {
      MCAData[i] = cnt1[i];
    }
  } else {
    set2 = map->aPoint( ix+1, iy );
    if ( ( set2 == NULL ) || ( ! set2->isValid() ) )
      return;
    cnt2 = set2->Ch[ cMCACh ].cnt;

    // 前進で測定しているときは val(ix+1, iy) - val(ix, iy)
    // 後進で測定しているときは val(ix, iy) - val(ix+1, iy)
    // 片道スキャンなら、最初の行が前進ならずっと前進、後進ならずっと後進
    // 往復スキャンなら、奇数行は最初の行の逆向けのスキャン
    dx = ( S2DI.startDir == FORWARD ) ? 1 : -1;
    if (( S2DI.ScanBothDir )&&( iy % 2 == 1 )) dx *= -1;
    for ( int i = 0; i < MCALength; i++ ) {
      MCAData[i] = ( cnt2[i] - cnt1[i] ) * dx;
    }
  }
  
  cMCAView->update();
}
#endif

void MainWindow::S2DChangeMCACh( int dCh )
{
  int ch = cMCACh + dCh;

  while ( ch < 0 )
    ch += MaxSSDs;
  while ( ch >= MaxSSDs )
    ch -= MaxSSDs;

  MCACh->setValue( ch );
}

#if 0
void MainWindow::S2DShowIntMCA( int ix, int iy, aMCASet *set )
{
  if (( ! S2DI.valid )||( inS2D )||( ! S2DI.isSFluo )
      ||( cMCAView == NULL )||( MCAData== NULL ))
    return;

  //  aMCASet *set;
  quint32 *cnt;

  set = map->aPoint( ix, iy );
  if ( ( set == NULL ) || ( ! set->isValid() ) )
    return;
  cnt = set->Ch[ cMCACh ].cnt;

  for ( int i = 0; i < MCALength; i++ ) {
    MCAData[i] = cnt[i];
  }

  cMCAView->update();
}
#endif
