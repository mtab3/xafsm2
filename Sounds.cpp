
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
  // ������ QSound *sound = new QSound; �����
  // �Ǹ��(or �ɤ�����) delete sound; ���ڥ���ɬ�ס�
  // ���δؿ�̵���Ǥ����Ȥ�ʤ����Τʤ顢
  // local �ѿ��ˤ��� new, delete ̵���ˤ�������������

  sound.setLoops( times );
  sound.play();
}
