#ifndef AUNITXMAP2_H
#define AUNITXMAP2_H

#include "ASensor.h"
#include "AUnitXMAP.h"   // AUnitXMAP �� AUnitXMAP2 �ϷѾ��ط��ˤʤ�

class AUnitXMAP2 : public ASensor
{
  Q_OBJECT

 public:
  AUnitXMAP2( void ) {};
  void init0( void );

  double stat( STATELM i );
  bool SetRealTime( double val );
};

#endif


