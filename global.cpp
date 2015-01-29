
#include <math.h>
#include <QFileInfo>

#include "global.h"

double prec( double x, int n )
{
  double p = pow( 10, n );
  return ((int)( x * p ))/p;
}

QString CheckFNameExt( QString fname, QString ext )
{
  QFileInfo fi( fname );
  if ( fi.suffix() != ext )
    return fname + "." + ext;

  return fname;
}
