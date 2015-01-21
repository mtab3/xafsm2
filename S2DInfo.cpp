
#include "S2DInfo.h"

S2DInfo::S2DInfo( void )
{
  valid = false;

  motors = 3;
  ScanBothDir = false;
  Use3rdAx = false;
  isSFluo = false;
  unit << NULL << NULL << NULL;
  used << true << true << false;
  ps << 0 << 0 << 0;
  now << 0 << 0 << 0;
  sx << 0 << 0 << 0;
  ex << 0 << 0 << 0;
  dx << 0 << 0 << 0;
  i << 0 << 0 << 0;
  ScanMode = STEP;
}

void S2DInfo::saveS2DInfo( QTextStream &out )
{
}

void S2DInfo::loadS2DInfo( QTextStream &in )
{
}


