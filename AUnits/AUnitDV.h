
#ifndef AUNITDV_H
#define AUNITDV_H

class AUnitDV : public ASensor
{
  Q_OBJECT

  int points;           // Measured Data Points : 34410
  bool HasMaxIntTime;
  double MaxIntTime;    // Maximum integration time
  
 public:
  AUnitDV( void ) {};
  virtual void init00( Stars *s );

  void setMaxIntTime( double t ) { MaxIntTime = t; HasMaxIntTime = true; };
};

class AUnitDV2 : public AUnitDV
{
  Q_OBJECT

 public:
  AUnitDV2( void ) {};
};

#endif
