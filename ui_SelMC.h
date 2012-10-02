/********************************************************************************
** Form generated from reading UI file 'SelMC.ui'
**
** Created: Fri Mar 9 14:19:56 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELMC_H
#define UI_SELMC_H

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
#include <QtGui/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_SelMC
{
public:
    QGridLayout *gridLayout;
    QFrame *frame_12;
    QGridLayout *gridLayout_13;
    QLabel *label_42;
    QComboBox *SelectMonoCry;
    QLabel *ShowMCLC;
    QToolButton *ChgMonoCry;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SelMC)
    {
        if (SelMC->objectName().isEmpty())
            SelMC->setObjectName(QString::fromUtf8("SelMC"));
        SelMC->resize(379, 88);
        gridLayout = new QGridLayout(SelMC);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frame_12 = new QFrame(SelMC);
        frame_12->setObjectName(QString::fromUtf8("frame_12"));
        frame_12->setFrameShape(QFrame::Box);
        frame_12->setFrameShadow(QFrame::Raised);
        gridLayout_13 = new QGridLayout(frame_12);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        gridLayout_13->setHorizontalSpacing(5);
        gridLayout_13->setVerticalSpacing(1);
        gridLayout_13->setContentsMargins(-1, 5, -1, 5);
        label_42 = new QLabel(frame_12);
        label_42->setObjectName(QString::fromUtf8("label_42"));

        gridLayout_13->addWidget(label_42, 0, 1, 1, 1);

        SelectMonoCry = new QComboBox(frame_12);
        SelectMonoCry->setObjectName(QString::fromUtf8("SelectMonoCry"));

        gridLayout_13->addWidget(SelectMonoCry, 0, 2, 1, 1);

        ShowMCLC = new QLabel(frame_12);
        ShowMCLC->setObjectName(QString::fromUtf8("ShowMCLC"));

        gridLayout_13->addWidget(ShowMCLC, 0, 3, 1, 1);

        ChgMonoCry = new QToolButton(frame_12);
        ChgMonoCry->setObjectName(QString::fromUtf8("ChgMonoCry"));
        ChgMonoCry->setMaximumSize(QSize(16777215, 20));

        gridLayout_13->addWidget(ChgMonoCry, 0, 0, 1, 1);


        gridLayout->addWidget(frame_12, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(SelMC);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(SelMC);
        QObject::connect(buttonBox, SIGNAL(rejected()), SelMC, SLOT(reject()));
        QObject::connect(buttonBox, SIGNAL(accepted()), SelMC, SLOT(accept()));

        QMetaObject::connectSlotsByName(SelMC);
    } // setupUi

    void retranslateUi(QDialog *SelMC)
    {
        SelMC->setWindowTitle(QApplication::translate("SelMC", "Set Monochro Crystal", 0, QApplication::UnicodeUTF8));
        label_42->setText(QApplication::translate("SelMC", "Monochro Crystal", 0, QApplication::UnicodeUTF8));
        ShowMCLC->setText(QApplication::translate("SelMC", "3.1355316 A", 0, QApplication::UnicodeUTF8));
        ChgMonoCry->setText(QApplication::translate("SelMC", "Change", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SelMC: public Ui_SelMC {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELMC_H
