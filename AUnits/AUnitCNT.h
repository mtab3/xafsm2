
#ifndef AUNITCNT_H
#define AUNITCNT_H

class AUnitCNT : public ASensor
{
  Q_OBJECT

 public:
  AUnitCNT( void ) {};
  virtual void init00( Stars *s );
};

class AUnitCNT2 : public AUnitCNT
{
  Q_OBJECT

 public:
  AUnitCNT2( void ) {};

  bool isAutoRangeAvailable( void ) { return true; }
};

#endif
