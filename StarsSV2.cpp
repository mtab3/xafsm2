#include <QtGui>

#include "StarsSV2.h"

StarsSV2::StarsSV2() : QFrame()
{
  setupUi( this );

  StarsSVA->addItem( "192.168.51.200" );
  StarsSVA->addItem( "133.6.107.214" );
  StarsSVA->addItem( "localhost" );
  StarsSVP->addItem( "6057" );
  StarsSVP->addItem( "6058" );

  connect( StarsSVA, SIGNAL( currentIndexChanged( const QString & ) ),
	   this, SIGNAL( SSVNewAddress( const QString & ) ) );
  connect( StarsSVP, SIGNAL( currentIndexChanged( const QString & ) ),
	   this, SIGNAL( SSVNewPort( const QString & ) ) );
  connect( ReConnect, SIGNAL( clicked() ), this, SIGNAL( AskReConnect() ) );

  ReadHistory();
}

void StarsSV2::RecordSSVHistoryA( const QString &item )
{
  QFile f( SSVHISTORY );
  if ( !f.open( QIODevice::Append | QIODevice::Text ) )
    return;
  QTextStream out( &f );
  out << "A: " << item << endl;
  f.close();
}

void StarsSV2::RecordSSVHistoryP( const QString &item )
{
  QFile f( SSVHISTORY );
  if ( !f.open( QIODevice::Append | QIODevice::Text ) )
    return;
  QTextStream out( &f );
  out << "P: " << item << endl;
  f.close();
}

void StarsSV2::ReadHistory( void )
{  
  QStringList rvs;
  QString item;
  int fc;

  QFile f( SSVHISTORY );

  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return;

  QTextStream in( &f );
  while( !in.atEnd() ) {
    item = in.readLine();
    if ( !item.isEmpty() ) {
      rvs = item.simplified().split( QChar( ' ' ) );
      if ( rvs[0] == "A:" ) {
	if ( ( fc = StarsSVA->findText( rvs[1] ) ) == -1 ) {
	  StarsSVA->addItem( rvs[1] );
	  StarsSVA->setCurrentIndex( StarsSVA->count()-1 );
	} else {
	  StarsSVA->setCurrentIndex( fc );
	}
      }
      if ( rvs[0] == "P:" ) {
	if ( ( fc = StarsSVP->findText( rvs[1] ) ) == -1 ) {
	  StarsSVP->addItem( rvs[1] );
	  StarsSVP->setCurrentIndex( StarsSVP->count()-1 );
	} else {
	  StarsSVP->setCurrentIndex( fc );
	}
      }
    }
  }

  f.close();

  // 記録ファイルは新しい入力があるたびにどんどん書き込まれて行くので
  // 記録ファイルを読み込んだ直後の(同一アイテムを整理してしまった) ComboBox の
  // 中身を書き出すことで整理する。
  if ( !f.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;
  QTextStream out( &f );
  for ( int i = 0; i < StarsSVA->count(); i++ ) {
    out << "A: " << StarsSVA->itemText( i ) << endl;
  }
  out << "A: " << StarsSVA->itemText( StarsSVA->currentIndex() ) << endl;
  // 最後に書いたものが、次回デフォルトに選ばれる
  for ( int i = 0; i < StarsSVP->count(); i++ ) {
    out << "P: " << StarsSVP->itemText( i ) << endl;
  }
  out << "P: " << StarsSVP->itemText( StarsSVP->currentIndex() ) << endl;
  // 最後に書いたものが、次回デフォルトに選ばれる
  f.close();
}
