#ifndef AUNITPM_H
#define AUNITPM_H

class AUnitPM : public AMotor
{
  Q_OBJECT

  int AccRate;   // 加減速レート ( AccRage ms/1000pps )
  int AccRateNo; // 対応する加減速レートの PM16C のテーブル番号

 public:
  AUnitPM( void );

  int accRate( void ) { return AccRate; };    // 加減速レート
  void setAccRate( int r ) { AccRate = r; };
  int accRateNo( void ) { return AccRateNo; }; // 加減速レートのテーブル番号
  void setAccRateNo( int n ) { AccRateNo = n; };
};

#endif
