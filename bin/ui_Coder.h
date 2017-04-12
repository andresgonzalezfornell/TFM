/********************************************************************************
** Form generated from reading UI file 'Coder.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CODER_H
#define UI_CODER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Coder
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *input_group;
    QVBoxLayout *verticalLayout_3;
    QLineEdit *input_filename;
    QDialogButtonBox *input_controls;
    QGroupBox *coding;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *bitstream_group;
    QHBoxLayout *bitstream_layout;
    QLineEdit *bitstream_filename;
    QCheckBox *bitstream_buried;
    QGroupBox *tree_group;
    QHBoxLayout *tree_layout;
    QRadioButton *tree_5151;
    QRadioButton *tree_5152;
    QRadioButton *tree_525;
    QDialogButtonBox *coding_controls;
    QGroupBox *output_group;
    QVBoxLayout *verticalLayout;
    QLineEdit *output_filename;
    QDialogButtonBox *output_controls;

    void setupUi(QDialog *Coder)
    {
        if (Coder->objectName().isEmpty())
            Coder->setObjectName(QStringLiteral("Coder"));
        Coder->setWindowModality(Qt::WindowModal);
        Coder->resize(677, 183);
        horizontalLayout = new QHBoxLayout(Coder);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        input_group = new QGroupBox(Coder);
        input_group->setObjectName(QStringLiteral("input_group"));
        verticalLayout_3 = new QVBoxLayout(input_group);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(5, 5, 5, 5);
        input_filename = new QLineEdit(input_group);
        input_filename->setObjectName(QStringLiteral("input_filename"));
        input_filename->setEnabled(false);

        verticalLayout_3->addWidget(input_filename);

        input_controls = new QDialogButtonBox(input_group);
        input_controls->setObjectName(QStringLiteral("input_controls"));
        input_controls->setStandardButtons(QDialogButtonBox::Open);
        input_controls->setCenterButtons(true);

        verticalLayout_3->addWidget(input_controls);


        horizontalLayout->addWidget(input_group);

        coding = new QGroupBox(Coder);
        coding->setObjectName(QStringLiteral("coding"));
        verticalLayout_2 = new QVBoxLayout(coding);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(5, 5, 5, 5);
        bitstream_group = new QGroupBox(coding);
        bitstream_group->setObjectName(QStringLiteral("bitstream_group"));
        bitstream_layout = new QHBoxLayout(bitstream_group);
        bitstream_layout->setObjectName(QStringLiteral("bitstream_layout"));
        bitstream_layout->setContentsMargins(5, 5, 5, 5);
        bitstream_filename = new QLineEdit(bitstream_group);
        bitstream_filename->setObjectName(QStringLiteral("bitstream_filename"));
        bitstream_filename->setMinimumSize(QSize(0, 20));

        bitstream_layout->addWidget(bitstream_filename);

        bitstream_buried = new QCheckBox(bitstream_group);
        bitstream_buried->setObjectName(QStringLiteral("bitstream_buried"));

        bitstream_layout->addWidget(bitstream_buried);


        verticalLayout_2->addWidget(bitstream_group);

        tree_group = new QGroupBox(coding);
        tree_group->setObjectName(QStringLiteral("tree_group"));
        tree_layout = new QHBoxLayout(tree_group);
        tree_layout->setObjectName(QStringLiteral("tree_layout"));
        tree_layout->setContentsMargins(5, 5, 5, 5);
        tree_5151 = new QRadioButton(tree_group);
        tree_5151->setObjectName(QStringLiteral("tree_5151"));

        tree_layout->addWidget(tree_5151);

        tree_5152 = new QRadioButton(tree_group);
        tree_5152->setObjectName(QStringLiteral("tree_5152"));

        tree_layout->addWidget(tree_5152);

        tree_525 = new QRadioButton(tree_group);
        tree_525->setObjectName(QStringLiteral("tree_525"));

        tree_layout->addWidget(tree_525);


        verticalLayout_2->addWidget(tree_group);

        coding_controls = new QDialogButtonBox(coding);
        coding_controls->setObjectName(QStringLiteral("coding_controls"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(coding_controls->sizePolicy().hasHeightForWidth());
        coding_controls->setSizePolicy(sizePolicy);
        coding_controls->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Reset);
        coding_controls->setCenterButtons(true);

        verticalLayout_2->addWidget(coding_controls);


        horizontalLayout->addWidget(coding);

        output_group = new QGroupBox(Coder);
        output_group->setObjectName(QStringLiteral("output_group"));
        verticalLayout = new QVBoxLayout(output_group);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        output_filename = new QLineEdit(output_group);
        output_filename->setObjectName(QStringLiteral("output_filename"));
        output_filename->setEnabled(false);

        verticalLayout->addWidget(output_filename);

        output_controls = new QDialogButtonBox(output_group);
        output_controls->setObjectName(QStringLiteral("output_controls"));
        sizePolicy.setHeightForWidth(output_controls->sizePolicy().hasHeightForWidth());
        output_controls->setSizePolicy(sizePolicy);
        output_controls->setOrientation(Qt::Horizontal);
        output_controls->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        output_controls->setCenterButtons(true);

        verticalLayout->addWidget(output_controls);


        horizontalLayout->addWidget(output_group);


        retranslateUi(Coder);
        QObject::connect(output_controls, SIGNAL(accepted()), Coder, SLOT(accept()));
        QObject::connect(output_controls, SIGNAL(rejected()), Coder, SLOT(reject()));

        QMetaObject::connectSlotsByName(Coder);
    } // setupUi

    void retranslateUi(QDialog *Coder)
    {
        Coder->setWindowTitle(QApplication::translate("Coder", "Coder", Q_NULLPTR));
        input_group->setTitle(QApplication::translate("Coder", "Input", Q_NULLPTR));
        coding->setTitle(QApplication::translate("Coder", "Coding", Q_NULLPTR));
        bitstream_group->setTitle(QApplication::translate("Coder", "Bitstream", Q_NULLPTR));
        bitstream_buried->setText(QApplication::translate("Coder", "Buried", Q_NULLPTR));
        tree_group->setTitle(QApplication::translate("Coder", "Tree config", Q_NULLPTR));
        tree_5151->setText(QApplication::translate("Coder", "5151", Q_NULLPTR));
        tree_5152->setText(QApplication::translate("Coder", "5152", Q_NULLPTR));
        tree_525->setText(QApplication::translate("Coder", "525", Q_NULLPTR));
        output_group->setTitle(QApplication::translate("Coder", "Output", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Coder: public Ui_Coder {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CODER_H
