#include <QtGui>
#include <math.h>

#include "MainWindow.h"

#define BLNAME    ( "BL5S1" )

int MainWindow::GetDFName0()
{
  DFName0 = EditDFName->text();

  if ( DFName0.isEmpty() ) {
    return 0;
  }

  if ( DFName0.toUpper().lastIndexOf( ".DAT" ) == DFName0.length() - 4 ) {
    DFName0 = DFName0.left( DFName0.length() - 4 );
  }

  return 1;
}

void MainWindow::SetDFName( int i )
{
  QString buf;

  if ( i == 0 ) {
    DFName = DFName0 + ".dat";
  } else {
    buf.sprintf( ".%03d", i );
    DFName = DFName0 + buf;
  }
}

QString MainWindow::fixS( QString s, int l )
{
  QString r;

  r.fill( ' ', l - s.size() );
  s += r;
  s.left( l );

  return s;
}

void MainWindow::WriteHeader( int Rpt )
{
  bool newf = conds->isUse1303Format();
  QString NFM = "";   // new file mark

  int cnt;
  SetDFName( Rpt );   // Generate a file name with repitation number
  
  QFile file( DFName );
  if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;
  
  // Writing fixed headers
  QTextStream out(&file);

  if ( newf ) {
    out << "##  1303" << endl;
    out << "### An extended 9809 format file." << endl;
    out << "### To convert the 1303 format file to 9809 format file," << endl;
    out << "###   1) When a line is started with single '#'," << endl;
    out << "###        the one character should be deleted," << endl;
    out << "###   2) When a line is started with '##' or '###'," << endl;
    out << "###        remove the line simply," << endl;
    out << "###   3) When '###' appears at the middle of a line," << endl;
    out << "###        the rest of the line including the '###' should be omitted.";
    out << endl;
    NFM = "#";
  }

  out << NFM << " " << fixS( " 9809     AichiSR", 20 ) << fixS( BLNAME, 5 ) << endl;

  out << NFM << " " << fixS( DFName, 14 ) << "  "
      << QDateTime::currentDateTime().toString("yy.MM.dd hh:mm") << " - "
      << "%001%" << endl; // 置換用のマーク 001 番目 14桁終了日時 "yy.MM.dd hh:mm"
  out << " " << fixS( EditCMT->text(), 80 ) << endl;

  out << NFM << " " << "Ring : " << "  1.2 GeV"
      << QString( "  %1 mA - " ).arg( SLS->value().toDouble(), 6, 'f', 1, ' ' )
      << "%002%"          // 置換用のマーク 002 番目 実数6.1リング電流
      << " mA" << endl;

  out << NFM << " " << "Mono :   " << fixS( mccd[ selmc->MC() ]->getMCName(), 10 )
      << QString( "    D= %1 A    " ).arg( mccd[ selmc->MC() ]->getD(), 8, 'f', 5, ' ' )
      << QString( "Initial angle=%1 deg" ).arg(u->keV2deg( InitialKeV ), 9, 'f', 5, ' ')
      << endl;

  out << NFM << " " << fixS( BLNAME, 5 ) << "    "
      << fixS( CMode[ MeasFileType ], 13 ) << QString( "(%1)" ).arg( MeasFileType, 2 )
      << QString( "   Repitition=%1" ).arg( MeasR + 1, 3 )
      << QString( "     Points=%1" ).arg( TP, 5 ) << endl;
  
  if ( SBLKUnit == DEG ) {
    out << NFM << " "
        << QString( "Param file : %1  angle axis (1)     Block =%2" )
           .arg( fixS( "DUMMYNAME.prm", 14 ) ).arg( Blocks, 2 ) << endl;
    out << endl;

    out << NFM << " "
        << "Block      Init-ang  final-ang     Step/deg     Time/s       Num" << endl;

    for ( int i = 0; i < Blocks; i++ ) {
      out << NFM << " "
          << QString( "%1     %2%3%4%5%6" )
             .arg( i+1, 5 )
             .arg( SBlockStart[i], 10, 'f', 5 )
             .arg( SBlockStart[i+1], 10, 'f', 5 )
             .arg( SBlockStep[i], 13, 'f', 6 )
             .arg( SBlockDwell[i], 11, 'f', 2 )
             .arg( SBlockPoints[i], 10 )
          << endl;
    }
  } else {
    out << NFM << " "
        << QString( "Param file : %1  energy axis (2)     Block =%2" )
           .arg( fixS( "DUMMYNAME.prm", 14 ) ).arg( Blocks, 2 ) << endl;
    out << endl;

    out << NFM << " "
        << "Block      Init-Eng  final-Eng     Step/eV     Time/s       Num" << endl;
    for ( int i = 0; i < Blocks; i++ ) {
      out << NFM << " "
          << QString( "%1     %2%3%4%5%6" )
             .arg( i+1, 5 )
             .arg( u->any2keV( SBLKUnit, SBlockStart[i] ) * 1000, 10, 'f', 2 )
             .arg( u->any2keV( SBLKUnit, SBlockStart[i+1] ) * 1000, 10, 'f', 2 )
             .arg( u->any2keV( SBLKUnit, SBlockStep[i] ) * 1000, 13, 'f', 2 )
             .arg( SBlockDwell[i], 11, 'f', 2 )
             .arg( SBlockPoints[i], 10 )
          << endl;
    }
  }
  out << NFM << " " << QString( "SCALE( 2)     NDCH =%1" ).arg( MeasChNo, 2 ) << endl;

  QVector<double> darks;
  switch( MeasFileType ) {
  case TRANS:
    out << NFM << "  Angle(c)  Angle(o)    time/s";
    for ( int i = 0; i < MeasChNo; i++ )
      out << QString( "%1" ).arg( i+1, 10 );
    out << endl;

    out << NFM << QString( "      Mode         0         0"
                           "%1%2" ).arg( 1, 10 ).arg( 2, 10 ) << endl;

    out << NFM << QString( "    Offset         0         0"
                           "%1%2" ).arg( mUnits.at(0)->getDark(), 10, 'f', 3 )
           .arg( mUnits.at(1)->getDark(), 10, 'f', 3 ) << endl;
    break;

  case FLUO:    // FLUO と EXTRA は一度は同じ(一つ)になったのに、
    // I0 の位置を変えないといけないことが判明。なのでまた分離。
    out << NFM << "  Angle(c)  Angle(o)    time/s";
    // FLUO の時 mUnits の要素の並びは必ず I0, 19ch SSD になってるはず
    for ( int j = 0; j < MaxSSDs; j++ ) {
      out << QString( "%1" ).arg( j+1, 10 );  // 19ch SSD
    }
    out << QString( "%1" ).arg( MaxSSDs + 1, 10 );    // I0
    for ( int j = 0; j < MaxSSDs; j++ ) {
      // ICR  19ch SSD の番号とそろえる
      out << QString( "%1" ).arg( j+1, 10 );
    }
    out << QString( "%1" ).arg( MaxSSDs + 1, 10 );    // resets
    out << endl;

    out << NFM << QString( "      Mode         0         0" );    // Modes Line
    for ( int j = 0; j < MaxSSDs; j++ ) {
      out << QString( "%1" ).arg( FLUO, 10 );  // 19ch SSD
    }
    out << QString( "%1" ).arg( 1, 10 );       // I0 : I0 は決め打ちで 1
    for ( int j = 0; j < MaxSSDs; j++ ) {
      out << QString( "%1" ).arg( FLUO + 100, 10 );  // ICR
    }
    out << QString( "%1" ).arg( 101, 10 );     // resets
    out << endl;

    out << NFM << QString( "    Offset         0         0" ); //Offsets Line(per socond)
    darks = SFluo->getDarkCountsInROI();
    for ( int j = 0; j < MaxSSDs; j++ ) {            // 19ch SSD -- in ROI
      out << QString( "%1" ).arg( darks[j], 10, 'f', 3 );
    }
    out << QString( "%1" ).arg( mUnits.at(0)->getDark(), 10, 'f', 3 ); // I0
    darks = SFluo->getDarkICRs();
    for ( int j = 0; j < MaxSSDs; j++ ) {             // 19ch SSD -- ICR
      out << QString( "%1" ).arg( darks[j], 10, 'f', 3 );
    }
    out << QString( "%1" ).arg( 0., 10, 'f', 3 );      // リセット回数 : 0 にしてる !!
    out << endl;

    break;

    // EXTRA は面倒くさい。
    // (特に MeasDispMode は描画の方でも使うので 19 ch 分拡張するわけにいかない)
    // Use19chSSD->isChecked() == true なら、19ch SSD を使う。
    // その時 UseI1->isChecked() == true かどうかで
    // 19ch 分のデータが展開される位置が変わる。
    // 具体的に考えてみると
    // 19ch SSD : No.   簡単
    //    MeasChNo 分 i ループを回して、
    //    素直に MeasDispMode[i] とか Offsets[i] とかを書けばいい
    // 19ch SSD : Yes.
    //   I1 : No.
    //     角度、角度、時間、I0、19ch 分展開、19ch分 ICR、リセット回数、その他残り
    //   I1 : Yes.
    //     角度、角度、時間、I0、I1、19ch 分展開、19ch分 ICR、リセット回数、その他残り
    // になる。

  case EXTRA:
    out << NFM << "  Angle(c)  Angle(o)    time/s";
    cnt = 1;
    for ( int i = 0; i < mUnits.count(); i++ ) {
      if ( mUnits.at(i) != SFluo ) {
        out << QString( "%1" ).arg( cnt, 10 );
      } else {
        for ( int j = 0; j < MaxSSDs; j++ ) {
          out << QString( "%1" ).arg( cnt, 10 );  // 19ch SSD
          cnt++;
        }
        out << QString( "%1" ).arg( 0, 10 );    // resets
        cnt++;
        for ( int j = 0; j < MaxSSDs; j++ ) {
          out << QString( "%1" ).arg( cnt - MaxSSDs - 2, 10 );
          // ICR  19ch SSD の番号とそろえる
          cnt++;
        }
        cnt--;
      }
      cnt++;
    }
    out << endl;

    out << NFM << QString( "      Mode         0         0" );    // Modes Line
    for ( int i = 0; i < mUnits.count(); i++ ) {
      if ( mUnits.at(i) != SFluo ) {
        out << QString( "%1" ).arg( MeasDispMode[i], 10 );
      } else {
        for ( int j = 0; j < MaxSSDs; j++ ) {
          out << QString( "%1" ).arg( FLUO, 10 );  // 19ch SSD
        }
        out << QString( "%1" ).arg( 5, 10 );    // resets
        for ( int j = 0; j < MaxSSDs; j++ ) {
          out << QString( "%1" ).arg( 5, 10 );  // ICR
        }
      }
    }
    out << endl;

    out << NFM << QString( "    Offset         0         0" ); //Offsets Line(per socond)
    for ( int i = 0; i < mUnits.count(); i++ ) {
      if ( mUnits.at(i) != SFluo ) {
        out << QString( "%1" ).arg( mUnits.at(i)->getDark(), 10, 'f', 3 );
      } else {
        QVector<double> darks;
        darks = SFluo->getDarkCountsInROI();
        for ( int j = 0; j < MaxSSDs; j++ ) {            // 19ch SSD -- in ROI
          out << QString( "%1" ).arg( darks[j], 10, 'f', 3 );
        }
        out << QString( "%1" ).arg( 0., 10, 'f', 3 );      // リセット回数 : 0 にしてる !!
        darks = SFluo->getDarkICRs();
        for ( int j = 0; j < MaxSSDs; j++ ) {             // 19ch SSD -- ICR
          out << QString( "%1" ).arg( darks[j], 10, 'f', 3 );
        }
      }
    }
    out << endl;

    break;
  default:
    qDebug() << "Unknown Measuremet type";
  }

  if ( newf ) {
    out << "## Channel Names:";
    for ( int i = 0; i < mUnits.count(); i++ )
      out << QString( " \"%1\"" ).arg( mUnits.at(i)->getName() );
    out << endl;
    
    out << "## Select Button Names:";
    for ( int i = 0; i < GSBs.count(); i++ )
      out << QString( " \"%1\"" ).arg( GSBs[i]->text() );
    out << endl;
    
    out << "## Select Button State:";
    for ( int i = 0; i < GSBs.count(); i++ )
      out << QString( " \"%1\"" ).arg( GSBs[i]->isChecked() );
    out << endl;

    if ( isSFluo ) {
      out << "## Sum up channel : " << SFluoLine << " :";
      for ( int i = 0; i < 19; i++ ) {
        out << " " << SFluoLine + i + 1;
      }
      out << endl;
    }
  }

  file.close();
}

