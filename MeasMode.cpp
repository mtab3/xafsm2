
#include <QRegExp>
#include <QStringList>

#include "MeasMode.h"

MeasMode::MeasMode( void )
{
  name = "none";
  useI0 = useI1 = useA1 = useA2 = false;
  useFs.clear();
}

void MeasMode::setMode( QString mode )
{
  QStringList is = mode.split( QRegExp( "\\s+" ) );
  int c = is.count();
  if ( c < 1 ) return;
  if (( is[0] == "I0" )&&( c > 1 )) { useI0 = true; devI0 = is[1]; }
  if (( is[0] == "I1" )&&( c > 1 )) { useI1 = true; devI1 = is[1]; }
  if (( is[0] == "A1" )&&( c > 2 )) { useA1 = true; devA1 = is[1]; dModeA1 = is[2].toInt(); }
  if (( is[0] == "A2" )&&( c > 2 )) { useA2 = true; devA2 = is[1]; dModeA2 = is[2].toInt(); }
  if ( is[0].left(1) == "F" ) { setF( is[0].mid(1).toInt(), true ); }
}

void MeasMode::setF( int ch, bool f )
{
  if ( ch < 0 )
    return;
  if ( ch >= 0 ) {
    int nowSize = useFs.count();
    if ( ch >= useFs.count() ) {
      useFs.resize( ch + 1 );
    }
    for ( int i = nowSize; i < useFs.count(); i++ )
      if ( i >= 0 ) useFs[i] = false;
    useFs[ch] = f;
  }
}
