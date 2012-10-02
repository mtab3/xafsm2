#include <QtGui>

#include "XafsM.h"
#include "MainWindow.h"
#include "SelMC.h"
#include "Stars.h"

MOTORD Motors[ MOTORS ] = {
  { MONOCHRO1, "Monochro 1", "monochro1", "", 0, 0,  0, 0, 0, 0 },
                                   // 注: １番のモーターは monochro と決め打ちにしている
  { MOTOR1, "Motor 1", "motor1", "", 100, 100,  100, 1, 0, 1000 },
  { MOTOR2, "Motor 2", "motor2", "", 100, 100,  100, 1, 0, 1000 },
  { MOTOR3, "Motor 3", "motor3", "", 100, 100,  100, 1, 0, 1000 },
  { MOTOR4, "Motor 4", "motor4", "", 100, 100,  100, 1, 0, 1000 },
};

MainWindow::MainWindow() : QMainWindow()
{
  setupUi( this );

  starsSV = new StarsSV;
  selMC = new SelMC;
  ReadCfgFile();

  setupLogArea();     // ログに対する書き出しがある可能性があるので最初にイニシャライズ

  sks = new Stars;
  sks->ReadStarsKeys( MYNAME ); // Stars とのコネクション確立の準備
  setupCommonArea();
  setupSetupArea();
  setupMeasArea();
  setupView();

  QString msg = "XafsMsg_" + QLocale::system().name();
  NewLogMsg( msg + "\n" );
  NewLogMsg( QString( "Mono: at %1 keV\n" ).arg( GetCurPosKeV() ) );

  NewLogMsg( QString( tr( "Mono: %1 (%2 A)\n" ) )
	     .arg( mccd[ selMC->MC() ].MCName ).arg( mccd[ selMC->MC() ].d ) );

  connect( sks, SIGNAL( aMessage( QString, int ) ),
	   this, SLOT( ShowMessageOnSBar( QString, int ) ) );
  connect( action_Quit, SIGNAL( triggered() ), qApp, SLOT( closeAllWindows() ) );
  connect( action_SelMC, SIGNAL( triggered() ), selMC, SLOT( show() ) );
  connect( selMC, SIGNAL( NewLogMsg( QString ) ),
	   this, SLOT( NewLogMsg( QString ) ) );
  connect( selMC, SIGNAL( NewLatticeConstant( double ) ),
	   this, SLOT( SetNewLatticeConstant( double ) ) );
  connect( action_SetSSV, SIGNAL( triggered() ), starsSV, SLOT( show() ) );
  connect( starsSV, SIGNAL( SSVNewAddress( const QString & ) ),
	   sks, SLOT( SetNewSVAddress( const QString & ) ) );
  connect( starsSV, SIGNAL( SSVNewPort( const QString & ) ),
	   sks, SLOT( SetNewSVPort( const QString & ) ) );
  connect( sks, SIGNAL( RecordSSVHistoryA( const QString & ) ),
	   starsSV, SLOT( RecordSSVHistoryA( const QString & ) ) );
  connect( sks, SIGNAL( RecordSSVHistoryP( const QString & ) ),
	   starsSV, SLOT( RecordSSVHistoryP( const QString & ) ) );
}

void MainWindow::ShowMessageOnSBar( QString msg, int time )
{
  statusbar->showMessage( msg, time );
}

void MainWindow::ReadCfgFile( void )
{
}

