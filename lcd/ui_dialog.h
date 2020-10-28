/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created: Tue Oct 20 15:34:21 2020
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
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_line;
    QPushButton *pushButton_rect;
    QPushButton *pushButton_circle;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(480, 260);
        horizontalLayoutWidget = new QWidget(Dialog);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(33, 69, 411, 111));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_line = new QPushButton(horizontalLayoutWidget);
        pushButton_line->setObjectName(QString::fromUtf8("pushButton_line"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/line.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_line->setIcon(icon);
        pushButton_line->setIconSize(QSize(96, 96));

        horizontalLayout->addWidget(pushButton_line);

        pushButton_rect = new QPushButton(horizontalLayoutWidget);
        pushButton_rect->setObjectName(QString::fromUtf8("pushButton_rect"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/images/rect.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_rect->setIcon(icon1);
        pushButton_rect->setIconSize(QSize(96, 96));

        horizontalLayout->addWidget(pushButton_rect);

        pushButton_circle = new QPushButton(horizontalLayoutWidget);
        pushButton_circle->setObjectName(QString::fromUtf8("pushButton_circle"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/images/circle.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_circle->setIcon(icon2);
        pushButton_circle->setIconSize(QSize(96, 96));

        horizontalLayout->addWidget(pushButton_circle);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton_line->setText(QString());
        pushButton_rect->setText(QString());
        pushButton_circle->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
