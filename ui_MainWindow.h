/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Fri Mar 9 14:19:56 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "XView.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Quit;
    QAction *actionJapanese;
    QAction *actionEnglish;
    QAction *action_SelMC;
    QAction *action_SetSSV;
    QWidget *centralwidget;
    QGridLayout *gridLayout_21;
    QFrame *frame_4;
    QGridLayout *gridLayout_3;
    QLabel *Select1;
    QComboBox *SelectTA;
    QComboBox *SelectTE;
    QLabel *label_3;
    QLabel *label_13;
    QLineEdit *ManTEdeg;
    QLineEdit *ManTEkeV;
    QLabel *label_15;
    QPushButton *ShowPT;
    QSpacerItem *horizontalSpacer_4;
    QFrame *frame_3;
    QGridLayout *gridLayout_4;
    QLabel *label_26;
    QLabel *label_25;
    QLabel *CurrentEnergy;
    QLabel *CurrentAngle;
    QLabel *label_22;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_13;
    QVBoxLayout *verticalLayout_6;
    QFrame *frame_2;
    QGridLayout *gridLayout_2;
    QLabel *label_45;
    QComboBox *GoUnit0;
    QLabel *label_11;
    QLineEdit *GoPos1;
    QComboBox *GoUnit1;
    QPushButton *GoTo1;
    QLabel *label_30;
    QLineEdit *GoPos2;
    QComboBox *GoUnit2;
    QPushButton *GoTo2;
    QLabel *label_31;
    QLineEdit *GoPos3;
    QComboBox *GoUnit3;
    QPushButton *GoTo3;
    QLabel *label_32;
    QLineEdit *GoPos4;
    QComboBox *GoUnit4;
    QPushButton *GoTo4;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_4;
    QFrame *frame_9;
    QGridLayout *gridLayout_9;
    QLabel *label_41;
    QLineEdit *GoMotorPos;
    QPushButton *GoMotor;
    QLabel *label_44;
    QLineEdit *GoMUpStep;
    QPushButton *GoMotorU;
    QLabel *label_47;
    QLineEdit *GoMDownStep;
    QPushButton *GoMotorD;
    QFrame *line_2;
    QLabel *label_35;
    QComboBox *SelectD1;
    QComboBox *SorPS;
    QLabel *label_24;
    QLineEdit *SPSsP;
    QLabel *label_29;
    QLineEdit *SPSeP;
    QPushButton *SPSAScan;
    QLabel *label_36;
    QLineEdit *SPSstep;
    QLabel *label_37;
    QLineEdit *SPSdwell;
    QPushButton *SPSRScan;
    QComboBox *MotorS;
    QLabel *MCurPos;
    QComboBox *MotorN;
    QLabel *label_46;
    QLabel *label_42;
    QFrame *frame_10;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_38;
    QComboBox *SelectD2;
    QLabel *DevCurV;
    QComboBox *SelectScale;
    QPushButton *MStart;
    QSpacerItem *verticalSpacer_3;
    QWidget *tab_2;
    QGridLayout *gridLayout_7;
    QFrame *frame_5;
    QHBoxLayout *horizontalLayout_8;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QPushButton *StdEXAFS;
    QPushButton *StdXANES;
    QPushButton *SaveBLKs;
    QPushButton *LoadBLKs;
    QGridLayout *gridLayout;
    QLineEdit *BLKs01;
    QLabel *label_4;
    QLineEdit *BLKstep01;
    QLabel *label_5;
    QLineEdit *BLKs02;
    QLineEdit *BLKstep02;
    QLabel *label_6;
    QLineEdit *BLKs03;
    QLineEdit *BLKstep03;
    QLabel *label_7;
    QLineEdit *BLKs04;
    QLineEdit *BLKstep04;
    QLabel *label_8;
    QLineEdit *BLKs05;
    QLineEdit *BLKstep05;
    QLineEdit *BLKstep06;
    QLineEdit *BLKs06;
    QLabel *label_9;
    QLabel *label_10;
    QLineEdit *BLKs07;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QComboBox *SelBLKUnit;
    QSpinBox *SelBLKs;
    QLabel *label_33;
    QLabel *label_34;
    QLineEdit *BLKdwell01;
    QLineEdit *BLKdwell02;
    QLineEdit *BLKdwell03;
    QLineEdit *BLKdwell04;
    QLineEdit *BLKdwell05;
    QLineEdit *BLKdwell06;
    QLineEdit *DwellAll;
    QLabel *label_14;
    QLineEdit *BLKpoints01;
    QLineEdit *BLKpoints02;
    QLineEdit *BLKpoints03;
    QLineEdit *BLKpoints04;
    QLineEdit *BLKpoints05;
    QLineEdit *BLKpoints06;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_11;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout_9;
    QGridLayout *gridLayout_8;
    QLabel *label_27;
    QRadioButton *ModeTrans;
    QRadioButton *ModeFluo;
    QRadioButton *ModeAux;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QGridLayout *SSDSelectLayout;
    QPushButton *SSDE01;
    QPushButton *SSDE02;
    QPushButton *SSDE04;
    QPushButton *SSDE05;
    QPushButton *SSDE06;
    QPushButton *SSDE07;
    QPushButton *SSDE08;
    QPushButton *SSDE09;
    QPushButton *SSDE10;
    QPushButton *SSDE11;
    QPushButton *SSDE12;
    QPushButton *SSDE13;
    QPushButton *SSDE14;
    QPushButton *SSDE15;
    QPushButton *SSDE16;
    QPushButton *SSDE17;
    QPushButton *SSDE18;
    QPushButton *SSDE19;
    QLabel *label_12;
    QPushButton *SSDE03;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_7;
    QSpacerItem *verticalSpacer_2;
    QFrame *frame_7;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_20;
    QLabel *TPoints;
    QLabel *TTime;
    QFrame *line;
    QLabel *label_19;
    QComboBox *OnFinishP;
    QFrame *frame_8;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *EditDFName;
    QToolButton *SelDFName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *EditCMT;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_28;
    QSpinBox *SelRPT;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *MeasStart;
    QPushButton *MeasPause;
    QWidget *tab_8;
    QGridLayout *gridLayout_19;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_39;
    QLineEdit *LogFileName;
    QToolButton *LogFileSelect;
    QVBoxLayout *verticalLayout_5;
    QTextEdit *LogMsgs;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_40;
    QLineEdit *LogComment;
    QSpacerItem *horizontalSpacer_6;
    QTabWidget *ViewTab;
    QWidget *tab_3;
    QGridLayout *gridLayout_11;
    XView *View1;
    QWidget *tab_4;
    QGridLayout *gridLayout_12;
    XView *View2;
    QWidget *tab_6;
    QGridLayout *gridLayout_14;
    XView *View3;
    QWidget *tab_7;
    QGridLayout *gridLayout_15;
    XView *View4;
    QWidget *tab_5;
    QGridLayout *gridLayout_16;
    XView *View5;
    QWidget *tab_9;
    QGridLayout *gridLayout_17;
    XView *View6;
    QWidget *tab_10;
    QGridLayout *gridLayout_18;
    XView *View7;
    QWidget *tab_11;
    QGridLayout *gridLayout_20;
    XView *View8;
    QWidget *tab_12;
    QGridLayout *gridLayout_22;
    XView *View9;
    QWidget *tab_13;
    QGridLayout *gridLayout_23;
    XView *View10;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menu_Config;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(783, 929);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        action_Quit = new QAction(MainWindow);
        action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
        actionJapanese = new QAction(MainWindow);
        actionJapanese->setObjectName(QString::fromUtf8("actionJapanese"));
        actionEnglish = new QAction(MainWindow);
        actionEnglish->setObjectName(QString::fromUtf8("actionEnglish"));
        action_SelMC = new QAction(MainWindow);
        action_SelMC->setObjectName(QString::fromUtf8("action_SelMC"));
        action_SetSSV = new QAction(MainWindow);
        action_SetSSV->setObjectName(QString::fromUtf8("action_SetSSV"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_21 = new QGridLayout(centralwidget);
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        frame_4 = new QFrame(centralwidget);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy1);
        frame_4->setFrameShape(QFrame::Box);
        frame_4->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame_4);
        gridLayout_3->setSpacing(3);
        gridLayout_3->setContentsMargins(5, 5, 5, 5);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        Select1 = new QLabel(frame_4);
        Select1->setObjectName(QString::fromUtf8("Select1"));

        gridLayout_3->addWidget(Select1, 0, 0, 1, 1);

        SelectTA = new QComboBox(frame_4);
        SelectTA->setObjectName(QString::fromUtf8("SelectTA"));
        SelectTA->setMaximumSize(QSize(80, 16777215));

        gridLayout_3->addWidget(SelectTA, 0, 1, 1, 1);

        SelectTE = new QComboBox(frame_4);
        SelectTE->setObjectName(QString::fromUtf8("SelectTE"));
        SelectTE->setMaximumSize(QSize(50, 16777215));

        gridLayout_3->addWidget(SelectTE, 0, 3, 1, 1);

        label_3 = new QLabel(frame_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 0, 4, 1, 1);

        label_13 = new QLabel(frame_4);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_3->addWidget(label_13, 0, 8, 1, 1);

        ManTEdeg = new QLineEdit(frame_4);
        ManTEdeg->setObjectName(QString::fromUtf8("ManTEdeg"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ManTEdeg->sizePolicy().hasHeightForWidth());
        ManTEdeg->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(ManTEdeg, 0, 7, 1, 1);

        ManTEkeV = new QLineEdit(frame_4);
        ManTEkeV->setObjectName(QString::fromUtf8("ManTEkeV"));
        sizePolicy2.setHeightForWidth(ManTEkeV->sizePolicy().hasHeightForWidth());
        ManTEkeV->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(ManTEkeV, 0, 5, 1, 1);

        label_15 = new QLabel(frame_4);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_3->addWidget(label_15, 0, 6, 1, 1);

        ShowPT = new QPushButton(frame_4);
        ShowPT->setObjectName(QString::fromUtf8("ShowPT"));
        ShowPT->setMaximumSize(QSize(16777215, 21));

        gridLayout_3->addWidget(ShowPT, 0, 2, 1, 1);


        gridLayout_21->addWidget(frame_4, 0, 0, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(50, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_21->addItem(horizontalSpacer_4, 0, 1, 1, 1);

        frame_3 = new QFrame(centralwidget);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::Box);
        frame_3->setFrameShadow(QFrame::Raised);
        frame_3->setLineWidth(1);
        gridLayout_4 = new QGridLayout(frame_3);
        gridLayout_4->setSpacing(3);
        gridLayout_4->setContentsMargins(5, 5, 5, 5);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_26 = new QLabel(frame_3);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_4->addWidget(label_26, 0, 4, 1, 1);

        label_25 = new QLabel(frame_3);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout_4->addWidget(label_25, 0, 2, 1, 1);

        CurrentEnergy = new QLabel(frame_3);
        CurrentEnergy->setObjectName(QString::fromUtf8("CurrentEnergy"));

        gridLayout_4->addWidget(CurrentEnergy, 0, 3, 1, 1);

        CurrentAngle = new QLabel(frame_3);
        CurrentAngle->setObjectName(QString::fromUtf8("CurrentAngle"));

        gridLayout_4->addWidget(CurrentAngle, 0, 1, 1, 1);

        label_22 = new QLabel(frame_3);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        gridLayout_4->addWidget(label_22, 0, 0, 1, 1);


        gridLayout_21->addWidget(frame_3, 0, 2, 1, 1);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_13 = new QGridLayout(tab);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        frame_2 = new QFrame(tab);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        QFont font;
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(50);
        font.setStrikeOut(false);
        frame_2->setFont(font);
        frame_2->setFrameShape(QFrame::Box);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(5);
        gridLayout_2->setVerticalSpacing(1);
        gridLayout_2->setContentsMargins(-1, 5, -1, 5);
        label_45 = new QLabel(frame_2);
        label_45->setObjectName(QString::fromUtf8("label_45"));

        gridLayout_2->addWidget(label_45, 0, 0, 1, 1);

        GoUnit0 = new QComboBox(frame_2);
        GoUnit0->setObjectName(QString::fromUtf8("GoUnit0"));

        gridLayout_2->addWidget(GoUnit0, 0, 2, 1, 1);

        label_11 = new QLabel(frame_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setMinimumSize(QSize(0, 0));
        label_11->setMaximumSize(QSize(16777215, 18));

        gridLayout_2->addWidget(label_11, 1, 0, 1, 1);

        GoPos1 = new QLineEdit(frame_2);
        GoPos1->setObjectName(QString::fromUtf8("GoPos1"));
        GoPos1->setMinimumSize(QSize(80, 0));

        gridLayout_2->addWidget(GoPos1, 1, 1, 1, 1);

        GoUnit1 = new QComboBox(frame_2);
        GoUnit1->setObjectName(QString::fromUtf8("GoUnit1"));

        gridLayout_2->addWidget(GoUnit1, 1, 2, 1, 1);

        GoTo1 = new QPushButton(frame_2);
        GoTo1->setObjectName(QString::fromUtf8("GoTo1"));
        GoTo1->setMaximumSize(QSize(16777215, 20));

        gridLayout_2->addWidget(GoTo1, 1, 3, 1, 1);

        label_30 = new QLabel(frame_2);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_2->addWidget(label_30, 2, 0, 1, 1);

        GoPos2 = new QLineEdit(frame_2);
        GoPos2->setObjectName(QString::fromUtf8("GoPos2"));

        gridLayout_2->addWidget(GoPos2, 2, 1, 1, 1);

        GoUnit2 = new QComboBox(frame_2);
        GoUnit2->setObjectName(QString::fromUtf8("GoUnit2"));

        gridLayout_2->addWidget(GoUnit2, 2, 2, 1, 1);

        GoTo2 = new QPushButton(frame_2);
        GoTo2->setObjectName(QString::fromUtf8("GoTo2"));
        GoTo2->setMaximumSize(QSize(16777215, 20));

        gridLayout_2->addWidget(GoTo2, 2, 3, 1, 1);

        label_31 = new QLabel(frame_2);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        gridLayout_2->addWidget(label_31, 3, 0, 1, 1);

        GoPos3 = new QLineEdit(frame_2);
        GoPos3->setObjectName(QString::fromUtf8("GoPos3"));

        gridLayout_2->addWidget(GoPos3, 3, 1, 1, 1);

        GoUnit3 = new QComboBox(frame_2);
        GoUnit3->setObjectName(QString::fromUtf8("GoUnit3"));

        gridLayout_2->addWidget(GoUnit3, 3, 2, 1, 1);

        GoTo3 = new QPushButton(frame_2);
        GoTo3->setObjectName(QString::fromUtf8("GoTo3"));
        GoTo3->setMaximumSize(QSize(16777215, 20));

        gridLayout_2->addWidget(GoTo3, 3, 3, 1, 1);

        label_32 = new QLabel(frame_2);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_2->addWidget(label_32, 4, 0, 1, 1);

        GoPos4 = new QLineEdit(frame_2);
        GoPos4->setObjectName(QString::fromUtf8("GoPos4"));

        gridLayout_2->addWidget(GoPos4, 4, 1, 1, 1);

        GoUnit4 = new QComboBox(frame_2);
        GoUnit4->setObjectName(QString::fromUtf8("GoUnit4"));

        gridLayout_2->addWidget(GoUnit4, 4, 2, 1, 1);

        GoTo4 = new QPushButton(frame_2);
        GoTo4->setObjectName(QString::fromUtf8("GoTo4"));
        GoTo4->setMaximumSize(QSize(16777215, 20));

        gridLayout_2->addWidget(GoTo4, 4, 3, 1, 1);


        verticalLayout_6->addWidget(frame_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer);


        gridLayout_13->addLayout(verticalLayout_6, 0, 0, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(1);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        frame_9 = new QFrame(tab);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        frame_9->setFrameShape(QFrame::Box);
        frame_9->setFrameShadow(QFrame::Raised);
        gridLayout_9 = new QGridLayout(frame_9);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_9->setHorizontalSpacing(2);
        gridLayout_9->setVerticalSpacing(1);
        gridLayout_9->setContentsMargins(-1, 5, -1, 5);
        label_41 = new QLabel(frame_9);
        label_41->setObjectName(QString::fromUtf8("label_41"));
        label_41->setMinimumSize(QSize(0, 0));
        label_41->setMaximumSize(QSize(16777215, 18));
        label_41->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_9->addWidget(label_41, 1, 1, 1, 2);

        GoMotorPos = new QLineEdit(frame_9);
        GoMotorPos->setObjectName(QString::fromUtf8("GoMotorPos"));
        GoMotorPos->setMinimumSize(QSize(0, 0));
        GoMotorPos->setMaximumSize(QSize(16777215, 20));

        gridLayout_9->addWidget(GoMotorPos, 1, 3, 1, 1);

        GoMotor = new QPushButton(frame_9);
        GoMotor->setObjectName(QString::fromUtf8("GoMotor"));
        GoMotor->setMaximumSize(QSize(16777215, 20));

        gridLayout_9->addWidget(GoMotor, 1, 4, 1, 1);

        label_44 = new QLabel(frame_9);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_9->addWidget(label_44, 2, 1, 1, 2);

        GoMUpStep = new QLineEdit(frame_9);
        GoMUpStep->setObjectName(QString::fromUtf8("GoMUpStep"));
        GoMUpStep->setMaximumSize(QSize(16777215, 20));

        gridLayout_9->addWidget(GoMUpStep, 2, 3, 1, 1);

        GoMotorU = new QPushButton(frame_9);
        GoMotorU->setObjectName(QString::fromUtf8("GoMotorU"));
        GoMotorU->setMaximumSize(QSize(16777215, 20));

        gridLayout_9->addWidget(GoMotorU, 2, 4, 1, 1);

        label_47 = new QLabel(frame_9);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        label_47->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_9->addWidget(label_47, 3, 1, 1, 2);

        GoMDownStep = new QLineEdit(frame_9);
        GoMDownStep->setObjectName(QString::fromUtf8("GoMDownStep"));
        GoMDownStep->setMaximumSize(QSize(16777215, 20));

        gridLayout_9->addWidget(GoMDownStep, 3, 3, 1, 1);

        GoMotorD = new QPushButton(frame_9);
        GoMotorD->setObjectName(QString::fromUtf8("GoMotorD"));
        GoMotorD->setMaximumSize(QSize(16777215, 20));

        gridLayout_9->addWidget(GoMotorD, 3, 4, 1, 1);

        line_2 = new QFrame(frame_9);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_9->addWidget(line_2, 7, 0, 1, 5);

        label_35 = new QLabel(frame_9);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_9->addWidget(label_35, 8, 0, 1, 1);

        SelectD1 = new QComboBox(frame_9);
        SelectD1->setObjectName(QString::fromUtf8("SelectD1"));
        SelectD1->setMaximumSize(QSize(16777215, 20));

        gridLayout_9->addWidget(SelectD1, 8, 1, 1, 1);

        SorPS = new QComboBox(frame_9);
        SorPS->setObjectName(QString::fromUtf8("SorPS"));
        SorPS->setMaximumSize(QSize(16777215, 20));
        SorPS->setMinimumContentsLength(0);

        gridLayout_9->addWidget(SorPS, 8, 2, 1, 2);

        label_24 = new QLabel(frame_9);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_9->addWidget(label_24, 9, 0, 1, 1);

        SPSsP = new QLineEdit(frame_9);
        SPSsP->setObjectName(QString::fromUtf8("SPSsP"));
        SPSsP->setMaximumSize(QSize(16777215, 20));

        gridLayout_9->addWidget(SPSsP, 9, 1, 1, 1);

        label_29 = new QLabel(frame_9);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_9->addWidget(label_29, 9, 2, 1, 1);

        SPSeP = new QLineEdit(frame_9);
        SPSeP->setObjectName(QString::fromUtf8("SPSeP"));
        SPSeP->setMaximumSize(QSize(16777215, 20));

        gridLayout_9->addWidget(SPSeP, 9, 3, 1, 1);

        SPSAScan = new QPushButton(frame_9);
        SPSAScan->setObjectName(QString::fromUtf8("SPSAScan"));
        SPSAScan->setMaximumSize(QSize(16777215, 20));

        gridLayout_9->addWidget(SPSAScan, 9, 4, 1, 1);

        label_36 = new QLabel(frame_9);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_9->addWidget(label_36, 10, 0, 1, 1);

        SPSstep = new QLineEdit(frame_9);
        SPSstep->setObjectName(QString::fromUtf8("SPSstep"));
        SPSstep->setMinimumSize(QSize(0, 0));
        SPSstep->setMaximumSize(QSize(16777215, 20));
        SPSstep->setSizeIncrement(QSize(0, 0));
        SPSstep->setBaseSize(QSize(0, 0));

        gridLayout_9->addWidget(SPSstep, 10, 1, 1, 1);

        label_37 = new QLabel(frame_9);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_9->addWidget(label_37, 10, 2, 1, 1);

        SPSdwell = new QLineEdit(frame_9);
        SPSdwell->setObjectName(QString::fromUtf8("SPSdwell"));
        SPSdwell->setMaximumSize(QSize(16777215, 20));

        gridLayout_9->addWidget(SPSdwell, 10, 3, 1, 1);

        SPSRScan = new QPushButton(frame_9);
        SPSRScan->setObjectName(QString::fromUtf8("SPSRScan"));
        SPSRScan->setMaximumSize(QSize(16777215, 20));

        gridLayout_9->addWidget(SPSRScan, 10, 4, 1, 1);

        MotorS = new QComboBox(frame_9);
        MotorS->setObjectName(QString::fromUtf8("MotorS"));
        MotorS->setMaximumSize(QSize(16777215, 20));

        gridLayout_9->addWidget(MotorS, 0, 4, 1, 1);

        MCurPos = new QLabel(frame_9);
        MCurPos->setObjectName(QString::fromUtf8("MCurPos"));

        gridLayout_9->addWidget(MCurPos, 0, 3, 1, 1);

        MotorN = new QComboBox(frame_9);
        MotorN->setObjectName(QString::fromUtf8("MotorN"));
        MotorN->setMaximumSize(QSize(16777215, 20));

        gridLayout_9->addWidget(MotorN, 0, 1, 1, 1);

        label_46 = new QLabel(frame_9);
        label_46->setObjectName(QString::fromUtf8("label_46"));

        gridLayout_9->addWidget(label_46, 0, 0, 1, 1);

        label_42 = new QLabel(frame_9);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        label_42->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_9->addWidget(label_42, 0, 2, 1, 1);


        verticalLayout_4->addWidget(frame_9);

        frame_10 = new QFrame(tab);
        frame_10->setObjectName(QString::fromUtf8("frame_10"));
        frame_10->setFrameShape(QFrame::Box);
        frame_10->setFrameShadow(QFrame::Raised);
        horizontalLayout_12 = new QHBoxLayout(frame_10);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(-1, 5, -1, 5);
        label_38 = new QLabel(frame_10);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        horizontalLayout_12->addWidget(label_38);

        SelectD2 = new QComboBox(frame_10);
        SelectD2->setObjectName(QString::fromUtf8("SelectD2"));

        horizontalLayout_12->addWidget(SelectD2);

        DevCurV = new QLabel(frame_10);
        DevCurV->setObjectName(QString::fromUtf8("DevCurV"));

        horizontalLayout_12->addWidget(DevCurV);

        SelectScale = new QComboBox(frame_10);
        SelectScale->setObjectName(QString::fromUtf8("SelectScale"));

        horizontalLayout_12->addWidget(SelectScale);

        MStart = new QPushButton(frame_10);
        MStart->setObjectName(QString::fromUtf8("MStart"));
        MStart->setMaximumSize(QSize(16777215, 20));

        horizontalLayout_12->addWidget(MStart);


        verticalLayout_4->addWidget(frame_10);

        verticalSpacer_3 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);


        gridLayout_13->addLayout(verticalLayout_4, 0, 1, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_7 = new QGridLayout(tab_2);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        frame_5 = new QFrame(tab_2);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setMaximumSize(QSize(320, 250));
        frame_5->setFrameShape(QFrame::Box);
        frame_5->setFrameShadow(QFrame::Raised);
        horizontalLayout_8 = new QHBoxLayout(frame_5);
        horizontalLayout_8->setSpacing(3);
        horizontalLayout_8->setContentsMargins(5, 5, 5, 5);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setSizeConstraint(QLayout::SetMinimumSize);
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_6->setVerticalSpacing(0);
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(1);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        StdEXAFS = new QPushButton(frame_5);
        StdEXAFS->setObjectName(QString::fromUtf8("StdEXAFS"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(2);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(StdEXAFS->sizePolicy().hasHeightForWidth());
        StdEXAFS->setSizePolicy(sizePolicy3);

        gridLayout_5->addWidget(StdEXAFS, 0, 0, 1, 1);

        StdXANES = new QPushButton(frame_5);
        StdXANES->setObjectName(QString::fromUtf8("StdXANES"));
        sizePolicy3.setHeightForWidth(StdXANES->sizePolicy().hasHeightForWidth());
        StdXANES->setSizePolicy(sizePolicy3);

        gridLayout_5->addWidget(StdXANES, 0, 1, 1, 1);

        SaveBLKs = new QPushButton(frame_5);
        SaveBLKs->setObjectName(QString::fromUtf8("SaveBLKs"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(1);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(SaveBLKs->sizePolicy().hasHeightForWidth());
        SaveBLKs->setSizePolicy(sizePolicy4);

        gridLayout_5->addWidget(SaveBLKs, 0, 2, 1, 1);

        LoadBLKs = new QPushButton(frame_5);
        LoadBLKs->setObjectName(QString::fromUtf8("LoadBLKs"));
        sizePolicy4.setHeightForWidth(LoadBLKs->sizePolicy().hasHeightForWidth());
        LoadBLKs->setSizePolicy(sizePolicy4);

        gridLayout_5->addWidget(LoadBLKs, 0, 3, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 1, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(1);
        gridLayout->setVerticalSpacing(2);
        BLKs01 = new QLineEdit(frame_5);
        BLKs01->setObjectName(QString::fromUtf8("BLKs01"));

        gridLayout->addWidget(BLKs01, 2, 1, 1, 1);

        label_4 = new QLabel(frame_5);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAutoFillBackground(false);
        label_4->setFrameShape(QFrame::NoFrame);
        label_4->setAlignment(Qt::AlignCenter);
        label_4->setIndent(0);

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        BLKstep01 = new QLineEdit(frame_5);
        BLKstep01->setObjectName(QString::fromUtf8("BLKstep01"));

        gridLayout->addWidget(BLKstep01, 2, 2, 1, 1);

        label_5 = new QLabel(frame_5);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        BLKs02 = new QLineEdit(frame_5);
        BLKs02->setObjectName(QString::fromUtf8("BLKs02"));

        gridLayout->addWidget(BLKs02, 3, 1, 1, 1);

        BLKstep02 = new QLineEdit(frame_5);
        BLKstep02->setObjectName(QString::fromUtf8("BLKstep02"));

        gridLayout->addWidget(BLKstep02, 3, 2, 1, 1);

        label_6 = new QLabel(frame_5);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        BLKs03 = new QLineEdit(frame_5);
        BLKs03->setObjectName(QString::fromUtf8("BLKs03"));

        gridLayout->addWidget(BLKs03, 4, 1, 1, 1);

        BLKstep03 = new QLineEdit(frame_5);
        BLKstep03->setObjectName(QString::fromUtf8("BLKstep03"));

        gridLayout->addWidget(BLKstep03, 4, 2, 1, 1);

        label_7 = new QLabel(frame_5);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        BLKs04 = new QLineEdit(frame_5);
        BLKs04->setObjectName(QString::fromUtf8("BLKs04"));

        gridLayout->addWidget(BLKs04, 5, 1, 1, 1);

        BLKstep04 = new QLineEdit(frame_5);
        BLKstep04->setObjectName(QString::fromUtf8("BLKstep04"));

        gridLayout->addWidget(BLKstep04, 5, 2, 1, 1);

        label_8 = new QLabel(frame_5);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_8, 6, 0, 1, 1);

        BLKs05 = new QLineEdit(frame_5);
        BLKs05->setObjectName(QString::fromUtf8("BLKs05"));

        gridLayout->addWidget(BLKs05, 6, 1, 1, 1);

        BLKstep05 = new QLineEdit(frame_5);
        BLKstep05->setObjectName(QString::fromUtf8("BLKstep05"));

        gridLayout->addWidget(BLKstep05, 6, 2, 1, 1);

        BLKstep06 = new QLineEdit(frame_5);
        BLKstep06->setObjectName(QString::fromUtf8("BLKstep06"));

        gridLayout->addWidget(BLKstep06, 8, 2, 1, 1);

        BLKs06 = new QLineEdit(frame_5);
        BLKs06->setObjectName(QString::fromUtf8("BLKs06"));

        gridLayout->addWidget(BLKs06, 8, 1, 1, 1);

        label_9 = new QLabel(frame_5);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_9, 8, 0, 1, 1);

        label_10 = new QLabel(frame_5);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_10, 9, 0, 1, 1);

        BLKs07 = new QLineEdit(frame_5);
        BLKs07->setObjectName(QString::fromUtf8("BLKs07"));

        gridLayout->addWidget(BLKs07, 9, 1, 1, 1);

        label_16 = new QLabel(frame_5);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(label_16->sizePolicy().hasHeightForWidth());
        label_16->setSizePolicy(sizePolicy5);
        label_16->setMaximumSize(QSize(16777215, 14));

        gridLayout->addWidget(label_16, 1, 1, 1, 1);

        label_17 = new QLabel(frame_5);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        sizePolicy5.setHeightForWidth(label_17->sizePolicy().hasHeightForWidth());
        label_17->setSizePolicy(sizePolicy5);
        label_17->setMaximumSize(QSize(16777215, 14));

        gridLayout->addWidget(label_17, 1, 2, 1, 1);

        label_18 = new QLabel(frame_5);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setMaximumSize(QSize(16777215, 14));

        gridLayout->addWidget(label_18, 1, 3, 1, 1);

        SelBLKUnit = new QComboBox(frame_5);
        SelBLKUnit->setObjectName(QString::fromUtf8("SelBLKUnit"));

        gridLayout->addWidget(SelBLKUnit, 0, 4, 1, 1);

        SelBLKs = new QSpinBox(frame_5);
        SelBLKs->setObjectName(QString::fromUtf8("SelBLKs"));
        SelBLKs->setMinimum(1);
        SelBLKs->setMaximum(6);
        SelBLKs->setValue(4);

        gridLayout->addWidget(SelBLKs, 0, 3, 1, 1);

        label_33 = new QLabel(frame_5);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_33, 0, 2, 1, 1);

        label_34 = new QLabel(frame_5);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout->addWidget(label_34, 1, 4, 1, 1);

        BLKdwell01 = new QLineEdit(frame_5);
        BLKdwell01->setObjectName(QString::fromUtf8("BLKdwell01"));

        gridLayout->addWidget(BLKdwell01, 2, 4, 1, 1);

        BLKdwell02 = new QLineEdit(frame_5);
        BLKdwell02->setObjectName(QString::fromUtf8("BLKdwell02"));

        gridLayout->addWidget(BLKdwell02, 3, 4, 1, 1);

        BLKdwell03 = new QLineEdit(frame_5);
        BLKdwell03->setObjectName(QString::fromUtf8("BLKdwell03"));

        gridLayout->addWidget(BLKdwell03, 4, 4, 1, 1);

        BLKdwell04 = new QLineEdit(frame_5);
        BLKdwell04->setObjectName(QString::fromUtf8("BLKdwell04"));

        gridLayout->addWidget(BLKdwell04, 5, 4, 1, 1);

        BLKdwell05 = new QLineEdit(frame_5);
        BLKdwell05->setObjectName(QString::fromUtf8("BLKdwell05"));

        gridLayout->addWidget(BLKdwell05, 6, 4, 1, 1);

        BLKdwell06 = new QLineEdit(frame_5);
        BLKdwell06->setObjectName(QString::fromUtf8("BLKdwell06"));

        gridLayout->addWidget(BLKdwell06, 8, 4, 1, 1);

        DwellAll = new QLineEdit(frame_5);
        DwellAll->setObjectName(QString::fromUtf8("DwellAll"));

        gridLayout->addWidget(DwellAll, 9, 4, 1, 1);

        label_14 = new QLabel(frame_5);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_14, 9, 3, 1, 1);

        BLKpoints01 = new QLineEdit(frame_5);
        BLKpoints01->setObjectName(QString::fromUtf8("BLKpoints01"));

        gridLayout->addWidget(BLKpoints01, 2, 3, 1, 1);

        BLKpoints02 = new QLineEdit(frame_5);
        BLKpoints02->setObjectName(QString::fromUtf8("BLKpoints02"));

        gridLayout->addWidget(BLKpoints02, 3, 3, 1, 1);

        BLKpoints03 = new QLineEdit(frame_5);
        BLKpoints03->setObjectName(QString::fromUtf8("BLKpoints03"));

        gridLayout->addWidget(BLKpoints03, 4, 3, 1, 1);

        BLKpoints04 = new QLineEdit(frame_5);
        BLKpoints04->setObjectName(QString::fromUtf8("BLKpoints04"));

        gridLayout->addWidget(BLKpoints04, 5, 3, 1, 1);

        BLKpoints05 = new QLineEdit(frame_5);
        BLKpoints05->setObjectName(QString::fromUtf8("BLKpoints05"));

        gridLayout->addWidget(BLKpoints05, 6, 3, 1, 1);

        BLKpoints06 = new QLineEdit(frame_5);
        BLKpoints06->setObjectName(QString::fromUtf8("BLKpoints06"));

        gridLayout->addWidget(BLKpoints06, 8, 3, 1, 1);


        gridLayout_6->addLayout(gridLayout, 0, 0, 1, 1);


        horizontalLayout_8->addLayout(gridLayout_6);


        gridLayout_7->addWidget(frame_5, 0, 0, 1, 1);

        verticalLayout_8 = new QVBoxLayout();
#ifndef Q_OS_MAC
        verticalLayout_8->setSpacing(-1);
#endif
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        horizontalLayout_11 = new QHBoxLayout();
#ifndef Q_OS_MAC
        horizontalLayout_11->setSpacing(-1);
#endif
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        frame_6 = new QFrame(tab_2);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setMaximumSize(QSize(280, 175));
        frame_6->setFrameShape(QFrame::Box);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout_9 = new QHBoxLayout(frame_6);
        horizontalLayout_9->setSpacing(3);
        horizontalLayout_9->setContentsMargins(5, 5, 5, 5);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_8->setVerticalSpacing(0);
        label_27 = new QLabel(frame_6);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        gridLayout_8->addWidget(label_27, 0, 0, 1, 2);

        ModeTrans = new QRadioButton(frame_6);
        ModeTrans->setObjectName(QString::fromUtf8("ModeTrans"));
        ModeTrans->setChecked(true);

        gridLayout_8->addWidget(ModeTrans, 2, 0, 1, 1);

        ModeFluo = new QRadioButton(frame_6);
        ModeFluo->setObjectName(QString::fromUtf8("ModeFluo"));

        gridLayout_8->addWidget(ModeFluo, 2, 1, 1, 1);

        ModeAux = new QRadioButton(frame_6);
        ModeAux->setObjectName(QString::fromUtf8("ModeAux"));

        gridLayout_8->addWidget(ModeAux, 2, 2, 1, 1);

        frame = new QFrame(frame_6);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        SSDSelectLayout = new QGridLayout();
        SSDSelectLayout->setSpacing(1);
        SSDSelectLayout->setObjectName(QString::fromUtf8("SSDSelectLayout"));
        SSDE01 = new QPushButton(frame);
        SSDE01->setObjectName(QString::fromUtf8("SSDE01"));
        SSDE01->setCheckable(true);
        SSDE01->setChecked(false);
        SSDE01->setAutoExclusive(false);
        SSDE01->setAutoDefault(false);
        SSDE01->setDefault(false);
        SSDE01->setFlat(false);

        SSDSelectLayout->addWidget(SSDE01, 0, 2, 1, 2);

        SSDE02 = new QPushButton(frame);
        SSDE02->setObjectName(QString::fromUtf8("SSDE02"));
        SSDE02->setStyleSheet(QString::fromUtf8(""));
        SSDE02->setCheckable(true);
        SSDE02->setChecked(false);

        SSDSelectLayout->addWidget(SSDE02, 0, 4, 1, 2);

        SSDE04 = new QPushButton(frame);
        SSDE04->setObjectName(QString::fromUtf8("SSDE04"));
        SSDE04->setCheckable(true);
        SSDE04->setChecked(false);

        SSDSelectLayout->addWidget(SSDE04, 1, 1, 1, 2);

        SSDE05 = new QPushButton(frame);
        SSDE05->setObjectName(QString::fromUtf8("SSDE05"));
        SSDE05->setCheckable(true);
        SSDE05->setChecked(false);

        SSDSelectLayout->addWidget(SSDE05, 1, 3, 1, 2);

        SSDE06 = new QPushButton(frame);
        SSDE06->setObjectName(QString::fromUtf8("SSDE06"));
        SSDE06->setCheckable(true);
        SSDE06->setChecked(false);

        SSDSelectLayout->addWidget(SSDE06, 1, 5, 1, 2);

        SSDE07 = new QPushButton(frame);
        SSDE07->setObjectName(QString::fromUtf8("SSDE07"));
        SSDE07->setCheckable(true);
        SSDE07->setChecked(false);

        SSDSelectLayout->addWidget(SSDE07, 1, 7, 1, 2);

        SSDE08 = new QPushButton(frame);
        SSDE08->setObjectName(QString::fromUtf8("SSDE08"));
        SSDE08->setCheckable(true);
        SSDE08->setChecked(false);

        SSDSelectLayout->addWidget(SSDE08, 2, 0, 1, 2);

        SSDE09 = new QPushButton(frame);
        SSDE09->setObjectName(QString::fromUtf8("SSDE09"));
        SSDE09->setCheckable(true);
        SSDE09->setChecked(false);
        SSDE09->setAutoRepeat(false);

        SSDSelectLayout->addWidget(SSDE09, 2, 2, 1, 2);

        SSDE10 = new QPushButton(frame);
        SSDE10->setObjectName(QString::fromUtf8("SSDE10"));
        SSDE10->setCheckable(true);
        SSDE10->setChecked(false);

        SSDSelectLayout->addWidget(SSDE10, 2, 4, 1, 2);

        SSDE11 = new QPushButton(frame);
        SSDE11->setObjectName(QString::fromUtf8("SSDE11"));
        SSDE11->setCheckable(true);
        SSDE11->setChecked(false);

        SSDSelectLayout->addWidget(SSDE11, 2, 6, 1, 2);

        SSDE12 = new QPushButton(frame);
        SSDE12->setObjectName(QString::fromUtf8("SSDE12"));
        SSDE12->setCheckable(true);
        SSDE12->setChecked(false);

        SSDSelectLayout->addWidget(SSDE12, 2, 8, 1, 2);

        SSDE13 = new QPushButton(frame);
        SSDE13->setObjectName(QString::fromUtf8("SSDE13"));
        SSDE13->setCheckable(true);
        SSDE13->setChecked(false);

        SSDSelectLayout->addWidget(SSDE13, 3, 1, 1, 2);

        SSDE14 = new QPushButton(frame);
        SSDE14->setObjectName(QString::fromUtf8("SSDE14"));
        SSDE14->setCheckable(true);
        SSDE14->setChecked(false);

        SSDSelectLayout->addWidget(SSDE14, 3, 3, 1, 2);

        SSDE15 = new QPushButton(frame);
        SSDE15->setObjectName(QString::fromUtf8("SSDE15"));
        SSDE15->setCheckable(true);
        SSDE15->setChecked(false);

        SSDSelectLayout->addWidget(SSDE15, 3, 5, 1, 2);

        SSDE16 = new QPushButton(frame);
        SSDE16->setObjectName(QString::fromUtf8("SSDE16"));
        SSDE16->setCheckable(true);
        SSDE16->setChecked(false);

        SSDSelectLayout->addWidget(SSDE16, 3, 7, 1, 2);

        SSDE17 = new QPushButton(frame);
        SSDE17->setObjectName(QString::fromUtf8("SSDE17"));
        SSDE17->setCheckable(true);
        SSDE17->setChecked(false);

        SSDSelectLayout->addWidget(SSDE17, 4, 2, 1, 2);

        SSDE18 = new QPushButton(frame);
        SSDE18->setObjectName(QString::fromUtf8("SSDE18"));
        SSDE18->setCheckable(true);
        SSDE18->setChecked(false);

        SSDSelectLayout->addWidget(SSDE18, 4, 4, 1, 2);

        SSDE19 = new QPushButton(frame);
        SSDE19->setObjectName(QString::fromUtf8("SSDE19"));
        SSDE19->setCheckable(true);
        SSDE19->setChecked(false);

        SSDSelectLayout->addWidget(SSDE19, 4, 6, 1, 2);

        label_12 = new QLabel(frame);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        SSDSelectLayout->addWidget(label_12, 0, 0, 1, 2);

        SSDE03 = new QPushButton(frame);
        SSDE03->setObjectName(QString::fromUtf8("SSDE03"));
        SSDE03->setStyleSheet(QString::fromUtf8(""));
        SSDE03->setCheckable(true);
        SSDE03->setChecked(false);

        SSDSelectLayout->addWidget(SSDE03, 0, 6, 1, 2);


        horizontalLayout_3->addLayout(SSDSelectLayout);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        gridLayout_8->addWidget(frame, 3, 0, 1, 3);


        horizontalLayout_9->addLayout(gridLayout_8);


        horizontalLayout_11->addWidget(frame_6);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalSpacer_2 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_7->addItem(verticalSpacer_2);

        frame_7 = new QFrame(tab_2);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        frame_7->setFrameShape(QFrame::Box);
        frame_7->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_7);
        verticalLayout_3->setSpacing(1);
        verticalLayout_3->setContentsMargins(3, 3, 3, 3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_20 = new QLabel(frame_7);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        verticalLayout_3->addWidget(label_20);

        TPoints = new QLabel(frame_7);
        TPoints->setObjectName(QString::fromUtf8("TPoints"));

        verticalLayout_3->addWidget(TPoints);

        TTime = new QLabel(frame_7);
        TTime->setObjectName(QString::fromUtf8("TTime"));

        verticalLayout_3->addWidget(TTime);

        line = new QFrame(frame_7);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line);

        label_19 = new QLabel(frame_7);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_19);

        OnFinishP = new QComboBox(frame_7);
        OnFinishP->setObjectName(QString::fromUtf8("OnFinishP"));

        verticalLayout_3->addWidget(OnFinishP);


        verticalLayout_7->addWidget(frame_7);


        horizontalLayout_11->addLayout(verticalLayout_7);


        verticalLayout_8->addLayout(horizontalLayout_11);

        frame_8 = new QFrame(tab_2);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setFrameShape(QFrame::Box);
        frame_8->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame_8);
#ifndef Q_OS_MAC
        horizontalLayout_5->setSpacing(-1);
#endif
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(5, 3, 5, 3);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(frame_8);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        EditDFName = new QLineEdit(frame_8);
        EditDFName->setObjectName(QString::fromUtf8("EditDFName"));

        horizontalLayout->addWidget(EditDFName);

        SelDFName = new QToolButton(frame_8);
        SelDFName->setObjectName(QString::fromUtf8("SelDFName"));

        horizontalLayout->addWidget(SelDFName);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(frame_8);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        EditCMT = new QLineEdit(frame_8);
        EditCMT->setObjectName(QString::fromUtf8("EditCMT"));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(EditCMT->sizePolicy().hasHeightForWidth());
        EditCMT->setSizePolicy(sizePolicy6);

        horizontalLayout_2->addWidget(EditCMT);


        verticalLayout_2->addLayout(horizontalLayout_2);


        horizontalLayout_5->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_28 = new QLabel(frame_8);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        horizontalLayout_10->addWidget(label_28);

        SelRPT = new QSpinBox(frame_8);
        SelRPT->setObjectName(QString::fromUtf8("SelRPT"));
        SelRPT->setMinimum(1);
        SelRPT->setValue(1);

        horizontalLayout_10->addWidget(SelRPT);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        MeasStart = new QPushButton(frame_8);
        MeasStart->setObjectName(QString::fromUtf8("MeasStart"));
        MeasStart->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_4->addWidget(MeasStart);

        MeasPause = new QPushButton(frame_8);
        MeasPause->setObjectName(QString::fromUtf8("MeasPause"));
        MeasPause->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_4->addWidget(MeasPause);


        verticalLayout->addLayout(horizontalLayout_4);


        horizontalLayout_5->addLayout(verticalLayout);


        verticalLayout_8->addWidget(frame_8);


        gridLayout_7->addLayout(verticalLayout_8, 0, 1, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName(QString::fromUtf8("tab_8"));
        gridLayout_19 = new QGridLayout(tab_8);
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_39 = new QLabel(tab_8);
        label_39->setObjectName(QString::fromUtf8("label_39"));

        horizontalLayout_7->addWidget(label_39);

        LogFileName = new QLineEdit(tab_8);
        LogFileName->setObjectName(QString::fromUtf8("LogFileName"));
        sizePolicy6.setHeightForWidth(LogFileName->sizePolicy().hasHeightForWidth());
        LogFileName->setSizePolicy(sizePolicy6);

        horizontalLayout_7->addWidget(LogFileName);

        LogFileSelect = new QToolButton(tab_8);
        LogFileSelect->setObjectName(QString::fromUtf8("LogFileSelect"));

        horizontalLayout_7->addWidget(LogFileSelect);


        gridLayout_19->addLayout(horizontalLayout_7, 0, 0, 1, 1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        LogMsgs = new QTextEdit(tab_8);
        LogMsgs->setObjectName(QString::fromUtf8("LogMsgs"));

        verticalLayout_5->addWidget(LogMsgs);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_40 = new QLabel(tab_8);
        label_40->setObjectName(QString::fromUtf8("label_40"));

        horizontalLayout_6->addWidget(label_40);

        LogComment = new QLineEdit(tab_8);
        LogComment->setObjectName(QString::fromUtf8("LogComment"));

        horizontalLayout_6->addWidget(LogComment);


        verticalLayout_5->addLayout(horizontalLayout_6);


        gridLayout_19->addLayout(verticalLayout_5, 1, 0, 1, 2);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_19->addItem(horizontalSpacer_6, 0, 1, 1, 1);

        tabWidget->addTab(tab_8, QString());

        gridLayout_21->addWidget(tabWidget, 1, 0, 1, 3);

        ViewTab = new QTabWidget(centralwidget);
        ViewTab->setObjectName(QString::fromUtf8("ViewTab"));
        QSizePolicy sizePolicy7(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(2);
        sizePolicy7.setHeightForWidth(ViewTab->sizePolicy().hasHeightForWidth());
        ViewTab->setSizePolicy(sizePolicy7);
        ViewTab->setMinimumSize(QSize(700, 500));
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_11 = new QGridLayout(tab_3);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        View1 = new XView(tab_3);
        View1->setObjectName(QString::fromUtf8("View1"));
        QSizePolicy sizePolicy8(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(2);
        sizePolicy8.setHeightForWidth(View1->sizePolicy().hasHeightForWidth());
        View1->setSizePolicy(sizePolicy8);
        View1->setMinimumSize(QSize(0, 50));

        gridLayout_11->addWidget(View1, 0, 0, 1, 1);

        ViewTab->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout_12 = new QGridLayout(tab_4);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        View2 = new XView(tab_4);
        View2->setObjectName(QString::fromUtf8("View2"));
        sizePolicy8.setHeightForWidth(View2->sizePolicy().hasHeightForWidth());
        View2->setSizePolicy(sizePolicy8);
        View2->setMinimumSize(QSize(0, 50));

        gridLayout_12->addWidget(View2, 0, 0, 1, 1);

        ViewTab->addTab(tab_4, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        gridLayout_14 = new QGridLayout(tab_6);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        View3 = new XView(tab_6);
        View3->setObjectName(QString::fromUtf8("View3"));
        sizePolicy8.setHeightForWidth(View3->sizePolicy().hasHeightForWidth());
        View3->setSizePolicy(sizePolicy8);
        View3->setMinimumSize(QSize(0, 50));

        gridLayout_14->addWidget(View3, 0, 0, 1, 1);

        ViewTab->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QString::fromUtf8("tab_7"));
        gridLayout_15 = new QGridLayout(tab_7);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        View4 = new XView(tab_7);
        View4->setObjectName(QString::fromUtf8("View4"));
        sizePolicy8.setHeightForWidth(View4->sizePolicy().hasHeightForWidth());
        View4->setSizePolicy(sizePolicy8);
        View4->setMinimumSize(QSize(0, 50));

        gridLayout_15->addWidget(View4, 0, 0, 1, 1);

        ViewTab->addTab(tab_7, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        gridLayout_16 = new QGridLayout(tab_5);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        View5 = new XView(tab_5);
        View5->setObjectName(QString::fromUtf8("View5"));
        sizePolicy8.setHeightForWidth(View5->sizePolicy().hasHeightForWidth());
        View5->setSizePolicy(sizePolicy8);
        View5->setMinimumSize(QSize(0, 50));

        gridLayout_16->addWidget(View5, 0, 0, 1, 1);

        ViewTab->addTab(tab_5, QString());
        tab_9 = new QWidget();
        tab_9->setObjectName(QString::fromUtf8("tab_9"));
        gridLayout_17 = new QGridLayout(tab_9);
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        View6 = new XView(tab_9);
        View6->setObjectName(QString::fromUtf8("View6"));
        sizePolicy8.setHeightForWidth(View6->sizePolicy().hasHeightForWidth());
        View6->setSizePolicy(sizePolicy8);
        View6->setMinimumSize(QSize(0, 50));

        gridLayout_17->addWidget(View6, 0, 0, 1, 1);

        ViewTab->addTab(tab_9, QString());
        tab_10 = new QWidget();
        tab_10->setObjectName(QString::fromUtf8("tab_10"));
        gridLayout_18 = new QGridLayout(tab_10);
        gridLayout_18->setObjectName(QString::fromUtf8("gridLayout_18"));
        View7 = new XView(tab_10);
        View7->setObjectName(QString::fromUtf8("View7"));
        sizePolicy8.setHeightForWidth(View7->sizePolicy().hasHeightForWidth());
        View7->setSizePolicy(sizePolicy8);
        View7->setMinimumSize(QSize(0, 50));

        gridLayout_18->addWidget(View7, 0, 0, 1, 1);

        ViewTab->addTab(tab_10, QString());
        tab_11 = new QWidget();
        tab_11->setObjectName(QString::fromUtf8("tab_11"));
        gridLayout_20 = new QGridLayout(tab_11);
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        View8 = new XView(tab_11);
        View8->setObjectName(QString::fromUtf8("View8"));
        sizePolicy8.setHeightForWidth(View8->sizePolicy().hasHeightForWidth());
        View8->setSizePolicy(sizePolicy8);
        View8->setMinimumSize(QSize(0, 50));

        gridLayout_20->addWidget(View8, 0, 0, 1, 1);

        ViewTab->addTab(tab_11, QString());
        tab_12 = new QWidget();
        tab_12->setObjectName(QString::fromUtf8("tab_12"));
        gridLayout_22 = new QGridLayout(tab_12);
        gridLayout_22->setObjectName(QString::fromUtf8("gridLayout_22"));
        View9 = new XView(tab_12);
        View9->setObjectName(QString::fromUtf8("View9"));
        sizePolicy8.setHeightForWidth(View9->sizePolicy().hasHeightForWidth());
        View9->setSizePolicy(sizePolicy8);
        View9->setMinimumSize(QSize(0, 50));

        gridLayout_22->addWidget(View9, 0, 0, 1, 1);

        ViewTab->addTab(tab_12, QString());
        tab_13 = new QWidget();
        tab_13->setObjectName(QString::fromUtf8("tab_13"));
        gridLayout_23 = new QGridLayout(tab_13);
        gridLayout_23->setObjectName(QString::fromUtf8("gridLayout_23"));
        View10 = new XView(tab_13);
        View10->setObjectName(QString::fromUtf8("View10"));
        sizePolicy8.setHeightForWidth(View10->sizePolicy().hasHeightForWidth());
        View10->setSizePolicy(sizePolicy8);
        View10->setMinimumSize(QSize(0, 50));

        gridLayout_23->addWidget(View10, 0, 0, 1, 1);

        ViewTab->addTab(tab_13, QString());

        gridLayout_21->addWidget(ViewTab, 2, 0, 1, 3);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 783, 25));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menu_Config = new QMenu(menubar);
        menu_Config->setObjectName(QString::fromUtf8("menu_Config"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menu_Config->menuAction());
        menu_File->addAction(action_Quit);
        menu_Config->addAction(action_SelMC);
        menu_Config->addAction(action_SetSSV);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);
        ViewTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        action_Quit->setText(QApplication::translate("MainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
        actionJapanese->setText(QApplication::translate("MainWindow", "Japanese", 0, QApplication::UnicodeUTF8));
        actionEnglish->setText(QApplication::translate("MainWindow", "English", 0, QApplication::UnicodeUTF8));
        action_SelMC->setText(QApplication::translate("MainWindow", "Set &Lattice Const. Monochro Crystal", 0, QApplication::UnicodeUTF8));
        action_SetSSV->setText(QApplication::translate("MainWindow", "Set &Stars Server Address", 0, QApplication::UnicodeUTF8));
        Select1->setText(QApplication::translate("MainWindow", "Target", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        SelectTA->setToolTip(QApplication::translate("MainWindow", "Select an Atom", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        SelectTA->setWhatsThis(QApplication::translate("MainWindow", "Select a Target Atom", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        SelectTE->setToolTip(QApplication::translate("MainWindow", "Select an absorption edge", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_3->setText(QApplication::translate("MainWindow", "  Edge", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("MainWindow", "Deg.", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("MainWindow", "KeV  ", 0, QApplication::UnicodeUTF8));
        ShowPT->setText(QApplication::translate("MainWindow", "PeriodicT", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("MainWindow", "KeV", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("MainWindow", "Deg.  ", 0, QApplication::UnicodeUTF8));
        CurrentEnergy->setText(QApplication::translate("MainWindow", "10.0000", 0, QApplication::UnicodeUTF8));
        CurrentAngle->setText(QApplication::translate("MainWindow", "10.0000", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("MainWindow", "Cur. Pos.: ", 0, QApplication::UnicodeUTF8));
        label_45->setText(QApplication::translate("MainWindow", "Monochro", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "Move To (1)", 0, QApplication::UnicodeUTF8));
        GoTo1->setText(QApplication::translate("MainWindow", "Go", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("MainWindow", "Move To (2)", 0, QApplication::UnicodeUTF8));
        GoTo2->setText(QApplication::translate("MainWindow", "Go", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("MainWindow", "Move To (3)", 0, QApplication::UnicodeUTF8));
        GoTo3->setText(QApplication::translate("MainWindow", "Go", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("MainWindow", "Move To (4)", 0, QApplication::UnicodeUTF8));
        GoTo4->setText(QApplication::translate("MainWindow", "Go", 0, QApplication::UnicodeUTF8));
        label_41->setText(QApplication::translate("MainWindow", "Move To", 0, QApplication::UnicodeUTF8));
        GoMotor->setText(QApplication::translate("MainWindow", "Go", 0, QApplication::UnicodeUTF8));
        label_44->setText(QApplication::translate("MainWindow", "Up (step)", 0, QApplication::UnicodeUTF8));
        GoMotorU->setText(QApplication::translate("MainWindow", "Go Up", 0, QApplication::UnicodeUTF8));
        label_47->setText(QApplication::translate("MainWindow", "Down (step)", 0, QApplication::UnicodeUTF8));
        GoMotorD->setText(QApplication::translate("MainWindow", "Go Down", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("MainWindow", "Monitor:", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("MainWindow", "StartP:", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("MainWindow", "EndP:", 0, QApplication::UnicodeUTF8));
        SPSeP->setText(QString());
        SPSAScan->setText(QApplication::translate("MainWindow", "Abs. Scn", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("MainWindow", "Step:", 0, QApplication::UnicodeUTF8));
        label_37->setText(QApplication::translate("MainWindow", " Dwell [s]:", 0, QApplication::UnicodeUTF8));
        SPSdwell->setText(QString());
        SPSRScan->setText(QApplication::translate("MainWindow", "Rel. Scn", 0, QApplication::UnicodeUTF8));
        MCurPos->setText(QApplication::translate("MainWindow", "0.000", 0, QApplication::UnicodeUTF8));
        label_46->setText(QApplication::translate("MainWindow", "Motor", 0, QApplication::UnicodeUTF8));
        label_42->setText(QApplication::translate("MainWindow", "Cur. Pos.: ", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("MainWindow", "Monitor", 0, QApplication::UnicodeUTF8));
        DevCurV->setText(QApplication::translate("MainWindow", "0.000", 0, QApplication::UnicodeUTF8));
        MStart->setText(QApplication::translate("MainWindow", "Mon. Start", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Set up Conditions", 0, QApplication::UnicodeUTF8));
        StdEXAFS->setText(QApplication::translate("MainWindow", "Std. EXAFS", 0, QApplication::UnicodeUTF8));
        StdXANES->setText(QApplication::translate("MainWindow", "Std. XANES", 0, QApplication::UnicodeUTF8));
        SaveBLKs->setText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
        LoadBLKs->setText(QApplication::translate("MainWindow", "Load", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "2", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "3", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "4", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "5", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "6", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "7", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("MainWindow", "StartP", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("MainWindow", "Step", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("MainWindow", "Steps", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("MainWindow", "Blocks: ", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("MainWindow", "Dwell [s]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        DwellAll->setToolTip(QApplication::translate("MainWindow", "Set all dwell times at once", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_14->setText(QApplication::translate("MainWindow", "All: ", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("MainWindow", "Measurement Mode", 0, QApplication::UnicodeUTF8));
        ModeTrans->setText(QApplication::translate("MainWindow", "Trans.", 0, QApplication::UnicodeUTF8));
        ModeFluo->setText(QApplication::translate("MainWindow", "Fluo. (SSD)", 0, QApplication::UnicodeUTF8));
        ModeAux->setText(QApplication::translate("MainWindow", "Aux.", 0, QApplication::UnicodeUTF8));
        SSDE01->setText(QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8));
        SSDE02->setText(QApplication::translate("MainWindow", "2", 0, QApplication::UnicodeUTF8));
        SSDE04->setText(QApplication::translate("MainWindow", "4", 0, QApplication::UnicodeUTF8));
        SSDE05->setText(QApplication::translate("MainWindow", "5", 0, QApplication::UnicodeUTF8));
        SSDE06->setText(QApplication::translate("MainWindow", "6", 0, QApplication::UnicodeUTF8));
        SSDE07->setText(QApplication::translate("MainWindow", "7", 0, QApplication::UnicodeUTF8));
        SSDE08->setText(QApplication::translate("MainWindow", "8", 0, QApplication::UnicodeUTF8));
        SSDE09->setText(QApplication::translate("MainWindow", "9", 0, QApplication::UnicodeUTF8));
        SSDE10->setText(QApplication::translate("MainWindow", "10", 0, QApplication::UnicodeUTF8));
        SSDE11->setText(QApplication::translate("MainWindow", "11", 0, QApplication::UnicodeUTF8));
        SSDE12->setText(QApplication::translate("MainWindow", "12", 0, QApplication::UnicodeUTF8));
        SSDE13->setText(QApplication::translate("MainWindow", "13", 0, QApplication::UnicodeUTF8));
        SSDE14->setText(QApplication::translate("MainWindow", "14", 0, QApplication::UnicodeUTF8));
        SSDE15->setText(QApplication::translate("MainWindow", "15", 0, QApplication::UnicodeUTF8));
        SSDE16->setText(QApplication::translate("MainWindow", "16", 0, QApplication::UnicodeUTF8));
        SSDE17->setText(QApplication::translate("MainWindow", "17", 0, QApplication::UnicodeUTF8));
        SSDE18->setText(QApplication::translate("MainWindow", "18", 0, QApplication::UnicodeUTF8));
        SSDE19->setText(QApplication::translate("MainWindow", "19", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "SSD", 0, QApplication::UnicodeUTF8));
        SSDE03->setText(QApplication::translate("MainWindow", "3", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("MainWindow", "Check Cond.", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        TPoints->setToolTip(QApplication::translate("MainWindow", "Total points which will be measured", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        TPoints->setWhatsThis(QApplication::translate("MainWindow", "Total points which will be measured\n"
"", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        TPoints->setText(QApplication::translate("MainWindow", "Points:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        TTime->setToolTip(QApplication::translate("MainWindow", "Total measurement time", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        TTime->setText(QApplication::translate("MainWindow", "Time:", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("MainWindow", "On Finish", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Data File Name", 0, QApplication::UnicodeUTF8));
        SelDFName->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Comment", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("MainWindow", "Repeat", 0, QApplication::UnicodeUTF8));
        MeasStart->setText(QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
        MeasPause->setText(QApplication::translate("MainWindow", "Pause", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Measurement", 0, QApplication::UnicodeUTF8));
        label_39->setText(QApplication::translate("MainWindow", "Log File Name:", 0, QApplication::UnicodeUTF8));
        LogFileSelect->setText(QApplication::translate("MainWindow", "New", 0, QApplication::UnicodeUTF8));
        label_40->setText(QApplication::translate("MainWindow", "Add Log Comment:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_8), QApplication::translate("MainWindow", "Log / Record", 0, QApplication::UnicodeUTF8));
        ViewTab->setTabText(ViewTab->indexOf(tab_3), QApplication::translate("MainWindow", "View 1", 0, QApplication::UnicodeUTF8));
        ViewTab->setTabText(ViewTab->indexOf(tab_4), QApplication::translate("MainWindow", "View 2", 0, QApplication::UnicodeUTF8));
        ViewTab->setTabText(ViewTab->indexOf(tab_6), QApplication::translate("MainWindow", "View 3", 0, QApplication::UnicodeUTF8));
        ViewTab->setTabText(ViewTab->indexOf(tab_7), QApplication::translate("MainWindow", "View 4", 0, QApplication::UnicodeUTF8));
        ViewTab->setTabText(ViewTab->indexOf(tab_5), QApplication::translate("MainWindow", "View5", 0, QApplication::UnicodeUTF8));
        ViewTab->setTabText(ViewTab->indexOf(tab_9), QApplication::translate("MainWindow", "View6", 0, QApplication::UnicodeUTF8));
        ViewTab->setTabText(ViewTab->indexOf(tab_10), QApplication::translate("MainWindow", "View7", 0, QApplication::UnicodeUTF8));
        ViewTab->setTabText(ViewTab->indexOf(tab_11), QApplication::translate("MainWindow", "View8", 0, QApplication::UnicodeUTF8));
        ViewTab->setTabText(ViewTab->indexOf(tab_12), QApplication::translate("MainWindow", "View9", 0, QApplication::UnicodeUTF8));
        ViewTab->setTabText(ViewTab->indexOf(tab_13), QApplication::translate("MainWindow", "View10", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menu_Config->setTitle(QApplication::translate("MainWindow", "&Config", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
