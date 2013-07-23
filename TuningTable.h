#ifndef TUNINGTABLE_H
#define TUNINGTABLE_H

#include <QObject>
#include <QVector>
#include <QStringList>

struct aPoint
{
 public:

  bool operator<( const aPoint &one ) const { return eng < one.eng; }; 

  double eng;
  int pulse;
};

class TuningTable : public QObject
{
  Q_OBJECT

  QVector<aPoint> Points;
  QStringList Comments;

public:
  TuningTable( void );
  void ReadTable( QString fname );
  void AddAPoint0( double e, int p );
  void AddAPoint( double e, int p );
  bool isAvailable( void );
  int p( double E );

public slots:
  void SaveTuneTable( void );
};

#endif
