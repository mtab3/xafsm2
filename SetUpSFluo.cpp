
#include "SetUpSFluo.h"

#include "global.h"

SetUpSFluo::SetUpSFluo( QWidget *p ) : QWidget( p )
{
  setupUi( this );

  k2pFile = "KeV2MCApix.txt";
  llFile = "SSDLowLimits.txt";
  pagFile = "SSDPreAMPGains.txt";
  s = NULL;
  SFluo0 = new AUnitSFluo( this );
  mcaView = new MCAView( NULL, this, p );
  k2p = new KeV2Pix;
  MCATimer = new QTimer;
  fdbase = NULL;
  hvControllable = false;
  hvOn = false;
  
  // setUpMCAView( mcaView );

  MCAClearRequest = false;
  MCADataStat = MCANameStat = OLD;
  MaxMCAEnergy = 20;
  MCAStage = 0;
  MCAPreAMPGainHasSet = false;

  connect( MCATimer, SIGNAL( timeout() ), this, SLOT( MCASequence() ),
	   Qt::UniqueConnection );
#if 0
  connect( mcaView, SIGNAL( newROIinEng( double, double ) ),
	   this, SLOT( setROIs( void ) ), Qt::UniqueConnection );
  connect( p, SIGNAL( NewEnergy( double ) ), mcaView, SLOT( NewEnergy( double ) ),
	   Qt::UniqueConnection );
#endif
  connect( this, SIGNAL( showMyMCAView( SetUpSFluo * ) ),
	   p, SLOT( showOnesMCAView( SetUpSFluo * ) ), Qt::UniqueConnection );
}

void SetUpSFluo::setUpMCAView( MCAView *view )
{
  view->setKeV2Pix( k2p );
  view->setShowElements( DispElmNames->isChecked() );
  view->setShowElementsAlways( ShowAlwaysSelElm->isChecked() );
  view->setShowElementsEnergy( ShowElmEnergy->isChecked() );
  view->setShowDiff( ShowDiff->isChecked() );
  view->setShowSmoothed( ShowSmoothed->isChecked() );
  view->setFitToRaw( FitToRaw->isChecked() );
  view->setLog( SetDisplayLog->isChecked() );
  view->setMaxMCAEnergy( MaxMCAEnergy );
  view->setMaxLoop( MaxLoop->text().toInt() );
  view->setDampFact( DampFact->text().toDouble() );
  view->setPrec1( Prec1->text().toDouble() );
  view->setPrec2( Prec2->text().toDouble() );
  // 最初に SetUpSFluo のコンストラクタから呼ばれる時は fdbase は NULL だが、
  // 以降、外部の MCAView を SetUpSFluo に結びつけるために呼ばれる時は、! NULL のはず
  view->setFDBase( fdbase );
  //  view->setNewPSSens( PeakSearchSensitivity->text() );
  //  view->setPeakSearch( MCAPeakSearch->isChecked() );
  //  view->setLimitPSEnergy( LimitPSEnergy->isChecked() );
}

void SetUpSFluo::setROIs( void )
{
  if ( AutoROIsetAll->isChecked() )
    setAllROIs();
}

