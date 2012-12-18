#ifndef MCAVIEW_H
#define MCAVIEW_H

#include "ui_MCAView.h"

class MCAView : public QFrame, private Ui::MCAView
{
  Q_OBJECT

private:
  int valid;
  double *MCA;
  int MCALen;

  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );

public:
  MCAView( QWidget *parent = NULL );
  void setMCAdataPointer( double *mca, int length );
};

#endif
