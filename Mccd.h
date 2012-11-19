
#ifndef MCCD_H
#define MCCD_H

//enum MCC { Si111, Si333, Ge111, Ge333, OTHERMCCS };
class MCCD 
{
public:
  MCCD( QString cname, QString latticec ) { MCName = cname; d = latticec.toDouble(); };
  void setMCName( QString name ) { MCName = name; };
  void setD( double D ) { d = D; };
  QString getMCName( void ) { return MCName; };
  double getD( void ) { return d; };

private:
  QString MCName;
  double d;
};

#endif
