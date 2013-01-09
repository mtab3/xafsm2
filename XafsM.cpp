#include <QApplication>
#include <QtGui>

#include <stdio.h>

#include "XafsM.h"
#include "MainWindow.h"
#include "Atoms.h"

enum LANG { English, Japanese, LANGS };
QString DefFileName;

int main( int argc, char *argv[] )
{
  LANG Lang = Japanese;
  DefFileName = "XAFSM.def";

  QString myname = QString( argv[0] )
    .section( "/", -1 )  // "/" ����؂�ɐ؂�o�����g�[�N���̉E���琔����1��
    .section( "\\", -1 ) // "\\" (�G���L��)����؂�ɁA����
                         // ���� 2�ŁAunix �n�ł� msdos �n�ł��t�@�C�������E����͂��B
    .section( ".", 0, 0 );
    // �c�����t�@�C�����ɑ΂��� "." ����؂�ɐ؂�o�����ŏ��̃g�[�N��

  for ( int i = 1; i < argc; i++ ) {
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
  // tr() �}�N�����̕�����̃R�[�f�B���O�w��
  QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "Shift-JIS" ) );
  // const char * "" ���Öق� QString �ɕϊ����鎞�̃R�[�f�B���O�w��

  app.setStyle( "plastique" );

  MainWindow *mw = new MainWindow( myname );
  mw->show();

  return app.exec();
}
