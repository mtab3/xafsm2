#ifndef SELECTCH_H
#define SELECTCH_H

#include "ui_SelectCh.h"

//const QString SSDActive = "background-color: #f0f0d0"; //  XafsM.h にあるこの記述は
//const QString SSDnotActive = "";                       //  最終的には消す

const QString ActiveStyle = "background-color: #f0f0d0";
const QString inActiveStyle = "";

enum TYPES { SSD19CH, SDD7CH };

class SelectCh : public QWidget, private Ui::SelectCh
{
  Q_OBJECT

  bool excMode;
  TYPES type;
  QVector<QPushButton *> Bs0, Bs;

 private slots:
  void clicked( void );
  
 public:
  SelectCh( QWidget *p = NULL );

  void setExcl( bool f ) { excMode = f; selACh( 0 ); };
  void setType( TYPES type );
  void setTitle( QString title ) { Title->setText( title ); };
  bool isSelected( int i );
  void selACh( int i );
  int chs( void ) { return Bs.count(); };

 signals:
  void selectedACh( int i, bool f );
  void selectedSingleCh( int i );
};

#endif
