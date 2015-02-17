#ifndef PMLINE_H
#define PMLINE_H

#include <QLabel>
#include <QLineEdit>

#include "AUnits/AMotor.h"

class PMLine
{
 public:
  AMotor *am;
  
  QLabel *PMNo;
  QLabel *PMName;
  QLineEdit *Origin;
  QLineEdit *HSpeed;
  QLineEdit *MSpeed;
  QLineEdit *LSpeed;
  
 public:
  PMLine( void );
};


#endif
