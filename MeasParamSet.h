#ifndef MEASPARAMSET_H
#define MEASPARAMSET_H

struct MeasPSet {
  bool valid;
  bool normallyFinished;
  bool qXafsMode;
  bool TuneAtEachStep;

  MUnits mUnits;
  QString fname;
  QString fname00;
  bool isI1;
  QVector<bool> isSFluos;
  int totalPoints;
  int rpt;
  int finalRpt;
  int finalPnt;
  QVector<double*> i0s;

  bool TuneESAbs;
  double TuneESStart;
  double TuneESEnd;
  int TuneESSteps;
  bool TuneESQuick;
  double TuneESQuickTime;

  MeasPSet() {
    valid = false;
    normallyFinished = false;
    qXafsMode = false;
    TuneAtEachStep = false;
    TuneESQuick = false;
  };
};

#endif
