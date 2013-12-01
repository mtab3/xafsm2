#ifndef MEASPARAMSET_H
#define MEASPARAMSET_H

struct MeasPSet {
  bool valid;

  QString fname;
  bool isI1;
  bool isSFluo;
  int totalPoints;
  int rpt;
  QVector< QVector<double> > i0s;

  MeasPSet() { valid = false; };
};

#endif
