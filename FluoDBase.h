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
  QVector<Fluo> nears( double E, double range, double dE );
  QVector<Fluo> nears( double E, double dE );
  QVector<Fluo> inRange( double Es, double Ee, double dE );
  int nearest( double E, int s, int e );

 private:
  QVector<Fluo> fluos;
  QVector<Fluo> removeTooNears( QVector<Fluo> list, double dE );

 public slots:
  void ElementSelected( bool f, int i );
};

#endif
