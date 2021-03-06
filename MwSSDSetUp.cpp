
#include "global.h"
#include "SMsg.h"
#include "MainWindow.h"
#include "MCAView.h"

//#include "PeakFit.h"

void MainWindow::setupSetupSSDArea( void )   /* 測定エリア */
{
  MwSSDGotMCALen = false;
  MCAStart->setStyleSheet( NormalEXECB );
  ROIStart = ROIEnd = NULL;

  MaxMCAEnergyInput->setText( QString::number( MaxMCAEnergy ) );
  connect( MaxMCAEnergyInput, SIGNAL( editingFinished() ),
	   this, SLOT( newMaxMCAEnergy() ), Qt::UniqueConnection );

  SSDbs << SSDE01 << SSDE02 << SSDE03 << SSDE04 << SSDE05
        << SSDE06 << SSDE07 << SSDE08 << SSDE09 << SSDE10
        << SSDE11 << SSDE12 << SSDE13 << SSDE14 << SSDE15
        << SSDE16 << SSDE17 << SSDE18 << SSDE19;
  SSDbs2 << SSDE01_2 << SSDE02_2 << SSDE03_2 << SSDE04_2 << SSDE05_2
	 << SSDE06_2 << SSDE07_2 << SSDE08_2 << SSDE09_2 << SSDE10_2
	 << SSDE11_2 << SSDE12_2 << SSDE13_2 << SSDE14_2 << SSDE15_2
	 << SSDE16_2 << SSDE17_2 << SSDE18_2 << SSDE19_2;
  
  for ( int i = 0; i < SSDbs.count(); i++ ) {
    SSDbs[i]->setStyleSheet( SSDActive );
    SSDbs[i]->setToolTip( tr( "Active" ) );
    connect( SSDbs[i],  SIGNAL( clicked() ), this, SLOT( SelSSDs0() ),
	     Qt::UniqueConnection );
  }

  for ( int i = 0; i < SSDbs2.count(); i++ ) {
    SSDbs2[i]->setStyleSheet( SSDActive );
    SSDbs2[i]->setToolTip( tr( "Active" ) );
    SSDbs2[i]->setChecked( PBTrue );
    connect( SSDbs2[i], SIGNAL( clicked() ), this, SLOT( SelSSDs20() ),
	     Qt::UniqueConnection );
  }

  connect( s, SIGNAL( AnsGetPeakingTime( SMsg ) ),
	   this, SLOT( showPeakingTime( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGetThreshold( SMsg ) ), this, SLOT( showThreshold( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGetCalibration( SMsg ) ),
	   this, SLOT( showCalibration( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGetDynamicRange( SMsg ) ),
	   this, SLOT( showDynamicRange( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGetPreAMPGain( SMsg ) ), this, SLOT( showPreAMPGain( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGetMCALength( SMsg ) ), this, SLOT( getMCALen( SMsg ) ),
	   Qt::UniqueConnection );
  
  connect( MCAStart, SIGNAL( clicked() ), this, SLOT( StartMCA() ),
	   Qt::UniqueConnection );
  connect( MCACh, SIGNAL( valueChanged( int ) ), this, SLOT( MCAChSelected( int ) ),
	   Qt::UniqueConnection );
  connect( ROIStartInput, SIGNAL( textEdited( const QString & ) ),
	   this, SLOT( newROIStart( const QString & ) ),
	   Qt::UniqueConnection );
  connect( ROIEndInput, SIGNAL( textEdited( const QString & ) ),
	   this, SLOT( newROIEnd( const QString & ) ),
	   Qt::UniqueConnection );
  connect( MCAClear, SIGNAL( clicked() ), this, SLOT( clearMCA() ),
	   Qt::UniqueConnection );
  
  connect( SelRealTime, SIGNAL( clicked() ), this, SLOT( RealTimeIsSelected() ),
	   Qt::UniqueConnection );
  connect( SelLiveTime, SIGNAL( clicked() ), this, SLOT( LiveTimeIsSelected() ),
	   Qt::UniqueConnection );

  MCAFSel = new QFileDialog;
  MCAFSel->setAcceptMode( QFileDialog::AcceptSave );
  MCAFSel->setDirectory( QDir::currentPath() );
  MCAFSel->setNameFilter( "*.dat" );

  connect( GainInput, SIGNAL( editingFinished() ), 
	   this, SLOT( newGain() ),
	   Qt::UniqueConnection );

  connect( SelMCARecFile, SIGNAL( clicked() ), MCAFSel, SLOT( show() ),
	   Qt::UniqueConnection );
  connect( MCAFSel, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( setSelectedMCAFName( const QString & ) ),
	   Qt::UniqueConnection );
  //  connect( MCARec, SIGNAL( clicked() ), this, SLOT( saveMCAData0() ),
  //           Qt::UniqueConnection );
  connect( MCARec, SIGNAL( clicked() ), this, SLOT( saveMCAData() ),
	   Qt::UniqueConnection );
  connect( SFluo, SIGNAL( NewMCAsAvailable( char * ) ),
	   this, SLOT( ShowNewMCAStat( char * ) ),
	   Qt::UniqueConnection);

  connect( SFluo, SIGNAL( ReceivedNewMCARealTime( int ) ),
	   this, SLOT( ShowNewMCARealTime( int ) ),
	   Qt::UniqueConnection );
  connect( SFluo, SIGNAL( ReceivedNewMCALiveTime( int ) ),
	   this, SLOT( ShowNewMCALiveTime( int ) ),
	   Qt::UniqueConnection );

  connect( ROIsetAll, SIGNAL( clicked() ), this, SLOT( setAllROIs() ),
	   Qt::UniqueConnection );

  inMCAMeas = false;
  validMCAData = false;
  MCAData = NULL;
  cMCAView = NULL;
  cMCACh = 0;
  //  oldMCACh = -1;
  StartResume = MCA_START;
  cMCAViewTabNo = -1;

  SelSSDs( 0 );

  NonSelC = "#ccccaa";
  SelectC = "#ffffdd";
  PT2 = new PeriodicTable( Vic );
  PT2->SetActionOnSelected( PT_STAY );
  PT2->SetActionOnClosed( PT_CLOSE );
  PT2->SetCheckable( true );
  PT2->ShowAllNoneBs( true );
  for ( int i = 0; i < PT->Atoms(); i++ ) {
    PT2->SetAColor( i, NonSelC );
  }
  connect( SelectElmNames, SIGNAL( clicked() ), PT2, SLOT( show() ),
	   Qt::UniqueConnection );
  PT2->setAll();
  connect( PT2, SIGNAL( AtomToggled( bool, int ) ),
	   fdbase, SLOT( ElementSelected( bool, int ) ),
	   Qt::UniqueConnection );

  connect( SetDisplayLog, SIGNAL( clicked( bool ) ),
	   this, SIGNAL( SignalMCAViewSetDisplayLog( bool ) ),
	   Qt::UniqueConnection );
  connect( DispElmNames, SIGNAL( toggled( bool ) ),
	   this, SIGNAL( SignalMCAViewSetShowElements( bool ) ),
	   Qt::UniqueConnection );
  connect( ShowAlwaysSelElm, SIGNAL( toggled( bool ) ),
	   this, SIGNAL( SignalMCAViewShowAlwaysSelElm( bool ) ),
	   Qt::UniqueConnection );
  connect( ShowElmEnergy, SIGNAL( toggled( bool ) ),
	   this, SIGNAL( SignalMCAViewShowElmEnergy( bool ) ),
	   Qt::UniqueConnection );

  connect( PeakSearchSensitivity, SIGNAL( editingFinished() ), 
	   this, SLOT( newPSSens() ),
	   Qt::UniqueConnection );
  connect( ShowDiff, SIGNAL( toggled( bool ) ), this, SLOT( SelectedShowDiff( bool ) ),
	   Qt::UniqueConnection );
  connect( LimitPSEnergy, SIGNAL( toggled( bool ) ),
	   this, SLOT( SelectedLimitPSEnergy( bool ) ),
	   Qt::UniqueConnection );
  connect( MCAPeakSearch, SIGNAL( toggled( bool ) ),
	   this, SLOT( SelectedPeakSearch( bool ) ),
	   Qt::UniqueConnection );
  connect( ShowSmoothed, SIGNAL( toggled( bool ) ),
	   this, SLOT( SelectedShowSmoothed( bool ) ),
	   Qt::UniqueConnection );
  connect( MCAReFit, SIGNAL( clicked() ), this, SLOT( PushedReFit() ),
	   Qt::UniqueConnection );
  connect( ClearMCAPeaks, SIGNAL( clicked() ), this, SLOT( PushedClearMCAPeaks() ),
	   Qt::UniqueConnection );
  connect( FitToRaw, SIGNAL( toggled( bool ) ),
	   this, SLOT( SelectedFitToRaw( bool ) ),
	   Qt::UniqueConnection );
  connect( PeakCalibrate, SIGNAL( editingFinished() ),
	   this, SLOT( newCalibration() ),
	   Qt::UniqueConnection );
  connect( MaxLoop, SIGNAL( editingFinished() ), this, SLOT( newMaxLoop() ),
	   Qt::UniqueConnection );
  connect( DampFact, SIGNAL( editingFinished() ), this, SLOT( newDampFact() ),
	   Qt::UniqueConnection );
  connect( Prec1, SIGNAL( editingFinished() ), this, SLOT( newPrec1() ),
	   Qt::UniqueConnection );
  connect( Prec2, SIGNAL( editingFinished() ), this, SLOT( newPrec2() ),
	   Qt::UniqueConnection );
  
  // Calibration Tab
  // D.T. Calib.
  if ( SChangers.count() > 0 ) {
    for ( int i = 0; i < SChangers.count(); i++ ) {
      AttChSelect->addItem( SChangers[i]->name() );
    }
    NewAttenCh( 0 );
  } else {
    AttChSelect->setEnabled( false );
    AttenSelect->setEnabled( false );
    AttenMoveTo->setEnabled( false );
    DTAutoCalib->setEnabled( false );
  }
  //  AttenDx = AttenDy = 0;
  connect( AttChSelect, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( NewAttenCh( int ) ), Qt::UniqueConnection );
  connect( AttenMoveTo, SIGNAL( clicked() ), this, SLOT( NewAttenPos() ),
	   Qt::UniqueConnection );
  connect( DTAutoCalib, SIGNAL( clicked() ), this, SLOT( DTAutoCalibStart() ),
	   Qt::UniqueConnection );

  // SSD Energy Calib.
  if ( SSDCalibEnergys.count() > 0 ) {
    for ( int i = 0; i < SSDCalibEnergys.count(); i++ ) {
      EnergySelect->addItem( SSDCalibEnergys[i] );
    }
  } else {
    EnergySelect->setEnabled( false );
    EnergyMoveTo->setEnabled( false );
    // SSDEngAutoCalib->setEnabled( false ); // energy 選択のコンボボックスに値がなくても
                                           // スクリプトがあれば動けるので false にしない
  }
  connect( EnergyMoveTo, SIGNAL( clicked() ),
	   this, SLOT( MoveToNewCaribEnergy() ), Qt::UniqueConnection );
  connect( SSDEngAutoCalib, SIGNAL( clicked() ), this, SLOT( SSDEngAutoCalibStart() ),
	   Qt::UniqueConnection );
}

void MainWindow::newMaxMCAEnergy( void )
{
  MaxMCAEnergy = MaxMCAEnergyInput->text().toDouble();
  if ( MaxMCAEnergy < 20 ) {
    MaxMCAEnergy = 20;
    MaxMCAEnergyInput->setText( QString::number( MaxMCAEnergy ) );
  }

  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setMaxEnergy( MaxMCAEnergy );
      view->update();
    }
  }
}

void MainWindow::newMaxLoop( void )
{
  int maxLoop = MaxLoop->text().toInt();
  if ( maxLoop < 1 ) {
    maxLoop = 1;
  }
  MaxLoop->setText( QString::number( maxLoop ) );

  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setMaxLoop( maxLoop );
      view->update();
    }
  }
}

void MainWindow::newDampFact( void )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setDampFact( DampFact->text().toDouble() );
      view->update();
    }
  }
}

