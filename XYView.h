#ifndef XYVIEW_H
#define XYVIEW_H

#include <QWidget>
#include "ui_XView.h"

#include "MouseC.h"
#include "ChCoord.h"

#define MAXPOINTS ( 10000 )
#define MAXLINES  ( 30 )

enum LINEF { NODRAW, LEFT, RIGHT, LINEFS };
enum SCALET { FULLSCALE, I0TYPE, SCALETS }; 
enum LRAX { LEFT_AX, RIGHT_AX };

class XYView : public QFrame, private Ui::XView
{
  Q_OBJECT

private:
  ChCoord cc;
  bool autoScale;

  QColor bgColor, BLACK;
  QVector<QColor> LC;
  QColor MCLineC;          // mouse cursor line color
  QColor ASelC;

  LRAX LineLR[ MAXLINES ];
  SCALET scaleType[ MAXLINES ];
  QString LeftName, RightName;
  QVector<QString> LNames;
  QString XName;
  QString XUnitName;
  bool valid;
  double upp;
  double center;
  bool AreaSelecting;
  int SelLR[2];

  double origMinx, origMaxx;
  double XShift, XShift0, xshift;
  double YShift[ MAXLINES ], YShift0[ MAXLINES ], yshift[ MAXLINES ];

  int lines;
  bool dispf[ MAXLINES ];
  int points[ MAXLINES ];
  double x[ MAXLINES ][ MAXPOINTS ];
  double y[ MAXLINES ][ MAXPOINTS ];
  double miny[ MAXLINES ];
  double maxy[ MAXLINES ];
  double SaveYatNowXp[ MAXLINES ];

  MouseC m;
  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );
  void wheelEvent( QWheelEvent *e );

public:
  XYView( QWidget *parent = NULL );

  void NewPoint( int l, double xx, double yy );
  void Clear( void );
  // XYView では、複数の線を同時に描けるが、軸として右軸、左軸どちらを使うかを選択する
  void SetLR( int L, LRAX lr ) { LineLR[ L ] = lr; };
  // スケールのタイプ : フルスケールにするか、I0 の様に少し上にずらすか
  // ループ単位で指定。
  void SetScaleType( int l, SCALET t ) { scaleType[ l ] = t; };
  void SetColor( int l, QColor c ) { LC[ l ] = c; };
  QColor GetColor( int l ) { return LC.at( l ); };

  // 縦軸は左右一本ずつ合計 2本しかないので、縦軸を持てるグループは最大 2 つだけ。
  void SetLineName( int l, QString Name ) { LNames[l] = Name; };
  void SetLeftName( QString Name ) { LeftName = Name; };
  void SetRightName( QString Name ) { RightName = Name; };
  void SetXName( QString Name ) { XName = Name; };
  void SetXUnitName( QString name ) { XUnitName = name; };
  void makeValid( bool v = true ) { valid = v; };
  int GetPoints( int l );
  int GetLines( void ) { return lines; };
  double GetX( int l, int p );
  double GetY( int l, int p );
  double *GetXp( int l ) { return x[l]; };
  double *GetYp( int l ) { return y[l]; };
  void SetWindow( double x1, double y1, double x2, double y2 )
  { cc.SetRealCoord( x1, y1, x2, y2 ); };
  void SetWindow0( double x1, double y1, double x2, double y2 )
    { origMinx = x1; origMaxx = x2; cc.SetRealCoord0( x1, y1, x2, y2 ); };
  void SetUpp( double upp0 ) { upp = upp0; };
  void SetCenter( double center0 ) { center = center0; };
  void SetAutoScale( bool ascale ) { autoScale = ascale; };
  void SetLLine( int l ) { SelLR[ LEFT_AX ] = l; };
  void SetRLine( int l ) { SelLR[ RIGHT_AX ] = l; };

public slots:
  void ChooseAG( int i, bool f );

private:
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );
  void UpDateYWindow( int g, SCALET s );
  void ShowAScaleButton( QPainter *p );
  void CheckASPush( void );
};

#endif
