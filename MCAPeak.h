#ifndef MCAPEAK_H
#define MCAPEAK_H

struct MCAPeak {
  //  int sp, ep, cp;
  //  double sE, eE;
  //  int peakH0;
  double A;   // peakH は A
  double BinE;    // エネルギー単位のピーク位置
  double BinP;    // pixel 単位のピーク位置
  double CinE;    // C は A exp C * (x-B)*(x-B) の C
  double CinP;    // pixel 単位に直した C
  double WinE;    // エネルギー単位の半値全幅
  double WinP;    // pixel 単位の半値全幅
};

#endif
