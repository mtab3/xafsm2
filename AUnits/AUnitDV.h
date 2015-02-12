
#ifndef AUNITDV_H
#define AUNITDV_H

#include "ASensor.h"

class AUnitDV : public ASensor
{
  Q_OBJECT

  int points;           // Measured Data Points : 34410
  bool HasMaxIntTime;
  double MaxIntTime;    // Maximum integration time
  
 public:
  AUnitDV( void ) {};
  void init0( void );
  virtual void init00( void );

  void setMaxIntTime( double t ) { MaxIntTime = t; HasMaxIntTime = true; };

  bool QStart( void );
  bool QRead( void );
  bool QEnd( void );
};

class AUnitDV2 : public AUnitDV
{
  Q_OBJECT

 public:
  AUnitDV2( void ) {};
  void init00( void );

  void AskIsBusy( void );
};

#endif
