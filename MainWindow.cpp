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

MainWindow::MainWindow() : QMainWindow()
{
  setupUi( this );

  starsSV = new StarsSV;
  selMC = new SelMC;

  setupLogArea();     // ログに対する書き出しがある可能性があるので最初にイニシャライズ

  s = new Stars;      // モータ類のイニシャライズの前に Stars の準備はしておく
  s->ReadStarsKeys( "XafsM2", "XafsM2" ); // Stars とのコネクション確立の準備

  ReadDef( "XAFSM.def" );
  InitAndIdentifyMotors();
  InitAndIdentifySensors();

  setupCommonArea();
  setupSetupArea();
  setupMeasArea();
  setupView();

  QString msg = "XafsMsg_" + QLocale::system().name();
  NewLogMsg( msg + "\n" );

  NewLogMsg( QString( tr( "Mono: %1 (%2 A)\n" ) )
	     .arg( mccd[ selMC->MC() ].MCName ).arg( mccd[ selMC->MC() ].d ) );

  connect( s, SIGNAL( AskShowStat( QString, int ) ),
	   this, SLOT( ShowMessageOnSBar( QString, int ) ) );
  connect( action_Quit, SIGNAL( triggered() ), qApp, SLOT( closeAllWindows() ) );
  connect( action_SelMC, SIGNAL( triggered() ), selMC, SLOT( show() ) );
  connect( selMC, SIGNAL( NewLogMsg( QString ) ),
	   this, SLOT( NewLogMsg( QString ) ) );
  connect( selMC, SIGNAL( NewLatticeConstant( double ) ),
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
}

void MainWindow::ShowMessageOnSBar( QString msg, int time )
{
  statusbar->showMessage( msg, time );
}

void MainWindow::InitAndIdentifyMotors( void )
{
  AUnit *am;
  for ( int i = 0; i < AMotors.count(); i++ ) {
    am = AMotors.value(i);
    am->Initialize( s );
    am->setUniqID( QString::number( i ) );
    if ( am->getID() == "THETA" ) {
      MMainTh = am;
    }
  }
  connect( s, SIGNAL( AnsGetValue( SMsg ) ), this, SLOT( ShowCurThPos( SMsg ) ) );
  connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( ShowCurThPos( SMsg ) ) );
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
  }
}

void MainWindow::ShowCurThPos( SMsg msg )
{
  QString buf;
  double deg;

  if ( ( msg.From() == MMainTh->getDevCh() )
       && ( ( msg.Msgt() == GETVALUE ) || ( msg.Msgt() == EvCHANGEDVALUE ) ) ) {
    deg = ( msg.Val().toDouble() - MMainTh->getCenter() ) * MMainTh->getUPP();
    buf.sprintf( UnitName[KEV].form, deg );
    CurrentAngle->setText( buf );
    buf.sprintf( UnitName[DEG].form, CurPosKeV = deg2keV( deg ) );
    CurrentEnergy->setText( buf );

    NewLogMsg( tr( "Current Position [%1] keV" ).arg( buf ) );
  }
}
