#ifndef PMCONDITIONS_H
#define PMCONDITIONS_H

#include <QObject>
#include <QFrame>
#include <QLayout>
#include <QVector>

#include "AUnit.h"

class PMConditions : public QObject
{
  Q_OBJECT

  QFrame *mainFrame;
    
 public:
  PMConditions();

  QWidget *getWidget( void ) { return mainFrame; };

  void setMotors( QVector<AUnit*> *ams );
};


#endif
