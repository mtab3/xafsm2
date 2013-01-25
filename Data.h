#ifndef DATA_H
#define DATA_H

#include <QFileDialog>
#include <QColorDialog>

#include "ui_Data.h"

class ViewCTRL;

enum DATATYPE { MEASDATA, SCANDATA, MONDATA, MCADATA, NONDATA };

class Data : public QFrame, private Ui::Data
{
  Q_OBJECT

 private:
  QFileDialog *FSDialog;
  QVector<QLabel*> DColors;

  void CheckFileType( const QString &fname );
  DATATYPE dataType;
  QStringList DataTypeNames;

  void showMeasData( QTextStream &in, ViewCTRL *view );
  void showScanData( QTextStream &in, ViewCTRL *view );
  void showMonData( QTextStream &in, ViewCTRL *view );
  void showMCAData( QTextStream &in, ViewCTRL *view );

 public:
  Data( QWidget *p = NULL );
  ~Data();

 private slots:
  void ShowFName( const QString &fname );
  void StartToShowData( void );
  void GotNewView( QObject *to, ViewCTRL *view );

 public slots:
  void SetColor( int i, const QColor &selcolor );

 signals:
  void AskToGetNewView( DATATYPE dtype );
  void showMessage( QString msg, int time );
};

#endif
