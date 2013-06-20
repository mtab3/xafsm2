#ifndef UNITS_H
#define UNITS_H

#include <math.h>

#include <QObject>
#include <QString>

#define PI ( 3.1415926535 )
#define CC ( 2.99792458e8 )
#define hb ( 1.05457266e-34 )
#define EE ( 1.60217733e-19 )
#define E2A ( 2 * PI * hb * CC / EE * 1e10 )
#define KE2A ( 2 * PI * hb * CC / EE / 1000. * 1e10 )

struct UNITNAME {
  QString name;
  const char *form;
};
enum UNIT { EV, KEV, ANGS, DEG, UNITS };
const UNITNAME UnitName[ UNITS ] = {
  { "eV",  (const char*)"% 8.2f", },
  { "keV", (const char*)"% 8.5f", },
  { "A",   (const char*)"% 8.5f", },
  { "deg", (const char*)"% 8.5f", },
};

class Units : public QObject
{
  Q_OBJECT

  double d;

 public:
  Units( void ) { d = 3.13553; };

  double eV2deg( double eV ) {
    double tmp = eV2a( eV )/( 2.* d );
    return ( fabs( tmp ) > 1 ) ? -1 : asin( tmp )/(PI)*180.;
  }
  double keV2deg( double eV ) {
    double tmp = keV2a( eV )/( 2.* d );
    return ( fabs( tmp ) > 1 ) ? -1 : asin( tmp )/(PI)*180.;
  }
  double deg2keV( double deg ) { return a2keV( sin( deg / 180. * PI ) * 2.* d ); }
  double deg2eV( double deg ) { return a2eV( sin( deg / 180. * PI ) * 2.* d ); }
  double eV2a( double eV ) { return E2A / eV; }
  double keV2a( double keV ) { return KE2A / keV; }
  double a2eV( double a ) { return E2A / a; }
  double a2keV( double a ) { return KE2A / a; }
  double keV2any( UNIT i, double keV ) {
    switch( (int)i ) {
    case EV: return keV * 1000;
    case KEV: return keV;
    case DEG: return keV2deg( keV );
    case ANGS: return keV2a( keV );
    }
    return 0;
  }
  double any2keV( UNIT i, double any ) {
    switch( (int)i ) {
    case EV: return any / 1000.;
    case KEV: return any;
    case DEG: return deg2keV( any );
    case ANGS: return a2keV( any );
    }
    return 0;
  }

  public slots:
  void setD( double D ) { d = D; };

};


#endif
