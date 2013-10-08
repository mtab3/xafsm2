#include "DarkTable.h"

DarkTable::DarkTable( QWidget *p ) : QFrame( p )
{
  setupUi( this );

  connect( closeB, SIGNAL( clicked() ), this, SLOT( hide() ),
	   Qt::UniqueConnection );
  resize( 430, 300 );
}

void DarkTable::clearItems( void )
{
  for ( int i = 0; i < DTable->columnCount(); i++ ) {
    for ( int j = 0; j < DTable->rowCount(); j++ ) {
      if ( DTable->item( j, i ) != NULL ) {
	delete DTable->item( j, i );
      }
    }
  }
  DTable->setColumnWidth( 0, 250 );
  DTable->setColumnWidth( 1, 100 );
}
