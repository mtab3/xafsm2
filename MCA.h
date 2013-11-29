#ifndef MCA_H
#define MCA_H

#include <QtGlobal>

#define SAVEMCASize  ( 2048 )
#define SAVEMCACh    ( 19 )

struct aCh {
  double E[ SAVEMCASize ];
  quint32 cnt[ SAVEMCASize ];
};

struct aMCASet {
  bool valid;
  aCh Ch[ SAVEMCACh ];
  MCAHead Heads[ SAVEMCACh ];

  double RINGCurrent;
  double I0;

  aMCASet() { valid = false; };
  void setValid( bool f ) { valid = f; };
  bool isValid( void ) { return valid; };
};

struct aMCAMap {
  int iX, iY;
  aMCASet *Points;

  aMCAMap() { iX = iY = 0; Points = NULL; };
  ~aMCAMap() { if ( Points != NULL ) delete [] Points; };
  void New( int ix, int iy )
  {
    if ( Points != NULL ) {
      delete [] Points;
    }
    iX = ix;
    iY = iy;
    Points = new aMCASet[ iX * iY ];
    qDebug() << "IX IY " << iX << iY;
  };
  aMCASet *aPoint( int ix, int iy )
  {
    if (( ix < iX )&&( iy < iY ))
      return &(Points[ iy * iX + ix ]);
    return NULL;
  };
};

#endif
