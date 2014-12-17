#include "PMLine.h"

PMLine::PMLine( void )
{
  PMNo = new QLabel;
  PMName = new QLabel;
  Origin = new QLineEdit;
  HSpeed = new QLineEdit;
  MSpeed = new QLineEdit;
  LSpeed = new QLineEdit;

  PMNo->setFrameShape( QFrame::StyledPanel );
  PMNo->setStyleSheet( "background-color: rgb(255, 255, 221);" );
  PMName->setFrameShape( QFrame::StyledPanel );
  PMName->setStyleSheet( "background-color: rgb(255, 255, 221);" );

  Origin->setStyleSheet( "background-color: rgb(255, 255, 255);" );
  HSpeed->setStyleSheet( "background-color: rgb(255, 255, 255);" );
  MSpeed->setStyleSheet( "background-color: rgb(255, 255, 255);" );
  LSpeed->setStyleSheet( "background-color: rgb(255, 255, 255);" );

  Origin->setAlignment( Qt::AlignRight );
  HSpeed->setAlignment( Qt::AlignRight );
  MSpeed->setAlignment( Qt::AlignRight );
  LSpeed->setAlignment( Qt::AlignRight );
}
