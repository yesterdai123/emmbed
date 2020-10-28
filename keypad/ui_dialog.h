/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created: Tue Oct 20 17:15:06 2020
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QToolButton *toolButton_7;
    QToolButton *toolButton_8;
    QToolButton *toolButton_9;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *toolButton_4;
    QToolButton *toolButton_5;
    QToolButton *toolButton_6;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *toolButton_1;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(480, 260);
        gridLayoutWidget = new QWidget(Dialog);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(9, 10, 451, 231));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        toolButton_7 = new QToolButton(gridLayoutWidget);
        toolButton_7->setObjectName(QString::fromUtf8("toolButton_7"));

        horizontalLayout->addWidget(toolButton_7);

        toolButton_8 = new QToolButton(gridLayoutWidget);
        toolButton_8->setObjectName(QString::fromUtf8("toolButton_8"));

        horizontalLayout->addWidget(toolButton_8);

        toolButton_9 = new QToolButton(gridLayoutWidget);
        toolButton_9->setObjectName(QString::fromUtf8("toolButton_9"));

        horizontalLayout->addWidget(toolButton_9);


        gridLayout->addLayout(horizontalLayout, 3, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        toolButton_4 = new QToolButton(gridLayoutWidget);
        toolButton_4->setObjectName(QString::fromUtf8("toolButton_4"));

        horizontalLayout_3->addWidget(toolButton_4);

        toolButton_5 = new QToolButton(gridLayoutWidget);
        toolButton_5->setObjectName(QString::fromUtf8("toolButton_5"));

        horizontalLayout_3->addWidget(toolButton_5);

        toolButton_6 = new QToolButton(gridLayoutWidget);
        toolButton_6->setObjectName(QString::fromUtf8("toolButton_6"));

        horizontalLayout_3->addWidget(toolButton_6);


        gridLayout->addLayout(horizontalLayout_3, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        toolButton_1 = new QToolButton(gridLayoutWidget);
        toolButton_1->setObjectName(QString::fromUtf8("toolButton_1"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../images/1.png"), QSize(), QIcon::Normal, QIcon::On);
        icon.addFile(QString::fromUtf8("../images/y1.png"), QSize(), QIcon::Selected, QIcon::On);
        toolButton_1->setIcon(icon);

        horizontalLayout_2->addWidget(toolButton_1);

        toolButton_2 = new QToolButton(gridLayoutWidget);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));

        horizontalLayout_2->addWidget(toolButton_2);

        toolButton_3 = new QToolButton(gridLayoutWidget);
        toolButton_3->setObjectName(QString::fromUtf8("toolButton_3"));

        horizontalLayout_2->addWidget(toolButton_3);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Keypad", 0, QApplication::UnicodeUTF8));
        toolButton_7->setText(QApplication::translate("Dialog", "...", 0, QApplication::UnicodeUTF8));
        toolButton_8->setText(QApplication::translate("Dialog", "...", 0, QApplication::UnicodeUTF8));
        toolButton_9->setText(QApplication::translate("Dialog", "...", 0, QApplication::UnicodeUTF8));
        toolButton_4->setText(QApplication::translate("Dialog", "...", 0, QApplication::UnicodeUTF8));
        toolButton_5->setText(QApplication::translate("Dialog", "...", 0, QApplication::UnicodeUTF8));
        toolButton_6->setText(QApplication::translate("Dialog", "...", 0, QApplication::UnicodeUTF8));
        toolButton_1->setText(QApplication::translate("Dialog", "...", 0, QApplication::UnicodeUTF8));
        toolButton_2->setText(QApplication::translate("Dialog", "...", 0, QApplication::UnicodeUTF8));
        toolButton_3->setText(QApplication::translate("Dialog", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
