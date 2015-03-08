#ifndef DATA_H
#define DATA_H

#include <QFileDialog>
#include <QColorDialog>

#include "AUnits/ASensor.h"
#include "AUnits/AMotor.h"

#include "ui_Data.h"
#include "global.h"
#include "Units.h"
#include "S2DInfo.h"
#include "S2DB.h"
#include "SetUpSFluo.h"

class ViewCTRL;
class XYView;
class TYView;
class MCAView;
class S2DView;

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
  QVector<SetUpSFluo*> *SSFluos;

  void CheckFileType( const QString &fname );
  DATATYPE dataType;
  //  QStringList Checks;
  QString FName;

  ViewCTRL *theViewC;
  XYView *theXYView;
  TYView *theTYView;
  MCAView *theMCAView;
  S2DView *theS2DView;
  S2DB *theS2DB;
  int XYLine0, XYLines; 

  bool MCADataIsValid;
  int cMCACh;
  quint32 *cMCA;
  //  QVector<quint32 *> MCAs0, MCAs;
  //  QVector<double *> MCAEs;
  aMCASet aMCA0, aMCA;
  S2DInfo S2DI;

  //  void showMeasData( QTextStream &in );
  void showScanData( QTextStream &in,
		     QVector<AMotor*> &AMotors, QVector<ASensor*> &ASensors );
  void showMonData( QTextStream &in, QVector<ASensor*> &ASensors );
  void showMCAData( QTextStream &in );
  void getNewMCAs( int length );
  void showS2DData( QTextStream &in,
		    QVector<AMotor*> &AMotors, QVector<ASensor*> &ASensors );

 public:
  Data( QWidget *p = NULL );
  ~Data();
  void GotNewView( ViewCTRL *view,
		   QVector<AMotor*> &AMotors, QVector<ASensor*> &ASensors );
  void GotCurrentView( void *view );
  void setDataRoot( const QString &dataRoot );
  void setSSFluos( QVector<SetUpSFluo*> *sSFluos ) { SSFluos = sSFluos; };

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