void MainWindow::WriteHeader2( int Rpt )
// スキャン終了時、終了時でないと決まらない情報をヘッダに書き加える。
{
  SetDFName( Rpt );   // Generate a file name with repitation number

  QFile file( DFName );
  if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return;

  // Writing fixed headers
  QTextStream in(&file);

  QStringList AllLines;
  QString line;
  int pos;

  while ( !in.atEnd() ) {
    line = in.readLine();
    if ( ( pos = line.indexOf( "%001%" ) ) > 0 ) {
      line = line.left( pos )
          + QDateTime::currentDateTime().toString("yy.MM.dd hh:mm")
          + line.mid( pos + 5 );
    }
    if ( ( pos = line.indexOf( "%002%" ) ) > 0 ) {
      line = line.left( pos )
          + QString( "%1" ).arg( SLS->value().toDouble(), 6, 'f', 1, ' ' )
          + line.mid( pos + 5 );
    }
    AllLines << line;
  }

  file.close();

  QFile file2( DFName );
  if ( !file2.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;

  // Writing fixed headers
  QTextStream out(&file2);
  for ( int i = 0; i < AllLines.count(); i++ ) {
    out << AllLines[i] << "\n";
  }

  file2.close();

  // Playing a sound
  QSound *sound=new QSound("finished.wav");
  sound->setLoops(3);
  sound->play();
}


// エンコーダの角度で記録するか選択した角度で記録するかは
//     conds->isEncAsTh()
// で決まる。
// 選択したのがどちらかは
//     EncOrPM
// でわかる。
// 何にせよ、全部の値を最後に記録するかどうかは
//     conds->isAddInfos()
// で決まる。

void MainWindow::RecordData( void )
// MeasVals は dark 補正済み count
// dark 補正済み cps が欲しければ MeasCPSs
// (dark 補正がかかっているのは Measurement で readValue するとき
//  dark 補正のオプションを付けているから)
{
  SetDFName( MeasR );
  QFile file( DFName );
  double recTh;
  double encTh, PMTh;
  if ( file.open( QIODevice::Append | QIODevice::Text ) ) {
    QTextStream out( &file );
    QString buf;
    // 行の先頭は 目標角度、エンコーダ読み角度、測定時間、I0
    // Should be changed depending on the detector (ammeter or counter)

    encTh = SelectedCurPosDeg( XENC );  // エンコーダと
    PMTh = SelectedCurPosDeg( XPM );    // PM のそれぞれで見た角度
    // エンコーダの角度で記録するか、選択した角度で記録するか決定
    if ( conds->isEncAsTh() ) {
      recTh = encTh;
    } else {
      recTh = SelectedCurPosDeg( EncOrPM );
    }
    buf.sprintf( "%10.5f" " %9.5f" " %9.2f",
                 u->keV2deg( GoToKeV ), recTh, NowDwell );
    out << buf;

    if ( MeasFileType != FLUO ) {
      // I0 の値が整数かどうかで、記録時のフォーマットを変えようとしている
      if ( (int)(MeasVals[MC_I0]) == MeasVals[MC_I0] ) {
        buf.sprintf( " %9d",                 // 整数 : %9d
                     (int)MeasVals[ MC_I0 ] );
      } else {
        buf.sprintf( " %9.6g",               // 実数 : %9.6g
                     MeasVals[ MC_I0 ] );
      }
      out << buf;
    }

    // その後に測定データの並び
    for ( int i = 1; i < mUnits.count(); i++ ) {
      if ( mUnits.at(i) != SFluo ) {
        // Quick hack for pico-ammeter
        //double v = mUnits.at(i)->value().toDouble();
        double v = mUnits.at(i)->value().toDouble() - mUnits.at(i)->getDark() * NowDwell; // asakura
        if ( v < 1e-10 )
          v = 0.0;
        if ( (int)(v) == v ) {
          buf.sprintf(" %9d", (int)v );
        } else {
          buf.sprintf(" %9.6g", v );
        }
        out << buf;
      } else {
        QVector<int> vals = SFluo->getCountsInROI();           // vals は count
        QVector<double> darks = SFluo->getDarkCountsInROI();   // darks は cps 
        for ( int j = 0; j < MaxSSDs; j++ ) {   // 19ch SSD  in ROI
          buf.sprintf(" %9d",
		      (int)( vals[j] - ( darks[j] * SFluo->GetSetTime() ) ) );  // Orig.
	  // オリジナル(count)に戻した。ファイルに書くのはカウントだったはず
	  // cps にする場合でも by H.A. は間違ってた。
	  // vals[j] は count, darks[j] は cps なので、H.A 風にやるなら NowDwell で割る
	  // ただそれだと、darks に関しては、一度時間を掛けてまた割ってることになるので
	  // vals を時間で割ったほうがいい。
	  // さらには NowDwell は、「設定しようとした時間」なので
	  // 「設定された時間」 GetSetTime() を使ったほうが良い
//      (int)( ( vals[j] - ( darks[j] * SFluo->GetSetTime() ) ) * NowDwell ) ); // by H.A.
//      (int)( vals[j] / SFluo->GetSetTime() - darks[j] );  // by. M.T. (cps version)
          out << buf;
        }
        if ( MeasFileType == FLUO ) {
          // I0 の値が整数かどうかで、記録時のフォーマットを変えようとしている
          if ( (int)(MeasVals[MC_I0]) == MeasVals[MC_I0] ) {
            buf.sprintf( " %9d",                 // 整数 : %9d
                         (int)MeasVals[ MC_I0 ] );
          } else {
            buf.sprintf( " %9.6g",               // 実数 : %9.6g
                         MeasVals[ MC_I0 ] );
          }
          out << buf;
        }
        QVector<double> icrs = SFluo->getICRs();
        for ( int j = 0; j < MaxSSDs; j++ ) {   // 19ch SSD  ICR ( per second )
          buf.sprintf(" %9d", (int)( icrs[j] * NowDwell ) );      // by H.A.
          out << buf;
        }
        buf.sprintf(" %9d", 0 );           // リセット回数 : 0 にしてる
        out << buf;
      }
    }
    // 末尾に情報追加。
    // 設定エネルギー、エンコーダ読みの角度、エンコーダの読みのエネルギー換算、
    //                 PM計算角、PM計算角のエネルギー換算
    //
    // ルーチンの頭に読んでしまった値を使ってるのは、このルーチン実行中に
    // 裏で AUnit 内部の value が変わってしまうのを警戒して。(ないはずだけど)
    //
    if (( conds->isAddInfos() )||( conds->isUse1303Format() )) {
      buf.sprintf( " %9.5f" " %9.5f" " %9.5f" " %9.5f" " %9.5f",
                   GoToKeV,
                   encTh, u->deg2keV( encTh ),
                   PMTh, u->deg2keV( PMTh ) );
      if ( conds->isUse1303Format() )
        out << " ###";
      out << buf;
    }
    out << endl;
    file.close();
  }
}

