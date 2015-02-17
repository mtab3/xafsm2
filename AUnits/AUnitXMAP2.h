#ifndef AUNITXMAP2_H
#define AUNITXMAP2_H

#include "ASensor.h"
#include "AUnitXMAP.h"   // AUnitXMAP $B$H(B AUnitXMAP2 $B$O7Q>54X78$K$J$$(B

class AUnitXMAP2 : public ASensor
{
  Q_OBJECT

 public:
  AUnitXMAP2( void ) {};
  void init0( void );

  double stat( STATELM i );
  bool SetRealTime( double val );
  double SetTime( double t );

 private slots:
  void getNewValue( QString v );   // only for SSD childlen
  void getNewDark( double d );     // only for SSD childlen

};

#endif


