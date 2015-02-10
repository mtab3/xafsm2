#ifndef AUNITPM_H
#define AUNITPM_H

class AUnitPM : public AMotor
{
  Q_OBJECT

  double Center;        // Center position in puls : only for PM

 public:
  AUnitPM( void );

  double getCenter( void ) { return Center; };
  void setCenter( QString center ) { Center = center.toDouble(); };
  void setCenter( double center ) { Center = center; };
  double getCeter( void ) { return Center; };

};

#endif
