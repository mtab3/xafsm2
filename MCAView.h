#ifndef MCAVIEW_H
#define MCAVIEW_H

#include "ui_MCAView.h"
#include "MouseC.h"
#include "ChCoord.h"
#include "FluoDBase.h"

class MCAView : public QFrame, private Ui::MCAView
{
  Q_OBJECT

private:
  int valid;
  int *MCA;
  int MCALen;
  int MCACh;
  double realTime;
  double liveTime;

  bool dispLog;
  FluoDBase *fdbase;

  ChCoord cc;
  double rROIsx, rROIex;
  double AA, BB;    // ��������������
  double MaxE; // ɽ���κ��票�ͥ륮��

  int nearX;
  bool nearf;

  QColor Black, White;
  QColor ROIRangeC, ExROIRangeC, ROIEdgeC;
  QColor GridC, MCursorC;

  void Draw( QPainter *p );

  void paintEvent( QPaintEvent *e );
  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );
  MouseC m;
public:
  MCAView( QWidget *parent = NULL );
  ~MCAView( void );

  int *setMCAdataPointer( int length );
  void SetRealTime( double rt ) { realTime = rt; };
  void SetLiveTime( double lt ) { liveTime = lt; };
  void SetMCACh( int i ) { MCACh = i; };
  void makeValid( bool v ) { valid = v; };
  double getRealTime( void ) { return realTime; };
  double getLiveTime( void ) { return liveTime; };

public slots:
  void setLog( bool f ) { dispLog = f; update(); };
  void setROI( int s, int e );   // MCA pixel

signals:
  void CurrentValues( int atCur, int inROI );
  void newROI( int ROIstart, int ROIend );
};

#endif
