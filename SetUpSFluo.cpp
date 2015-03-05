
#include "SetUpSFluo.h"

#include "global.h"

SetUpSFluo::SetUpSFluo( QWidget *p ) : QWidget( p )
{
  setupUi( this );

  s = NULL;
  SFluo0 = new AUnitSFluo( this );
  mcaView = new MCAView( NULL, this );
  k2p = new KeV2Pix;
  MCATimer = new QTimer;
  fdbase = NULL;

  mcaView->setKeV2Pix( k2p );
  mcaView->setShowElements( DispElmNames->isChecked() );
  mcaView->setShowElementsAlways( ShowAlwaysSelElm->isChecked() );
  mcaView->setShowElementsEnergy( ShowElmEnergy->isChecked() );
  mcaView->setShowDiff( ShowDiff->isChecked() );
  mcaView->setShowSmoothed( ShowSmoothed->isChecked() );
  mcaView->setFitToRaw( FitToRaw->isChecked() );
  mcaView->setLog( SetDisplayLog->isChecked() );
  mcaView->setMaxEnergy( MaxMCAEnergy );
  mcaView->setMaxLoop( MaxLoop->text().toInt() );
  mcaView->setDampFact( DampFact->text().toDouble() );
  mcaView->setPrec1( Prec1->text().toDouble() );
  mcaView->setPrec2( Prec2->text().toDouble() );
  //  mcaView->setNewPSSens( PeakSearchSensitivity->text() );
  //  mcaView->setPeakSearch( MCAPeakSearch->isChecked() );
  //  mcaView->setLimitPSEnergy( LimitPSEnergy->isChecked() );

  MCAClearRequest = false;
  MCADataStat = MCANameStat = OLD;
  MaxMCAEnergy = 20;
  MCAStage = 0;
  MCAPreAMPGainHasSet = false;

  connect( MCATimer, SIGNAL( timeout() ), this, SLOT( MCASequence() ),
	   Qt::UniqueConnection );
  connect( mcaView, SIGNAL( newROIinEng( double, double ) ),
	   this, SLOT( setROIs( void ) ), Qt::UniqueConnection );
  connect( p, SIGNAL( NewEnergy( double ) ), mcaView, SLOT( NewEnergy( double ) ),
	   Qt::UniqueConnection );
  connect( this, SIGNAL( showMyMCAView( SetUpSFluo * ) ),
	   p, SLOT( showOnesMCAView( SetUpSFluo * ) ), Qt::UniqueConnection );
}

void SetUpSFluo::setROIs( void )
{
  if ( AutoROIsetAll->isChecked() )
    setAllROIs();
}

void SetUpSFluo::setFDBase( FluoDBase *fb )
{
  fdbase = fb;
  mcaView->setFDBase( fdbase );
}

