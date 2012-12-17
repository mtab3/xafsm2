#ifndef MULTIUNITS_H
#define MULTIUNITS_H

#include <QVector>
#include <QObject>
#include "AUnit.h"

struct MUElement {   // Multi Unit �������륨����ȡ�
                     //    ��˥å�(�ºݤˤϥ��󥵡�����)�Υݥ��󥿤ȡ�
                     //    dwell time �����Ȥ��ƻ���
  AUnit *au;
  double dt;
};

class MUnits : public QObject
{
  Q_OBJECT

  QVector<MUElement*> Units;     // �ޥ����˥åȤ������륨����Ȥΰ���
  QVector<MUElement*> PUnits;    // ������ȤοƤν�ʣ�ʤ��ΰ���

 public:
  MUnits( QObject *p = 0 );

  int count( void ) { return Units.count(); };
  bool isParent( void ) { return ( PUnits.count() > 0 ); };
  void clearUnits( void );
  void addUnit( AUnit *au, double dt );
  void setDwellTimes( double dt );
  bool isBusy( void );
  void clearStage( void );
  bool init( void );
  void setDwellTime( void );
  bool getValue0( void );
  bool getValue( void );
  void readValue( double *rvs );
  QString getName( int i ) { return Units.at(i)->au->getName(); };
  bool getMCA( int ch );
};


#endif
