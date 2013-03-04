#ifndef VIEWCTRL_H
#define VIEWCTRL_H

#include <QObject>
#include <QWidget>
#include <QLayout>

#include "Data.h"
#include "XYView.h"
#include "TYView.h"
#include "MCAView.h"

enum VTYPE { XYVIEW, TYVIEW, MCAVIEW, NONVIEW };

class ViewCTRL : public QObject
{
  Q_OBJECT

  QWidget* ViewBase;
  void* nowView;
  VTYPE nowVType;
  DATATYPE nowDType;
  bool deletable;

  QStringList BNames;

 public:
  ViewCTRL( void );

  void setViewBase( QWidget *base ) { ViewBase = base; };
  void setNowView( void *nowv ) { nowView = nowv; };
  void setNowVType( VTYPE nvtype ) { nowVType = nvtype; };
  bool setView( void *view, VTYPE vtype );
  void *getView( void ) { return nowView; };
  bool deleteView( void );
  bool isDeletable( void ) { return deletable; };
  void setIsDeletable( bool Deletable ) { deletable = Deletable; };
  void setNowDType ( DATATYPE dtype ) { nowDType = dtype; };
  DATATYPE getNowDType ( void ) { return nowDType; };
  void setBNames( QStringList bn ) { BNames = bn; };
  QStringList getBNames( void ) { return BNames; };
};


#endif
