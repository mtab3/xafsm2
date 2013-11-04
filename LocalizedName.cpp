#include "MainWindow.h"

QString MainWindow::LocalizedName( QString name )
{
  // 名前の中の特殊文字の置換
  // tr でタグ付けされた文字列にしておいて、
  // lupdate, linguist ペアで ja_JP.qm ファイルを作ることで全角文字に対応
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
