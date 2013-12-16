
#include <stdio.h>

#include "XafsM.h"
#include "MainWindow.h"
#include "SelMC2.h"
#include "Stars.h"

const QString CMode[ MEASMODES + 1 ] = {
  "Transmission",
  "Fluorescence",
  "Aux input",
  "Extra mode",
  NULL,
};

MainWindow::MainWindow( QString myname ) : QMainWindow()
{
  setupUi( this );

  MainTab->setCurrentIndex( 0 );
  RWDXMCenterF = false;
  DXMCenterFile = DXMCENTERFILE0;
  DataRoot0 = "";
  MCACanSaveAllOnMem = false;

  // Monitor の中で SSD の強度を別ファイルに書き出すときの時間を測るため
  T = new QTime;
  T->start();

  FSTATMsgs[0][0] = tr( "Both the name and the data is old." );
  FSTATMsgs[0][1] = tr( "The name is new, but the data is old." );
  FSTATMsgs[1][0] = tr( "The data is new, but the name is old." );
  FSTATMsgs[1][1] = tr( "The data and the name is new, but not saved." );
  MeasDataStat = MeasNameStat = OLD;
  ScanDataStat = ScanNameStat = OLD;
  MonDataStat = MonNameStat = OLD;
  MCADataStat = MCANameStat = OLD;
  S2DDataStat = S2DNameStat = OLD;

  isQXafsModeAvailable = false;

  kev2pix = new KeV2Pix;
  fdbase = new FluoDBase;
  u = new Units;
#if 0
  int dim = kev2pix->getDim();
  for ( int i = 0; i < MaxSSDs; i++ ) {
    const QVector<double>& ab = kev2pix->getAB( i );
    for ( int j = 0; j < dim + 1; j++ ) {
      qDebug() << "ab[i][j] = " << ab[j];
    }
  }
#endif

  MMainTh = MDTh1 = EncMainTh = Enc2 = NULL;
  SLS = SI0 = SI1 = SFluo = NULL;
  //  ChangerX = ChangerZ = NULL;
  oldDeg = -100;
  AllInited = MotorsInited = SensorsInited = false;
  EncOrPM = XENC;
  MCAGains.clear();
  MeasA = 0;
  OldDTh1 = 0;

  StatDisp = new Status();
  StatTab->layout()->addWidget( StatDisp );

  XAFSName = myname;
  XAFSKey = myname;
  XAFSTitle = myname;

  starsSV = new StarsSV2;

  setupLogArea();     // ログに対する書き出しがある可能性があるので最初にイニシャライズ
  ReadDef( DefFileName );
  selmc = new SelMC2( mccd );
  setWindowTitle( XAFSTitle );
  s = new Stars;      // モータ類のイニシャライズの前に Stars の準備はしておく
  s->ReadStarsKeys( XAFSKey, XAFSName ); // Stars とのコネクション確立の準備
  s->SetNewSVAddress( starsSV->SSVAddress() );
  s->SetNewSVPort( starsSV->SSVPort() );

  TTable = new TuningTable;

  Initialize();
  setupView();
  setupCommonArea();
  setupSetupArea();     // AUnit 関係の Initialize 後でないとだめ
  if ( SFluo != NULL ) {
    setupSetupSSDArea();
  } else {
    MainTab->removeTab( MainTab->indexOf( SSDTab ) );
  }
  setupChangerArea();
  setupQXafsMode();
  setupMeasArea();
  setupReadDataArea();
  setupScan2DArea();
  setupWebView();
  setupAutoSequence();

  conds = new Conditions;
  conds->setVersionInfo( VERSION, __DATE__, __TIME__ );
#if 0
  conds->setEncAsTh( true );
  conds->setAddInfos( true );
#endif
  //  useFixedDelta = false;
  connect( conds, SIGNAL( SetDXMPMC() ), this, SLOT( SetDXMPMC() ),
	   Qt::UniqueConnection );

  StatDisp->setupStatArea( &AMotors, &ASensors, starsSV, selmc, conds );
  connect( StatDisp, SIGNAL( NeedListNodes() ), this, SLOT( SendListNodes() ),
	   Qt::UniqueConnection );
  //  QString msg = "XafsMsg_" + QLocale::system().name();
  //  NewLogMsg( msg );
  NewLogMsg( QString( tr( "Mono: %1 (%2 A)" ) )
	     .arg( mccd[ selmc->MC() ]->getMCName() )
	     .arg( mccd[ selmc->MC() ]->getD() ) );

  connect( s, SIGNAL( AskShowStat( QString, int ) ),
	   this, SLOT( ShowMessageOnSBar( QString, int ) ),
	   Qt::UniqueConnection );
  connect( action_Quit, SIGNAL( triggered() ), qApp, SLOT( closeAllWindows() ),
	   Qt::UniqueConnection );
  //  connect( action_SelMC, SIGNAL( triggered() ), selmc, SLOT( show() ),
  //       Qt::UniqueConnection );
  connect( selmc, SIGNAL( NewLogMsg( QString ) ),
	   this, SLOT( NewLogMsg( QString ) ),
	   Qt::UniqueConnection );
  connect( selmc, SIGNAL( NewLatticeConstant( double ) ),
	   u, SLOT( setD( double ) ),
	   Qt::UniqueConnection );
  //  connect( action_SetSSV, SIGNAL( triggered() ), starsSV, SLOT( show() ),
  //           Qt::UniqueConnection );

  connect( starsSV, SIGNAL( SSVNewAddress( const QString & ) ),
	   s, SLOT( SetNewSVAddress( const QString & ) ),
	   Qt::UniqueConnection );
  connect( starsSV, SIGNAL( SSVNewPort( const QString & ) ),
	   s, SLOT( SetNewSVPort( const QString & ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( RecordSSVHistoryA( const QString & ) ),
	   starsSV, SLOT( RecordSSVHistoryA( const QString & ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( RecordSSVHistoryP( const QString & ) ),
	   starsSV, SLOT( RecordSSVHistoryP( const QString & ) ),
	   Qt::UniqueConnection );
  connect( starsSV, SIGNAL( AskReConnect() ), s, SLOT( ReConnect() ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( ReConnected() ), this, SLOT( InitializeUnitsAgain() ),
	   Qt::UniqueConnection );
  //  connect( starsSV, SIGNAL( accepted() ), s, SLOT( ReConnect() ),
  //           Qt::UniqueConnection );

  connect( s, SIGNAL( ConnectionIsReady( void ) ), this, SLOT( Initialize( void ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsListNodes( SMsg ) ), this, SLOT( RcvListNodes( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( EvConnected( SMsg ) ),
	   this, SLOT( SomeDrvIsConnected( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( EvDisconnected( SMsg ) ),
	   this, SLOT( SomeDrvIsDisconnected( SMsg ) ),
	   Qt::UniqueConnection );

  if ( ! isQXafsModeAvailable ) {
    QXafsMode->setChecked( false );
    QXafsMode->setEnabled( false );
  }

  GoTimer = new QTimer;
  MCATimer = new QTimer;
  ScanTimer = new QTimer;
  MonTimer = new QTimer;
  MeasTimer = new QTimer;
  MeasDarkTimer = new QTimer;

  connect( GoTimer, SIGNAL( timeout() ), this, SLOT( MotorMove() ),
	   Qt::UniqueConnection );
  connect( MCATimer, SIGNAL( timeout() ), this, SLOT( MCASequence() ),
	   Qt::UniqueConnection );
  connect( ScanTimer, SIGNAL( timeout() ), this, SLOT( ScanSequence() ),
	   Qt::UniqueConnection );
  connect( MonTimer, SIGNAL( timeout() ), this, SLOT( MonSequence() ),
	   Qt::UniqueConnection );
  connect( MeasTimer, SIGNAL( timeout() ), this, SLOT( MeasSequence() ),
	   Qt::UniqueConnection );
  connect( MeasDarkTimer, SIGNAL( timeout() ), this, SLOT( MeasDarkSequence() ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( SSisActive( bool ) ), StatDisp, SLOT( SetSSVStat( bool ) ),
	   Qt::UniqueConnection );

  connect( AddNewDTh1TPoint, SIGNAL( clicked() ), this, SLOT( AddNewDTh1TunePoint() ),
	   Qt::UniqueConnection );
  connect( conds, SIGNAL( AskToSaveDTh1TTable() ), TTable, SLOT( SaveTuneTable() ),
	   Qt::UniqueConnection );
  connect( conds, SIGNAL( AskToShowDTh1TTable() ), TTable, SLOT( ShowTuneTable() ),
	   Qt::UniqueConnection );
  connect( conds, SIGNAL( NewDiff1( int ) ), this, SIGNAL( NewDiff1( int ) ),
	   Qt::UniqueConnection );
  connect( conds, SIGNAL( NewDiff2( int ) ), this, SIGNAL( NewDiff2( int ) ),
	   Qt::UniqueConnection );

  setupDataRoot();      // 他のファイルダイアログが全部 new されていないとダメ !

  s->AskStatus();
  s->MakeConnection();
}

void MainWindow::SetDXMPMC( void )
{
  int dP = MMainTh->value().toInt() - EncMainTh->value().toDouble() / MMainTh->getUPP();
  int oldCenter = MMainTh->getCenter();

  qDebug() << "dP " << dP
	   << "Enc " << EncMainTh->value().toDouble()
	   << "Enc " << EncMainTh->value().toDouble() / MMainTh->getUPP()
	   << "MTh " << MMainTh->value().toInt()
	   << "recal " << ( MMainTh->value().toInt() - dP ) * MMainTh->getUPP();

  MMainTh->setCenter( dP );
  MMainTh->GetValue();

  NewLogMsg( tr( "DXM center was changed from %1 to %2." )
	     .arg( oldCenter ).arg( dP ) );

  if ( RWDXMCenterF ) {    // 変更を DXMCenter.cfg に書くかどうかは XAFSM.def しだい
    QFile f( DXMCenterFile );
    if ( !f.open( QIODevice::Append | QIODevice::Text ) )
      return;
    QTextStream out( &f );
    out << dP << " \t"
	<< "DXM center was changed from " << oldCenter << " to " << dP << " : "
	<< QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" ) << "\n";
    f.close();
  }
}

void MainWindow::Initialize( void )
{
  InitAndIdentifyMotors();
  InitAndIdentifySensors();
  if ( ! AllInited ) {
    AllInited = true;
    connect( SelThEncorder, SIGNAL( toggled( bool ) ), this, SLOT( ShowCurThPos() ),
	     Qt::UniqueConnection );
    connect( SelThCalcPulse, SIGNAL( toggled( bool ) ), this, SLOT( ShowCurThPos() ),
	     Qt::UniqueConnection );
  }
  resize( 1, 1 );
  SendListNodes();
  if ( SFluo != NULL ) {
    getMCASettings( MCACh->text().toInt() );
    s->SendCMD2( "SetUpMCA", SFluo->getDriver(), "GetMCALength" );
    for ( int i = 0; i < MCAGains.count(); i++ ) {
      SFluo->setGain( MCAGains[i]->ch, MCAGains[i]->gain );
    }
  }
  for ( int i = 0; i < DriverList.count(); i++ ) {
    s->SendCMD2( "Initialize", "System", "flgon", DriverList.at(i) );
  }
}

void MainWindow::SendListNodes( void )
{
  s->SendCMD2( "Initialize", "System", "listnodes" );
}

void MainWindow::ShowMessageOnSBar( QString msg, int time )
{
  statusbar->showMessage( msg, time );
}

void MainWindow::InitializeUnitsAgain( void )
{
  s->MakeConnection();
}

void MainWindow::InitAndIdentifyMotors( void )
{
  AUnit *am;
  for ( int i = 0; i < AMotors.count(); i++ ) {
    am = AMotors.value(i);
    am->Initialize( s );
    if ( am->getID() == "THETA" ) {
      if ( MMainTh != NULL ) {
        disconnect( MMainTh, SIGNAL( newValue( QString ) ),
                    this, SLOT( ShowCurThPos() ) );
      }
      MMainTh = am;
      connect( MMainTh, SIGNAL( newValue( QString ) ), this, SLOT( ShowCurThPos() ),
	       Qt::UniqueConnection );
    }

    if ( am->getID() == "DTH1" ) {
      if ( MDTh1 != NULL ) {
        disconnect( MDTh1, SIGNAL( newValue( QString ) ),
                    this, SLOT( ShowCurDTh1() ) );
      }
      MDTh1 = am;
      connect( MDTh1, SIGNAL( newValue( QString ) ), this, SLOT( ShowCurDTh1() ),
	       Qt::UniqueConnection );
    }
  }
  if ( MMainTh != NULL ) {
    SetMainThCenter();
  }
}

void MainWindow::SetMainThCenter( void )
{
  if ( !RWDXMCenterF )
    return;

  QFile f( DXMCenterFile );
  
  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return;
  
  QTextStream in( &f );
  while( !in.atEnd() ) {
    QString buf = in.readLine().simplified().split( QRegExp( "\\s+" )  )[0];
    MMainTh->setCenter( buf.toInt() );
    qDebug() << "internal DXM center is set to " << buf;
    
    MMainTh->GetValue();
    f.close();
  }
}

void MainWindow::InitAndIdentifySensors( void )
{
  AUnit *as;

  for ( int i = 0; i < ASensors.count(); i++ ) {
    as = ASensors.value(i);
    as->Initialize( s );
    connect( as, SIGNAL( LogMsg( QString ) ), this, SLOT( NewLogMsg( QString ) ),
	     Qt::UniqueConnection );
    if ( as->getID() == "I0" ) { SI0 = as; }
    if ( as->getID() == "I1" ) { SI1 = as; }
    if ( as->getID() == "TotalF" ) { SFluo = as; }
    if ( as->getID() == "LS" ) {
      if ( SLS != NULL ) {
	disconnect( SLS, SIGNAL( NewRingCurrent( QString, QStringList ) ),
		    this, SLOT( ShowNewRingCurrent( QString, QStringList ) ) );
      }
      SLS = as;
      connect( SLS, SIGNAL( NewRingCurrent( QString, QStringList ) ),
	       this, SLOT( ShowNewRingCurrent( QString, QStringList ) ),
	       Qt::UniqueConnection );
    }
    if ( as->getID() == "ENCTH" ) {
      if ( EncMainTh != NULL ) {
	disconnect( EncMainTh, SIGNAL( newValue( QString ) ),
		    this, SLOT( ShowCurThPos() ) );
	disconnect( EncMainTh, SIGNAL( newValue( QString ) ),
		    StatDisp, SLOT( newEncTh( QString ) ) );
      }
      EncMainTh = as;
      connect( EncMainTh, SIGNAL( newValue( QString ) ), this, SLOT( ShowCurThPos() ),
	       Qt::UniqueConnection );
      connect( EncMainTh, SIGNAL( newValue( QString ) ),
	       StatDisp, SLOT( newEncTh( QString ) ),
	       Qt::UniqueConnection );
    }
    if ( as->getID() == "ENCTH2" ) {
      Enc2 = as;
    }
  }
  
  if ( SFluo != NULL ) {
    SFluo->setROIs( ROIStart, ROIEnd );
    for ( int i = 0; i < ASensors.count(); i++ ) {  // SFluo が確定してから
      as = ASensors.value(i);
      if (( as->getTheParent() == SFluo )&&( as != SFluo )) {
	connect( SFluo, SIGNAL( newValue( QString ) ),
		 as, SLOT( getNewValue( QString ) ),
		 Qt::UniqueConnection );
	connect( SFluo, SIGNAL( newDark( double ) ), as, SLOT( getNewDark( double ) ),
		 Qt::UniqueConnection );
      }
    }
  }

  if ( ! SensorsInited ) {
    SensorsInited = true;
    connect( StatDisp, SIGNAL( setEncNewTh( QString, QString ) ),
	     this, SLOT( setEncNewTh( QString, QString ) ),
	     Qt::UniqueConnection );
  }
}

void MainWindow::setEncNewTh( QString orig, QString newv )
{
  NewLogMsg( tr( "Encorder is set from %1 to %2" )
	     .arg( orig ).arg( newv ) );
  EncMainTh->SetValue( newv.toDouble() );
  EncMainTh->GetValue();
}

void MainWindow::RcvListNodes( SMsg msg )
{
  for ( int i = 0; i < msg.Vals().count(); i++ ) {
    SetEnableOfUnits( msg.Vals().at(i), true );
  }
}

void MainWindow::SomeDrvIsConnected( SMsg msg )
{
  SetEnableOfUnits( msg.From(), true );
}

void MainWindow::SomeDrvIsDisconnected( SMsg msg )
{
  SetEnableOfUnits( msg.From(), false );
}

void MainWindow::SetEnableOfUnits( QString drv, bool enable )
{
  AUnit *am, *as;

  for ( int j = 0; j < AMotors.count(); j++ ) {
    am = AMotors.value( j );
    if ( am->getDriver() == drv ) {
      am->setEnable( enable );
      if ( enable ) 
	am->Initialize( s );
    }
  }
  for ( int j = 0; j < ASensors.count(); j++ ) {
    as = ASensors.value( j );
    if ( as->getDriver() == drv ) {
      as->setEnable( enable );
      if ( enable ) 
	as->Initialize( s );
    }
  }
}

void MainWindow::ShowCurDTh1( void )
{
  if ( MDTh1 != NULL )
    DispDTh1->setText( MDTh1->value() );
}

void MainWindow::ShowCurThPos( void )
{
  QString buf1, buf2;
  double deg;

  if ( SelThEncorder->isChecked() ) {
    deg = EncMainTh->value().toDouble();
  } else {
    deg = ( MMainTh->value().toDouble() - MMainTh->getCenter() ) * MMainTh->getUPP();
  }
  if ( deg != oldDeg ) {
    oldDeg = deg;

    buf1.sprintf( UnitName[KEV].form, deg );
    ShowCurrentAngle->setText( buf1 );
    buf2.sprintf( UnitName[DEG].form, u->deg2keV( deg ) );
    ShowCurrentEnergy->setText( buf2 );
    emit NewEnergy( u->deg2keV( deg ) );
    NewLogMsg( tr( "Current Position [%1] deg [%2] keV" ).arg( buf1 ).arg( buf2 ) );
  }
}

double MainWindow::SelectedCurPosDeg( ENCORPM EncOrPM )
{
  double rv;

  switch( EncOrPM ) {
  case XENC:
    rv = EncMainTh->value().toDouble();
    break;
  case XPM:
    rv = ( MMainTh->value().toDouble() - MMainTh->getCenter() ) * MMainTh->getUPP();
    break;
  default:
    qDebug() << "Selected Enc or PM is wrong, anyway return Encorder !";
    rv = EncMainTh->value().toDouble();
    break;
  }
  return rv;
}

ViewCTRL *MainWindow::SetUpNewView( VTYPE vtype )
{
  void *newView = NULL;
  switch( vtype ) {
  case XYVIEW:
    newView = (void *)(new XYView);
    ((XYView*)newView)->setParent( this );
    ((XYView*)newView)->setDiffType1( conds->Diff1Type() );
    ((XYView*)newView)->setDiffType2( conds->Diff2Type() );
    break;
  case TYVIEW:
    newView = (void *)(new TYView); break;
  case MCAVIEW:
    newView = (void *)(new MCAView( this ));
    ((MCAView *)newView)->setKeV2Pix( kev2pix );
    ((MCAView *)newView)->setFDBase( fdbase );
    ((MCAView *)newView)->setShowElements( DispElmNames->isChecked() );
    ((MCAView *)newView)->setShowElementsAlways( ShowAlwaysSelElm->isChecked() );
    ((MCAView *)newView)->setShowElementsEnergy( ShowElmEnergy->isChecked() );
    ((MCAView *)newView)->setLimitPSEnergy( LimitPSEnergy->isChecked() );
    ((MCAView *)newView)->setShowDiff( ShowDiff->isChecked() );
    ((MCAView *)newView)->setPeakSearch( MCAPeakSearch->isChecked() );
    ((MCAView *)newView)->setPeakFit( MCAPeakFit->isChecked() );
    ((MCAView *)newView)->setLog( SetDisplayLog->isChecked() );
    ((MCAView *)newView)->setNewPSSens( PeakSearchSensitivity->text() );
    break;
  default:
    return NULL;
  }

  if ( ! ViewCtrls[ ViewTab->currentIndex() ]->setView( newView, vtype ) ) {
    // current tab is not available.
    int i;
    for ( i = 0; i < ViewTab->count(); i++ ) {
      if ( ViewCtrls[ i ]->setView( newView, vtype ) ) {
	break;
      }
    }
    if ( i < ViewTab->count() ) {          // an available tab is found.
      ViewTab->setCurrentIndex( i );       // make it current tab.
    } else {
      // no tab is available.
      statusbar->showMessage( tr( "No Scree is available!" ), 2000 );
      switch( vtype ) {
      case XYVIEW:
	delete (XYView *)newView; break;
      case TYVIEW:
	delete (TYView *)newView; break;
      case MCAVIEW:
	delete (MCAView *)newView; break;
      default:
	qDebug() << "Unknow vewType was passed to SetUpNewView";
      }
      newView = NULL;
      return NULL;
    }
  }

  return ViewCtrls[ ViewTab->currentIndex() ];
}

void MainWindow::ShowNewRingCurrent( QString Val, QStringList )
{
  RingCurrent->setText( Val );
}

#if 0
bool MainWindow::isAnyOtherProcess( void )
{
  if ( inMeas || inSPSing || inMonitor || inMMove || inMCAMeas || inS2D ) {
    statusbar
      ->showMessage( tr( "Can't start. Othre Process is going on." ), 2000 );
    return true;
  }
  return false;
}
#endif

