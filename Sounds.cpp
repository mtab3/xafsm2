
#include "MainWindow.h"

void MainWindow::PlayEndingSound( void )
{
  PlaySound( "finished.wav", 3 );
}

void MainWindow::PlayGoOnSound( void )
{
  PlaySound( "finished.wav", 3 );
}

void MainWindow::PlaySound( QString name, int times )
{
  QSound sound( name );
  // ここで QSound *sound = new QSound; すると
  // 最後に(or どこかで) delete sound; がペアで必要。
  // この関数無いでしか使わない実体なら、
  // local 変数にして new, delete 無しにした方がいい。

  sound.setLoops( times );
  sound.play();
}
