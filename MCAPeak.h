#ifndef MCAPEAK_H
#define MCAPEAK_H

struct MCAPeak {
  double start;      // start, end, center の単位は MCA pixel だけど、フィッティングして
  double end;        // 整数以下の桁が出る可能性があるので、実数
  double center;
  double centerE;
  int peakH0, peakH;
};

#endif
