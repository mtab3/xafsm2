
#ifndef AUNITPZ_H
#define AUNITPZ_H

#include "AMotor.h"

class AUnitPZ : public AMotor
{
  Q_OBJECT

  double MaxV, MinV;

 public:
  AUnitPZ( void );
  void init0( void );

  void setMaxV( QString maxv ) { MaxV = maxv.toDouble(); };
  void setMinV( QString minv ) { MinV = minv.toDouble(); };
};

#endif
