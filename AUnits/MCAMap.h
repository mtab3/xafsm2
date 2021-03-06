#ifndef MCA_H
#define MCA_H

#include <QDebug>
#include <QTextStream>
#include <QStringList>

#include "../FCheck.h"
#include "XMAPHead.h"
#include "KeV2Pix.h"

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
  FileCheck fc;

  int Length, CHs;
  
  QString date;

  aCH *Ch;
  XMAPHead *Heads;

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
    Heads = new XMAPHead[ CHs ];
    ROIStart = new QString[ CHs ];
    ROIEnd = new QString[ CHs ];
    for ( int i = 0; i < CHs; i++ ) {
      Ch[i].setLength( Length );
    }
  }

  int length( void ) { return Length; };
  int chs( void ) { return CHs; };
  void setValid( bool f ) { valid = f; };
  bool isValid( void ) { return valid; };
  void save( QString fname, QString title );
  //  void writeHead0( QTextStream &out );  // old
  void writeHead( QTextStream &out );  // new
  void writeData( QTextStream &out );
  void load( QString fname, QString title );
  void load( QTextStream &in, QString title );
  void loadHeader( QTextStream &in );   // new
  void loadHeader0( QTextStream &in );  // old 
  void correctE( KeV2Pix *k2p );
  void copyCnt( int ch, quint32 *cnt );
};

struct aMCAMap {
  int iX, iY;
  QVector<aMCASet> MCASets;

  aMCAMap() { iX = iY = 0; };
  ~aMCAMap() {};

  void New( int ix, int iy, int length, int CHs );
  aMCASet *aPoint( int ix, int iy );
  bool valid( int ix, int iy );
  aMCASet *lastP( void );

};

#endif
