
#ifndef MONLINE_H
#define MONLINE_H

#include <QObject>
#include <QRadioButton>
#include <QComboBox>
#include <QLabel>

class MonLine : public QObject
{
  Q_OBJECT

  QRadioButton *Sel;
  QComboBox *Dev;
  QLabel *Val;

 public:
  MonLine();
  MonLine( QRadioButton *s, QComboBox *d, QLabel *v );

  QRadioButton *sel( void ) { return Sel; };
  QComboBox *dev( void ) { return Dev; };
  QLabel *val( void ) { return Val; };

  QString selStyle( void ) { return Sel->styleSheet(); };
  QString devStyle( void ) { return Dev->styleSheet(); };
  QString valStyle( void ) { return Val->styleSheet(); };
  QLabel::Shape valShape( void ) { return Val->frameShape(); };
  Qt::Alignment valAlign( void ) { return Val->alignment(); };
  
  bool isChecked( void ) { return Sel->isChecked(); };
  int currentIndex( void ) { return Dev->currentIndex(); };
  QString text( void ) { return Val->text(); };
  void addItem( QString name ) { Dev->addItem( name ); };
  void setText( QString text ) { Val->setText( text ); };

  
  void copyStyles( MonLine &m );
};

#endif
