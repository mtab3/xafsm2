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
  QColor ASBBorderC, ASBOnC, ASBOffC;

  int Groups;
  int LeftG, RightG;
  int LineG[ MAXLINES ];
  LINEF LineLR[ MAXGRPS ];
  SCALET ScaleType[ MAXGRPS ];
  QVector<QString> LNames;
  QString XName;
  QString XUnitName;
  bool valid;
  double upp;
  double center;

  int lines;
  int points[ MAXLINES ];
  double x[ MAXLINES ][ MAXPOINTS ];
  double y[ MAXLINES ][ MAXPOINTS ];
  double miny[ MAXLINES ];
  double maxy[ MAXLINES ];

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
  void SetLGroups( int g ) { Groups = g; };
  // XYView では、複数の線を同時に描けるが、同じスケールで描くものをグループ分けする。
  void SetLineG( int f00 = -1, int f01 = -1,  // default では 全部非表示
		 int f02 = -1, int f03 = -1, int f04 = -1, int f05 = -1,
		 int f06 = -1, int f07 = -1, int f08 = -1, int f09 = -1,
		 int f10 = -1, int f11 = -1, int f12 = -1, int f13 = -1,
		 int f14 = -1, int f15 = -1, int f16 = -1, int f17 = -1,
		 int f18 = -1, int f19 = -1, int f20 = -1, int f21 = -1,
		 int f22 = -1, int f23 = -1, int f24 = -1, int f25 = -1,
		 int f26 = -1, int f27 = -1, int f28 = -1, int f29 = -1 )
  {
    LineG[ 0] = f00; LineG[ 1] = f01; LineG[ 2] = f02; LineG[ 3] = f03; LineG[ 4] = f04;
    LineG[ 5] = f05; LineG[ 6] = f06; LineG[ 7] = f07; LineG[ 8] = f08; LineG[ 9] = f09;
    LineG[10] = f10; LineG[11] = f11; LineG[12] = f12; LineG[13] = f13; LineG[14] = f14;
    LineG[15] = f15; LineG[16] = f16; LineG[17] = f17; LineG[18] = f18; LineG[19] = f19;
    LineG[20] = f20; LineG[21] = f21; LineG[22] = f22; LineG[23] = f23; LineG[24] = f24;
    LineG[25] = f25; LineG[26] = f26; LineG[27] = f27; LineG[28] = f28; LineG[29] = f29;
  };
  // スケールのタイプ : フルスケールにするか、I0 の様に少し上にずらすか
  // これもグループ単位で指定。
  void SetScaleType( SCALET f00 = FULLSCALE, SCALET f01 = I0TYPE,
		     SCALET f02 = FULLSCALE, SCALET f03 = FULLSCALE,
		     SCALET f04 = FULLSCALE, SCALET f05 = FULLSCALE,
		     SCALET f06 = FULLSCALE, SCALET f07 = FULLSCALE,
		     SCALET f08 = FULLSCALE, SCALET f09 = FULLSCALE,
		     SCALET f10 = FULLSCALE, SCALET f11 = FULLSCALE,
		     SCALET f12 = FULLSCALE, SCALET f13 = FULLSCALE,
		     SCALET f14 = FULLSCALE, SCALET f15 = FULLSCALE,
		     SCALET f16 = FULLSCALE, SCALET f17 = FULLSCALE,
		     SCALET f18 = FULLSCALE, SCALET f19 = FULLSCALE,
		     SCALET f20 = FULLSCALE, SCALET f21 = FULLSCALE,
		     SCALET f22 = FULLSCALE, SCALET f23 = FULLSCALE,
		     SCALET f24 = FULLSCALE, SCALET f25 = FULLSCALE,
		     SCALET f26 = FULLSCALE, SCALET f27 = FULLSCALE,
		     SCALET f28 = FULLSCALE, SCALET f29 = FULLSCALE )
  {
    ScaleType[ 0] = f00; ScaleType[ 1] = f01;
    ScaleType[ 2] = f02; ScaleType[ 3] = f03; ScaleType[ 4] = f04; ScaleType[ 5] = f05;
    ScaleType[ 6] = f06; ScaleType[ 7] = f07; ScaleType[ 8] = f08; ScaleType[ 9] = f09;
    ScaleType[10] = f10; ScaleType[11] = f11; ScaleType[12] = f12; ScaleType[13] = f13;
    ScaleType[14] = f14; ScaleType[15] = f15; ScaleType[16] = f16; ScaleType[17] = f17;
    ScaleType[18] = f18; ScaleType[19] = f19; ScaleType[20] = f20; ScaleType[21] = f21;
    ScaleType[22] = f22; ScaleType[23] = f23; ScaleType[24] = f24; ScaleType[25] = f25;
    ScaleType[26] = f26; ScaleType[27] = f27; ScaleType[28] = f28; ScaleType[29] = f29;
  };

  // 縦軸は左右一本ずつ合計 2本しかないので、縦軸を持てるグループは最大 2 つだけ。
  void SetLRGroup( int leftG = 0, int rightG = 1 ) { LeftG = leftG; RightG = rightG; };
  void SetLineName( int i, QString Name ) { LNames.insert( i, Name ); }; // 多分間違ってる
  void SetXName( QString Name ) { XName = Name; };
  void SetXUnitName( QString name ) { XUnitName = name; };
  void makeValid( bool v = true ) { valid = v; };
  int getPoints( int l );
  double getX( int l, int p );
  double getY( int l, int p );
  void SetWindow( double x1, double y1, double x2, double y2 )
    { cc.SetRealCoord( x1, y1, x2, y2 ); };
  void SetWindow0( double x1, double y1, double x2, double y2 )
    { cc.SetRealCoord0( x1, y1, x2, y2 ); };
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