void SetUpSFluo::setFDBase( FluoDBase *fb )
{
  fdbase = fb;
  emit askSetFDBase( fdbase ); 
  //  mcaView->setFDBase( fdbase );
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
#if 0  
  if ( SFluo0->chs() == 7 )
    SELBs1->setType( SDD7CH );
#endif
  qDebug() << "SetupSetupSFluo : DType = " << DType;
  if ( DType == "DType19Ch" )
    SELBs1->setType( SSD19CH );
  if ( DType == "DType7Ch" )
    SELBs1->setType( SDD7CH );
  
  MCAData = mcaView->setMCAdataPointer( SFluo0->length() );
  for ( int i = 0; i < SFluo0->length(); i++ ) MCAData[i] = 0;
  
  connect( SELBs1, SIGNAL( selectedSingleCh( int ) ), MCACh, SLOT( setValue( int ) ),
	   Qt::UniqueConnection );
  connect( SELBs1, SIGNAL( selectedSingleCh( int ) ), this, SLOT( MCAChSelected( int ) ),
	   Qt::UniqueConnection );

  SELBs2->setTitle( tr( "Select SSD channels (XAFS meas.)" ) );
  SELBs2->setType( SSD19CH );   // デフォルトは 19ch SSD 型
#if 0
  if ( SFluo0->chs() == 7 )
    SELBs2->setType( SDD7CH );
#endif
  if ( DType == "DType19Ch" )
    SELBs2->setType( SSD19CH );
  if ( DType == "DType7Ch" )
    SELBs2->setType( SDD7CH );
  SELBs2->setExcl( false );
  
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
#if 0
  connect( s, SIGNAL( AnsGetMCALength( SMsg ) ), this, SLOT( getMCALen( SMsg ) ),
	   Qt::UniqueConnection );
#endif
  setROILen();

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

  connect( ShowDiff, SIGNAL( toggled( bool ) ), this, SIGNAL( askSetShowDiff( bool ) ),
	   Qt::UniqueConnection );
#if 0
  connect( PeakSearchSensitivity, SIGNAL( editingFinished() ), 
	   this, SLOT( newPSSens() ),
	   Qt::UniqueConnection );
  connect( LimitPSEnergy, SIGNAL( toggled( bool ) ),
	   this, SLOT( SelectedLimitPSEnergy( bool ) ),
	   Qt::UniqueConnection );
  connect( MCAPeakSearch, SIGNAL( toggled( bool ) ),
	   this, SIGNAL( askSetPeakSearch( bool ) ),
	   Qt::UniqueConnection );
#endif
  connect( ShowSmoothed, SIGNAL( toggled( bool ) ),
	   this, SIGNAL( askSetShowSmoothed( bool ) ),
	   Qt::UniqueConnection );
  connect( MCAReFit, SIGNAL( clicked() ), this, SIGNAL( askDoPeakFitWCPoints() ), 
	   Qt::UniqueConnection );
  connect( ClearMCAPeaks, SIGNAL( clicked() ), this, SIGNAL( askClearMCAPeaks() ),
	   Qt::UniqueConnection );
  connect( FitToRaw, SIGNAL( toggled( bool ) ),
	   this, SIGNAL( askSetFitToRaw( bool ) ),
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

  if ( hvControllable ) {
    HVOn->setHidden( false );
    connect( HVOn, SIGNAL( clicked() ), this, SLOT( clickedHVOn() ), Qt::UniqueConnection );
    connect( SFluo0, SIGNAL( NewHVStat( int ) ), this, SLOT( showHVStat( int ) ), Qt::UniqueConnection );
    showHVStat( -1 );
  } else {
    HVOn->setHidden( true );
  }
}

static QString HV_OFF_STATE       = "background-color: rgb( 200, 255, 200 )";
static QString HV_TRANSIENT_STATE = "background-color: rgb( 250, 210, 180 )";
static QString HV_ON_STATE        = "background-color: rgb( 255, 255, 180 )";
static QString HV_UNKNOWN_STATE   = "background-color: rgb( 190, 190, 190 )";

void SetUpSFluo::showHVStat( int stat )
{
  switch( stat ) {
  case 0:
    HVOn->setStyleSheet( HV_OFF_STATE );
    HVOn->setText( tr( "HV Off" ) );
    hvOn = false;
    break;
  case 1:
    HVOn->setStyleSheet( HV_TRANSIENT_STATE );
    HVOn->setText( tr( "Moving" ) );
    break;
  case 2: HVOn->setStyleSheet( HV_ON_STATE );
    HVOn->setText( tr( "HV On" ) );
    hvOn = true;
    break;
  default:
    HVOn->setStyleSheet( HV_UNKNOWN_STATE );
    break;
  }
}

void SetUpSFluo::clickedHVOn( void )
{
  SFluo0->HVOn( !hvOn );
  showHVStat( 1 );       // とにかく「遷移中」の表示にしておく
}

void SetUpSFluo::newMaxMCAEnergy( void )
{
  MaxMCAEnergy = MaxMCAEnergyInput->text().toDouble();
  if ( MaxMCAEnergy < 20 ) {
    MaxMCAEnergy = 20;
    MaxMCAEnergyInput->setText( QString::number( MaxMCAEnergy ) );
  }

  emit askSetMaxMCAEnergy( MaxMCAEnergy );
//  mcaView->setMaxEnergy( MaxMCAEnergy );
//  mcaView->update();
}

void SetUpSFluo::newMaxLoop( void )
{
  int maxLoop = MaxLoop->text().toInt();
  if ( maxLoop < 1 ) {
    maxLoop = 1;
  }
  MaxLoop->setText( QString::number( maxLoop ) );

  emit askSetMaxLoop( maxLoop );
//  mcaView->setMaxLoop( maxLoop );
}

void SetUpSFluo::newDampFact( void )
{
  emit askSetDampFact( DampFact->text().toDouble() );
//  mcaView->setDampFact( DampFact->text().toDouble() );
}

void SetUpSFluo::newPrec1( void )
{
  emit askSetPrec1( Prec1->text().toDouble() );
//  mcaView->setPrec1( Prec1->text().toDouble() );
}

void SetUpSFluo::newPrec2( void )
{
  emit askSetPrec2( Prec2->text().toDouble() );
//  mcaView->setPrec2( Prec2->text().toDouble() );
}


void SetUpSFluo::ReadLowerLimitSetting( void )
{
  QString fname = llFile;
  QFileInfo fi( fname );
  if ( ! fi.exists() ) {
    fname = QString( ":" ) + fi.baseName() + "0.txt";
  }

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
  emit askSetLimitPSEnergy( f );
//  mcaView->setLimitPSEnergy( f );
}

#if 0
void SetUpSFluo::SelectedShowDiff( bool f )
{
  emit askSetShowDiff( f );
//  mcaView->setShowDiff( f );
}
#endif

#if 0				  
void SetUpSFluo::SelectedPeakSearch( bool f )
{
  emit askSetPeakSearch( f )
//  mcaView->setPeakSearch( f );
}
#endif

#if 0			 
void SetUpSFluo::SelectedShowSmoothed( bool f )
{
  emit askSetShowSmoothed( f );
//  mcaView->setShowSmoothed( f );
}
#endif

#if 0		    
void SetUpSFluo::SelectedFitToRaw( bool f )
{
  emit askSetFitToRaw( f );
//  mcaView->setFitToRaw( f );
}
#endif

#if 0	
void SetUpSFluo::PushedReFit( void )
{
  emit askDoPeakFitWCPoints();
//  mcaView->doPeakFitWCPoints();
}
#endif

#if 0
void SetUpSFluo::PushedClearMCAPeaks( void )
{
  emit askClearMCAPeaks();
//  mcaView->clearMCAPeaks();
}
#endif

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
  emit askToSaveMCAData( this, MCARecFile->text() );
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

void SetUpSFluo::GetMCASettings( int ch )
{
  SFluo0->getMCASettings( ch );
}
/*
 *void SetUpSFluo::getMCASettings( int ch )
 *{
 *  if ( s == NULL )
 *    return;
 *  
 *  s->SendCMD2( "SetUpMCA", SFluo0->dev(), "GetPeakingTime", QString::number( ch ) );
 *  s->SendCMD2( "SetUpMCA", SFluo0->dev(), "GetThreshold", QString::number( ch ) );
 *  s->SendCMD2( "SetUpMCA", SFluo0->dev(), "GetCalibration", QString::number( ch ) );
 *  s->SendCMD2( "SetUpMCA", SFluo0->dev(), "GetDynamicRange", QString::number( ch ) );
 *  s->SendCMD2( "SetUpMCA", SFluo0->dev(), "GetPreAMPGain", QString::number( ch ) );
 *  
 *  SFluo0->GetMCAs();
 *}
 */

void SetUpSFluo::setROILen( void )
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

  emit askSetROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
#if 0
  mcaView->setROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
  mcaView->update();
#endif

  if ( AutoROIsetAll->isChecked() )
    setAllROIs();
  emit ReCalcXAFSWithMCA();
  emit ReCalcS2DMap( this );
  // S2DReCalcMap0();    !!!!!
}


