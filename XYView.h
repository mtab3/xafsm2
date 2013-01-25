#ifndef XYVIEW_H
#define XYVIEW_H

#include <QWidget>
#include "ui_XView.h"

#include "MouseC.h"
#include "ChCoord.h"

#define MAXPOINTS ( 10000 )
#define MAXLINES  ( 30 )
#define MAXGRPS   ( 30 )

enum LINEF { NODRAW, LEFT, RIGHT, LINEFS };
enum SCALET { FULLSCALE, I0TYPE, SCALETS }; 

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

  int Groups;
  int LeftG, RightG;
  int LineG[ MAXLINES ];
  LINEF LineLR[ MAXGRPS ];
  SCALET ScaleType[ MAXGRPS ];
  QVector<QString> GNames;
  QString XName;
  QString XUnitName;
  bool valid;
  double upp;
  double center;
  bool AreaSelecting;

  double origMinx, origMaxx;
  double XShift, XShift0, xshift;
  double YShift[ MAXLINES ], YShift0[ MAXLINES ], yshift[ MAXLINES ];

  int lines;
  int points[ MAXLINES ];
  double x[ MAXLINES ][ MAXPOINTS ];
  double y[ MAXLINES ][ MAXPOINTS ];
  double miny[ MAXGRPS ];
  double maxy[ MAXGRPS ];
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
  void setGroups( int g ) { Groups = g; };
  int getGroups( void ) { return Groups; };
  // XYView では、複数の線を同時に描けるが、同じスケールで描くものをグループ分けする。
  void setLineG( int L, int g ) { LineG[ L ] = g; };
  int getLineG( int L ) { return LineG[ L ]; };
  // スケールのタイプ : フルスケールにするか、I0 の様に少し上にずらすか
  // ループ単位で指定。
  void setScaleType( int g, SCALET t ) { ScaleType[ g ] = t; };
  void setColor( int g, QColor c ) { LC.value( g ) = c; };
  QColor getColor( int g ) { return LC.at( g ); };

  // 縦軸は左右一本ずつ合計 2本しかないので、縦軸を持てるグループは最大 2 つだけ。
  void SetLGroup( int leftG = 0 ) { LeftG = leftG; };
  void SetRGroup( int rightG = 1 ) { RightG = rightG; };
  void SetGName( int g, QString Name ) { GNames[g] = Name; }; // 多分間違ってる
  void SetXName( QString Name ) { XName = Name; };
  void SetXUnitName( QString name ) { XUnitName = name; };
  void makeValid( bool v = true ) { valid = v; };
  int getPoints( int l );
  int getLines( void ) { return lines; };
  double getX( int l, int p );
  double getY( int l, int p );
  void SetWindow( double x1, double y1, double x2, double y2 )
  { cc.SetRealCoord( x1, y1, x2, y2 ); };
  void SetWindow0( double x1, double y1, double x2, double y2 )
    { origMinx = x1; origMaxx = x2; cc.SetRealCoord0( x1, y1, x2, y2 ); };
  void setUpp( double upp0 ) { upp = upp0; };
  void setCenter( double center0 ) { center = center0; };
  void setAutoScale( bool ascale ) { autoScale = ascale; };

public slots:

private:
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );
  void UpDateYWindow( int g, SCALET s );
  void ShowAScaleButton( QPainter *p );
  void CheckASPush( void );
};

#endif