void MainWindow::newPrec1( void )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setPrec1( Prec1->text().toDouble() );
      view->update();
    }
  }
}

void MainWindow::newPrec2( void )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setPrec2( Prec2->text().toDouble() );
      view->update();
    }
  }
}

void MainWindow::MoveToNewCaribEnergy( void )
{
  MoveCurThPosKeV( EnergySelect->currentText().toDouble() );
}

void MainWindow::NewAttenPos( void )
{
  SpecChanger *sc = SChangers[ AttChSelect->currentIndex() ];

  Changer *changer = Changers[ ChangerSelect->currentIndex() ];
  movingSC1 = changer->unit1();
  movingSC2 = changer->unit2();
  connect( movingSC1, SIGNAL( ChangedIsBusy1( QString ) ),
	   this, SLOT( SChangerReached( QString ) ),
	   Qt::UniqueConnection );
  connect( movingSC2, SIGNAL( ChangedIsBusy1( QString ) ),
	   this, SLOT( SChangerReached( QString ) ),
	   Qt::UniqueConnection );

  moveToTarget( sc->baseChangerP(),
		sc->specName( AttenSelect->currentIndex() )->position(),
		AttDx->text().toDouble(), AttDy->text().toDouble() );

  AttenMoveTo->setText( tr( "Moving" ) );
  AttenMoveTo->setStyleSheet( InActive );
  AttenMoveTo->setEnabled( false );
}

