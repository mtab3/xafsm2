#ifndef FCHECK_H
#define FCHECK_H

struct FileCheck {
  bool CheckHeadLine( QString main, QString sub, QString line, int skip )
  {
    QString Head = QString( "%1 %2" ).arg( main ).arg( sub );
    Head = Head.simplified();
    return ( line.mid( skip, QString( Head ).length() ) == QString( Head ) );
  }
  
  // The style of the line should be '# ITEMNAME : value...'.
  bool CheckItem( QString itemName, QString line, int skip, QString &val )
  {
    if ( line.mid( skip, QString( itemName ).length() ) == QString( itemName ) ) {
      val = line.mid( skip + QString( itemName ).length() + 3 );
      return true;
    } else {
      val = "";
      return false;
    }
  }
};

#endif
