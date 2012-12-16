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
  bool isParent;                 // ��ĤǤ�ƥ�˥åȤ����뤫

 public:
  MUnits( QObject *p = 0 );

  void clearUnits( void );
  void addUnit( AUnit *au, double dt );
};


#endif
