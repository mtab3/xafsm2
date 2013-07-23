#ifndef TUNETABLE_H
#define TUNETABLE_H

#include "ui_TuneTable.h"

class TuneTable : public QFrame, private Ui::TuneTable
{
  Q_OBJECT

 private:

 public:
  TuneTable( QWidget *p = NULL );

  void clearItems( void );
  void setRowCol( int r, int c )
  { TTable->setRowCount( r ); TTable->setColumnCount( c ); };
  void setItem( int r, int c, QTableWidgetItem *item )
  { TTable->setItem( r, c, item ); };
  QTableWidgetItem* getItem( int r, int c ) { return TTable->item( r, c ); };
  int rowCount( void ) { return TTable->rowCount(); };

 signals:
  void Ok( void );
};

#endif
