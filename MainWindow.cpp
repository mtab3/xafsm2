
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

  //  ViewTab->setTabBarAutoHide( true );   // Qt 5.4 以上用
  
  MainTab->setCurrentIndex( 0 );
  RWDXMCenterF = false;
  DXMCenterFile = DXMCENTERFILE0;
  MCACanSaveAllOnMem = false;
  //  MCAPreAMPGainHasSet = false;
  MStabOk = false;
  MStabDelegate = "";

#if 0
  AutoShutter->setChecked( false );  // 自動シャッターのボタンはデフォルトでは
  AutoShutter->setEnabled( false );  // 使えなくしておく
#endif

  scanFSel = monFSel = S2DFileSel
    = SelDFND = SelWBFND = SelRBFND = SelLFND = NULL;

  // Monitor の中で SSD の強度を別ファイルに書き出すときの時間を測るため
  T = new QTime;
  T->start();

  QStringList m0, m1;
  m0 << tr( "Both the name and the data is old." );           // 0, 0
  m0 << tr( "The name is new, but the data is old." );        // 0, 1
  m1 << tr( "The data is new, but the name is old." );        // 1, 0
  m1 << tr( "The data and the name is new, but not saved." ); // 1, 1
  FSTATMsgs << m0 << m1;
  MeasDataStat = MeasNameStat = OLD;
  ScanDataStat = ScanNameStat = OLD;
  MonDataStat = MonNameStat = OLD;
  S2DDataStat = S2DNameStat = OLD;

  isQXafsModeAvailable = false;

  //  XMAPk2p = new KeV2Pix;
  fdbase = new FluoDBase;
  u = new Units;

  MMainTh = NULL;
  MDTh1 = NULL;
  EncMainTh = NULL;
  Enc2 = NULL;
  SLS = NULL;
  SI0 = NULL;
  SI1 = NULL;
  MMStab = NULL;
  SFluos.clear();
  SSFluos.clear();

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
  remote = new AUnitRemote;
  connect( remote, SIGNAL( setMeasBlockF( bool ) ), MeasBlockB, SLOT( setChecked( bool ) ), Qt:: UniqueConnection );
  
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
  setupSFluoRelated();
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
  //  MCATimer = new QTimer;
  ScanTimer = new QTimer;
  MonTimer = new QTimer;
  MeasTimer = new QTimer;
  MeasDarkTimer = new QTimer;

  connect( GoTimer, SIGNAL( timeout() ), this, SLOT( MotorMove() ),
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
    AMotor *am = AMotors[i];
    qDebug() << am->id();
    if ( ( am->id() == "STAGEX" ) || ( am->id() == "StageX" ) ){
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
    smAm->SetValue( smAm->value().toInt() + 2.5 / smAm->upp() );
    smStage = 1;
    break;
  case 1:
    smAm->SetValue( smAm->value().toInt() - 5.0 / smAm->upp() );
    smStage = 2;
    break;
  case 2:
    smAm->SetValue( smAm->value().toInt() + 5.0 / smAm->upp() );
    smStage = 1;
    break;
  }
}

void MainWindow::SetDXMPMC( void )
{
  int dP = MMainTh->value().toInt() - EncMainTh->value().toDouble() / MMainTh->upp();
  int oldCenter = MMainTh->getCenter();

  qDebug() << "dP " << dP
	   << "Enc " << EncMainTh->value().toDouble()
	   << "Enc " << EncMainTh->value().toDouble() / MMainTh->upp()
	   << "MTh " << MMainTh->value().toInt()
	   << "recal " << ( MMainTh->value().toInt() - dP ) * MMainTh->upp();

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
  remote->Initialize( s );
  if ( ! AllInited ) {
    AllInited = true;
    connect( SelThEncorder, SIGNAL( toggled( bool ) ), this, SLOT( ShowCurThPos() ),
	     Qt::UniqueConnection );
    connect( SelThCalcPulse, SIGNAL( toggled( bool ) ), this, SLOT( ShowCurThPos() ),
	     Qt::UniqueConnection );
  }
  resize( 1, 1 );
  SendListNodes();
  for ( int i = 0; i < SFluos.count(); i++ ) {
    SSFluos[i]->getMCASettings( SSFluos[i]->mcaCh() );
    s->SendCMD2( "SetUpMCA", SFluos[i]->dev(), "GetMCALength" );
  }
  for ( int i = 0; i < MCAGains.count(); i++ ) {
    SFluos[ MCAGains[i]->dNo ]->setGain( MCAGains[i]->ch, MCAGains[i]->gain );
  }
#if 0  // 各 Unit(Driver)の Initialize でやる
  for ( int i = 0; i < DriverList.count(); i++ ) {
    s->SendCMD2( "Initialize", "System", "flgon", DriverList.at(i) ); // not be used
  }
#endif
  pmConds->Initialize();
}

