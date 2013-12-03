#ifndef MEASPARAMSET_H
#define MEASPARAMSET_H

struct MeasPSet {
  bool valid;

  MUnits mUnits;
  QString fname;
  QString fname00;
  bool isI1;
  bool isSFluo;
  int totalPoints;
  int rpt;
  QVector< QVector<double> > i0s;

  MeasPSet() { valid = false; };
};

#endif
