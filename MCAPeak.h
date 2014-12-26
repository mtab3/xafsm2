#ifndef MCAPEAK_H
#define MCAPEAK_H

struct MCAPeak {
  int sp, ep, cp;
  double sE, eE, cE, C;   // C �� A exp C * (x-B)*(x-B) �� C
  int peakH0, peakH;
};

#endif
