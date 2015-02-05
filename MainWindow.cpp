
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
  MCACanSaveAllOnMem = false;
  MCAPreAMPGainHasSet = false;
  MaxMCAEnergy = 20;
  MStabOk = false;
  MStabDelegate = "";

#if 0
  AutoShutter->setChecked( false );  // 自動シャッターのボタンはデフォルトでは
  AutoShutter->setEnabled( false );  // 使えなくしておく
#endif

  MCAFSel = scanFSel = monFSel = S2DFileSel
    = SelDFND = SelWBFND = SelRBFND = SelLFND = NULL;

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

  MMainTh = MDTh1 = EncMainTh = Enc2 = NULL;
  SLS = SI0 = SI1 = SFluo = NULL;
  MMStab = NULL;

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
  conds = new Conditions;
  conds->setVersionInfo( VERSION, __DATE__, __TIME__ );
  alarms = new Alarms;
  connect( alarms, SIGNAL( alarmOn() ),
	   this, SLOT( alarmOn() ), Qt::UniqueConnection );
  connect( alarms, SIGNAL( alarmOff() ),
	   this, SLOT( alarmOff() ), Qt::UniqueConnection );
  
  setupLogArea();     // ログに対する書き出しがある可能性があるので最初にイニシャライズ
  ReadDef( DefFileName );
  selmc = new SelMC2( mccd );
  setWindowTitle( XAFSTitle );
  s = new Stars;      // モータ類のイニシャライズの前に Stars の準備はしておく
  s->ReadStarsKeys( XAFSKey, XAFSName ); // Stars とのコネクション確立の準備
  s->SetNewSVAddress( starsSV->SSVAddress() );
  s->SetNewSVPort( starsSV->SSVPort() );
  connect( s, SIGNAL( EvAll( SMsg ) ), this, SLOT( RcvEvAll( SMsg ) ), Qt::UniqueConnection );

  TTable = new TuningTable;
  pmConds = new PMConditions;

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

  //  useFixedDelta = false;
  connect( conds, SIGNAL( SetDXMPMC() ), this, SLOT( SetDXMPMC() ),
	   Qt::UniqueConnection );

  StatDisp->setupStatArea( &AMotors, &ASensors, starsSV, selmc, conds, pmConds );

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

  connect( Special, SIGNAL( clicked() ), this, SLOT( SpecialMove() ) );
  
  conds->setupDataRoot();      // 他のファイルダイアログが全部 new されていないとダメ !

  s->AskStatus();
  s->MakeConnection();
}

void MainWindow::SpecialMove( void )
{
  smAm = NULL;
  for ( int i = 0; i < AMotors.count(); i++ ) {
    AUnit *am = AMotors[i];
    qDebug() << am->getID();
    if ( ( am->getID() == "STAGEX" ) || ( am->getID() == "StageX" ) ){
      smAm = am;
      break;
    }
  }
  if ( smAm == NULL )
    return;
  
  QTimer *smt = new QTimer;
  smt->setInterval( 100 );
  connect( smt, SIGNAL( timeout() ), this, SLOT( SpecialMoveCore() ) );
  smStage = 0;
  smt->start();
}

void MainWindow::SpecialMoveCore( void )
{
  if ( smAm->isBusy() )
    return;

  switch( smStage ) {
  case 0:
    smAm->SetValue( smAm->value().toInt() + 2.5 / smAm->getUPP() );
    smStage = 1;
    break;
  case 1:
    smAm->SetValue( smAm->value().toInt() - 5.0 / smAm->getUPP() );
    smStage = 2;
    break;
  case 2:
    smAm->SetValue( smAm->value().toInt() + 5.0 / smAm->getUPP() );
    smStage = 1;
    break;
  }
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
    //    setPreAMPGains();
  }
  for ( int i = 0; i < DriverList.count(); i++ ) {
    s->SendCMD2( "Initialize", "System", "flgon", DriverList.at(i) );
  }
  pmConds->Initialize();
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

    if ( MStabOk && ( MStabDelegate != "" ) && ( am->getUid() == MStabDelegate ) ) {
      MMStab = am;
    }
  }

  if ( MMainTh != NULL ) {
    SetMainThCenter();
  }
  if ( MMStab == NULL ) {
    MStabOk = false;
    TuneAtEachStp->setEnabled( false );
    TuneAtEachStp->setChecked( false );
    AutoShutter->setEnabled( false );
    AutoShutter->setEnabled( false );
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
    connect( as, SIGNAL( LogMsg( QString ) ),
	     this, SLOT( NewLogMsg( QString ) ),
	     Qt::UniqueConnection );
#if 0
    connect( as, SIGNAL( Alarm( QString, QString ) ),
			 alarms, SLOT( chkAlarm( QString, QString ) ),
			 Qt::UniqueConnection );
#endif
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

void MainWindow::ShowNewRingCurrent( QString Val, QStringList )
{
  RingCurrent->setText( Val );
}

void MainWindow::SignalToStars( QString event )
{
  s->SendEvent( event );
}

void MainWindow::RcvEvAll( SMsg msg )
{
  qDebug() << "Event" << msg.From();
  for ( int i = 0; i < ASensors.count(); i++ ) {
    qDebug() << "unit " << ASensors[i]->getDevCh() << msg.From();
    if ( ASensors[i]->getDevCh() == msg.From() ) {
      qDebug() << "uid " << ASensors[i]->getUid();
      qDebug() << "msg " << QString( "%1 %2" ).arg( msg.Msg() ).arg( msg.Val() );
      alarms->chkAlarm( ASensors[i]->getUid(),
	       QString( "%1 %2" ).arg( msg.Msg() ).arg( msg.Val() ) );
    }
  }
}

void MainWindow::alarmOn( void )
{
  ViewTab->setStyleSheet( ALARMCOLOR );
}

void MainWindow::alarmOff( void )
{
  ViewTab->setStyleSheet( "" );
}
