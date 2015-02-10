
#ifndef AUNITMSTAB_H
#define AUNITMSTAB_H

#include "AMotor.h"

class AUnitMStab : public AMotor
{
  Q_OBJECT

 public:
  AUnitMStab( void );

  void CloseShutter( bool close );
};

#endif
