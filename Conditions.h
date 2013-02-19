#ifndef CONDITIONS_H
#define CONDITIONS_H

#include "ui_Conditions.h"

class Conditions : public QFrame, private Ui::Conditions
{
  Q_OBJECT

public:
  Conditions();

  void setEncAsTh( bool encAsTh );
  void setAddInfos( bool addInfos );
  bool isEncAsTh( void );
  bool isAddInfos( void );
};

#endif

