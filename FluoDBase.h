#ifndef FLUODBASE_H
#define FLUODBASE_H

#include <QObject>
#include <QVector>

class Fluo
{
 public:
  Fluo() {};

  bool operator<( const Fluo &one ) const { return val < one.val; }; 

  bool dispf;
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
  QVector<Fluo> nears( double E, double range );
  QVector<Fluo> nears( double E );
  QVector<Fluo> inRange( double Es, double Ee );
  int nearest( double E, int s, int e );

 private:
  QVector<Fluo> fluos;

 public slots:
  void ElementSelected( bool f, int i );
};

#endif
