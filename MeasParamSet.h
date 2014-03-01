#ifndef MEASPARAMSET_H
#define MEASPARAMSET_H

struct MeasPSet {
  bool valid;
  bool normallyFinished;

  MUnits mUnits;
  QString fname;
  QString fname00;
  bool isI1;
  bool isSFluo;
  int totalPoints;
  int rpt;
  int finalRpt;
  int finalPnt;
  QVector<double*> i0s;

  MeasPSet() { valid = false; normallyFinished = false; };
};

#endif
