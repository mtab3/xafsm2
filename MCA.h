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
  QString date;

  aCh Ch[ SAVEMCACh ];
  MCAHead Heads[ SAVEMCACh ];

  QString ROIStart[ SAVEMCACh ];
  QString ROIEnd[ SAVEMCACh ];

  QStringList Elms;

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
    try {
      Points = new aMCASet[ iX * iY ];
    }
    catch(...) {
      Points = NULL;
    }
  };

  aMCASet *aPoint( int ix, int iy )
  {
    if ( Points == NULL )
      return NULL;
    if (( ix < iX )&&( iy < iY ))
      return &(Points[ iy * iX + ix ]);
    return NULL;
  };
};

#endif
