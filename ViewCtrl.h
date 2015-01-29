#ifndef VIEWCTRL_H
#define VIEWCTRL_H

#include <QObject>
#include <QWidget>
#include <QLayout>

#include "Data.h"
#include "XYView.h"
#include "TYView.h"
#include "MCAView.h"
#include "S2DB.h"
#include "S2DView.h"
#include "GSBStats.h"

enum VTYPE { XYVIEW, TYVIEW, MCAVIEW, S2DVIEW, NONVIEW };

class ViewCTRL : public QObject
{
  Q_OBJECT

  QWidget* ViewBase;
  void* nowView;
  VTYPE nowVType;
  DATATYPE nowDType;
  bool deletable;
  GSBStats *gsbStat;

 public:
  ViewCTRL( void );
  ~ViewCTRL( void );

  void setViewBase( QWidget *base ) { ViewBase = base; };
  //  void setNowView( void *nowv ) { nowView = nowv; };
  //  void setNowVType( VTYPE nvtype ) { nowVType = nvtype; };
  bool setView( void *view, VTYPE vtype, DATATYPE dtype );
  void *getView( void ) { return nowView; };
  VTYPE getVType( void ) { return nowVType; };
  DATATYPE getDType( void ) { return nowDType; };
  bool deleteView( void );
  bool isDeletable( void ) { return deletable; };
  void setDeletable( bool Deletable ) { deletable = Deletable; };
  //  void setDType ( DATATYPE dtype ) { nowDType = dtype; };
  // //  DATATYPE getNowDType ( void ) { return nowDType; };
  void addAGSBStat( QString label, bool f );
  void setGSBStats( QVector<aGSBS> GSBSs );
  void setGSBStat( int button, bool f );
  QString getAGSBSLabel( int n );
  bool getAGSBSStat( int n );
  QStringList getGSBLabels( void );
  QVector<bool> getGSBFlags( void );
  //  void setView( QWidget *view );
  void layoutViewAgain( void );
};


#endif
