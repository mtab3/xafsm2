#ifndef S2DB_H
#define S2DB_H

#include <QFrame>
#include <QFileDialog>
#include <QTimer>

#include "AUnits/MCA.h"
#include "S2DInfo.h"
#include "MCAView.h"
#include "KeV2Pix.h"
#include "S2DInfo.h"

#include "ui_S2DB.h"


class S2DB : public QFrame, private Ui::S2DB
{
  Q_OBJECT

  QWidget *parent;
  KeV2Pix *kev2pix;
  QFileDialog *MCAsDirSel;
  QString mcaMapDir;
  aMCAMap mcaMap;
  S2DInfo S2Di;
  bool Read;

  QTimer *mapLoadTimer;
  QFileInfoList LoadMCAFList;
  bool loadingAMCA;
  bool loadingMCAMap;
  
  QTimer *mapSaveTimer;
  QStringList SaveMCAFList;
  bool savingAMCA;
  bool savingMCAMap;

  QDialog *PopDialog;
  bool popping;
  QLayout *layout;

 public:
  S2DB( QWidget *p );
  ~S2DB( void );

  S2DView *getView( void ) { return S2DV; };
  void setParent( QWidget *p );
  void setLoadBHidden( bool f ) { LoadMCAsBFrame->setHidden( f ); };
  void setRead( bool f ) { Read = f; };
  void setDataRoot( QString root ) { MCAsDirSel->setDirectory( root ); };
  void getNewMCAMap( int length, int chs );
  void setS2DI( S2DInfo s2di ) { S2Di = s2di; };
  void setLayout( QLayout *l ) { layout = l; };

  void mapNew( int ix, int iy, int l, int chs ) { mcaMap.New( ix, iy, l, chs ); };
  aMCASet *mapAPoint( int ix, int iy ) { return mcaMap.aPoint( ix, iy ); };
  //  aMCAMap *getMCAMap( void ) { return &mcaMap; };

  double ReCalcAMapPointOnMem( int ix, int iy,
			       QString *RS, QString *RE,
			       QVector<QPushButton *> &ssdbs2 );

  void SaveMCAs( const QString &fname );
  
public slots:
  void newZZ( QString max, QString min )
  {
    zmax->setText( max );
    zmin->setText( min );
  }
  void print( QPrinter *p );

private slots:
  void newInputZmax( void ) { CBar->newInputZmax( zmax->text().toDouble() ); };
  void newInputZmin( void ) { CBar->newInputZmin( zmin->text().toDouble() ); };
  void LoadMCAs( const QString &fname );
  void ShowInfoAtNewPosition( int ix, int iy );
  void ShowIntMCA( void );
  void ReCalcMap( QString *RS, QString *RE, QVector<QPushButton*> &ssdbs2 );
  void loadNextMap( void );
  void saveNextMap( void );
  void PopUp( void );

signals:
  void askToGetNewMCAView( S2DB *s2db );
  //  void PointerMovedToNewPosition( int ix, int iy, aMCAMap *map );
  //  void PointerMovedOnIntMCA( int ix, int iy, aMCAMap *map );
  void ShowMCASpectrum( aMCASet *set1, aMCASet *set2 );
  void ShowMessage( QString msg, int time );
#if 0
  void print( void );
  void popup( void );
#endif
};

#endif
