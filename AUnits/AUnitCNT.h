
#ifndef AUNITCNT_H
#define AUNITCNT_H

#include "ASensor.h"

class AUnitCNT : public ASensor
{
  Q_OBJECT

 public:
  AUnitCNT( void ) {};
  void init0( void );
  virtual void init00( void );

  void AskIsBusy( void );
  virtual void _AskIsBusy( void );

  bool Close( void );
};

class AUnitCNT2 : public AUnitCNT
{
  Q_OBJECT

 public:
  AUnitCNT2( void ) {};

  void init00( void );

  void _AskIsBusy( void ) {};

  bool isAutoRangeAvailable( void ) { return true; }
  bool GetRange( void );

 private slots:
  void ReactGetRange( SMsg msg );

};

#endif
