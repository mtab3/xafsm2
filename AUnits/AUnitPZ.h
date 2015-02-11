
#ifndef AUNITPZ_H
#define AUNITPZ_H

class AUnitPZ : public AMotor
{
  Q_OBJECT

  double MaxV, MinV;

 public:
  AUnitPZ( void );

  void setMaxV( QString maxv ) { MaxV = maxv.toDouble(); };
  void setMinV( QString minv ) { MinV = minv.toDouble(); };
};

#endif
