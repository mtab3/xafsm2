#include <QGridLayout>
#include <QLabel>

#include "PMConditions.h"

PMConditions::PMConditions( void )
{
  mainFrame = new QFrame;
  mainFrame->setStyleSheet( "background-color: rgb(255, 240, 220);" );
  mainFrame->setFrameShape( QFrame::StyledPanel );
}

void PMConditions::setMotors( QVector<AUnit *> *ams )
{
  aMs = ams;
  int row = 0;
  AUnit *am;

  QGridLayout *L = new QGridLayout;
  mainFrame->setLayout( L );
  
  QLabel *LOrigin = new QLabel;
  QLabel *LHSpeed = new QLabel;
  QLabel *LMSpeed = new QLabel;
  QLabel *LLSpeed = new QLabel;
  int c = 2;
  L->addWidget( LOrigin, row, c++ );
  L->addWidget( LHSpeed, row, c++ );
  L->addWidget( LMSpeed, row, c++ );
  L->addWidget( LLSpeed, row, c++ );
  LOrigin->setText( tr( "Origin(pulse)" ) );
  LHSpeed->setText( tr( "H(pps)" ) );
  LMSpeed->setText( tr( "M(pps)" ) );
  LLSpeed->setText( tr( "L(pps)" ) );
  row++;

  for ( int i = 0; i < ams->count(); i++ ) {
    am = ams->at(i);
    if ( am->getType() == "PM" ) {
      PMLine *p = new PMLine;
      pmls << p;
      p->am = am;

      int c = 0;
      L->addWidget( p->PMNo,    row, c++ );
      L->addWidget( p->PMName,  row, c++ );
      L->addWidget( p->Origin,  row, c++ );
      L->addWidget( p->HSpeed,  row, c++ );
      L->addWidget( p->MSpeed,  row, c++ );
      L->addWidget( p->LSpeed,  row, c++ );
      row++;

      p->PMNo->setText( am->getUid() );
      p->PMName->setText( am->getName() );
      p->Origin->setText( QString::number( am->getCenter() ) );
      p->HSpeed->setText( QString::number( am->getHighS() ) );
      p->MSpeed->setText( QString::number( am->getMiddleS() ) );
      p->LSpeed->setText( QString::number( am->getLowS() ) );

      connect( p->Origin, SIGNAL( editingFinished() ), this, SLOT( newOrigin() ) );
      connect( p->HSpeed, SIGNAL( editingFinished() ), this, SLOT( newHSpeed() ) );
      connect( p->MSpeed, SIGNAL( editingFinished() ), this, SLOT( newMSpeed() ) );
      connect( p->LSpeed, SIGNAL( editingFinished() ), this, SLOT( newLSpeed() ) );
    }
  }
}

void PMConditions::newOrigin( void )
{
  for ( int i = 0; i < pmls.count(); i++ ) {
    if ( sender() == pmls[i]->Origin ) {
      qDebug() << "New Origin " << pmls[i]->am->getName();
    }
  }
}

void PMConditions::newHSpeed( void ) {}
void PMConditions::newMSpeed( void ) {}
void PMConditions::newLSpeed( void ) {}

