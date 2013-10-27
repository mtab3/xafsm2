#ifndef BLOCKINFO_H
#define BLOCKINFO_H

#include "XafsM.h"

struct BlockLine {
public:
  QString start;
  QString step;
  QString points;
  QString dwell;

  BlockLine( void ) { start = step = points = dwell = ""; };
};

class BlockInfo {
 public:
  UNIT Unit;
  int Blocks;
  BlockLine Block[ MaxBLKs + 1 ];

  BlockInfo( void ) { Blocks = 0; Unit = EV; };
};

#endif
