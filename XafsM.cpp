#include <QtGlobal>
#include <QSplashScreen>
#include <QStyleFactory>
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QDebug>

#include <stdio.h>

#include "XafsM.h"
#include "MainWindow.h"
#include "Atoms.h"

QSplashScreen *ShowSplashScreen( int m, int d );

enum LANG { English, Japanese, LANGS };
QString DefFileName;
bool newFluoMode;
int qMainVer, qSubVer, qSubSubVer;

int main( int argc, char *argv[] )
{
  int month = QDate::currentDate().month();
  int day = QDate::currentDate().day();

  printf( "starting\n" );
  
  getQVersion();
  if ( qMainVer >= 5 )
    QApplication::setStyle( "Fusion" );
  else if ( qMainVer >= 4 ) {
    QApplication::setStyle( "Cleanlooks" );
  }
  
  LANG Lang = Japanese;
  DefFileName = "XAFSM.def";
  newFluoMode = false;

  QString myname = QString( argv[0] )
    .section( "/", -1 )  // "/" を区切りに切り出したトークンの右から数えて1つめ
    .section( "\\", -1 ) // "\\" (エン記号)を区切りに、同上
                         // この 2つで、unix 系でも msdos 系でもファイル名が拾えるはず。
    .section( ".", 0, 0 );
    // 残ったファイル名に対して "." を区切りに切り出した最初のトークン

  for ( int i = 1; i < argc; i++ ) {
    if ( QString( argv[i] ) == "-nF" ) { newFluoMode = true; };
    if ( QString( argv[i] ) == "-j" ) { Lang = Japanese; };
    if ( QString( argv[i] ) == "-e" ) { Lang = English; };
    if ( QString( argv[i] ) == "-d" ) {
      if ( i+1 < argc ) {
	DefFileName = QString( argv[i+1] );
      }
    }
    if ( QString( argv[i] ) == "-m" ) {
      if ( i+1 < argc ) {
	QString MD = QString( argv[i+1] );
	month = (int)(MD.toInt() / 100);
	day = MD.toInt() - month * 100;
	qDebug() << "md " << month << day;
      }
    }
  }

  if ( QString( argv[0] ).contains( "XafsM2e" ) == true ) {
    Lang = English;
  }

  QApplication app( argc, argv );

  QSplashScreen *splash = ShowSplashScreen( month, day );
  
  QTranslator appTr;
  switch( (int)Lang ) {
  case English:
    break;
  case Japanese:
    appTr.load( ":XafsMsg_ja_JP", qApp->applicationDirPath() );
    app.installTranslator( &appTr );
    break;
  }
  //QTextCodec::setCodecForTr( QTextCodec::codecForName( "Shift-JIS" ) );
  // tr() マクロ中の文字列のコーディング指定
  //QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "Shift-JIS" ) );
  // const char * "" を暗黙に QString に変換する時のコーディング指定
  QTextCodec::setCodecForLocale( QTextCodec::codecForName( "Shift-JIS" ) );

  /****** global initialize ******/
  global_init();
  /*******************************/
  
  ReadVicF();

  MainWindow *mw = new MainWindow( myname );
  mw->show();
  mw->InitSize();

  splash->finish( mw );
  delete splash;
  return app.exec();
}

void getQVersion( void )
{
  qMainVer = qSubVer = qSubSubVer = 0;
  QString ver = QString( qVersion() );
  QStringList vers = ver.split( "." );
  if ( vers.count() >= 1 )
    qMainVer = vers[0].toInt();
  if ( vers.count() >= 2 )
    qSubVer = vers[1].toInt();
  if ( vers.count() >= 3 )
    qSubSubVer = vers[2].toInt();
}

QSplashScreen *ShowSplashScreen( int m, int d )
{
  // Show Splash Screen depending on month of a year
  QPixmap SSPixmap; //Insert your splash page image here
  SSPixmap.load( ":XafsM2.png" );
  switch( m ) {
  case 1:
    if ( d < 16 )
      SSPixmap.load( ":Monthly/Month01.gif" );
    break;
  case 2:
    if ( d < 4 )
      SSPixmap.load( ":Monthly/Month02_03.gif" );
    break;
  case 3:
    if ( d < 4 )
      SSPixmap.load( ":Monthly/Month03_03.gif" );
    break;
  case 4:
    if ( d < 16 )
      SSPixmap.load( ":Monthly/Month04.png" );
    break;
  case 5:
    if ( d < 6 )
      SSPixmap.load( ":Monthly/Month05.png" );
    break;
  default: break;
  }

  QSplashScreen *splash = new QSplashScreen( SSPixmap );
  splash->show();
  // This line represents the alignment of text, color and position
  splash->showMessage( QObject::tr( "Starting XafsM2..." ),
		      Qt::AlignHCenter | Qt::AlignVCenter, QColor( 0, 0, 255 ) );
  // This is used to accept a click on the screen so that user can cancel the screen
  qApp->processEvents();

  return splash;
}

