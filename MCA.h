#ifndef MCA_H
#define MCA_H

#include <QStringList>

#include "MCAHead.h"

struct aCH
{
  int Length;
  double *E;
  quint32 *cnt;
    
  aCH( void ) {
    Length = 0;
    E = NULL;
    cnt = NULL;
  };
  ~aCH( void ) {
    delete [] E;
    delete [] cnt;
  };
  void setLength( int l )
  {
    Length = l;
    if ( E != NULL ) delete [] E;
    if ( cnt != NULL ) delete [] cnt;
    E = new double [ l ];
    cnt = new quint32 [ l ];
  }
};

struct aMCASet {
  bool valid;

  int Length, CHs;
  
  QString date;

  aCH *Ch;
  MCAHead *Heads;

  QString *ROIStart;
  QString *ROIEnd;

  QStringList Elms;

  double RINGCurrent;
  double I0;

  aMCASet( void )
  {
    valid = false;
    Length = 0;
    CHs = 0;
    Ch = NULL;
    Heads = NULL;
    ROIStart = NULL;
    ROIEnd = NULL;
  };
  ~aMCASet( void )
  {
    if ( Ch != NULL )       delete [] Ch;
    if ( Heads != NULL )    delete [] Heads;
    if ( ROIStart != NULL ) delete [] ROIStart;
    if ( ROIEnd != NULL )   delete [] ROIEnd;
  }
  void setSize( int length, int chs )
  {
    valid = false;
    Length = length;
    CHs = chs;
    if ( Ch != NULL )       delete [] Ch;
    if ( Heads != NULL )    delete [] Heads;
    if ( ROIStart != NULL ) delete [] ROIStart;
    if ( ROIEnd != NULL )   delete [] ROIEnd;
    Ch = new aCH[ CHs ];
    Heads = new MCAHead[ CHs ];
    ROIStart = new QString[ CHs ];
    ROIEnd = new QString[ CHs ];
    for ( int i = 0; i < CHs; i++ ) {
      Ch[i].setLength( Length );
    }
  }
  
  void setValid( bool f ) { valid = f; };
  bool isValid( void ) { return valid; };
  void save( QString fname, QStringList &Elms );
};

struct aMCAMap {
  int iX, iY;
  aMCASet *Points;

  aMCAMap() { iX = iY = 0; Points = NULL; };
  ~aMCAMap() { if ( Points != NULL ) delete [] Points; };

  void New( int length, int CHs, int ix, int iy )
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
    if ( Points != NULL ) {
      for ( int i = 0; i < iX * iY; i++ ) {
	Points[i].setSize( length, CHs );
      }
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
