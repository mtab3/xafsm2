#include <QApplication>
#include <QtGui>

#include <stdio.h>

#include "XafsM.h"
#include "MainWindow.h"
#include "Atoms.h"

enum LANG { English, Japanese, LANGS };

int main( int argc, char *argv[] )
{
  LANG Lang = English;

  if ( QString( argv[0] ).contains( "XafsMj" ) == true ) {
    Lang = Japanese;
  }

  QApplication app( argc, argv );

  ReadVicF();

  QTranslator appTr;
  switch( (int)Lang ) {
  case English:
    break;
  case Japanese:
    appTr.load( ":XafsMsg_ja_JP", qApp->applicationDirPath() );
    app.installTranslator( &appTr );
    break;
  }

  QTextCodec::setCodecForTr( QTextCodec::codecForName( "Shift-JIS" ) );
  // tr() マクロ中の文字列のコーディング指定
  QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "Shift-JIS" ) );
  // const char * "" を暗黙に QString に変換する時のコーディング指定

  app.setStyle( "plastique" );

  MainWindow *mw = new MainWindow;
  mw->show();

  return app.exec();
}
