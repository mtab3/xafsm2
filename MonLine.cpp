
#include "MonLine.h"

MonLine::MonLine( void )
{
  Sel = new QRadioButton;
  Dev = new QComboBox;
  Val = new QLabel;

  Sel->setAutoExclusive( false );
}

MonLine::MonLine( QRadioButton *s, QComboBox *d, QLabel *v )
{
  Sel = s;
  Dev = d;
  Val = v;
}

void MonLine::copyStyles( MonLine &m )
{
  Sel->setStyleSheet( m.selStyle() );
  Dev->setStyleSheet( m.devStyle() );
  Val->setStyleSheet( m.valStyle() );

  Val->setFrameShape( m.valShape() );
  Val->setAlignment( m.valAlign() );
  Val->setText( m.text() );
}
