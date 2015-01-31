
#include "MainWindow.h"

void MainWindow::SaveS2DMCAs( void )
{
  // S2D �λ��奻����
  // S2D �η�¬�˻Ȥ� View �� View ���֤ˤ�̵���ᥤ�����˰�Ĥ����ʤΤ�
  // ¾�� View ���ͤˤ���ʥ��Ȥ����ɤ줬�оݤȤ��򵤤ˤ��ʤ��Ƥ���
  
  if ( S2DBase == NULL ) {
    statusbar->showMessage( tr( "S2D data is not valid" ), 2000 );
    return;
  }
  if ( inS2D ) {
    statusbar->showMessage( tr( "Can not save MCA spectra while scanning." ), 2000 );
    return;
  }

  S2DBase->SaveMCAs( S2DFileName0->text() );
}

