#ifndef MEASPARAMSET_H
#define MEASPARAMSET_H

struct MeasPSet {
  bool valid;
  bool normallyFinished;
  bool qXafsMode;
  bool TuneAtEachStep;

  MUnits mUnits;
  QString fname;
  // QString fname00;
  bool isI1;
  QVector<bool> isSFluos;
  bool isNoSFluo;
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
  bool isUsingSFluo( void ) {
    for ( int i = 0; i < isSFluos.count(); i++ ) {
      if ( isSFluos[i] ) {
	return true;
      }
    }
    return false;
  }
#if 0
  int selectedSFluoCHs( void ) {
    int rv = 0;
    for ( int i = 0; i < isSFluos.count(); i++ ) {
      if ( isSFluos[i] ) {
	rv += SFluos[i]->chs();
      }
    }
    return rv;
  }
#endif
};

#endif
