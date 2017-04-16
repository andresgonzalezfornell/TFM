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
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
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
    QAction *menu_source_load;
    QAction *menu_source_encode;
    QAction *menu_debuggermode;
    QAction *menu_upmixtype_normal;
    QAction *menu_upmixtype_blind;
    QAction *menu_upmixtype_binaural;
    QAction *menu_upmixtype_stereo;
    QAction *menu_decodingtype_low;
    QAction *menu_decodingtype_high;
    QAction *menu_binauralquality_parametric;
    QAction *menu_binauralquality_filtering;
    QAction *menu_htrfmodel_kemar;
    QAction *menu_htrfmodel_vast;
    QAction *menu_htrfmodel_mpsvt;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *input_group;
    QVBoxLayout *input_layout;
    QHBoxLayout *source_layout;
    QLabel *source_label;
    QLineEdit *source_filename;
    QHBoxLayout *inputfile_layout;
    QLabel *input_label;
    QLineEdit *input_filename;
    QScrollArea *input_channels;
    QWidget *input_list;
    QVBoxLayout *input_list_layout;
    QWidget *input_chart;
    QPushButton *input_start;
    QGroupBox *effect_group;
    QGroupBox *output_group;
    QVBoxLayout *output_layout;
    QScrollArea *output_channels;
    QWidget *output_list;
    QVBoxLayout *output_list_layout;
    QWidget *output_chart;
    QMenuBar *menuBar;
    QMenu *menu_source;
    QMenu *menu_decoder;
    QMenu *menu_upmixtype;
    QMenu *menu_decodingtype;
    QMenu *menu_binauralquality;
    QMenu *menu_htrfmodel;
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
        menu_source_load = new QAction(Decoder);
        menu_source_load->setObjectName(QStringLiteral("menu_source_load"));
        menu_source_encode = new QAction(Decoder);
        menu_source_encode->setObjectName(QStringLiteral("menu_source_encode"));
        menu_debuggermode = new QAction(Decoder);
        menu_debuggermode->setObjectName(QStringLiteral("menu_debuggermode"));
        menu_debuggermode->setCheckable(true);
        menu_upmixtype_normal = new QAction(Decoder);
        menu_upmixtype_normal->setObjectName(QStringLiteral("menu_upmixtype_normal"));
        menu_upmixtype_normal->setCheckable(true);
        menu_upmixtype_blind = new QAction(Decoder);
        menu_upmixtype_blind->setObjectName(QStringLiteral("menu_upmixtype_blind"));
        menu_upmixtype_blind->setCheckable(true);
        menu_upmixtype_binaural = new QAction(Decoder);
        menu_upmixtype_binaural->setObjectName(QStringLiteral("menu_upmixtype_binaural"));
        menu_upmixtype_binaural->setCheckable(true);
        menu_upmixtype_stereo = new QAction(Decoder);
        menu_upmixtype_stereo->setObjectName(QStringLiteral("menu_upmixtype_stereo"));
        menu_upmixtype_stereo->setCheckable(true);
        menu_decodingtype_low = new QAction(Decoder);
        menu_decodingtype_low->setObjectName(QStringLiteral("menu_decodingtype_low"));
        menu_decodingtype_low->setCheckable(true);
        menu_decodingtype_high = new QAction(Decoder);
        menu_decodingtype_high->setObjectName(QStringLiteral("menu_decodingtype_high"));
        menu_decodingtype_high->setCheckable(true);
        menu_binauralquality_parametric = new QAction(Decoder);
        menu_binauralquality_parametric->setObjectName(QStringLiteral("menu_binauralquality_parametric"));
        menu_binauralquality_parametric->setCheckable(true);
        menu_binauralquality_filtering = new QAction(Decoder);
        menu_binauralquality_filtering->setObjectName(QStringLiteral("menu_binauralquality_filtering"));
        menu_binauralquality_filtering->setCheckable(true);
        menu_htrfmodel_kemar = new QAction(Decoder);
        menu_htrfmodel_kemar->setObjectName(QStringLiteral("menu_htrfmodel_kemar"));
        menu_htrfmodel_kemar->setCheckable(true);
        menu_htrfmodel_vast = new QAction(Decoder);
        menu_htrfmodel_vast->setObjectName(QStringLiteral("menu_htrfmodel_vast"));
        menu_htrfmodel_vast->setCheckable(true);
        menu_htrfmodel_mpsvt = new QAction(Decoder);
        menu_htrfmodel_mpsvt->setObjectName(QStringLiteral("menu_htrfmodel_mpsvt"));
        menu_htrfmodel_mpsvt->setCheckable(true);
        centralWidget = new QWidget(Decoder);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        input_group = new QGroupBox(centralWidget);
        input_group->setObjectName(QStringLiteral("input_group"));
        input_group->setMinimumSize(QSize(250, 0));
        input_group->setMaximumSize(QSize(250, 16777215));
        input_layout = new QVBoxLayout(input_group);
        input_layout->setSpacing(5);
        input_layout->setContentsMargins(11, 11, 11, 11);
        input_layout->setObjectName(QStringLiteral("input_layout"));
        input_layout->setContentsMargins(5, 5, 5, 5);
        source_layout = new QHBoxLayout();
        source_layout->setSpacing(5);
        source_layout->setObjectName(QStringLiteral("source_layout"));
        source_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
        source_label = new QLabel(input_group);
        source_label->setObjectName(QStringLiteral("source_label"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(source_label->sizePolicy().hasHeightForWidth());
        source_label->setSizePolicy(sizePolicy1);
        source_label->setMinimumSize(QSize(50, 0));

        source_layout->addWidget(source_label);

        source_filename = new QLineEdit(input_group);
        source_filename->setObjectName(QStringLiteral("source_filename"));
        source_filename->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(source_filename->sizePolicy().hasHeightForWidth());
        source_filename->setSizePolicy(sizePolicy2);
        source_filename->setReadOnly(true);

        source_layout->addWidget(source_filename);


        input_layout->addLayout(source_layout);

        inputfile_layout = new QHBoxLayout();
        inputfile_layout->setSpacing(5);
        inputfile_layout->setObjectName(QStringLiteral("inputfile_layout"));
        input_label = new QLabel(input_group);
        input_label->setObjectName(QStringLiteral("input_label"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(input_label->sizePolicy().hasHeightForWidth());
        input_label->setSizePolicy(sizePolicy3);
        input_label->setMinimumSize(QSize(50, 0));

        inputfile_layout->addWidget(input_label);

        input_filename = new QLineEdit(input_group);
        input_filename->setObjectName(QStringLiteral("input_filename"));
        input_filename->setReadOnly(true);

        inputfile_layout->addWidget(input_filename);


        input_layout->addLayout(inputfile_layout);

        input_channels = new QScrollArea(input_group);
        input_channels->setObjectName(QStringLiteral("input_channels"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(13);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(input_channels->sizePolicy().hasHeightForWidth());
        input_channels->setSizePolicy(sizePolicy4);
        input_channels->setFrameShape(QFrame::StyledPanel);
        input_channels->setFrameShadow(QFrame::Sunken);
        input_channels->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        input_channels->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        input_channels->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        input_channels->setWidgetResizable(true);
        input_channels->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        input_list = new QWidget();
        input_list->setObjectName(QStringLiteral("input_list"));
        input_list->setGeometry(QRect(0, 0, 215, 90));
        input_list_layout = new QVBoxLayout(input_list);
        input_list_layout->setSpacing(6);
        input_list_layout->setContentsMargins(11, 11, 11, 11);
        input_list_layout->setObjectName(QStringLiteral("input_list_layout"));
        input_list_layout->setContentsMargins(5, 5, 5, 5);
        input_channels->setWidget(input_list);

        input_layout->addWidget(input_channels);

        input_chart = new QWidget(input_group);
        input_chart->setObjectName(QStringLiteral("input_chart"));
        input_chart->setMinimumSize(QSize(200, 150));
        input_chart->setMaximumSize(QSize(300, 150));
        input_chart->setToolTipDuration(-1);

        input_layout->addWidget(input_chart);

        input_start = new QPushButton(input_group);
        input_start->setObjectName(QStringLiteral("input_start"));
        input_start->setCheckable(true);

        input_layout->addWidget(input_start);


        horizontalLayout->addWidget(input_group);

        effect_group = new QGroupBox(centralWidget);
        effect_group->setObjectName(QStringLiteral("effect_group"));

        horizontalLayout->addWidget(effect_group);

        output_group = new QGroupBox(centralWidget);
        output_group->setObjectName(QStringLiteral("output_group"));
        output_group->setMinimumSize(QSize(250, 0));
        output_group->setMaximumSize(QSize(250, 16777215));
        output_layout = new QVBoxLayout(output_group);
        output_layout->setSpacing(5);
        output_layout->setContentsMargins(11, 11, 11, 11);
        output_layout->setObjectName(QStringLiteral("output_layout"));
        output_layout->setContentsMargins(5, 5, 5, 5);
        output_channels = new QScrollArea(output_group);
        output_channels->setObjectName(QStringLiteral("output_channels"));
        sizePolicy4.setHeightForWidth(output_channels->sizePolicy().hasHeightForWidth());
        output_channels->setSizePolicy(sizePolicy4);
        output_channels->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        output_channels->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        output_channels->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        output_channels->setWidgetResizable(true);
        output_channels->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        output_list = new QWidget();
        output_list->setObjectName(QStringLiteral("output_list"));
        output_list->setGeometry(QRect(0, 0, 217, 312));
        output_list_layout = new QVBoxLayout(output_list);
        output_list_layout->setSpacing(6);
        output_list_layout->setContentsMargins(11, 11, 11, 11);
        output_list_layout->setObjectName(QStringLiteral("output_list_layout"));
        output_list_layout->setContentsMargins(5, 5, 5, 5);
        output_channels->setWidget(output_list);

        output_layout->addWidget(output_channels);

        output_chart = new QWidget(output_group);
        output_chart->setObjectName(QStringLiteral("output_chart"));
        output_chart->setMinimumSize(QSize(200, 150));
        output_chart->setMaximumSize(QSize(300, 150));

        output_layout->addWidget(output_chart);


        horizontalLayout->addWidget(output_group);

        Decoder->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Decoder);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 779, 22));
        menu_source = new QMenu(menuBar);
        menu_source->setObjectName(QStringLiteral("menu_source"));
        menu_decoder = new QMenu(menuBar);
        menu_decoder->setObjectName(QStringLiteral("menu_decoder"));
        menu_upmixtype = new QMenu(menu_decoder);
        menu_upmixtype->setObjectName(QStringLiteral("menu_upmixtype"));
        menu_decodingtype = new QMenu(menu_decoder);
        menu_decodingtype->setObjectName(QStringLiteral("menu_decodingtype"));
        menu_binauralquality = new QMenu(menu_decoder);
        menu_binauralquality->setObjectName(QStringLiteral("menu_binauralquality"));
        menu_htrfmodel = new QMenu(menu_decoder);
        menu_htrfmodel->setObjectName(QStringLiteral("menu_htrfmodel"));
        Decoder->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Decoder);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Decoder->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Decoder);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Decoder->setStatusBar(statusBar);

        menuBar->addAction(menu_source->menuAction());
        menuBar->addAction(menu_decoder->menuAction());
        menu_source->addAction(menu_source_load);
        menu_source->addAction(menu_source_encode);
        menu_decoder->addAction(menu_upmixtype->menuAction());
        menu_decoder->addAction(menu_decodingtype->menuAction());
        menu_decoder->addAction(menu_binauralquality->menuAction());
        menu_decoder->addAction(menu_htrfmodel->menuAction());
        menu_decoder->addAction(menu_debuggermode);
        menu_upmixtype->addAction(menu_upmixtype_normal);
        menu_upmixtype->addAction(menu_upmixtype_blind);
        menu_upmixtype->addAction(menu_upmixtype_binaural);
        menu_upmixtype->addAction(menu_upmixtype_stereo);
        menu_decodingtype->addAction(menu_decodingtype_low);
        menu_decodingtype->addAction(menu_decodingtype_high);
        menu_binauralquality->addAction(menu_binauralquality_parametric);
        menu_binauralquality->addAction(menu_binauralquality_filtering);
        menu_htrfmodel->addAction(menu_htrfmodel_kemar);
        menu_htrfmodel->addAction(menu_htrfmodel_vast);
        menu_htrfmodel->addAction(menu_htrfmodel_mpsvt);

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
        menu_source_load->setText(QApplication::translate("Decoder", "Load from a file", Q_NULLPTR));
        menu_source_encode->setText(QApplication::translate("Decoder", "Load from encoder", Q_NULLPTR));
        menu_debuggermode->setText(QApplication::translate("Decoder", "Debugger", Q_NULLPTR));
        menu_upmixtype_normal->setText(QApplication::translate("Decoder", "Normal", Q_NULLPTR));
        menu_upmixtype_blind->setText(QApplication::translate("Decoder", "Blind", Q_NULLPTR));
        menu_upmixtype_binaural->setText(QApplication::translate("Decoder", "Binaural", Q_NULLPTR));
        menu_upmixtype_stereo->setText(QApplication::translate("Decoder", "Stereo", Q_NULLPTR));
        menu_decodingtype_low->setText(QApplication::translate("Decoder", "Low", Q_NULLPTR));
        menu_decodingtype_high->setText(QApplication::translate("Decoder", "High", Q_NULLPTR));
        menu_binauralquality_parametric->setText(QApplication::translate("Decoder", "Parametric", Q_NULLPTR));
        menu_binauralquality_filtering->setText(QApplication::translate("Decoder", "Filtering", Q_NULLPTR));
        menu_htrfmodel_kemar->setText(QApplication::translate("Decoder", "Kemar", Q_NULLPTR));
        menu_htrfmodel_vast->setText(QApplication::translate("Decoder", "Vast", Q_NULLPTR));
        menu_htrfmodel_mpsvt->setText(QApplication::translate("Decoder", "MPS VT", Q_NULLPTR));
        input_group->setTitle(QApplication::translate("Decoder", "Input", Q_NULLPTR));
        source_label->setText(QApplication::translate("Decoder", "Source", Q_NULLPTR));
        input_label->setText(QApplication::translate("Decoder", "Input", Q_NULLPTR));
        input_start->setText(QApplication::translate("Decoder", "Start", Q_NULLPTR));
        effect_group->setTitle(QApplication::translate("Decoder", "Effect", Q_NULLPTR));
        output_group->setTitle(QApplication::translate("Decoder", "Output", Q_NULLPTR));
        menu_source->setTitle(QApplication::translate("Decoder", "Source", Q_NULLPTR));
        menu_decoder->setTitle(QApplication::translate("Decoder", "Decoder", Q_NULLPTR));
        menu_upmixtype->setTitle(QApplication::translate("Decoder", "Upmix type", Q_NULLPTR));
        menu_decodingtype->setTitle(QApplication::translate("Decoder", "Decoding type", Q_NULLPTR));
        menu_binauralquality->setTitle(QApplication::translate("Decoder", "Binaural upmix quality", Q_NULLPTR));
        menu_htrfmodel->setTitle(QApplication::translate("Decoder", "HTRF model", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Decoder: public Ui_Decoder {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DECODER_H
