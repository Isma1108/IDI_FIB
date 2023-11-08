/********************************************************************************
** Form generated from reading UI file 'MyForm.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYFORM_H
#define UI_MYFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDial>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyForm
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLCDNumber *lcdNumber;
    QDial *dial;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QLCDNumber *lcdNumber_2;
    QDial *dial_2;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QWidget *MyForm)
    {
        if (MyForm->objectName().isEmpty())
            MyForm->setObjectName(QString::fromUtf8("MyForm"));
        MyForm->resize(432, 392);
        horizontalLayout_2 = new QHBoxLayout(MyForm);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(MyForm);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(0, 20));

        verticalLayout->addWidget(label);

        lcdNumber = new QLCDNumber(MyForm);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lcdNumber->sizePolicy().hasHeightForWidth());
        lcdNumber->setSizePolicy(sizePolicy1);
        lcdNumber->setMinimumSize(QSize(0, 100));

        verticalLayout->addWidget(lcdNumber);

        dial = new QDial(MyForm);
        dial->setObjectName(QString::fromUtf8("dial"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dial->sizePolicy().hasHeightForWidth());
        dial->setSizePolicy(sizePolicy2);
        dial->setMinimumSize(QSize(130, 240));
        dial->setStyleSheet(QString::fromUtf8("background-color: rgb(235, 30, 30);"));
        dial->setMaximum(23);

        verticalLayout->addWidget(dial);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(MyForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(0, 20));

        verticalLayout_2->addWidget(label_2);

        lcdNumber_2 = new QLCDNumber(MyForm);
        lcdNumber_2->setObjectName(QString::fromUtf8("lcdNumber_2"));
        sizePolicy1.setHeightForWidth(lcdNumber_2->sizePolicy().hasHeightForWidth());
        lcdNumber_2->setSizePolicy(sizePolicy1);
        lcdNumber_2->setMinimumSize(QSize(0, 100));

        verticalLayout_2->addWidget(lcdNumber_2);

        dial_2 = new QDial(MyForm);
        dial_2->setObjectName(QString::fromUtf8("dial_2"));
        sizePolicy2.setHeightForWidth(dial_2->sizePolicy().hasHeightForWidth());
        dial_2->setSizePolicy(sizePolicy2);
        dial_2->setMinimumSize(QSize(130, 240));
        dial_2->setStyleSheet(QString::fromUtf8("background-color: rgb(235, 30, 30);"));
        dial_2->setMaximum(59);

        verticalLayout_2->addWidget(dial_2);


        horizontalLayout_2->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(MyForm);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(pushButton);


        verticalLayout_3->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout_3);


        retranslateUi(MyForm);
        QObject::connect(dial, SIGNAL(valueChanged(int)), lcdNumber, SLOT(display(int)));
        QObject::connect(pushButton, SIGNAL(clicked()), MyForm, SLOT(close()));
        QObject::connect(dial_2, SIGNAL(valueChanged(int)), lcdNumber_2, SLOT(display(int)));

        QMetaObject::connectSlotsByName(MyForm);
    } // setupUi

    void retranslateUi(QWidget *MyForm)
    {
        MyForm->setWindowTitle(QCoreApplication::translate("MyForm", "Form", nullptr));
        label->setText(QCoreApplication::translate("MyForm", "Hours", nullptr));
        label_2->setText(QCoreApplication::translate("MyForm", "Minutes", nullptr));
        pushButton->setText(QCoreApplication::translate("MyForm", "Exit", nullptr));
#if QT_CONFIG(shortcut)
        pushButton->setShortcut(QCoreApplication::translate("MyForm", "Alt+X", nullptr));
#endif // QT_CONFIG(shortcut)
    } // retranslateUi

};

namespace Ui {
    class MyForm: public Ui_MyForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYFORM_H
