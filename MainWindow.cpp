#include <QtGui>

#include "XafsM.h"
#include "MainWindow.h"
#include "SelMC.h"
#include "Stars.h"

#if 0
MOTORD Motors[ MOTORS ] = {
  { MONOCHRO1, "Monochro 1", "monochro1", "", 0, 0,  0, 0, 0, 0 },
                                   // 注: １番のモーターは monochro と決め打ちにしている
  { MOTOR1, "Motor 1", "motor1", "", 100, 100,  100, 1, 0, 1000 },
  { MOTOR2, "Motor 2", "motor2", "", 100, 100,  100, 1, 0, 1000 },
  { MOTOR3, "Motor 3", "motor3", "", 100, 100,  100, 1, 0, 1000 },
  { MOTOR4, "Motor 4", "motor4", "", 100, 100,  100, 1, 0, 1000 },
};
#endif

const char *CMode[ MEASMODES + 1 ] = {
  "Transmission",
  "Fluorescence",
  "Aux input",
  "Extra mode",
  NULL,
};

MainWindow::MainWindow( QString myname ) : QMainWindow()
{
  setupUi( this );

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
  setupMeasArea();
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
  s->MakeConnection();
}

void MainWindow::Initialize( void )
{
  InitAndIdentifyMotors();
  InitAndIdentifySensors();
  connect( SelThEncorder, SIGNAL( toggled( bool ) ), this, SLOT( ShowCurThPos() ) );
  connect( SelThCalcPulse, SIGNAL( toggled( bool ) ), this, SLOT( ShowCurThPos() ) );
  resize( 1, 1 );
}

void MainWindow::ShowMessageOnSBar( QString msg, int time )
{
  statusbar->showMessage( msg, time );
}

void MainWindow::InitializeUnitsAgain( void )
{
  s->MakeConnection();
  
#if 0
  AUnit *am, *as;
  
  for ( int i = 0; i < AMotors.count(); i++ ) {
    am = AMotors.value(i);
    am->Initialize( s );
    //    am->GetValue();
  }
  
  for ( int i = 0; i < ASensors.count(); i++ ) {
    as = ASensors.value(i);
    as->Initialize( s );
    as->GetValue();
  }
#endif
}

void MainWindow::InitAndIdentifyMotors( void )
{
  AUnit *am;
  for ( int i = 0; i < AMotors.count(); i++ ) {
    am = AMotors.value(i);
    am->Initialize( s );
    am->setUniqID( QString::number( i ) );
    if ( am->getID() == "THETA" ) { MMainTh = am; }
  }
  connect( MMainTh, SIGNAL( newValue( QString ) ), this, SLOT( ShowCurThPos() ) );
  MMainTh->AskIsBusy();
  MMainTh->GetValue();
}

void MainWindow::InitAndIdentifySensors( void )
{
  AUnit *as;

  for ( int i = 0; i < ASensors.count(); i++ ) {
    as = ASensors.value(i);
    as->Initialize( s );
    as->setUniqID( QString::number( i ) );
    if ( as->getID() == "I0" ) { SI0 = as; }
    if ( as->getID() == "I1" ) { SI1 = as; }
    if ( as->getID() == "TotalF" ) { SFluo = as; }
    if ( as->getID() == "ENCTH" ) { EncMainTh = as; }
  }
  connect( EncMainTh, SIGNAL( newValue( QString ) ), this, SLOT( ShowCurThPos() ) );
  EncMainTh->GetValue();
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
