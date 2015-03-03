#ifndef AUNITSFLUO2_H
#define AUNITSFLUO2_H

#include "ASensor.h"
#include "AUnitSFluo.h"   // AUnitXMAP と AUnitXMAP2 は継承関係にない

class AUnitSFluo2 : public ASensor
{
  Q_OBJECT

 public:
  AUnitSFluo2( void ) {};
  void init0( void );

  double stat( STATELM i );
  bool SetRealTime( double val );
  double SetTime( double t );

 private slots:
  void getNewValue( QString v );   // only for SSD childlen
  void getNewDark( double d );     // only for SSD childlen

};

#endif


