#ifndef DATA_H
#define DATA_H

#include <QFileDialog>
#include <QColorDialog>

#include "ui_Data.h"
#include "Units.h"

class ViewCTRL;
class XYView;

enum DATATYPE { MEASDATA, SCANDATA, MONDATA, MCADATA, NONDATA };

class Data : public QFrame, private Ui::Data
{
  Q_OBJECT

 private:
  Units *u;

  int SettingL;
  QColor SettingC;
  QFileDialog *FSDialog;
  QColorDialog *CSDialog;
  QVector<QPushButton*> DColors;

  void CheckFileType( const QString &fname );
  DATATYPE dataType;
  QStringList DataTypeNames;
  QString FName;

  ViewCTRL *viewCtrl;
  XYView *view0;
  int XYLine0, XYLines; 

  void showMeasData( QTextStream &in, ViewCTRL *view );
  void showScanData( QTextStream &in, ViewCTRL *view );
  void showMonData( QTextStream &in, ViewCTRL *view );
  void showMCAData( QTextStream &in, ViewCTRL *view );

 public:
  Data( QWidget *p = NULL );
  ~Data();
  void GotNewView( ViewCTRL *view );
  void GotCurrentView( void *view );

 private slots:
  void ShowFName( const QString &fname );
  void StartToShowData( void );

 public slots:
  void SetColor( int i, const QColor &selcolor );
  void callCSDialog( void );
  void newColorSelected( const QColor &c );

 signals:
  void AskToGetNewView( DATATYPE dtype );
  void showMessage( QString msg, int time );
  void GiveMeCurrentView( void );
};

#endif