void SetUpSFluo::setupSetupSFluo( Stars *S, QVector<QStringList> *fStatMsgs )
{
  s = S;
  FSTATMsgs = fStatMsgs;
  MwSSDGotMCALen = false;
  MCAStart->setStyleSheet( NormalEXECB );
  ROIStart = ROIEnd = NULL;

  MaxMCAEnergyInput->setText( QString::number( MaxMCAEnergy ) );
  connect( MaxMCAEnergyInput, SIGNAL( editingFinished() ),
	   this, SLOT( newMaxMCAEnergy() ), Qt::UniqueConnection );

  SELBs1->setTitle( tr( "Select a SSD channel (MCA spectrum)" ) );
  SELBs1->setExcl( true );
  SELBs1->setType( SSD19CH );   // デフォルトは 19ch SSD 型
  if ( SFluo0->chs() == 7 )
    SELBs1->setType( SDD7CH );

  MCAData = mcaView->setMCAdataPointer( SFluo0->length() );
  for ( int i = 0; i < SFluo0->length(); i++ ) MCAData[i] = 0;
  
  qDebug() << "mcadata " << MCAData;
  
  connect( SELBs1, SIGNAL( selectedSingleCh( int ) ), MCACh, SLOT( setValue( int ) ),
	   Qt::UniqueConnection );
  connect( SELBs1, SIGNAL( selectedSingleCh( int ) ), this, SLOT( MCAChSelected( int ) ),
	   Qt::UniqueConnection );


  SELBs2->setTitle( tr( "Select SSD channels (XAFS meas.)" ) );
  SELBs2->setType( SSD19CH );   // デフォルトは 19ch SSD 型
  if ( SFluo0->chs() == 7 )
    SELBs2->setType( SDD7CH );
  
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
  
  inMCAMeas = false;
  //  validMCAData = false;
  cMCACh = 0;
  //  oldMCACh = -1;
  StartResume = MCA_START;
  cMCAViewTabNo = -1;

  //  SelSSDs( 0 );

  NonSelC = "#ccccaa";
  SelectC = "#ffffdd";
  PT2 = new PeriodicTable( Vic );
  PT2->SetActionOnSelected( PT_STAY );
  PT2->SetActionOnClosed( PT_CLOSE );
  PT2->SetCheckable( true );
  PT2->ShowAllNoneBs( true );
  for ( int i = 0; i < PT2->Atoms(); i++ ) {
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

  connect( ShowDiff, SIGNAL( toggled( bool ) ), this, SLOT( SelectedShowDiff( bool ) ),
	   Qt::UniqueConnection );
#if 0
  connect( PeakSearchSensitivity, SIGNAL( editingFinished() ), 
	   this, SLOT( newPSSens() ),
	   Qt::UniqueConnection );
  connect( LimitPSEnergy, SIGNAL( toggled( bool ) ),
	   this, SLOT( SelectedLimitPSEnergy( bool ) ),
	   Qt::UniqueConnection );
  connect( MCAPeakSearch, SIGNAL( toggled( bool ) ),
	   this, SLOT( SelectedPeakSearch( bool ) ),
	   Qt::UniqueConnection );
#endif
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
  
  connect( SFluo0, SIGNAL( NewMCAsAvailable( char * ) ),
	   this, SLOT( ShowNewMCAStat( char * ) ),
	   Qt::UniqueConnection);
  connect( SFluo0, SIGNAL( ReceivedNewMCARealTime( int ) ),
	   this, SLOT( ShowNewMCARealTime( int ) ),
	   Qt::UniqueConnection );
  connect( SFluo0, SIGNAL( ReceivedNewMCALiveTime( int ) ),
	   this, SLOT( ShowNewMCALiveTime( int ) ),
	   Qt::UniqueConnection );
  connect( ROIsetAll, SIGNAL( clicked() ), this, SLOT( setAllROIs() ),
	   Qt::UniqueConnection );
}

void SetUpSFluo::newMaxMCAEnergy( void )
{
  MaxMCAEnergy = MaxMCAEnergyInput->text().toDouble();
  if ( MaxMCAEnergy < 20 ) {
    MaxMCAEnergy = 20;
    MaxMCAEnergyInput->setText( QString::number( MaxMCAEnergy ) );
  }

  mcaView->setMaxEnergy( MaxMCAEnergy );
  mcaView->update();
}

void SetUpSFluo::newMaxLoop( void )
{
  int maxLoop = MaxLoop->text().toInt();
  if ( maxLoop < 1 ) {
    maxLoop = 1;
  }
  MaxLoop->setText( QString::number( maxLoop ) );

  mcaView->setMaxLoop( maxLoop );
  mcaView->update();
}

void SetUpSFluo::newDampFact( void )
{
  mcaView->setDampFact( DampFact->text().toDouble() );
  mcaView->update();
}

void SetUpSFluo::newPrec1( void )
{
  mcaView->setPrec1( Prec1->text().toDouble() );
  mcaView->update();
}

void SetUpSFluo::newPrec2( void )
{
  mcaView->setPrec2( Prec2->text().toDouble() );
  mcaView->update();
}


void SetUpSFluo::ReadLowerLimitSetting( void )
{
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
	SFluo0->SetLowLimit( cols[0].toInt(),
			    k2p->E2p( cols[0].toInt(), cols[1].toDouble()/1000. ) );
      }
    }
  }

  file.close();
}

void SetUpSFluo::SelectedLimitPSEnergy( bool f )
{
  mcaView->setLimitPSEnergy( f );
}

