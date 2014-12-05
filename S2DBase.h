#ifndef S2DBASE_H
#define S2DBASE_H

#include <QFrame>

#include "ui_S2DBase.h"

class S2DBase : public QFrame, private Ui::S2DBase
{
  Q_OBJECT

 public:
  S2DBase( QWidget *p );

  S2DView *getView( void ) { return S2DV; };
 private:
};

#endif
