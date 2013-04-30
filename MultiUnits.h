#ifndef MULTIUNITS_H
#define MULTIUNITS_H

#include <QVector>
#include <QAbstractButton>
#include <QObject>
#include "AUnit.h"

struct MUElement
{ // Multi Unit を構成するエレメント。
  //    ユニット(実際にはセンサーだけ)のポインタと、
  //    dwell time を情報として持つ
  AUnit *au;
  double dt;
  bool done;
};

class MUnits : public QObject
{
  Q_OBJECT

  QVector<MUElement*> Units;     // マルチユニットを構成するエレメントの一覧
  QVector<MUElement*> PUnits;    // エレメントの親の重複なしの一覧
  bool OneByOne;

 public:
  MUnits( QObject *p = 0 );

  void setOneByOne( bool f ) { OneByOne = f; };
  int count( void ) { return Units.count(); };
  AUnit *at( int i ) { return Units.at(i)->au; };
  bool isParent( void ) { return ( PUnits.count() > 0 ); };
  void clearUnits( void );
  void addUnit( AUnit *au );
  void setDwellTimes( double dt );
  double getSetTime( int i );
  bool isBusy( void );
  void clearStage( void );
  bool init( void );
  void setDwellTime( void );
  bool getValue0( void );
  bool getValue( void );
  //  void readValue( double *rvs, bool correctBack );
  void readValue( double *rvs, double *cpss, bool correctBack );
  QString getName( int i ) { return Units.at(i)->au->getName(); };
  QString getUnit( int i ) { return Units.at(i)->au->getUnit(); };
  bool QStart( void );
  bool QRead( void );
  bool QEnd( void );

#if 0
  bool getMCA( int ch );
#endif
  void clearDoneF( void );

private slots:
  void ShownMessage( QAbstractButton * );
};


#endif
