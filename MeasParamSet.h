#ifndef MEASPARAMSET_H
#define MEASPARAMSET_H

struct MeasPSet {
  bool valid;

  bool isSFluo;
  int totalPoints;
  int rep;

  MeasPSet() { valid = false; };
};

#endif
