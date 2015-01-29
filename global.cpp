
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

QStringList FileIDs;
QStringList DataTypeNames;
QStringList ViewTypeNames;
QVector<int> viewCounts;

void global_init( void )
{
  // DATATYPE と一対一対応
  FileIDs.clear();
  FileIDs
    << "  9809     AichiSR"         // XAFSDATA
    << "# XafsM2 Scan Data"         // SCANDATA
    << "# XafsM2 Monitor Data"      // MONDATA
    << "# XafsM2 MCA Data"          // MCADATA
    << "# 1306 Aichi SR 2D Scan"    // S2DDATA
    << " 1304     AichiSR QXAFS base file"  // QXAFSBASEDATA
    << "  9809     AichiSR"         // XAFSSHOW
    << "# XafsM2 Scan Data"         // SCANSHOW
    << "# XafsM2 Monitor Data"      // MONSHOW
    << "# XafsM2 MCA Data"          // MCASHOW
    << "# 1306 Aichi SR 2D Scan"    // S2DSHOW
    << " 1304     AichiSR QXAFS base file"  // QXAFSBASESHOW
    << "";                          // NONDATA

  // DATATYPE と一対一対応
  DataTypeNames.clear();
  DataTypeNames
    << "Measured"
    << "Scaned"
    << "Monitored"
    << "MCA"
    << "S2D"
    << "QBASE"
    << "Measured"
    << "Scaned"
    << "Monitored"
    << "MCA"
    << "S2D"
    << "QBASE"
    << "";

  // DATATYPE と一対一対応
  ViewTypeNames.clear();
  ViewTypeNames
    << "XAFS"
    << "SCAN"
    << "MON."
    << "MCA"
    << "S2D"
    << "QBASE"
    << "D-XAFS"
    << "D-SCAN"
    << "D-MON."
    << "D-MCA"
    << "D-S2D"
    << "D-QBASE"
    << "";

  for ( int i = 0; i < FileIDs.count(); i++ )
    viewCounts << 0;
}
