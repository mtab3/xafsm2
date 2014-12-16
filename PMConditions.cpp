#include <QGridLayout>
#include <QLabel>

#include "PMConditions.h"
#include "PMLine.h"

PMConditions::PMConditions( void )
{
  mainFrame = new QFrame;
  mainFrame->setStyleSheet( "background-color: rgb(255, 240, 220);" );
  mainFrame->setFrameShape( QFrame::StyledPanel );
}

void PMConditions::setMotors( QVector<AUnit *> *ams )
{
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
      QLabel *PMNo = new QLabel;
      QLabel *PMName = new QLabel;
      QLineEdit *Origin = new QLineEdit;
      QLineEdit *HSpeed = new QLineEdit;
      QLineEdit *MSpeed = new QLineEdit;
      QLineEdit *LSpeed = new QLineEdit;

      int c = 0;
      L->addWidget( PMNo,    row, c++ );
      L->addWidget( PMName,  row, c++ );
      L->addWidget( Origin,  row, c++ );
      L->addWidget( HSpeed,  row, c++ );
      L->addWidget( MSpeed,  row, c++ );
      L->addWidget( LSpeed,  row, c++ );
      row++;

      PMNo->setText( am->getUid() );
      PMName->setText( am->getName() );
    }
  }
}