void MainWindow::NewAttenCh( int att )
{
  SpecChanger *c = SChangers[ att ];

  AttenSelect->clear();
  for ( int i = 0; i < c->specNames(); i++ ) {
    AttenSelect->addItem( c->specName(i)->name() );
  }
}

void MainWindow::ReadLowerLimitSetting( void )
{
  if ( SFluo == NULL )
    return;

  QString fname;
  QString basefname = "SSDLowLimits";
  QFileInfo fi( basefname + ".txt" );
  if ( fi.exists() )
    fname = basefname + ".txt";
  else 
    fname = QString( ":" ) + basefname + "0.txt";

  QFile file( fname );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    qDebug() << tr( "Cannot open [%1]" ).arg( fname );
    return;
  }
  
  QTextStream in(&file);

  while( !in.atEnd() ) {
    QStringList cols = in.readLine().simplified().split( ' ' );
    if ( cols.count() > 1 ) {
      if ( cols[0][0] != '#' ) {
	SFluo->SetLowLimit( cols[0].toInt(),
			    XMAPk2p->E2p( cols[0].toInt(), cols[1].toDouble()/1000. ) );
      }
    }
  }

  file.close();
}

void MainWindow::newCalibration( void )
{
  if ( inMCAMeas )
    return;
  if ( SFluo == NULL )
    return;

  MCAView *view;   // ここでは view は直接使わないが、MCAPeaks は view 内部へのポインタ
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      double oldE = (*MCAPeaks)[ MCAPeakList->currentIndex() ].BinE;
      if ( oldE <= 0 ) return;
      // 新旧の エネルギー比
      double ratio = PeakCalibrate->text().toDouble() / oldE;
      if ( ratio <= 0 ) return;
      // 一回の入力で、このルーチンに複数回入ってくるとおかしくなるので
      // ある数値で一旦設定したら、入力欄自体をクリアしてしまう
      PeakCalibrate->setText( "" );
      // gain の設定は何故か逆
      SFluo->setGain( MCACh->value(), GainInput->text().toDouble() / ratio );
      // 設定したゲインの読み出し
      SFluo->readGain( MCACh->value() );
#if 0
      s->SendCMD2( "SetUpMCA", SFluo->dev(),   /* not be used */
		   "GetPreAMPGain", QString::number( MCACh->value() ) );
#endif
    }
  }
}

