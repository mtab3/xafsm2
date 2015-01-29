#ifndef MCAVIEW_H
#define MCAVIEW_H

#include <QWidget>
#include <QPrinter>

#include "ui_MCAView.h"
#include "MouseC.h"
#include "ChCoord.h"
#include "FluoDBase.h"
#include "KeV2Pix.h"
#include "MCAPeak.h"
#include "Gs.h"

enum MMODE { M_ROI, M_POINT, M_H_SHIFT, M_NO };

class MCAView : public QFrame, private Ui::MCAView
{
  Q_OBJECT

private:
  QWidget *Parent;

  //  PeakFit *PF;
  Gs *Fit;
  int fitMaxLoop;
  double fitDampFact;
  double fitPrec1, fitPrec2;

  int valid;
  //  QVecotot<int *> MCAs;
  quint32 *MCA;
  double *rMCA;
  double *SMCA;                          // スムージング結果
  double *DMCA, *DMCA2, *DMCA3, *DMCA4;  // 1〜3次微分
  double *dMCA;                          // 統計変動
  double *E;
  QVector<double> FittedLine;
  QVector< QVector<double> > EachLine;

  double MaxEnergy;
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

  bool popDock;

  MMODE mMode;
  ChCoord cc;
  double rROIsx, rROIex;   // ROI の範囲
  double MinE, MaxE;       // 表示の最小、最大エネルギー
  QVector<double> cPoints; // カーソル表示を残す点
  QStringList selectedAtoms;
  double yRatio;           // 縦軸の拡大倍率

  bool ShowDiff;
  bool ShowSmoothed;
  bool DoPeakSearch;
  bool DoPeakFitToRaw;
  bool LimitPSEnergy;      // ピークサーチを I0 のエネルギーまででやめる。
  double I0Energy;         // その I0 のエネルギー保持
  double PSSens;           // ピークサーチの感度
  QVector<MCAPeak> MCAPeaks;

  int nearX;
  bool nearf;

  QColor Black, White;
  QColor ROIRangeC, ExROIRangeC, ROIEdgeC;
  QColor GridC, MCursorC, MCursorC2, MCursorC3, AListC;
  QColor SMCAC, DMCAC, DMCAC2, DMCAC3;
  QColor PEAKPOINTC, PEAKPOINTLC;
  QColor FLC, ELC, RLC;

  void Draw( QPainter *p );
  void PeakSearch( double Es, double Ee );

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
  void setPeakSearch( bool f ) { DoPeakSearch = f; update(); };
  void setShowSmoothed( bool f ) { ShowSmoothed = f; update(); };
  void setFitToRaw( bool f ) { DoPeakFitToRaw = f; update(); };
  void setLimitPSEnergy( bool f ) { LimitPSEnergy = f; update(); };
  void setMaxEnergy( double e ) { MaxEnergy = e; };
  void setMaxLoop( int L ) { fitMaxLoop = L; };
  void setDampFact( double damp ) { fitDampFact = damp; };
  void setPrec1( double prec1 ) { fitPrec1 = prec1; };
  void setPrec2( double prec2 ) { fitPrec2 = prec2; };
  QStringList getSelectedElms( void );
  double *getMCAEnergys( void ) { return E; };
  void print( QPrinter *p );
  KeV2Pix *keV2Pix( void ) { return k2p; };

  QVector<MCAPeak> *getMCAPeaks( void ) { return &MCAPeaks; };
  int getMCALength( void ) { return MCALen; };
  double *getSMCA( void ) { return SMCA; };
  void doPeakFitWCPoints( void );
  void newFit( int peaks );
  void setFittedLines( int peaks, double *E );
  void setMCAPeaksByFit( void );

public slots:
  void setROI( int s, int e );   // MCA pixel
  void setLog( bool f ) { dispLog = f; update(); };
  void setShowElements( bool show ) { showElements = show; update(); };
  void setShowElementsAlways( bool show ) { showElementsAlways = show; update(); };
  void setShowElementsEnergy( bool show ) { showElementsEnergy = show; update(); };
  //  void doPeakFit( void );
  void NewEnergy( double e ) { I0Energy = e; };
  void clearMCAPeaks( void );

signals:
  void CurrentValues( int atCur, int inROI );
  void newROI( int ROIstart, int ROIend );
  void newROIinEng( double s, double e );
  void newPeakList( QVector<MCAPeak>* );
  void popDockIsChanged( bool popDock );
};

#endif
