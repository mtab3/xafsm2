#ifndef CCOLORS_H
#define CCOLORS_H

// generate cyclic colors

class CColors : public QObject
{
  Q_OBJECT

  QVectoc<QColor> ccolors;
  
 public:
  CColors( void );
  CColors( QVector<QColor> peakColors, int colors );
}

#endif
