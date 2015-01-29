
#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QStringList>

extern QStringList FileIDs;
extern QStringList DataTypeNames;

enum DATATYPE { MEASDATA, SCANDATA, MONDATA, MCADATA, S2DDATA, QXAFSBASEDATA,
		MEASSHOW, SCANSHOW, MONSHOW, MCASHOW, S2DSHOW, QXAFSBASESHOW, NONDATA };

extern void global_init( void );

extern double prec( double x, int n );
extern QString CheckFNameExt( QString fname, QString ext );


#endif