void SetUpSFluo::newROIEnd( const QString &newv )
{
  ROIEnd[ MCACh->text().toInt() ] = newv;

  emit askSetROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
#if 0
  mcaView->setROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
  mcaView->update();
#endif
  
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

  SFluo0->getMCASettings( cMCACh );
  ROIStartInput->setText( ROIStart[ cMCACh ] );
  ROIEndInput->setText( ROIEnd[ cMCACh ] );

  emit askSetMCACh( cMCACh );
  emit askSetROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
  
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
  // これが != になることは絶対ないはずだけど念の為 --> 今はこれが != になることがある
  //  if ( sender() == mcaView ) {
    ValAtCurDisp->setText( QString::number( atCur ) );
    ValInROIDisp->setText( QString::number( inROI ) );
  //  }
}

void SetUpSFluo::setNewROI( int s, int e )
{
//  if ( sender() == mcaView ) {   // これが != になることは絶対ないはずだけど念の為
    ROIStartInput->setText( ROIStart[ MCACh->text().toInt() ] = QString::number( s ) );
    ROIEndInput->setText( ROIEnd[ MCACh->text().toInt() ] = QString::number( e ) );
    if ( AutoROIsetAll->isChecked() )
      setAllROIs();
    emit ReCalcXAFSWithMCA();
    emit ReCalcS2DMap( this );
    //      S2DReCalcMap0();
//  }
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
  mcaView->SetRealTime( head.realTime );  // これは自分の子供の MCAView にしか繋がない
  mcaView->SetLiveTime( head.liveTime );
  mcaView->update();
}

void SetUpSFluo::ShowNewMCARealTime( int ch )
{
  if ( ch == cMCACh )   // これは自分の子供の MCAView にしか繋がない
    mcaView->SetRealTime( SFluo0->realTime( cMCACh ) );
}

void SetUpSFluo::ShowNewMCALiveTime( int ch )
{
  if ( ch == cMCACh )   // これは自分の子供の MCAView にしか繋がない
    mcaView->SetLiveTime( SFluo0->liveTime( cMCACh ) );
}

void SetUpSFluo::clearMCA( void )  // これは自分の子供の MCAView にしか繋がない
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

  QString fname = pagFile;
  QFileInfo fi( fname );
  if ( ! fi.exists() ) {
    fname = QString( ":" ) + fi.baseName() + "0.txt";
  }

  QFile f( fname );
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
  SFluo0->readGain( MCACh->text().toInt() );
  //  s->SendCMD2( "SetUpMCA", SFluo0->dev(),
  //	       "GetPreAMPGain", QString::number( MCACh->text().toInt() ) );
}

