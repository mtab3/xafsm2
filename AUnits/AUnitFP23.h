
#ifndef AUNITFP23_H
#define AUNITFP23_H

#include "ASensor.h"

class AUnitFP23 : public ASensor
{
  Q_OBJECT

 public:
  AUnitFP23( void );
  void init0( void );
//  bool GetValue( void );

 private slots:
   void OnReportValue( SMsg msg );

 signals:
   //  void NewFP23Temperature( QString val );  // --> NewValue( QString )

};

#endif
