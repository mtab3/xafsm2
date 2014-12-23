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

enum LANG { English, Japanese, LANGS };
QString DefFileName;
bool newFluoMode;
int qMainVer, qSubVer, qSubSubVer;

int main( int argc, char *argv[] )
{
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
  }

  if ( QString( argv[0] ).contains( "XafsM2e" ) == true ) {
    Lang = English;
  }

  QApplication app( argc, argv );

  // Show Splash Screen depending on month of a year
  QPixmap SSPixmap; //Insert your splash page image here
  switch( QDate::currentDate().month() ) {
  case 1:  SSPixmap.load( ":Month01.gif" ); break;
  default: SSPixmap.load( ":XafsM2.png" ); break;
  }
  QSplashScreen splash( SSPixmap );
  splash.show();
  // This line represents the alignment of text, color and position
  splash.showMessage( QObject::tr( "Starting XafsM2..." ),
		      Qt::AlignHCenter | Qt::AlignVCenter, QColor( 0, 0, 255 ) );
  // This is used to accept a click on the screen so that user can cancel the screen
  qApp->processEvents();
  // Show Splash Screen depending on month of a year

  
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

  ReadVicF();

  MainWindow *mw = new MainWindow( myname );
  mw->show();
  mw->InitSize();

  splash.finish( mw );
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
