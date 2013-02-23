#ifndef FLUODBASE_H
#define FLUODBASE_H

#include <QObject>
#include <QVector>

struct Fluo
{
  QString fullName;
  QString aName;
  QString transName;
  int aNumber;
  double val;
};

class FluoDBase : public QObject
{
  Q_OBJECT

 public:  
  FluoDBase();

 private:
  QVector<Fluo*> fluos;
};

#endif
