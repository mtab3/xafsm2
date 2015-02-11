
#ifndef AUNITOTC_H
#define AUNITOTC_H

class AUnitOTC : public ASensor
{
  Q_OBJECT

 public:
  AUnitOTC( void );

  virtual void init00( Stars *s );
};

class AUnitOTC2 : public AUnitOTC
{
  Q_OBJECT

 public:
  AUnitOTC2( void );

  bool isAutoRangeAvailable( void ) { return true; };
};

#endif
