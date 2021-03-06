
#include <ctype.h>

#include <QStringList>
#include <QTextStream>
#include <QDebug>
#include <QTextStream>
#include <QFile>

#include "Atoms.h"

const QString AbENames[ Edges ] = {
  "K",
  "LI",
  "LII",
  "LIII",
};

VICS Vic[ 100 ];
int Vics = 0;

double ltz( double m )
{
  if ( m > 0 )
    return m;
  return 0;
}

void ReadVicF( void )
{
  Vics = 0;

  QFile file( QString( ":Victoreen1.txt" ) );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    qDebug() << "Cannot open [Victoreen1.txt]\n";
    return;
  }
  
  QTextStream in(&file);

  while( !in.atEnd() ) {
    QStringList cols = in.readLine().simplified().split( ' ' );
    if ( cols[0].left( 1 ) != "#" ) {
      Vic[ Vics ].AN = cols[1].toInt();
      Vic[ Vics ].AE[ 0 ] = cols[2].toDouble();  // 吸収端のエネルギー (keV)
      Vic[ Vics ].C[ 0 ] = ltz( cols[4].toDouble() );  // 波長のカラム[3]は読み飛ばす
      Vic[ Vics ].D[ 0 ] = ltz( cols[5].toDouble() );
      Vic[ Vics ].AE[ 1 ] = ltz( cols[6].toDouble() );
      Vic[ Vics ].C[ 1 ] = ltz( cols[8].toDouble() );
      Vic[ Vics ].D[ 1 ] = ltz( cols[9].toDouble() );
      Vic[ Vics ].AE[ 2 ] = ltz( cols[10].toDouble() );
      Vic[ Vics ].C[ 2 ] = ltz( cols[12].toDouble() );
      Vic[ Vics ].D[ 2 ] = ltz( cols[13].toDouble() );
      Vic[ Vics ].AE[ 3 ] = ltz( cols[14].toDouble() );
      Vic[ Vics ].C[ 3 ] = ltz( cols[16].toDouble() );
      Vic[ Vics ].D[ 3 ] = ltz( cols[17].toDouble() );
      Vic[ Vics ].AE[ 4 ] = ltz( cols[18].toDouble() );
      Vic[ Vics ].C[ 4 ] = ltz( cols[19].toDouble() );
      Vic[ Vics ].D[ 4 ] = ltz( cols[20].toDouble() );
      Vics++;
    }
  }

  file.close();
}
