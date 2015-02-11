
#ifndef AUNITPAM_H
#define AUNITPAM_H

class AUnitPAM : public ASensor
{
  Q_OBJECT

 public:
  AUnitPAM( void ) {};

  virtual void init00( Stars *s );

  bool isAutoRangeAvailable( void ) { return true; };
};

class AUnitPAM2 : public AUnitPAM
{
  Q_OBJECT

 public:
  AUnitPAM2( void ) {};

};

#endif
