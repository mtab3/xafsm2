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

  QFileDialog *MCAsDirSel;
  QString mcaMapDir;
  aMCAMap mcaMap;
  S2DInfo S2DI;

 public:
  S2DB( QWidget *p );

  S2DView *getView( void ) { return S2DV; };
  void setLoadBHidden( bool f ) { LoadMCAsBFrame->setHidden( f ); };
  void setDataLoot( QString root ) { MCAsDirSel->setDirectory( root ); };
  void gotNewMCAView( MCAView *mcav );
  void setS2DI( S2DInfo s2di ) { S2DI = s2di; };

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

signals:
  void askToGetNewMCAView( S2DB *s2db );
#if 0
  void print( void );
  void popup( void );
#endif
};

#endif
