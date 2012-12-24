#include <QtGui>

#include "XafsM.h"
#include "MainWindow.h"
#include "SelMC.h"
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

  StatDisp = new Status();
  StatTab->layout()->addWidget( StatDisp );

  XAFSName = myname;
  XAFSKey = myname;
  XAFSTitle = myname;

  starsSV = new StarsSV;

  nowCurrent = 0;

  setupLogArea();     // ログに対する書き出しがある可能性があるので最初にイニシャライズ

  ReadDef( "XAFSM.def" );
  selmc = new SelMC( mccd );

  setWindowTitle( XAFSTitle );
  s = new Stars;      // モータ類のイニシャライズの前に Stars の準備はしておく
  s->ReadStarsKeys( XAFSKey, XAFSName ); // Stars とのコネクション確立の準備
  s->SetNewSVAddress( starsSV->SSVAddress() );
  s->SetNewSVPort( starsSV->SSVPort() );
  Initialize();

  setupView();
  setupCommonArea();
  setupSetupArea();
  setupSetupSSDArea();
  setupMeasArea();
  StatDisp->setupStatArea( &AMotors, &ASensors );
  connect( StatDisp, SIGNAL( NeedListNodes() ), this, SLOT( SendListNodes() ) );
  QString msg = "XafsMsg_" + QLocale::system().name();
  NewLogMsg( msg + "\n" );
  NewLogMsg( QString( tr( "Mono: %1 (%2 A)\n" ) )
	     .arg( mccd[ selmc->MC() ]->getMCName() )
	     .arg( mccd[ selmc->MC() ]->getD() ) );

  connect( s, SIGNAL( AskShowStat( QString, int ) ),
	   this, SLOT( ShowMessageOnSBar( QString, int ) ) );
  connect( action_Quit, SIGNAL( triggered() ), qApp, SLOT( closeAllWindows() ) );
  connect( action_SelMC, SIGNAL( triggered() ), selmc, SLOT( show() ) );
  connect( selmc, SIGNAL( NewLogMsg( QString ) ),
	   this, SLOT( NewLogMsg( QString ) ) );
  connect( selmc, SIGNAL( NewLatticeConstant( double ) ),
	   this, SLOT( SetNewLatticeConstant( double ) ) );
  connect( action_SetSSV, SIGNAL( triggered() ), starsSV, SLOT( show() ) );

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
  connect( starsSV, SIGNAL( accepted() ), s, SLOT( ReConnect() ) );

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

  connect( GoTimer, SIGNAL( timeout() ), this, SLOT( MotorMove() ) );
  connect( MCATimer, SIGNAL( timeout() ), this, SLOT( MCASequence() ) );
  connect( ScanTimer, SIGNAL( timeout() ), this, SLOT( ScanSequence() ) );
  connect( MonTimer, SIGNAL( timeout() ), this, SLOT( MonSequence() ) );
  connect( MeasTimer, SIGNAL( timeout() ), this, SLOT( MeasSequence() ) );

  s->MakeConnection();
}

void MainWindow::Initialize( void )
{
  InitAndIdentifyMotors();
  InitAndIdentifySensors();
  connect( SelThEncorder, SIGNAL( toggled( bool ) ), this, SLOT( ShowCurThPos() ) );
  connect( SelThCalcPulse, SIGNAL( toggled( bool ) ), this, SLOT( ShowCurThPos() ) );
  resize( 1, 1 );

  getMCASettings( MCACh->text().toInt() );
  SendListNodes();
  s->SendCMD2( "SetUpMCA", SFluo->getDriver(), "GetMCALength" );
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
    if ( am->getID() == "THETA" ) { MMainTh = am; }
  }
  connect( MMainTh, SIGNAL( newValue( QString ) ), this, SLOT( ShowCurThPos() ) );
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
    if ( as->getID() == "ENCTH" ) { EncMainTh = as; }
  }
  SFluo->setROIs( ROIStart, ROIEnd );
  connect( EncMainTh, SIGNAL( newValue( QString ) ), this, SLOT( ShowCurThPos() ) );
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


void MainWindow::ShowCurThPos( void )   // 値はあえて使わない
{
  QString buf;
  double deg;

  if ( SelThEncorder->isChecked() ) {
    deg = EncMainTh->value().toDouble();
  } else {
    deg = ( MMainTh->value().toDouble() - MMainTh->getCenter() ) * MMainTh->getUPP();
  }

  buf.sprintf( UnitName[KEV].form, deg );
  ShowCurrentAngle->setText( buf );
  buf.sprintf( UnitName[DEG].form, CurPosKeV = deg2keV( deg ) );
  ShowCurrentEnergy->setText( buf );
  
  NewLogMsg( tr( "Current Position [%1] keV\n" ).arg( buf ) );
}


ViewCTRL *MainWindow::SetUpNewView( VTYPE vtype )
{
  void *newView = NULL;
  switch( vtype ) {
  case XVIEW:
    newView = (void *)(new XView); break;
  case MCAVIEW:
    newView = (void *)(new MCAView); break;
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
      case XVIEW:
	delete (XView *)newView; break;
      case MCAVIEW:
	delete (MCAView *)newView; break;
      default:
	qDebug() << "Unknow vewType was passed to SetUpNewView";
      }
      newView = NULL;
    }
  }
  return ViewCtrls[ ViewTab->currentIndex() ];
}
