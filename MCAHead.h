#ifndef MCAHEAD_H
#define MCAHEAD_H

class MCAHead {
 public:
  qint64 ch;
  qint64 stat;
  qint64 len;
  double realTime;
  double liveTime;
  double icr;

  MCAHead() {
    ch = -1;
    stat = len = 0;
    realTime = liveTime = icr = 0;
  }
};

#endif
