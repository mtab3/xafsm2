#include <QtGui>

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

  // Monitor の中で SSD の強度を別ファイルに書き出すときの時間を測るため
  T = new QTime;
  T->start();

  kev2pix = new KeV2Pix;
  fdbase = new FluoDBase;
#if 0
  int dim = kev2pix->getDim();
  for ( int i = 0; i < MaxSSDs; i++ ) {
    const QVector<double>& ab = kev2pix->getAB( i );
    for ( int j = 0; j < dim + 1; j++ ) {
      qDebug() << "ab[i][j] = " << ab[j];
    }
  }
#endif

  MMainTh = EncMainTh = NULL;
  SLS = SI0 = SI1 = SFluo = NULL;
  oldDeg = -100;
  AllInited = MotorsInited = SensorsInited = false;
  EncOrPM = XENC;

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

  Initialize();
  setupView();
  setupCommonArea();
  setupSetupArea();     // AUnit 関係の Initialize 後でないとだめ
  if ( SFluo != NULL )
    setupSetupSSDArea();
  else {
    MainTab->removeTab( MainTab->indexOf( SSDTab ) );
  }
  setupMeasArea();
  setupReadDataArea();

  conds = new Conditions;
  conds->setEncAsTh( true );
  conds->setAddInfos( true );

  StatDisp->setupStatArea( &AMotors, &ASensors, starsSV, selmc, conds );
  connect( StatDisp, SIGNAL( NeedListNodes() ), this, SLOT( SendListNodes() ) );
  //  QString msg = "XafsMsg_" + QLocale::system().name();
  //  NewLogMsg( msg );
  NewLogMsg( QString( tr( "Mono: %1 (%2 A)" ) )
	     .arg( mccd[ selmc->MC() ]->getMCName() )
	     .arg( mccd[ selmc->MC() ]->getD() ) );

  connect( s, SIGNAL( AskShowStat( QString, int ) ),
	   this, SLOT( ShowMessageOnSBar( QString, int ) ) );
  connect( action_Quit, SIGNAL( triggered() ), qApp, SLOT( closeAllWindows() ) );
  //  connect( action_SelMC, SIGNAL( triggered() ), selmc, SLOT( show() ) );
  connect( selmc, SIGNAL( NewLogMsg( QString ) ),
	   this, SLOT( NewLogMsg( QString ) ) );
  connect( selmc, SIGNAL( NewLatticeConstant( double ) ),
	   this, SLOT( SetNewLatticeConstant( double ) ) );
  //  connect( action_SetSSV, SIGNAL( triggered() ), starsSV, SLOT( show() ) );

  connect( starsSV, SIGNAL( SSVNewAddress( const QString & ) ),
	   s, SLOT( SetNewSVAddress( const QString & ) ) );
  connect( starsSV, SIGNAL( SSVNewPort( const QString & ) ),
	   s, SLOT( SetNewSVPort( const QString & ) ) );
  connect( s, SIGNAL( RecordSSVHistoryA( const QString & ) ),
	   starsSV, SLOT( RecordSSVHistoryA( const QString & ) ) );
  connect( s, SIGNAL( RecordSSVHistoryP( const QString & ) ),
	   starsSV, SLOT( RecordSSVHistoryP( const QString & ) ) );
  connect( starsSV, SIGNAL( AskReConnect() ), s, SLOT( ReConnect() ) );
  connect( s, SIGNAL( ReConnected() ), this, SLOT( InitializeUnitsAgain() ) );
  //  connect( starsSV, SIGNAL( accepted() ), s, SLOT( ReConnect() ) );

  connect( s, SIGNAL( ConnectionIsReady( void ) ), this, SLOT( Initialize( void ) ) );
  connect( s, SIGNAL( AnsListNodes( SMsg ) ), this, SLOT( RcvListNodes( SMsg ) ) );
  connect( s, SIGNAL( EvConnected( SMsg ) ),
	   this, SLOT( SomeDrvIsConnected( SMsg ) ) );
  connect( s, SIGNAL( EvDisconnected( SMsg ) ),
	   this, SLOT( SomeDrvIsDisconnected( SMsg ) ) );

  GoTimer = new QTimer;
  MCATimer = new QTimer;
  ScanTimer = new QTimer;
  MonTimer = new QTimer;
  MeasTimer = new QTimer;
  MeasDarkTimer = new QTimer;

  connect( GoTimer, SIGNAL( timeout() ), this, SLOT( MotorMove() ) );
  connect( MCATimer, SIGNAL( timeout() ), this, SLOT( MCASequence() ) );
  connect( ScanTimer, SIGNAL( timeout() ), this, SLOT( ScanSequence() ) );
  connect( MonTimer, SIGNAL( timeout() ), this, SLOT( MonSequence() ) );
  connect( MeasTimer, SIGNAL( timeout() ), this, SLOT( MeasSequence() ) );
  connect( MeasDarkTimer, SIGNAL( timeout() ), this, SLOT( MeasDarkSequence() ) );

  //  connect( s, SIGNAL( ConnectingServer( QString )), StatDisp, SLOT( SetSSVA( QString )));
  //  connect( s, SIGNAL( ConnectingPort( qint16 ) ), StatDisp, SLOT( SetSSVP( qint16 ) ) );
  connect( s, SIGNAL( SSisActive( bool ) ), StatDisp, SLOT( SetSSVStat( bool ) ) );

  s->AskStatus();

  s->MakeConnection();
}

