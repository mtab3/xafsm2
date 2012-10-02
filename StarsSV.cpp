#include <QtGui>

#include "StarsSV.h"

StarsSV::StarsSV() : QDialog()
{
  setupUi( this );

  StarsSVA->addItem( "localhost" );
  StarsSVP->addItem( "6057" );

  ReadHistory();

  connect( StarsSVA, SIGNAL( currentIndexChanged( const QString & ) ),
	   this, SLOT( SSVAnewIndex( const QString & ) ) );
  connect( StarsSVP, SIGNAL( currentIndexChanged( const QString & ) ),
	   this, SLOT( SSVPnewIndex( const QString & ) ) );
}

void StarsSV::RecordSSVHistoryA( const QString &item )
{
  QFile f( SSVHISTORY );
  if ( !f.open( QIODevice::Append | QIODevice::Text ) )
    return;
  QTextStream out( &f );
  out << "A: " << item << endl;
  f.close();
}

void StarsSV::RecordSSVHistoryP( const QString &item )
{
  QFile f( SSVHISTORY );
  if ( !f.open( QIODevice::Append | QIODevice::Text ) )
    return;
  QTextStream out( &f );
  out << "P: " << item << endl;
  f.close();
}

void StarsSV::ReadHistory( void )
{  
  QStringList rvs;
  QString item;

  QFile f( SSVHISTORY );
  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return;

  QTextStream in( &f );
  while( !in.atEnd() ) {
    item = in.readLine();
    if ( !item.isEmpty() ) {
      rvs = item.simplified().split( QChar( ' ' ) );
      if ( rvs[0] == "A:" ) {
	if ( StarsSVA->findText( rvs[1] ) == -1 )
	  StarsSVA->addItem( rvs[1] );
      }
      if ( rvs[0] == "P:" ) {
	if ( StarsSVP->findText( rvs[1] ) == -1 )
	  StarsSVP->addItem( rvs[1] );
      }
    }
  }

  f.close();
}

void StarsSV::SSVAnewIndex( const QString &item )
{
  emit SSVNewAddress( item );
}

void StarsSV::SSVPnewIndex( const QString &item )
{
  emit SSVNewPort( item );
}