void SetUpSFluo::SelectedShowDiff( bool f )
{
  mcaView->setShowDiff( f );
}

void SetUpSFluo::SelectedPeakSearch( bool f )
{
  mcaView->setPeakSearch( f );
}

void SetUpSFluo::SelectedShowSmoothed( bool f )
{
  mcaView->setShowSmoothed( f );
}

void SetUpSFluo::SelectedFitToRaw( bool f )
{
  mcaView->setFitToRaw( f );
}

void SetUpSFluo::PushedReFit( void )
{
  mcaView->doPeakFitWCPoints();
}

void SetUpSFluo::PushedClearMCAPeaks( void )
{
  mcaView->clearMCAPeaks();
}

void SetUpSFluo::newGain( void )
{
  if ( inMCAMeas )
    return;

  SFluo0->setGain( MCACh->value(), GainInput->text().toDouble() );
}

void SetUpSFluo::setAllROIs( void )
{
  int ch = MCACh->text().toInt();
  double startE = k2p->p2E( ch, ROIStart[ ch ].toDouble() );
  double endE = k2p->p2E( ch, ROIEnd[ ch ].toDouble() );

  for ( int i = 0; i < SFluo0->chs(); i++ ) {
    ROIStart[ i ] = QString::number( k2p->E2p( i, startE ) );
    ROIEnd[ i ] = QString::number( k2p->E2p( i, endE ) );
  }
  emit ReCalcXAFSWithMCA();
  emit ReCalcS2DMap( this );
  //  //  S2DReCalcMap0();    // !!!!!!!!!!!!!!!!!!!!!!!!
}

void SetUpSFluo::saveMCAData( void )
{
#if 0
  if ( !validMCAData ) {
    emit ShowMessage( tr( "MCA data is not valid" ), 2000 );
    return;
  }
#endif
  if ( MCARecFile->text().isEmpty() ) {
    emit ShowMessage( tr( "Save file name is not selected" ), 2000 );
    return;
  }
  MCADataStat = OLD;
  MCANameStat = OLD;
  MCARecFile->setStyleSheet( FSTATCOLORS[ MCADataStat ][ MCANameStat ] );
  MCARecFile->setToolTip( FSTATMsgs->at( MCADataStat )[ MCANameStat ] );

//  for ( int i = 0; i < 1000; i++ ) {   // 1000面セーブ時間測定用
//    qDebug() << i;                     // i7 で 40 秒(0.04s/面)だった
//    // ROI の積分を XafsM2 側でやるようにし、フルレンジ(0-2047)を ROI の範囲にした場合
//    // 約 43 秒。ROI の積分時間は 最大 3ms 程度という事になる。
  aMCASet *set = new aMCASet;
  set->setSize( SFluo0->length(), SFluo0->chs() );
  SaveMCADataOnMem( set, this );
  //  saveMCAData0( MCARecFile->text(), set );
  set->save( MCARecFile->text(), "measured by SSD set up" );
  delete set;
}

void SetUpSFluo::setSelectedMCAFName( const QString &fname )
{
  MCARecFile->setText( CheckFNameExt( fname, "dat" ) );
  MCANameStat = NEW;
  MCARecFile->setStyleSheet( FSTATCOLORS[ MCADataStat ][ MCANameStat ] );
  MCARecFile->setToolTip( FSTATMsgs->at( MCADataStat )
			  [ MCANameStat ] );
}

void SetUpSFluo::RealTimeIsSelected( void )
{
  if ( SelRealTime->isChecked() ) {
    SelLiveTime->setChecked( false );
  }
}

void SetUpSFluo::LiveTimeIsSelected( void )
{
  if ( SelLiveTime->isChecked() ) {
    SelRealTime->setChecked( false );
  }
}

#if 0
void SetUpSFluo::SelSSDs0( void )
{
  for ( int i = 0; i < SSDbs.count(); i++ ) {
    if ( sender() == SSDbs.at(i) ) {
      SelSSDs( i );
      break;
    }
  }
}
#endif

