#ifndef XMAPHEAD_H
#define XMAPHEAD_H

class XMAPHead {
 public:
  qint64 ch;
  qint64 stat;
  qint64 len;
  double realTime;
  double liveTime;
  double icr;

  XMAPHead() {
    ch = -1;
    stat = len = 0;
    realTime = liveTime = icr = 0;
  }
};

#endif
