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

  QVector<PMLine *> pmls;
  QFrame *mainFrame;
    
 public:
  PMConditions();

  QWidget *getWidget( void ) { return mainFrame; };

  void setMotors( QVector<AUnit*> *ams );

 public slots:
   void Initialize( void );
   
   void recHighS( int s );
   void recMiddleS( int s );
   void recLowS( int s );

   void newOrigin( void );
   void newHSpeed( void );
   void newMSpeed( void );
   void newLSpeed( void );
};


#endif
