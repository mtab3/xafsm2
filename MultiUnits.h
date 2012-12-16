#ifndef MULTIUNITS_H
#define MULTIUNITS_H

#include <QVector>
#include <QObject>
#include "AUnit.h"

struct MUElement {   // Multi Unit を構成するエレメント。
                     //    ユニット(実際にはセンサーだけ)のポインタと、
                     //    dwell time を情報として持つ
  AUnit *au;
  double dt;
};

class MUnits : public QObject
{
  Q_OBJECT

  QVector<MUElement*> Units;     // マルチユニットを構成するエレメントの一覧
  QVector<MUElement*> PUnits;    // エレメントの親の重複なしの一覧
  bool isParent;                 // 一つでも親ユニットがあるか

 public:
  MUnits( QObject *p = 0 );

  void clearUnits( void );
  void addUnit( AUnit *au, double dt );
};


#endif
