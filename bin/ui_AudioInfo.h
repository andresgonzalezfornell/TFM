/********************************************************************************
** Form generated from reading UI file 'AudioInfo.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUDIOINFO_H
#define UI_AUDIOINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AudioInfo
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *infotable;

    void setupUi(QDialog *AudioInfo)
    {
        if (AudioInfo->objectName().isEmpty())
            AudioInfo->setObjectName(QStringLiteral("AudioInfo"));
        AudioInfo->resize(363, 419);
        verticalLayout = new QVBoxLayout(AudioInfo);
#ifndef Q_OS_MAC
        verticalLayout->setSpacing(-1);
#endif
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        infotable = new QTableWidget(AudioInfo);
        infotable->setObjectName(QStringLiteral("infotable"));
        infotable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        infotable->horizontalHeader()->setCascadingSectionResizes(false);
        infotable->horizontalHeader()->setStretchLastSection(true);
        infotable->verticalHeader()->setCascadingSectionResizes(false);
        infotable->verticalHeader()->setStretchLastSection(false);

        verticalLayout->addWidget(infotable);


        retranslateUi(AudioInfo);

        QMetaObject::connectSlotsByName(AudioInfo);
    } // setupUi

    void retranslateUi(QDialog *AudioInfo)
    {
        AudioInfo->setWindowTitle(QApplication::translate("AudioInfo", "Audio info", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AudioInfo: public Ui_AudioInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUDIOINFO_H