#if 0
void SetUpSFluo::SelSSDs20( void )
{
  for ( int i = 0; i < SSDbs2.count(); i++ ) {
    if ( sender() == SSDbs2.at(i) ) {
      if ( SSDbs2.at(i)->isChecked() == PBTrue ) {
	SSDbs2.at(i)->setStyleSheet( SSDActive );
	SSDbs2.at(i)->setToolTip( tr( "Active" ) );
	if ( SFluo != NULL ) SFluo->setSSDUsingCh( i, true );
	emit SelectedSSD( i, true );
      } else {
	SSDbs2.at(i)->setStyleSheet( SSDnotActive );
	SSDbs2.at(i)->setToolTip( tr( "Inactive" ) );
	if ( SFluo != NULL ) SFluo->setSSDUsingCh( i, false );
	emit SelectedSSD( i, false );
      }
    }
  }
  emit ReCalcXAFSWithMCA();
  emit ReCalcS2DMap( this );
  //  S2DReCalcMap0();     !!!!!!!!!!!!!!!!!!!!!!!
}
#endif

#if 0
void SetUpSFluo::SelSSDs( int ch )
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
#endif

void SetUpSFluo::getMCASettings( int ch )
{
  if ( s == NULL )
    return;
  
  s->SendCMD2( "SetUpMCA", SFluo0->dev(), "GetPeakingTime", QString::number( ch ) );
  s->SendCMD2( "SetUpMCA", SFluo0->dev(), "GetThreshold", QString::number( ch ) );
  s->SendCMD2( "SetUpMCA", SFluo0->dev(), "GetCalibration", QString::number( ch ) );
  s->SendCMD2( "SetUpMCA", SFluo0->dev(), "GetDynamicRange", QString::number( ch ) );
  s->SendCMD2( "SetUpMCA", SFluo0->dev(), "GetPreAMPGain", QString::number( ch ) );
  
  SFluo0->GetMCAs();
}

void SetUpSFluo::getMCALen( SMsg )  // 初期化の時に一回しか呼ばれないと信じる
{
  if ( MwSSDGotMCALen )
    return;
  MwSSDGotMCALen = true;

  ROIStart = new QString [ SFluo0->chs() ];
  ROIEnd = new QString [ SFluo0->chs() ];
  SFluo0->setROIs( ROIStart, ROIEnd );
  for ( int i = 0; i < SFluo0->chs(); i++ ) {
    ROIStart[i] = "0";
    ROIEnd[i] = QString::number( SFluo0->length() - 1 );
  }
  int ch = MCACh->text().toInt();
  ROIStartInput->setText( ROIStart[ ch ] );
  ROIEndInput->setText( ROIEnd[ ch ] );
  //  kev2pix->setMCALen( MCALength );
  ReadLowerLimitSetting();
}

void SetUpSFluo::newROIStart( const QString &newv )
{
  ROIStart[ MCACh->text().toInt() ] = newv;

  mcaView->setROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
  mcaView->update();

  if ( AutoROIsetAll->isChecked() )
    setAllROIs();
  emit ReCalcXAFSWithMCA();
  emit ReCalcS2DMap( this );
  // S2DReCalcMap0();    !!!!!
}


void SetUpSFluo::newROIEnd( const QString &newv )
{
  ROIEnd[ MCACh->text().toInt() ] = newv;

  mcaView->setROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
  mcaView->update();

  if ( AutoROIsetAll->isChecked() )
    setAllROIs();
  emit ReCalcXAFSWithMCA();
  emit ReCalcS2DMap( this );
  //    S2DReCalcMap0();
}

void SetUpSFluo::MCAChSelected( int i )
{
  if ( i == cMCACh )
    return;

  int MaxChs = SFluo0->chs();
  if ( SFluo0->chs() > SELBs1->chs() )
    MaxChs = SELBs1->chs();
  if ( i < 0 ) {
    MCACh->setValue( MaxChs - 1 );
    i = MaxChs - 1;
  }
  if ( i >= MaxChs ) { MCACh->setValue( 0 ); i = 0; }
  cMCACh = i;

  emit NewMCACh( cMCACh );
  getMCASettings( cMCACh );
  ROIStartInput->setText( ROIStart[ cMCACh ] );
  ROIEndInput->setText( ROIEnd[ cMCACh ] );

  mcaView->SetMCACh( cMCACh );
  mcaView->setROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
  SFluo0->GetRealTime( cMCACh );
  SFluo0->GetLiveTime( cMCACh );

  SELBs1->selACh( cMCACh );
}

