#ifndef S2DB_H
#define S2DB_H

#include <QFrame>

#include "ui_S2DB.h"

class S2DB : public QFrame, private Ui::S2DB
{
  Q_OBJECT

  QFileDialog *S2DMCAsDirSel;

 public:
  S2DB( QWidget *p, FileDialog *MCADirSel );

  S2DView *getView( void ) { return S2DV; };

public slots:
  void newZZ( QString max, QString min )
  {
    zmax->setText( max );
    zmin->setText( min );
  }

  void LoadS2DMCAs( const QString &fname );

private slots:
  void newInputZmax( void ) { CBar->newInputZmax( zmax->text().toDouble() ); };
  void newInputZmin( void ) { CBar->newInputZmin( zmin->text().toDouble() ); };

signals:
#if 0
  void print( void );
  void popup( void );
#endif
};

#endif
