#ifndef DATA_H
#define DATA_H

#include <QFileDialog>
#include <QColorDialog>

#include "ui_Data.h"
#include "Units.h"

class ViewCTRL;
class XYView;
class TYView;
class MCAView;

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

  ViewCTRL *theViewC;
  XYView *theXYView;
  TYView *theTYView;
  MCAView *theMCAView;
  int XYLine0, XYLines; 

  int MCALength;
  bool MCADataIsValid;
  int cMCACh;
  quint32 *cMCA;
  QVector<quint32 *> MCAs0, MCAs;
  QVector<double *> MCAEs;

  void showMeasData( QTextStream &in );
  void showScanData( QTextStream &in );
  void showMonData( QTextStream &in );
  void showMCAData( QTextStream &in );
  void getNewMCAs( int length );

 public:
  Data( QWidget *p = NULL );
  ~Data();
  void GotNewView( ViewCTRL *view );
  void GotCurrentView( void *view );
  void setDataRoot( const QString &dataRoot );

 private slots:
  void ShowFName( const QString &fname );
  void StartToShowData( void );

 public slots:
  void SetColor( int i, const QColor &selcolor );
  void callCSDialog( void );
  void newColorSelected( const QColor &c );
  void SelectedNewMCACh( int ch );

 signals:
  void AskToGetNewView( DATATYPE dtype );
  void showMessage( QString msg, int time );
  void GiveMeCurrentView( void );
};

#endif
