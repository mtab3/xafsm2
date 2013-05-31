#ifndef RELABS_H
#define RELABS_H

#include "ui_RelAbs.h"
#include "XafsM.h"

class RelAbs : public QFrame, private Ui::RelAbs
{
  Q_OBJECT

 private:
  RELABS Stat;

  void Show( void );

 private slots:
  void ToRel( void );
  void ToAbs( void );

 public:
  RelAbs( QWidget *p = NULL );
  RELABS stat( void ) { return Stat; };


};

#endif
