#include <QtGui>

#include "MainWindow.h"

void MainWindow::setupLogArea( void )   /* ログエリア */
{
  LogFileName->setText( NewLFName() );
  LogMsgs->setReadOnly( true );
  SelLFND = new QFileDialog;
  SelLFND->setAcceptMode( QFileDialog::AcceptSave );
  SelLFND->setDirectory( QDir::currentPath() );

  connect( LogFileSelect, SIGNAL( clicked() ), this, SLOT( SelLFN() ) );
  connect( SelLFND, SIGNAL( fileSelected( const QString & ) ),
			    this, SLOT( SetNewLFName( const QString & ) ) );
  connect( LogComment, SIGNAL( editingFinished() ), this, SLOT( AddLogComment() ) );
}

void MainWindow::NewLogMsg( QString msg )
{
  if ( msg.simplified().isEmpty() )
    return;
  if ( ! msg.endsWith( "\n" ) )
    msg += "\n";

  msg = QDateTime::currentDateTime().toString( "hh:mm:ss> " ) + msg;
  
  LogMsgs->insertPlainText( msg );
  LogMsgs->moveCursor( QTextCursor::End );

  QFile file( LogFileName->text() );
  if ( !file.open( QIODevice::Append ) ) {
    statusbar->showMessage( tr( "Cannot open Log File [%1]\n" )
			    .arg( LogFileName->text() ) );
    return;
  }
  QTextStream out( &file );
  out << msg;
  file.close();
}

QString MainWindow::NewLFName( void )
{
  //  これは、日時以下、時分秒までログファイル名にするので、少し鬱陶しい。
  //  return QDateTime::currentDateTime().toString( "Log-yyMMdd-hhmmss" ) + ".dat";

  //  こっちだと、デフォルトのログファイルは1日1個
  return QDateTime::currentDateTime().toString( "Log-yyMMdd" ) + ".dat";
}

void MainWindow::SelLFN( void )
{
  SelLFND->selectFile( NewLFName() );
  SelLFND->show();
}

void MainWindow::SetNewLFName( const QString &name )
{
  LogFileName->setText( name );
}

void MainWindow::AddLogComment( void )
{
  NewLogMsg( LogComment->text() );
  LogComment->setText( "" );
}
