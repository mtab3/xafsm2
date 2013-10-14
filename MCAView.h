#ifndef MCAVIEW_H
#define MCAVIEW_H

#include <QWidget>

#include "ui_MCAView.h"
#include "MouseC.h"
#include "ChCoord.h"
#include "FluoDBase.h"
#include "KeV2Pix.h"

enum MMODE { M_ROI, M_POINT, M_H_SHIFT, M_NO };

struct MCAPeak {
  int start;
  int end;
  double center;
  double centerE;
  int peakH;
};

class MCAView : public QFrame, private Ui::MCAView
{
  Q_OBJECT

private:
  QWidget *Parent;

  int valid;
  //  QVecotot<int *> MCAs;
  quint32 *MCA;
  double *SMCA, *DMCA, *dMCA;   // スムージング結果、1次微分、統計変動
  double *E;
  int MCALen;
  int MCACh;
  double realTime;
  double liveTime;
  double lowerLimit;

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

  bool ShowDiff;
  bool LimitPSEnergy;      // ピークサーチを I0 のエネルギーまででやめる。
  double I0Energy;         // その I0 のエネルギー保持
  double PSSens;           // ピークサーチの感度
  QVector<MCAPeak> MCAPeaks;

  int nearX;
  bool nearf;

  QColor Black, White;
  QColor ROIRangeC, ExROIRangeC, ROIEdgeC;
  QColor GridC, MCursorC, MCursorC2, MCursorC3, AListC;
  QColor SMCAC, DMCAC;
  QColor PEAKPOINTC;

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
  quint32 *setMCAdataPointer( int length );
  void SetRealTime( double rt ) { realTime = rt; };
  void SetLiveTime( double lt ) { liveTime = lt; };
  void SetLowLimit( double LL ) { lowerLimit = LL; };
  void SetMCACh( int i ) { MCACh = i; };
  void makeValid( bool v ) { valid = v; };
  double getRealTime( void ) { return realTime; };
  double getLiveTime( void ) { return liveTime; };
  void setSelectedAtoms( QStringList aList ) { selectedAtoms = aList; update(); };
  void setNewPSSens( QString newSens );
  void setShowDiff( bool f ) { ShowDiff = f; update(); };
  void setLimitPSEnergy( bool f ) { LimitPSEnergy = f; update(); };
  QStringList getSelectedElms( void );
  double *getMCAEnergys( void ) { return E; };

public slots:
  void setROI( int s, int e );   // MCA pixel
  void setLog( bool f ) { dispLog = f; update(); };
  void setShowElements( bool show ) { showElements = show; update(); };
  void setShowElementsAlways( bool show ) { showElementsAlways = show; update(); };
  void setShowElementsEnergy( bool show ) { showElementsEnergy = show; update(); };
  //  void doPeakFit( void );
  void NewEnergy( double e ) { I0Energy = e; };

signals:
  void CurrentValues( int atCur, int inROI );
  void newROI( int ROIstart, int ROIend );
  void newROIinEng( double s, double e );
  void newPeakList( QVector<MCAPeak>* );
};

#endif
