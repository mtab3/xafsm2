#ifndef S2DB_H
#define S2DB_H

#include <QFrame>
#include <QFileDialog>

#include "ui_S2DB.h"

class S2DB : public QFrame, private Ui::S2DB
{
  Q_OBJECT

  QFileDialog *MCAsDirSel;

 public:
  S2DB( QWidget *p );

  S2DView *getView( void ) { return S2DV; };
  void setLoadBHidden( bool f ) { LoadMCAsB->setHidden( f ); };
  void setDataLoot( QString root ) { MCAsDirSel->setDirectory( root ); };

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
#if 0
  void print( void );
  void popup( void );
#endif
};

#endif
