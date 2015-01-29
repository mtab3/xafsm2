#ifndef DATA_H
#define DATA_H

#include <QFileDialog>
#include <QColorDialog>

#include "ui_Data.h"
#include "Units.h"
#include "S2DInfo.h"
#include "S2DB.h"

class ViewCTRL;
class XYView;
class TYView;
class MCAView;
class S2DView;

enum DATATYPE { MEASDATA, SCANDATA, MONDATA, MCADATA, S2DDATA,
		MEASSHOW, SCANSHOW, MONSHOW, MCASHOW, S2DSHOW, NONDATA };

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
  QStringList Checks;
  QString FName;

  ViewCTRL *theViewC;
  XYView *theXYView;
  TYView *theTYView;
  MCAView *theMCAView;
  S2DView *theS2DView;
  S2DB *theS2DB;
  int XYLine0, XYLines; 

  int MCALength;
  bool MCADataIsValid;
  int cMCACh;
  quint32 *cMCA;
  //  QVector<quint32 *> MCAs0, MCAs;
  //  QVector<double *> MCAEs;
  aMCASet aMCA0, aMCA;
  S2DInfo S2DI;

  //  void showMeasData( QTextStream &in );
  void showScanData( QTextStream &in );
  void showMonData( QTextStream &in );
  void showMCAData( QTextStream &in );
  void getNewMCAs( int length );
  void showS2DData( QTextStream &in, QVector<AUnit*> &AMotors );

 public:
  Data( QWidget *p = NULL );
  ~Data();
  void GotNewView( ViewCTRL *view, QVector<AUnit*> &AMotors );
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
  void AskToGetNewView( DATATYPE dtype, QString dir );
  void showMessage( QString msg, int time );
  void GiveMeCurrentView( void );
  void setMCACh( int i );
};

#endif
