#ifndef PMLINE_H
#define PMLINE_H

#include<QFrame>

#include"ui_PMLine.h"

class PMLine : public QFrame, private Ui::PMLine
{
  Q_OBJECT

 public:
  PMLine( void );
};


#endif
