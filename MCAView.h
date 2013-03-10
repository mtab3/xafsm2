#ifndef MCAVIEW_H
#define MCAVIEW_H

#include <QWidget>

#include "ui_MCAView.h"
#include "MouseC.h"
#include "ChCoord.h"
#include "FluoDBase.h"
#include "KeV2Pix.h"

enum MMODE { M_ROI, M_POINT, M_NO };

class MCAView : public QFrame, private Ui::MCAView
{
  Q_OBJECT

private:
  QWidget *Parent;

  int valid;
  //  QVecotot<int *> MCAs;
  int *MCA;
  int MCALen;
  int MCACh;
  double realTime;
  double liveTime;

  bool dispLog;
  FluoDBase *fdbase;
  KeV2Pix *k2p;
  bool showElements;
  bool showElementsAlways;
  bool showElementsEnergy;

  MMODE mMode;
  ChCoord cc;
  double rROIsx, rROIex;   // ROI の範囲
  double MinE, MaxE;       // 表示の最小、最大エネルギー
  QVector<double> cPoints; // カーソル表示を残す点
  QStringList selectedAtoms;
  double yRatio;           // 縦軸の拡大倍率

  int nearX;
  bool nearf;

  QColor Black, White;
  QColor ROIRangeC, ExROIRangeC, ROIEdgeC;
  QColor GridC, MCursorC, MCursorC2, MCursorC3, AListC;

  void Draw( QPainter *p );

  void paintEvent( QPaintEvent *e );
  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );
  void wheelEvent( QWheelEvent *e );
  MouseC m;

public:
  MCAView( QWidget *parent = NULL );
  ~MCAView( void );

  void setKeV2Pix( KeV2Pix *K2P ) { k2p = K2P; };
  void setFDBase( FluoDBase *FDBase ) { fdbase = FDBase; };
  int *setMCAdataPointer( int length );
  void SetRealTime( double rt ) { realTime = rt; };
  void SetLiveTime( double lt ) { liveTime = lt; };
  void SetMCACh( int i ) { MCACh = i; };
  void makeValid( bool v ) { valid = v; };
  double getRealTime( void ) { return realTime; };
  double getLiveTime( void ) { return liveTime; };
  void setSelectedAtoms( QStringList aList ) { selectedAtoms = aList; update(); };

public slots:
  void setROI( int s, int e );   // MCA pixel
  void setLog( bool f ) { dispLog = f; update(); };
  void setShowElements( bool show ) { showElements = show; update(); };
  void setShowElementsAlways( bool show ) { showElementsAlways = show; update(); };
  void setShowElementsEnergy( bool show ) { showElementsEnergy = show; update(); };

signals:
  void CurrentValues( int atCur, int inROI );
  void newROI( int ROIstart, int ROIend );
};

#endif
