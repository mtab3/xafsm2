#ifndef MCAPEAK_H
#define MCAPEAK_H

struct MCAPeak {
  int sp, ep, cp;
  double sE, eE, cE, C;   // C ¤Ï A exp C * (x-B)*(x-B) ¤Î C
  int peakH0, peakH;
};

#endif
