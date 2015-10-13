#ifndef MEASMODE_H
#define MEASMODE_H

#include <QPushButton>
#include <QString>
#include <QVector>

class MeasMode
{
  QString name;
  QPushButton *pb;
  bool useI0, useI1, useA1, useA2;
  QString devI0, devI1, devA1, devA2;
  int dModeA1, dModeA2;
  QVector<bool> useFs;
  
 public:
  MeasMode();

  void setModeName( QString name ) { this->name = name; };
  void setMode( QString mode );
  void setButton( QPushButton *b ) { pb = b; }
  void setI0( bool f ) { useI0 = f; };
  void setI1( bool f ) { useI1 = f; };
  void setA1( bool f ) { useA1 = f; };
  void setA2( bool f ) { useA2 = f; };
  void setF( int ch, bool f );

  QString Name( void ) { return name; };
  QPushButton *Button( void ) { return pb; };
  bool isI0( void ) { return useI0; };
  bool isI1( void ) { return useI1; };
  bool isA1( void ) { return useA1; };
  bool isA2( void ) { return useA2; };
  bool isF( int ch ) { return (( ch >= 0 )&&( ch < useFs.count() )) ? useFs[ch] : false; };
  QString DevI0( void ) { return devI0; };
  QString DevI1( void ) { return devI1; };
  QString DevA1( void ) { return devA1; };
  QString DevA2( void ) { return devA2; };
  int DModeA1( void ) { return dModeA1; };
  int DModeA2( void ) { return dModeA2; };
};

#endif
