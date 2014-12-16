#ifndef PMLINE_H
#define PMLINE_H

#include<QFrame>

#include"ui_PMLine.h"

class PMLine : public QFrame, private Ui::PMLine
{
  Q_OBJECT

 public:
  PMLine( void );

  void setPMNo( QString pmNo ) { PMNo->setText( pmNo ); };
  void setPMName( QString pmName ) { PMName->setText( pmName ); };
};


#endif
