#ifndef MEASPARAMSET_H
#define MEASPARAMSET_H

struct MeasPSet {
  bool valid;

  bool isI1;
  bool isSFluo;
  int totalPoints;
  int rpt;

  MeasPSet() { valid = false; };
};

#endif
