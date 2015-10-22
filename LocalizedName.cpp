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
  name.replace( QString( "Sum" ),       tr( "Sum" ) );

  name.replace( QString( "MinH" ),      tr( "MinH" ) );
  name.replace( QString( "min Z" ),     tr( "miZ" ) );
  name.replace( QString( "max Z" ),     tr( "maZ" ) );

  name.replace( QString( "Series" ) ,   tr( "Series" ) );
  name.replace( QString( "Shutter" ),   tr( "Shutter" ) );
  name.replace( QString( "pin hole" ),  tr( "pin hole" ) );
  name.replace( QString( "um" ),        tr( "um" ) );

  name.replace( QString( "R. Cur." ),   tr( "R. Cur." ) );
  name.replace( QString( "U. Gap" ),    tr( "U. Gap" ) );
  name.replace( QString( "BPM H12" ),   tr( "BPM H12" ) );
  
  name.replace( QString( "ax" ),        tr( "axis" ) );

  name.replace( QString( "TransM" ),    tr( "Trans. Mode" ) );
  name.replace( QString( "FluoM" ),     tr( "Fluo. Mode" ) );
  name.replace( QString( "CEYM" ),      tr( "CEY Mode" ) );

  name.replace( QString( "CC-10" ),     tr( "CC-10" ) );
  name.replace( QString( "FP23" ),      tr( "FP23"  ) );
  name.replace( QString( "Vertical" ),        tr( "Vertical" ) );
  name.replace( QString( "Horizontal" ),      tr( "Horizontal" ) );

  
  name.replace( QString( "Encoder Th2" ),          tr( "EncoderTh2" ) );
  name.replace( QString( "Encoder Th" ),           tr( "EncoderTh" ) );
  name.replace( QString( "Ring Current Monitor" ),  tr( "RingCurrentMonitor" ) );
  name.replace( QString( "Injection Monitor" ),     tr( "InjectionMonitor" ) );
  name.replace( QString( "Current via epics" ),     tr( "CurrentViaEpics" ) );
  name.replace( QString( "Undulator Gap" ),   tr( "UndulatorGap" ) );
  name.replace( QString( "Wave mon. to ctrl. dTh1" ),   tr( "WaveMonToCTRLdTh1" ) );
  name.replace( QString( "I0 mon. to ctrl. dTh1" ),   tr( "I0MonToCTRLdTh1" ) );

  return name;
}