void MainWindow::SelectedLimitPSEnergy( bool f )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setLimitPSEnergy( f );
    }
  }
}

void MainWindow::SelectedShowDiff( bool f )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setShowDiff( f );
    }
  }
}

void MainWindow::SelectedPeakSearch( bool f )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setPeakSearch( f );
    }
  }
}

void MainWindow::SelectedShowSmoothed( bool f )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setShowSmoothed( f );
    }
  }
}

void MainWindow::SelectedFitToRaw( bool f )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setFitToRaw( f );
    }
  }
}

void MainWindow::PushedReFit( void )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      //      PeakFit *PF = new PeakFit;
      //      PF->init( view->getMCAPeaks(), view->getMCALength(), NULL, view->getSMCA() );
      //      PF->fit( true, view->getFLine() );
      view->doPeakFitWCPoints();
      //      delete PF;
    }
  }
}

void MainWindow::PushedClearMCAPeaks( void )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->clearMCAPeaks();
    }
  }
}

void MainWindow::newPSSens( void )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setNewPSSens( PeakSearchSensitivity->text() );
    }
  }
}

void MainWindow::newGain( void )
{
  if ( inMCAMeas )
    return;
  if ( SFluo == NULL )
    return;

  SFluo->setGain( MCACh->value(), GainInput->text().toDouble() );
}

#if 0
void MainWindow::doPeakFit( void )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->doPeakFit();
    }
  }
}
#endif

void MainWindow::setAllROIs( void )
{
  int ch = MCACh->text().toInt();
  double startE = XMAPk2p->p2E( ch, ROIStart[ ch ].toDouble() );
  double endE = XMAPk2p->p2E( ch, ROIEnd[ ch ].toDouble() );
  NewLogMsg( QString( "Set all ROI from %1 to %2 [keV]" ).arg( startE ).arg( endE ) );
  
  for ( int i = 0; i < SFluo->chs(); i++ ) {
    ROIStart[ i ] = QString::number( XMAPk2p->E2p( i, startE ) );
    ROIEnd[ i ] = QString::number( XMAPk2p->E2p( i, endE ) );
    NewLogMsg( QString( "  ROI Ch[%1] from %2 to %3 pix" )
	       .arg( i )
	       .arg( ROIStart[i] ).arg( ROIEnd[i] ) );
  }
  ReCalcXAFSWithMCA();
  ReCalcS2DMap();
  //  S2DReCalcMap0();    // !!!!!!!!!!!!!!!!!!!!!!!!
}

