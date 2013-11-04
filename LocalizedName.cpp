#include "MainWindow.h"

QString MainWindow::LocalizedName( QString name )
{
  // ̾��������ü�ʸ�����ִ�
  // tr �ǥ����դ����줿ʸ����ˤ��Ƥ����ơ�
  // lupdate, linguist �ڥ��� ja_JP.qm �ե�������뤳�Ȥ�����ʸ�����б�
  name.replace( QString( "CEY Stg." ),  tr( "CEY" ) );
  name.replace( QString( "Cap. Stg." ), tr( "CAP" ) );
  name.replace( QString( "Theta" ),     tr( "Theta!" ) );
  name.replace( QString( "Dth" ),       tr( "Dth" ) );
  name.replace( QString( "Sample" ),    tr( "Sample" ) );
  name.replace( QString( "Stage" ),     tr( "Stage" ) );
  name.replace( QString( "Stg." ),      tr( "Stage" ) );
  name.replace( QString( "Phi" ),       tr( "Phi" ) );
  name.replace( QString( "Omg" ),       tr( "Omg" ) );
  name.replace( QString( "open" ),      tr( "open" ) );
  name.replace( QString( "move" ),      tr( "move" ) );
  name.replace( QString( "Counter" ),   tr( "Counter" ) );

  name.replace( QString( "ax" ),        tr( "axis" ) );

  name.replace( QString( "Series" ) ,   tr( "Series" ) );
  name.replace( QString( "Shutter" ),   tr( "Shutter" ) );
  name.replace( QString( "pin hole" ),  tr( "pin hole" ) );
  name.replace( QString( "um" ),        tr( "um" ) );

  return name;
}
