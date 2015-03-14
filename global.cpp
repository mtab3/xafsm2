
#include <math.h>
#include <QFileInfo>
#include <QTextStream>

#include "global.h"

void dbg( QString /* msg */ )
{
#if 0
  QFile f( "/Users/tabuchi/Desktop/prog/XafsM2/dbg.dat" );
  f.open( QIODevice::Append | QIODevice::Text );
  QTextStream out( &f );
  out << msg << "\n";
  f.close();
#endif
}

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
QVector<QStringList> ViewTypeNames;
QVector<int> viewCounts;

void global_init( void )
{
  // DATATYPE と一対一対応
  FileIDs.clear();
  FileIDs
    << "  9809     AichiSR"         // XAFSDATA
    << "# XafsM2 Scan Data"         // SCANDATA
    << "# XafsM2 Monitor Data"      // MONDATA
    << "# XafsM2 MCA"               // MCADATA  Ver0 - "MCA Data", Ver1 - "MCA Ver. 1"
    << "# 1306 Aichi SR 2D Scan"    // S2DDATA
    << " 1304     AichiSR QXAFS base file"  // QXAFSBASEDATA
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
    << "";

  // DATATYPE と一対一対応
  ViewTypeNames.clear();
  QStringList names1, names2, names3;
  names1                  // D_ORIG : MEASEDD に対応
    << "XAFS"
    << "SCAN"
    << "MON."
    << "MCA"
    << "S2D"
    << "QBASE"
    << "";
  names2                  // D_ORIG : READD に対応 (VER_00)
    << "D-XAFS"
    << "D-SCAN"
    << "D-MON."
    << "D-MCA"
    << "D-S2D"
    << "D-QBASE"
    << "";
  names3                  // D_ORIG : READD に対応 (VER_01)
    << "D-XAFS1"
    << "D-SCAN1"
    << "D-MON.1"
    << "D-MCA1"
    << "D-S2D1"
    << "D-QBASE1"
    << "";
  ViewTypeNames << names1 << names2 << names3;
  
  for ( int i = 0; i < FileIDs.count(); i++ )
    viewCounts << 0;
}