void MainWindow::Initialize( void )
{
  InitAndIdentifyMotors();
  InitAndIdentifySensors();
  if ( ! AllInited ) {
    AllInited = true;
    connect( SelThEncorder, SIGNAL( toggled( bool ) ), this, SLOT( ShowCurThPos() ) );
    connect( SelThCalcPulse, SIGNAL( toggled( bool ) ), this, SLOT( ShowCurThPos() ) );
  }
  resize( 1, 1 );
  SendListNodes();
  if ( SFluo != NULL ) {
    getMCASettings( MCACh->text().toInt() );
    s->SendCMD2( "SetUpMCA", SFluo->getDriver(), "GetMCALength" );
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
	disconnect( MMainTh, SIGNAL( newValue( QString ) ), this, SLOT( ShowCurThPos() ) );
      }
      MMainTh = am;
      connect( MMainTh, SIGNAL( newValue( QString ) ), this, SLOT( ShowCurThPos() ) );
    }
  }
}

void MainWindow::InitAndIdentifySensors( void )
{
  AUnit *as;

  for ( int i = 0; i < ASensors.count(); i++ ) {
    as = ASensors.value(i);
    as->Initialize( s );
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
		  this, SLOT( ShowNewRingCurrent( QString, QStringList ) ) );
    }
    if ( as->getID() == "ENCTH" ) {
      if ( EncMainTh != NULL ) {
	disconnect( EncMainTh, SIGNAL( newValue( QString ) ), this, SLOT( ShowCurThPos() ) );
	disconnect( EncMainTh, SIGNAL( newValue( QString ) ),
		    StatDisp, SLOT( newEncTh( QString ) ) );
      }
      EncMainTh = as;
      connect( EncMainTh, SIGNAL( newValue( QString ) ), this, SLOT( ShowCurThPos() ) );
      connect( EncMainTh, SIGNAL( newValue( QString ) ),
	       StatDisp, SLOT( newEncTh( QString ) ) );
    }
  }
  
  if ( SFluo != NULL )
    SFluo->setROIs( ROIStart, ROIEnd );
  if ( ! SensorsInited ) {
    SensorsInited = true;
    connect( StatDisp, SIGNAL( setEncNewTh( QString, QString ) ),
	     this, SLOT( setEncNewTh( QString, QString ) ) );
  }
}

void MainWindow::setEncNewTh( QString orig, QString newv )
{
  NewLogMsg( tr( "Encorder is set from %1 to %2" )
	     .arg( orig ).arg( newv ) );
  EncMainTh->SetValue( newv.toDouble() );
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
    buf2.sprintf( UnitName[DEG].form, deg2keV( deg ) );
    ShowCurrentEnergy->setText( buf2 );
    
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
    newView = (void *)(new XYView); break;
  case TYVIEW:
    newView = (void *)(new TYView); break;
  case MCAVIEW:
    newView = (void *)(new MCAView);
    ((MCAView *)newView)->setKeV2Pix( kev2pix );
    ((MCAView *)newView)->setFDBase( fdbase );
    ((MCAView *)newView)->setShowElements( DispElmNames->isChecked() );
    ((MCAView *)newView)->setShowElementsAlways( ShowAlwaysSelElm->isChecked() );
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

void MainWindow::ShowNewRingCurrent( QString Val, QStringList Vals )
{
  RingCurrent->setText( Val );
}
