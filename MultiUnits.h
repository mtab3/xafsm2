#ifndef MULTIUNITS_H
#define MULTIUNITS_H

#include <QVector>
#include <QAbstractButton>
#include <QObject>
#include "AUnits/ASensor.h"

struct MUElement
{ // Multi Unit を構成するエレメント。
  //    ユニット(実際にはセンサーだけ)のポインタと、
  //    dwell time を情報として持つ
  ASensor *au;
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

  void setOneByOne( bool f ) { OneByOne = f; }
  int count( void ) { return Units.count(); }
  ASensor *at( int i ) { return Units.at(i)->au; }
  bool isParent( void ) { return ( PUnits.count() > 0 ); }
  void clearUnits( void );
  void addUnit( ASensor *au );
  void setDwellTimes( double dt );
  double getSetTime( int i );
  bool isBusy( void );
  bool isBusy2( void );
  void clearStage( void );
  bool init( void );
  void setDwellTime( void );
  bool getValue0( void );    // ステップ動作用
  bool getValue02( void );   // 連続スキャン用 (PresetTime 等を見ない設定)
  bool getValue( void );
  bool Close( void );
  //  void readValue( double *rvs, bool correctBack );
  void readValue( double *rvs, double *cpss, bool correctBack );
  QString getName( int i ) { return Units.at(i)->au->getName(); }
  QString getUnit( int i ) { return Units.at(i)->au->getUnit(); }
  bool QStart( void );
  bool QRead( void );
  bool QEnd( void );

  void clearDoneF( void );

private slots:
  void ShownMessage( QAbstractButton * );
};


#endif
