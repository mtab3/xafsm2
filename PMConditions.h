#ifndef PMCONDITIONS_H
#define PMCONDITIONS_H

#include <QObject>
#include <QFrame>
#include <QLayout>
#include <QVector>

#include "PMLine.h"
#include "AUnit.h"

class PMConditions : public QObject
{
  Q_OBJECT

  QVector<AUnit *> *aMs;
  QVector<PMLine *> pmls;
  QFrame *mainFrame;
    
 public:
  PMConditions();

  QWidget *getWidget( void ) { return mainFrame; };

  void setMotors( QVector<AUnit*> *ams );

 public slots:
   void newOrigin( void );
   void newHSpeed( void );
   void newMSpeed( void );
   void newLSpeed( void );
};


#endif