void SetUpSFluo::showPeakingTime( SMsg msg )
{
  if ( ( msg.From() == SFluo0->dev() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    PeakingTimeInput->setText( msg.Val() );
  }
}

void SetUpSFluo::showThreshold( SMsg msg )
{
  if ( ( msg.From() == SFluo0->dev() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    ThresholdInput->setText( msg.Val() );
  }
}

void SetUpSFluo::showCalibration( SMsg msg )
{
  if ( ( msg.From() == SFluo0->dev() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    CalibrationInput->setText( msg.Val() );
  }
}

void SetUpSFluo::showDynamicRange( SMsg msg )
{
  if ( ( msg.From() == SFluo0->dev() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    DynamicRangeInput->setText( msg.Val() );
  }
}

void SetUpSFluo::showPreAMPGain( SMsg msg )
{
  if ( ( msg.From() == SFluo0->dev() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    GainInput->setText( msg.Val() );
  }
}

void SetUpSFluo::StartMCA( void )
{
  if ( !inMCAMeas ) {
    if ( ! SFluo0->isEnable() ) {
      QString msg = QString( tr( "SSD is disabled" ) );
      emit ShowMessage( msg, 2000 );
      return;
    }
    if ( SFluo0->isUsing() ) {
      // 検出器が他のことに使われたらダメ
      emit ShowMessage( tr( "The Sensor [%1] is used by the process %2!" )
			.arg( SFluo0->name() ).arg( SFluo0->userName() ), 2000 );
      return;
    }

    GainInput->setReadOnly( true );
    GainInput->setStyleSheet( NONEDITABLELINE );
    PeakCalibrate->setReadOnly( true );
    PeakCalibrate->setStyleSheet( NONEDITABLELINE );
    MCAStart->setText( tr( "Stop" ) );
    MCAStart->setStyleSheet( InActive );

    inMCAMeas = true;

    emit showMyMCAView( this );
    
    MCADataStat = NEW;
    MCARecFile->setStyleSheet( FSTATCOLORS[ MCADataStat ][ MCANameStat ] );
    MCARecFile->setToolTip( FSTATMsgs->at( MCADataStat )[ MCANameStat ] );

    emit addAnUsingUnit( MCA_ID, SFluo0 );
    MCAClearRequest = false;
    SFluo0->RunStop();
    //    McaViewC->setDeletable( false );
    MCAStage = 0;
    MCATimer->start( 100 );
  } else {
    inMCAMeas = false;
    emit removeUsingUnits( MCA_ID, SFluo0 );
    GainInput->setReadOnly( false );
    GainInput->setStyleSheet( EDITABLELINE );
    PeakCalibrate->setReadOnly( false );
    PeakCalibrate->setStyleSheet( EDITABLELINE );
    SFluo0->RunStop();
    SFluo0->setSSDPresetType( "REAL" );
    MCATimer->stop();
    MCAStart->setText( tr( "Start" ) );
    MCAStart->setStyleSheet( NormalEXECB );
    //    cMCAViewC->setDeletable( true );
  }
}

void SetUpSFluo::showCurrentValues( int atCur, int inROI )
{
  if ( sender() == mcaView ) {   // これが != になることは絶対ないはずだけど念の為
    ValAtCurDisp->setText( QString::number( atCur ) );
    ValInROIDisp->setText( QString::number( inROI ) );
  }
}

void SetUpSFluo::setNewROI( int s, int e )
{
  if ( sender() == mcaView ) {   // これが != になることは絶対ないはずだけど念の為
    ROIStartInput->setText( ROIStart[ MCACh->text().toInt() ] = QString::number( s ) );
    ROIEndInput->setText( ROIEnd[ MCACh->text().toInt() ] = QString::number( e ) );
    if ( AutoROIsetAll->isChecked() )
      setAllROIs();
    emit ReCalcXAFSWithMCA();
    emit ReCalcS2DMap( this );
    //      S2DReCalcMap0();
  }
}

void SetUpSFluo::MCASequence( void )
{
  if ( MCAStage < 2 )         // MCA に RunStart をかけてしまうと、ずっと isBusy
    if ( SFluo0->isBusy() || SFluo0->isBusy2() )
      return;
  if ( SFluo0->isBusy2() ) {
    return;
  }

  if ( MCAClearRequest ) {
    MCAClearRequest = false;
    for ( int i = 0; i < SFluo0->length(); i++ ) MCAData[i] = 0;
    StartResume = MCA_START;
    MCAStage = 0;
  }

  qDebug() << "MCAStage" << MCAStage;
  
  switch( MCAStage ) {
  case 0:
    SFluo0->initLocalStage();
    MCAStage = 1;
  case 1:
    if ( SelRealTime->isChecked() ) {
      SFluo0->setSSDPresetType( "REAL" );
      MCAStage = 2;
    } else if ( SelLiveTime->isChecked() ) {
      SFluo0->setSSDPresetType( "LIVE" );
      MCAStage = 2;
    } else {
      SFluo0->setSSDPresetType( "NONE" );
      MCAStage = 3;
    }
    break;
  case 2:
    if ( SelRealTime->isChecked() || SelLiveTime->isChecked() ) {
      SFluo0->SetTime( PresetTime->text().toDouble() );
    }
    MCAStage = 3;
    break;
  case 3:
    if ( SFluo0->InitSensor() == false ) {  // true :: initializing
      if ( StartResume == MCA_START ) {
	SFluo0->RunStart();
	StartResume = MCA_RESUME;
      } else {
	SFluo0->RunResume();
      }
      SFluo0->initLocalStage();
      MCAStage = 4;
    }
    break;
  case 4:
    SFluo0->GetMCAs();
    MCAStage = 4;
    break;
  }
}

void SetUpSFluo::ShowNewMCAStat( char * )
{
  unsigned *aMca = SFluo0->getAMCA( cMCACh );
  for ( int i = 0; i < SFluo0->length(); i++ ) {
    MCAData[i] = aMca[i];
  }
  XMAPHead head = SFluo0->getAMCAHead( cMCACh );
  mcaView->SetRealTime( head.realTime );
  mcaView->SetLiveTime( head.liveTime );
  mcaView->update();
}

void SetUpSFluo::ShowNewMCARealTime( int ch )
{
  if ( ch == cMCACh )
    mcaView->SetRealTime( SFluo0->realTime( cMCACh ) );
}

void SetUpSFluo::ShowNewMCALiveTime( int ch )
{
  if ( ch == cMCACh )
    mcaView->SetLiveTime( SFluo0->liveTime( cMCACh ) );
}

void SetUpSFluo::clearMCA( void )
{
  if ( inMCAMeas ) {
    MCAClearRequest = true;
  } else {
    StartResume = MCA_START;
    for ( int i = 0; i < SFluo0->length(); i++ ) {
      MCAData[i] = 0;
    }
    MCAStage = 0;
    mcaView->update();
  }
}

void SetUpSFluo::gotNewPeakList( QVector<MCAPeak> *peaks )
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

void SetUpSFluo::setPreAMPGains( void )
{
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
	SFluo0->setGain( items[0].toInt(), items[1].toDouble() );
      }
    }
  }

  f.close();
}

void SetUpSFluo::nowFitStat( QString &stat )
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

void SetUpSFluo::newCalibration( void )
{
  if ( inMCAMeas )
    return;

  double oldE = (*MCAPeaks)[ MCAPeakList->currentIndex() ].BinE;
  if ( oldE <= 0 ) return;
  // 新旧の エネルギー比
  double ratio = PeakCalibrate->text().toDouble() / oldE;
  if ( ratio <= 0 ) return;
  // 一回の入力で、このルーチンに複数回入ってくるとおかしくなるので
  // ある数値で一旦設定したら、入力欄自体をクリアしてしまう
  PeakCalibrate->setText( "" );
  // gain の設定は何故か逆
  SFluo0->setGain( MCACh->text().toInt(), GainInput->text().toDouble() / ratio );
  // 設定したゲインの読み出し
  s->SendCMD2( "SetUpMCA", SFluo0->dev(),
	       "GetPreAMPGain", QString::number( MCACh->text().toInt() ) );
}

