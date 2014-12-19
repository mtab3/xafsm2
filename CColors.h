#ifndef CCOLORS_H
#define CCOLORS_H

#include <QObject>
#include <QVector>
#include <QColor>

// Generate series of colors

class CColors : public QObject
{
  Q_OBJECT

  QVector<QColor> ccolors;

  void generateColors( QVector<QColor> peakColors, int colors );
  int mix( int c1, int c2 );
  
 public:
  CColors( void );
  CColors( int colors );
  CColors( QVector<QColor> peakColors, int colors );

  int count( void ) { return ccolors.count(); };
  QColor at( int i ) { return ccolors[i]; };
  QColor operator[]( int i ) { return ccolors[i]; };

};

#endif
