#include "TuneTable.h"

TuneTable::TuneTable( QWidget *p ) : QFrame( p )
{
  setupUi( this );

  connect( OkB, SIGNAL( clicked() ), this, SLOT( hide() ) );
  connect( CancelB, SIGNAL( clicked() ), this, SLOT( hide() ) );
  connect( OkB, SIGNAL( clicked() ), this, SIGNAL( Ok() ) );
  resize( 430, 300 );
}

void TuneTable::clearItems( void )
{
  for ( int i = 0; i < TTable->columnCount(); i++ ) {
    for ( int j = 0; j < TTable->rowCount(); j++ ) {
      if ( TTable->item( j, i ) != NULL ) {
	delete TTable->item( j, i );
      }
    }
  }
  TTable->setColumnWidth( 0, 250 );
  TTable->setColumnWidth( 1, 100 );
}
