/********************************************************************************
** Form generated from reading UI file 'StarsSV.ui'
**
** Created: Fri Mar 9 14:19:56 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARSSV_H
#define UI_STARSSV_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_StarsSV
{
public:
    QGridLayout *gridLayout_2;
    QDialogButtonBox *buttonBox;
    QFrame *frame;
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *StarsSVA;
    QComboBox *StarsSVP;
    QLabel *label_2;

    void setupUi(QDialog *StarsSV)
    {
        if (StarsSV->objectName().isEmpty())
            StarsSV->setObjectName(QString::fromUtf8("StarsSV"));
        StarsSV->resize(383, 124);
        gridLayout_2 = new QGridLayout(StarsSV);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        buttonBox = new QDialogButtonBox(StarsSV);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_2->addWidget(buttonBox, 1, 0, 1, 1);

        frame = new QFrame(StarsSV);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        StarsSVA = new QComboBox(frame);
        StarsSVA->setObjectName(QString::fromUtf8("StarsSVA"));
        StarsSVA->setMinimumSize(QSize(200, 0));
        StarsSVA->setEditable(true);

        gridLayout->addWidget(StarsSVA, 0, 1, 1, 1);

        StarsSVP = new QComboBox(frame);
        StarsSVP->setObjectName(QString::fromUtf8("StarsSVP"));
        StarsSVP->setEditable(true);

        gridLayout->addWidget(StarsSVP, 1, 1, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);


        gridLayout_2->addWidget(frame, 0, 0, 1, 1);


        retranslateUi(StarsSV);
        QObject::connect(buttonBox, SIGNAL(accepted()), StarsSV, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), StarsSV, SLOT(reject()));

        QMetaObject::connectSlotsByName(StarsSV);
    } // setupUi

    void retranslateUi(QDialog *StarsSV)
    {
        StarsSV->setWindowTitle(QApplication::translate("StarsSV", "Set Stars Server", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("StarsSV", "Stars Server Address: ", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("StarsSV", "Port No.:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StarsSV: public Ui_StarsSV {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARSSV_H
