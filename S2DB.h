#ifndef S2DB_H
#define S2DB_H

#include <QFrame>
#include <QFileDialog>

#include "MCA.h"
#include "S2DInfo.h"
#include "MCAView.h"
#include "ui_S2DB.h"

class S2DB : public QFrame, private Ui::S2DB
{
  Q_OBJECT

  QWidget *parent;
  QFileDialog *MCAsDirSel;
  QString mcaMapDir;
  aMCAMap mcaMap;
  S2DInfo S2DI;
  bool Read;

 public:
  S2DB( QWidget *p );

  S2DView *getView( void ) { return S2DV; };
  void setParent( QWidget *p );
  void setLoadBHidden( bool f ) { LoadMCAsBFrame->setHidden( f ); };
  void setRead( bool f ) { Read = f; };
  void setDataRoot( QString root ) { MCAsDirSel->setDirectory( root ); };
  void gotNewMCAView( MCAView *mcav, int length, int chs );
  void setS2DI( S2DInfo s2di ) { S2DI = s2di; };

  void mapNew( int ix, int iy, int l, int chs ) { mcaMap.New( ix, iy, l, chs ); };
  aMCASet *mapAPoint( int ix, int iy ) { return mcaMap.aPoint( ix, iy ); };
  //  aMCAMap *getMCAMap( void ) { return &mcaMap; };

  double ReCalcAMapPoint( QString fname );
  double ReCalcAMapPointOnMem( int ix, int iy );
  QFileInfo GenerateMCAFileName( QDir dir, QString base, bool use3rdAx,
				    int i1, int i2, int i3 );
  
public slots:
  void newZZ( QString max, QString min )
  {
    zmax->setText( max );
    zmin->setText( min );
  }

private slots:
  void newInputZmax( void ) { CBar->newInputZmax( zmax->text().toDouble() ); };
  void newInputZmin( void ) { CBar->newInputZmin( zmin->text().toDouble() ); };
  void LoadMCAs( const QString &fname );
  void ShowInfoAtNewPosition( int ix, int iy );
  void ShowIntMCA( void );
  void ReCalcMap( bool onMem, QDir dir, QString base, bool use3rdAx );

signals:
  void askToGetNewMCAView( S2DB *s2db );
  //  void PointerMovedToNewPosition( int ix, int iy, aMCAMap *map );
  //  void PointerMovedOnIntMCA( int ix, int iy, aMCAMap *map );
  void ShowMCASpectrum( aMCASet *set1, aMCASet *set2 );
#if 0
  void print( void );
  void popup( void );
#endif
};

#endif
