/********************************************************************************
** Form generated from reading UI file 'Decoder.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DECODER_H
#define UI_DECODER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Decoder
{
public:
    QAction *channels_fs_0;
    QAction *channels_fs_1;
    QAction *channels_fs_2;
    QAction *channels_fs_3;
    QAction *channels_fs_4;
    QAction *menu_file_load;
    QAction *menu_file_encode;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *input;
    QVBoxLayout *input_layout;
    QScrollArea *input_channels;
    QWidget *input_list;
    QVBoxLayout *input_list_layout;
    QWidget *input_chart;
    QPushButton *input_start;
    QGroupBox *effect;
    QGroupBox *output;
    QVBoxLayout *output_layout;
    QScrollArea *output_channels;
    QWidget *output_list;
    QVBoxLayout *output_list_layout;
    QWidget *output_chart;
    QMenuBar *menuBar;
    QMenu *menu_file;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Decoder)
    {
        if (Decoder->objectName().isEmpty())
            Decoder->setObjectName(QStringLiteral("Decoder"));
        Decoder->resize(779, 571);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Decoder->sizePolicy().hasHeightForWidth());
        Decoder->setSizePolicy(sizePolicy);
        channels_fs_0 = new QAction(Decoder);
        channels_fs_0->setObjectName(QStringLiteral("channels_fs_0"));
        channels_fs_0->setCheckable(true);
        channels_fs_1 = new QAction(Decoder);
        channels_fs_1->setObjectName(QStringLiteral("channels_fs_1"));
        channels_fs_1->setCheckable(true);
        channels_fs_2 = new QAction(Decoder);
        channels_fs_2->setObjectName(QStringLiteral("channels_fs_2"));
        channels_fs_2->setCheckable(true);
        channels_fs_3 = new QAction(Decoder);
        channels_fs_3->setObjectName(QStringLiteral("channels_fs_3"));
        channels_fs_3->setCheckable(true);
        channels_fs_4 = new QAction(Decoder);
        channels_fs_4->setObjectName(QStringLiteral("channels_fs_4"));
        channels_fs_4->setCheckable(true);
        menu_file_load = new QAction(Decoder);
        menu_file_load->setObjectName(QStringLiteral("menu_file_load"));
        menu_file_encode = new QAction(Decoder);
        menu_file_encode->setObjectName(QStringLiteral("menu_file_encode"));
        centralWidget = new QWidget(Decoder);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        input = new QGroupBox(centralWidget);
        input->setObjectName(QStringLiteral("input"));
        input->setMinimumSize(QSize(250, 0));
        input->setMaximumSize(QSize(250, 16777215));
        input_layout = new QVBoxLayout(input);
        input_layout->setSpacing(0);
        input_layout->setContentsMargins(11, 11, 11, 11);
        input_layout->setObjectName(QStringLiteral("input_layout"));
        input_layout->setContentsMargins(0, 0, 0, 0);
        input_channels = new QScrollArea(input);
        input_channels->setObjectName(QStringLiteral("input_channels"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(13);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(input_channels->sizePolicy().hasHeightForWidth());
        input_channels->setSizePolicy(sizePolicy1);
        input_channels->setFrameShape(QFrame::StyledPanel);
        input_channels->setFrameShadow(QFrame::Sunken);
        input_channels->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        input_channels->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        input_channels->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        input_channels->setWidgetResizable(true);
        input_channels->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        input_list = new QWidget();
        input_list->setObjectName(QStringLiteral("input_list"));
        input_list->setGeometry(QRect(0, 0, 215, 295));
        input_list_layout = new QVBoxLayout(input_list);
        input_list_layout->setSpacing(6);
        input_list_layout->setContentsMargins(11, 11, 11, 11);
        input_list_layout->setObjectName(QStringLiteral("input_list_layout"));
        input_list_layout->setContentsMargins(5, 5, 5, 5);
        input_channels->setWidget(input_list);

        input_layout->addWidget(input_channels);

        input_chart = new QWidget(input);
        input_chart->setObjectName(QStringLiteral("input_chart"));
        input_chart->setMinimumSize(QSize(200, 150));
        input_chart->setMaximumSize(QSize(300, 150));
        input_chart->setToolTipDuration(-1);

        input_layout->addWidget(input_chart);

        input_start = new QPushButton(input);
        input_start->setObjectName(QStringLiteral("input_start"));
        input_start->setCheckable(true);

        input_layout->addWidget(input_start);


        horizontalLayout->addWidget(input);

        effect = new QGroupBox(centralWidget);
        effect->setObjectName(QStringLiteral("effect"));

        horizontalLayout->addWidget(effect);

        output = new QGroupBox(centralWidget);
        output->setObjectName(QStringLiteral("output"));
        output->setMinimumSize(QSize(250, 0));
        output->setMaximumSize(QSize(250, 16777215));
        output_layout = new QVBoxLayout(output);
        output_layout->setSpacing(0);
        output_layout->setContentsMargins(11, 11, 11, 11);
        output_layout->setObjectName(QStringLiteral("output_layout"));
        output_layout->setContentsMargins(0, 0, 0, 0);
        output_channels = new QScrollArea(output);
        output_channels->setObjectName(QStringLiteral("output_channels"));
        sizePolicy1.setHeightForWidth(output_channels->sizePolicy().hasHeightForWidth());
        output_channels->setSizePolicy(sizePolicy1);
        output_channels->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        output_channels->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        output_channels->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        output_channels->setWidgetResizable(true);
        output_channels->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        output_list = new QWidget();
        output_list->setObjectName(QStringLiteral("output_list"));
        output_list->setGeometry(QRect(0, 0, 227, 327));
        output_list_layout = new QVBoxLayout(output_list);
        output_list_layout->setSpacing(6);
        output_list_layout->setContentsMargins(11, 11, 11, 11);
        output_list_layout->setObjectName(QStringLiteral("output_list_layout"));
        output_list_layout->setContentsMargins(5, 5, 5, 5);
        output_channels->setWidget(output_list);

        output_layout->addWidget(output_channels);

        output_chart = new QWidget(output);
        output_chart->setObjectName(QStringLiteral("output_chart"));
        output_chart->setMinimumSize(QSize(200, 150));
        output_chart->setMaximumSize(QSize(300, 150));

        output_layout->addWidget(output_chart);


        horizontalLayout->addWidget(output);

        Decoder->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Decoder);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 779, 22));
        menu_file = new QMenu(menuBar);
        menu_file->setObjectName(QStringLiteral("menu_file"));
        Decoder->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Decoder);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Decoder->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Decoder);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Decoder->setStatusBar(statusBar);

        menuBar->addAction(menu_file->menuAction());
        menu_file->addAction(menu_file_load);
        menu_file->addAction(menu_file_encode);

        retranslateUi(Decoder);

        QMetaObject::connectSlotsByName(Decoder);
    } // setupUi

    void retranslateUi(QMainWindow *Decoder)
    {
        Decoder->setWindowTitle(QApplication::translate("Decoder", "Decoder", Q_NULLPTR));
        channels_fs_0->setText(QApplication::translate("Decoder", "8000 Hz", Q_NULLPTR));
        channels_fs_1->setText(QApplication::translate("Decoder", "11025 Hz", Q_NULLPTR));
        channels_fs_2->setText(QApplication::translate("Decoder", "22050 Hz", Q_NULLPTR));
        channels_fs_3->setText(QApplication::translate("Decoder", "32000 Hz", Q_NULLPTR));
        channels_fs_4->setText(QApplication::translate("Decoder", "44100 Hz", Q_NULLPTR));
        menu_file_load->setText(QApplication::translate("Decoder", "Load", Q_NULLPTR));
        menu_file_encode->setText(QApplication::translate("Decoder", "Encode new", Q_NULLPTR));
        input->setTitle(QApplication::translate("Decoder", "Input", Q_NULLPTR));
        input_start->setText(QApplication::translate("Decoder", "Start", Q_NULLPTR));
        effect->setTitle(QApplication::translate("Decoder", "Effect", Q_NULLPTR));
        output->setTitle(QApplication::translate("Decoder", "Output", Q_NULLPTR));
        menu_file->setTitle(QApplication::translate("Decoder", "File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Decoder: public Ui_Decoder {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DECODER_H
