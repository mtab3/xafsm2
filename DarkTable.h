#ifndef DARKTABLE_H
#define DARKTABLE_H

#include "ui_DarkTable.h"

class DarkTable : public QFrame, private Ui::DarkTable
{
  Q_OBJECT

 private:

 public:
  DarkTable( QWidget *p = NULL );

  void clearItems( void );
  void setRowCol( int r, int c )
  { DTable->setRowCount( r ); DTable->setColumnCount( c ); };
  void setItem( int r, int c, QTableWidgetItem *item )
  { DTable->setItem( r, c, item ); };
  QTableWidgetItem* getItem( int r, int c ) { return DTable->item( r, c ); };
};

#endif
