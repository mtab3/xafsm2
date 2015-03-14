
#ifndef GLOBAL_H
#define GLOBAL_H

#include <QVector>
#include <QString>
#include <QStringList>

extern QStringList FileIDs;
extern QStringList DataTypeNames;
extern QVector<QStringList> ViewTypeNames;
extern QVector<int> viewCounts;

enum D_TYPE { MEASDATA, SCANDATA, MONDATA, MCADATA,  S2DDATA, QXAFSBASEDATA, NODATA };
enum D_ORIG { MEASUREDD, READD, NOORIG };
enum D_VER  { VER_00, VER_01, VER_02, VER_03 };

#if 0
enum DATATYPE { MEASDATA, SCANDATA, MONDATA, MCADATA,  S2DDATA, QXAFSBASEDATA,
		MEASSHOW, SCANSHOW, MONSHOW, MCASHOW1, S2DSHOW, QXAFSBASESHOW,
		MCASHOW2, NONDATA };
#endif

extern void global_init( void );

extern double prec( double x, int n );
extern QString CheckFNameExt( QString fname, QString ext );

extern void dbg( QString msg );

#endif
