#ifndef S2DB_H
#define S2DB_H

#include <QFrame>

#include "ui_S2DB.h"

class S2DB : public QFrame, private Ui::S2DB
{
  Q_OBJECT

 public:
  S2DB( QWidget *p );

  S2DView *getView( void ) { return S2DV; };
 private:


signals:
  void print( void );
  void popup( void );
};

#endif
