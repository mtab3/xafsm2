#ifndef VIEWCTRL_H
#define VIEWCTRL_H

#include <QObject>
#include <QWidget>
#include <QLayout>

#include "XView.h"
#include "MCAView.h"

enum VTYPE { XVIEW, MCAVIEW, NONVIEW };

class ViewCTRL : public QObject
{
  Q_OBJECT

  QWidget* ViewBase;
  void* nowView;
  VTYPE nowVType;
  bool deletable;

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
};


#endif