void MainWindow::saveMCAData( void )
{
  // MCA の画面は一つしか無いはずだけど
  XYView *view;   // 現在表示しているのが MCA画面だったらその画面がセーブの対象
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getDType() == MCADATA ) {
    view = (XYView*)ViewCtrls[ ViewTab->currentIndex() ]->getView();
  } else {        // 違ったら、一番最近のMCA結果がセーブの対象
    view = (XYView*)findAView( MCADATA );
  }
  if ( !validMCAData ) {
    statusbar->showMessage( tr( "MCA data is not valid" ), 2000 );
    return;
  }
  if ( MCARecFile->text().isEmpty() ) {
    statusbar->showMessage( tr( "Save file name is not selected" ), 2000 );
    return;
  }
  MCADataStat = OLD;
  MCANameStat = OLD;
  MCARecFile->setStyleSheet( FSTATCOLORS[ MCADataStat ][ MCANameStat ] );
  MCARecFile->setToolTip( FSTATMsgs[ MCADataStat ][ MCANameStat ] );

//  for ( int i = 0; i < 1000; i++ ) {   // 1000面セーブ時間測定用
//    qDebug() << i;                     // i7 で 40 秒(0.04s/面)だった
//    // ROI の積分を XafsM2 側でやるようにし、フルレンジ(0-2047)を ROI の範囲にした場合
//    // 約 43 秒。ROI の積分時間は 最大 3ms 程度という事になる。
  aMCASet *set = new aMCASet;
  set->setSize( SFluo->length(), SFluo->chs() );
  SaveMCADataOnMem( set );
  //  saveMCAData0( MCARecFile->text(), set );
  set->save( MCARecFile->text(), "measured by SSD set up" );
  delete set;
}

#if 0
void MainWindow::saveMCAData0( QString fname, aMCASet *set )
{
  if ( set == NULL )
    return;

  QFile f( fname );
  if ( !f.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
    statusbar->showMessage( tr( "The file [%1] can not open to record the data" )
			    .arg( fname ),
			    2000 );
    return;
  }
  QTextStream out( &f );

  out << FileIDs[ MCADATA ] << "\n";
  out << "# " << set->date << "\n";

  set->writeHead( out );
  set->writeData( out );

  f.close();
}
#endif

void MainWindow::setSelectedMCAFName( const QString &fname )
{
  MCARecFile->setText( CheckFNameExt( fname, "dat" ) );
  MCANameStat = NEW;
  MCARecFile->setStyleSheet( FSTATCOLORS[ MCADataStat ][ MCANameStat ] );
  MCARecFile->setToolTip( FSTATMsgs[ MCADataStat ][ MCANameStat ] );
}

void MainWindow::RealTimeIsSelected( void )
{
  if ( SelRealTime->isChecked() ) {
    SelLiveTime->setChecked( false );
  }
}

void MainWindow::LiveTimeIsSelected( void )
{
  if ( SelLiveTime->isChecked() ) {
    SelRealTime->setChecked( false );
  }
}

void MainWindow::SelSSDs0( void )
{
  for ( int i = 0; i < SSDbs.count(); i++ ) {
    if ( sender() == SSDbs.at(i) ) {
      SelSSDs( i );
      break;
    }
  }
}

void MainWindow::SelSSDs20( void )
{
  for ( int i = 0; i < SSDbs2.count(); i++ ) {
    if ( sender() == SSDbs2.at(i) ) {
      if ( SSDbs2.at(i)->isChecked() == PBTrue ) {
	SSDbs2.at(i)->setStyleSheet( SSDActive );
	SSDbs2.at(i)->setToolTip( tr( "Active" ) );
	SFluo->setSSDUsingCh( i, true );
	emit SelectedSSD( i, true );
      } else {
	SSDbs2.at(i)->setStyleSheet( SSDnotActive );
	SSDbs2.at(i)->setToolTip( tr( "Inactive" ) );
	SFluo->setSSDUsingCh( i, false );
	emit SelectedSSD( i, false );
      }
    }
  }
  ReCalcXAFSWithMCA();
  //  S2DReCalcMap0();     !!!!!!!!!!!!!!!!!!!!!!!
  ReCalcS2DMap();
}

void MainWindow::SelSSDs( int ch )
{
  for ( int i = 0; i < SSDbs.count(); i++ ) {
    if ( i == ch ) {
      SSDbs[i]->setChecked( PBTrue );
      SSDbs[i]->setStyleSheet( SSDActive );
      SSDbs[i]->setToolTip( tr( "Active" ) );
      MCACh->setValue( i );
      MCAChSelected( i );
    } else {
      SSDbs[i]->setChecked( PBFalse );
      SSDbs[i]->setStyleSheet( SSDnotActive );
      SSDbs[i]->setToolTip( tr( "Inactive" ) );
    }
  }
}

