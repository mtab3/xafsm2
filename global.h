
#ifndef GLOBAL_H
#define GLOBAL_H

#include <QVector>
#include <QString>
#include <QStringList>

extern QStringList FileIDs;
extern QStringList DataTypeNames;
extern QStringList ViewTypeNames;
extern QVector<int> viewCounts;

enum DATATYPE { MEASDATA, SCANDATA, MONDATA, MCADATA, S2DDATA, QXAFSBASEDATA,
		MEASSHOW, SCANSHOW, MONSHOW, MCASHOW, S2DSHOW, QXAFSBASESHOW,
		MCADATA2, NONDATA };

extern void global_init( void );

extern double prec( double x, int n );
extern QString CheckFNameExt( QString fname, QString ext );


#endif