// これは個別のUnit(Driver)とは関係なく、Stars server と XafsM2 の間のやり取りなのでここで OK
void MainWindow::SendListNodes( void )
{
  s->SendCMD2( "Initialize", "System", "listnodes" );  // OK to be here
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
  AMotor *am;
  for ( int i = 0; i < AMotors.count(); i++ ) {
    am = AMotors.value(i);
    am->Initialize( s );
    if ( am->id() == "THETA" ) {
      if ( MMainTh != NULL ) {
        disconnect( MMainTh, SIGNAL( NewValue( QString ) ),
                    this, SLOT( ShowCurThPos() ) );
      }
      MMainTh = (AUnitPM*)am;
      //      iMMainTh = i;
      connect( MMainTh, SIGNAL( NewValue( QString ) ), this, SLOT( ShowCurThPos() ),
	       Qt::UniqueConnection );
    }

    if ( am->id() == "DTH1" ) {
      if ( MDTh1 != NULL ) {
        disconnect( MDTh1, SIGNAL( NewValue( QString ) ),
                    this, SLOT( ShowCurDTh1() ) );
      }
      MDTh1 = am;
      connect( MDTh1, SIGNAL( NewValue( QString ) ), this, SLOT( ShowCurDTh1() ),
	       Qt::UniqueConnection );
    }

    if ( MStabOk && ( MStabDelegate != "" ) && ( am->uid() == MStabDelegate ) ) {
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
  ASensor *as;

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
    if ( as->id() == "I0" ) { SI0 = as; }
    if ( as->id() == "I1" ) { SI1 = as; }
    //    if ( as->id() == "TotalF" ) { SFluo = (AUnitSFluo*)as; }
    if ( as->id() == "LS" ) {
      if ( SLS != NULL ) {
	disconnect( SLS, SIGNAL( NewRingCurrent( QString, QStringList ) ),
		    this, SLOT( ShowNewRingCurrent( QString, QStringList ) ) );
      }
      SLS = as;
      connect( SLS, SIGNAL( NewRingCurrent( QString, QStringList ) ),
	       this, SLOT( ShowNewRingCurrent( QString, QStringList ) ),
	       Qt::UniqueConnection );
    }
    if ( as->id() == "ENCTH" ) {
      if ( EncMainTh != NULL ) {
	disconnect( EncMainTh, SIGNAL( NewValue( QString ) ),
		    this, SLOT( ShowCurThPos() ) );
	disconnect( EncMainTh, SIGNAL( NewValue( QString ) ),
		    StatDisp, SLOT( newEncTh( QString ) ) );
      }
      EncMainTh = (AUnitENC*)as;
      connect( EncMainTh, SIGNAL( NewValue( QString ) ), this, SLOT( ShowCurThPos() ),
	       Qt::UniqueConnection );
      connect( EncMainTh, SIGNAL( NewValue( QString ) ),
	       StatDisp, SLOT( newEncTh( QString ) ),
	       Qt::UniqueConnection );
    }
    if ( as->id() == "ENCTH2" ) {
      Enc2 = (AUnitENC2*)as;
    }
  }
  

  if ( SFluos.count() > 0 )
    S2DBase->setSSFluos( SSFluos );     // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  for ( int i = 0; i < SFluos.count(); i++ ) {
    SSFluos[i]->K2P()
      ->MakeUpAB( SFluos[i]->length(), SFluos[i]->chs(), 2, SSFluos[i]->K2PFile() );
    //    SFluo->setROIs( ROIStart, ROIEnd );
    for ( int i = 0; i < ASensors.count(); i++ ) {  // SFluo が確定してから
      as = ASensors.value(i);
      if ( isASFluoUnit( as->theParent() ) && ( ! isASFluoUnit( as ) ) ) {
	connect( as->theParent(), SIGNAL( NewValue( QString ) ),
		 as, SLOT( getNewValue( QString ) ),
		 Qt::UniqueConnection );
	connect( as->theParent(), SIGNAL( NewDark( double ) ),
		 as, SLOT( getNewDark( double ) ),
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
  AMotor *am;
  ASensor *as;

  for ( int j = 0; j < AMotors.count(); j++ ) {
    am = AMotors.value( j );
    if ( am->dev() == drv ) {
      am->setEnable( enable );
      if ( enable ) 
	am->Initialize( s );
    }
  }
  for ( int j = 0; j < ASensors.count(); j++ ) {
    as = ASensors.value( j );
    if ( as->dev() == drv ) {
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
    deg = ( MMainTh->value().toDouble() - MMainTh->getCenter() ) * MMainTh->upp();
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
    rv = ( MMainTh->value().toDouble() - MMainTh->getCenter() ) * MMainTh->upp();
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
  for ( int i = 0; i < ASensors.count(); i++ ) {
    if ( ASensors[i]->devCh() == msg.From() ) {
      alarms->chkAlarm( ASensors[i]->uid(),
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