/*
 *void MainWindow::getMCASettings( int ch )
 *{
 *  s->SendCMD2( "SetUpMCA", SFluo->dev(), "GetPeakingTime", QString::number( ch ) );
 *  s->SendCMD2( "SetUpMCA", SFluo->dev(), "GetThreshold", QString::number( ch ) );
 *  s->SendCMD2( "SetUpMCA", SFluo->dev(), "GetCalibration", QString::number( ch ) );
 *  s->SendCMD2( "SetUpMCA", SFluo->dev(), "GetDynamicRange", QString::number( ch ) );
 *  s->SendCMD2( "SetUpMCA", SFluo->dev(), "GetPreAMPGain", QString::number( ch ) );
 *
 *  SFluo->GetMCAs();
 *}
 */

void MainWindow::getMCALen( SMsg /* msg */ )  // 初期化の時に一回しか呼ばれないと信じる
{
  if ( MwSSDGotMCALen )
    return;
  MwSSDGotMCALen = true;
#if 0
  if ( ( msg.From() == SFluo->dev() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    MCALength = msg.Val().toInt();
  }
#endif
  ROIStart = new QString [ SFluo->chs() ];
  ROIEnd = new QString [ SFluo->chs() ];
  SFluo->setROIs( ROIStart, ROIEnd );
  for ( int i = 0; i < SFluo->chs(); i++ ) {
    ROIStart[i] = "0";
    ROIEnd[i] = QString::number( SFluo->length() - 1 );
    NewLogMsg( QString( "  ROI Ch[%1] from %2 to %3 pix" )
	       .arg( i )
	       .arg( ROIStart[i] ).arg( ROIEnd[i] ) );
  }
  int ch = MCACh->text().toInt();
  ROIStartInput->setText( ROIStart[ ch ] );
  ROIEndInput->setText( ROIEnd[ ch ] );
  //  kev2pix->setMCALen( MCALength );
  ReadLowerLimitSetting();
}

void MainWindow::newROIStart( const QString &newv )
{
  if ( !inMeas ) {
    ROIStart[ MCACh->text().toInt() ] = newv;
    NewLogMsg( QString( "  ROI Start Ch[%1] = %2 pix" )
	       .arg( MCACh->text().toInt() ).arg( newv ) );
    if ( cMCAView != NULL ) {
      cMCAView->setROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
      cMCAView->update();
    }
    if ( AutoROIsetAll->isChecked() )
      setAllROIs();
    ReCalcXAFSWithMCA();
    // S2DReCalcMap0();    !!!!!
    ReCalcS2DMap();
  } else {
    statusbar->showMessage( tr( "ROI cannot change while the XAFS measurements" ), 2000 );
    ROIStartInput->setText( ROIStart[ MCACh->text().toInt() ] );
  }
}


void MainWindow::newROIEnd( const QString &newv )
{
  if ( !inMeas ) {
    ROIEnd[ MCACh->text().toInt() ] = newv;
    NewLogMsg( QString( "  ROI End Ch[%1] = %2 pix" )
	       .arg( MCACh->text().toInt() ).arg( newv ) );
    if ( cMCAView != NULL ) {
      cMCAView->setROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
      cMCAView->update();
    }
    if ( AutoROIsetAll->isChecked() )
      setAllROIs();
    ReCalcXAFSWithMCA();
    //    S2DReCalcMap0();
    ReCalcS2DMap();
  } else {
    statusbar->showMessage( tr( "ROI cannot change while the XAFS measurements" ), 2000 );
    ROIEndInput->setText( ROIEnd[ MCACh->text().toInt() ] );
  }
}

void MainWindow::MCAChSelected( int i )
{
  if ( i == cMCACh )
    return;

  if ( i < 0 ) { MCACh->setValue( SFluo->chs() - 1 ); i = SFluo->chs() - 1; }
  if ( i >= SFluo->chs() ) { MCACh->setValue( 0 ); i = 0; }
  cMCACh = i;

  emit NewMCACh( cMCACh );

  if ( SFluo != NULL )
    SFluo->getMCASettings( cMCACh );
  ROIStartInput->setText( ROIStart[ cMCACh ] );
  ROIEndInput->setText( ROIEnd[ cMCACh ] );
  if ( cMCAView != NULL ) {
    cMCAView->SetMCACh( cMCACh );
    cMCAView->setROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
    SFluo->GetRealTime( cMCACh );
    SFluo->GetLiveTime( cMCACh );
  }
  SelSSDs( cMCACh );
}

void MainWindow::showPeakingTime( SMsg msg )
{
  if ( ( msg.From() == SFluo->dev() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    PeakingTimeInput->setText( msg.Val() );
  }
}

void MainWindow::showThreshold( SMsg msg )
{
  if ( ( msg.From() == SFluo->dev() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    ThresholdInput->setText( msg.Val() );
  }
}

void MainWindow::showCalibration( SMsg msg )
{
  if ( ( msg.From() == SFluo->dev() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    CalibrationInput->setText( msg.Val() );
  }
}

void MainWindow::showDynamicRange( SMsg msg )
{
  if ( ( msg.From() == SFluo->dev() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    DynamicRangeInput->setText( msg.Val() );
  }
}

void MainWindow::showPreAMPGain( SMsg msg )
{
  if ( ( msg.From() == SFluo->dev() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    GainInput->setText( msg.Val() );
  }
}

void MainWindow::StartMCA( void )
{
  if ( !inMCAMeas ) {
    if ( ! SFluo->isEnable() ) {
      QString msg = QString( tr( "Scan cannot Start : SSD is disabled" ) );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
      return;
    }
    QString User;
    if ( ( User = UUnits.user( SFluo ) ) != "" ) {
      // 検出器が他のことに使われたらダメ
      statusbar->showMessage( tr( "The Sensor [%1] is used by the process %2!" )
			      .arg( SFluo->name() ).arg( User ), 2000 );
      return;
    }

    GainInput->setReadOnly( true );
    GainInput->setStyleSheet( NONEDITABLELINE );
    PeakCalibrate->setReadOnly( true );
    PeakCalibrate->setStyleSheet( NONEDITABLELINE );
    MCAStart->setText( tr( "Stop" ) );
    MCAStart->setStyleSheet( InActive );

    inMCAMeas = true;

    //    if ( cMCAView == NULL ) {
    getNewMCAView();
    //    }
    MCADataStat = NEW;
    MCARecFile->setStyleSheet( FSTATCOLORS[ MCADataStat ][ MCANameStat ] );
    MCARecFile->setToolTip( FSTATMsgs[ MCADataStat ][ MCANameStat ] );

    UUnits.addAnUnit( MCA_ID, SFluo );
    MCAClearRequest = false;
    SFluo->RunStop();
    cMCAViewC->setDeletable( false );
    MCAStage = 0;
    MCATimer->start( 100 );
  } else {
    inMCAMeas = false;
    UUnits.removeUnits( MCA_ID );
    GainInput->setReadOnly( false );
    GainInput->setStyleSheet( EDITABLELINE );
    PeakCalibrate->setReadOnly( false );
    PeakCalibrate->setStyleSheet( EDITABLELINE );
    SFluo->RunStop();
    SFluo->setSSDPresetType( "REAL" );
    MCATimer->stop();
    MCAStart->setText( tr( "Start" ) );
    MCAStart->setStyleSheet( NormalEXECB );
    cMCAViewC->setDeletable( true );
  }
}

void MainWindow::showCurrentValues( int atCur, int inROI )
{
  if ( sender() == cMCAView ) {
    ValAtCurDisp->setText( QString::number( atCur ) );
    ValInROIDisp->setText( QString::number( inROI ) );
  }
}

void MainWindow::setNewROI( int s, int e )
{
  if ( sender() == cMCAView ) {
    if ( !inMeas ) {
      ROIStartInput->setText( ROIStart[ MCACh->text().toInt() ] = QString::number( s ) );
      ROIEndInput->setText( ROIEnd[ MCACh->text().toInt() ] = QString::number( e ) );
      if ( AutoROIsetAll->isChecked() )
	setAllROIs();
      ReCalcXAFSWithMCA();
      //      S2DReCalcMap0();
      ReCalcS2DMap();
    }
  }
}

void MainWindow::MCASequence( void )
{
  if ( MCAStage < 2 )         // MCA に RunStart をかけてしまうと、ずっと isBusy
    if ( SFluo->isBusy() || SFluo->isBusy2() )
      return;
  if ( SFluo->isBusy2() ) {
    return;
  }

  if ( MCAClearRequest == true ) {
    MCAClearRequest = false;
    for ( int i = 0; i < SFluo->length(); i++ ) MCAData[i] = 0;
    StartResume = MCA_START;
    MCAStage = 0;
  }

  switch( MCAStage ) {
  case 0:
    SFluo->initLocalStage();
    MCAStage = 1;
  case 1:
    if ( SelRealTime->isChecked() ) {
      SFluo->setSSDPresetType( "REAL" );
      MCAStage = 2;
    } else if ( SelLiveTime->isChecked() ) {
      SFluo->setSSDPresetType( "LIVE" );
      MCAStage = 2;
    } else {
      SFluo->setSSDPresetType( "NONE" );
      MCAStage = 3;
    }
    break;
  case 2:
    if ( SelRealTime->isChecked() || SelLiveTime->isChecked() ) {
      SFluo->SetTime( PresetTime->text().toDouble() );
    }
    MCAStage = 3;
    break;
  case 3:
    if ( SFluo->InitSensor() == false ) {  // true :: initializing
      if ( StartResume == MCA_START ) {
	SFluo->RunStart();
	StartResume = MCA_RESUME;
      } else {
	SFluo->RunResume();
      }
      SFluo->initLocalStage();
      MCAStage = 4;
    }
    break;
  case 4:
    SFluo->GetMCAs();
    MCAStage = 4;
    break;
  }
}

void MainWindow::ShowNewMCAStat( char * )
{
  if ( cMCAView != NULL ) {
    unsigned *aMca = SFluo->getAMCA( cMCACh );
    for ( int i = 0; i < SFluo->length(); i++ ) {
      MCAData[i] = aMca[i];
    }
    XMAPHead head = SFluo->getAMCAHead( cMCACh );
    cMCAView->SetRealTime( head.realTime );
    cMCAView->SetLiveTime( head.liveTime );
    cMCAView->update();
  }
}

void MainWindow::ShowNewMCARealTime( int ch )
{
  if ( cMCAView != NULL ) {
    if ( ch == cMCACh ) {
      cMCAView->SetRealTime( SFluo->realTime( cMCACh ) );
    }
  }
}

void MainWindow::ShowNewMCALiveTime( int ch )
{
  if ( cMCAView != NULL ) {
    if ( ch == cMCACh )
      cMCAView->SetLiveTime( SFluo->liveTime( cMCACh ) );
  }
}

void MainWindow::clearMCA( void )
{
  if ( inMCAMeas ) {
    MCAClearRequest = true;
  } else {
    StartResume = MCA_START;
    if ( cMCAView != NULL ) {
      for ( int i = 0; i < SFluo->length(); i++ ) {
	MCAData[i] = 0;
      }
      MCAStage = 0;
      cMCAView->update();
    }
  }
}

void MainWindow::gotNewPeakList( QVector<MCAPeak> *peaks )
{
  MCAPeaks = peaks;
  MCAPeakList->clear();
  for ( int i = 0; i < peaks->count(); i++ ) {
    QString aPeak = tr( "%1: %2 [keV] (%3 pix), Height %4 Width %5[keV](%6 pix), Area %7" )
      .arg( i )
      .arg( prec( (*peaks)[i].BinE, 3 ) )
      .arg( (*peaks)[i].BinP  )
      .arg( (int)((*peaks)[i].A) )
      .arg( prec( (*peaks)[i].WinE, 3 ) )
      .arg( (int)((*peaks)[i].WinP) )
      .arg( prec( (*peaks)[i].Area(), 1 ) );
    
    if ( i >= MCAPeakList->count() ) {
      MCAPeakList->addItem( aPeak );
    } else {
      MCAPeakList->setItemText( i, aPeak );
    }
  }
}

void MainWindow::setPreAMPGains( void )
{
  if ( SFluo == NULL )
    return;
  if ( MCAPreAMPGainHasSet )
    return;

  MCAPreAMPGainHasSet = true;

  QFile f( "SSDPreAMPGains.txt" );
  if ( !f.exists() ) {
    f.setFileName( ":/SSDPreAMPGains0.txt" );
    if ( !f.exists() )
      return;
  }
  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    qDebug() << tr( "Error cannot open [%1]." ).arg( "SSDPreAMPGains" );
    return;
  }

  QTextStream in( &f );
  while( !in.atEnd() ) {
    QStringList items = in.readLine().split( QRegExp( "\\s+" ) );
    if ( items.count() > 1 ) {
      if ( items[0][0] != '#' ) {
	SFluo->setGain( items[0].toInt(), items[1].toDouble() );
      }
    }
  }

  f.close();
}

void MainWindow::nowFitStat( QString &stat )
{
  ShowFittingStat->setText( stat );
#if 1
  QFile f( "MCAPeakFitStat.dat" );
  f.open( QIODevice::Text | QIODevice::Append );
  QTextStream out( &f );
  out << stat << "\n";
  f.close();
#endif
}

void MainWindow::ReCalcS2DMap( void )
{
  //  if ( ( ! S2DI.valid ) || inMeas || inMCAMeas || inS2D ) {
  if ( inMeas || inMCAMeas || inS2D ) {
    return;
  }
  S2DBase->setS2DI( S2DI );
  emit ReCalcS2DMap0( ROIStart, ROIEnd, SSDbs2 );
}
