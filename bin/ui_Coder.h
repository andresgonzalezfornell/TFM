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
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Coder
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *input;
    QVBoxLayout *verticalLayout_3;
    QLineEdit *input_filename;
    QDialogButtonBox *input_controls;
    QGroupBox *coding;
    QVBoxLayout *verticalLayout_2;
    QDialogButtonBox *coding_controls;
    QGroupBox *output;
    QVBoxLayout *verticalLayout;
    QLineEdit *output_filename;
    QDialogButtonBox *output_controls;

    void setupUi(QDialog *Coder)
    {
        if (Coder->objectName().isEmpty())
            Coder->setObjectName(QStringLiteral("Coder"));
        Coder->setWindowModality(Qt::WindowModal);
        Coder->resize(631, 159);
        horizontalLayout = new QHBoxLayout(Coder);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        input = new QGroupBox(Coder);
        input->setObjectName(QStringLiteral("input"));
        verticalLayout_3 = new QVBoxLayout(input);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(5, 5, 5, 5);
        input_filename = new QLineEdit(input);
        input_filename->setObjectName(QStringLiteral("input_filename"));
        input_filename->setEnabled(false);

        verticalLayout_3->addWidget(input_filename);

        input_controls = new QDialogButtonBox(input);
        input_controls->setObjectName(QStringLiteral("input_controls"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(input_controls->sizePolicy().hasHeightForWidth());
        input_controls->setSizePolicy(sizePolicy);
        input_controls->setStandardButtons(QDialogButtonBox::Open);
        input_controls->setCenterButtons(true);

        verticalLayout_3->addWidget(input_controls);


        horizontalLayout->addWidget(input);

        coding = new QGroupBox(Coder);
        coding->setObjectName(QStringLiteral("coding"));
        verticalLayout_2 = new QVBoxLayout(coding);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(5, 5, 5, 5);
        coding_controls = new QDialogButtonBox(coding);
        coding_controls->setObjectName(QStringLiteral("coding_controls"));
        sizePolicy.setHeightForWidth(coding_controls->sizePolicy().hasHeightForWidth());
        coding_controls->setSizePolicy(sizePolicy);
        coding_controls->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Reset);
        coding_controls->setCenterButtons(true);

        verticalLayout_2->addWidget(coding_controls);


        horizontalLayout->addWidget(coding);

        output = new QGroupBox(Coder);
        output->setObjectName(QStringLiteral("output"));
        verticalLayout = new QVBoxLayout(output);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        output_filename = new QLineEdit(output);
        output_filename->setObjectName(QStringLiteral("output_filename"));
        output_filename->setEnabled(false);

        verticalLayout->addWidget(output_filename);

        output_controls = new QDialogButtonBox(output);
        output_controls->setObjectName(QStringLiteral("output_controls"));
        sizePolicy.setHeightForWidth(output_controls->sizePolicy().hasHeightForWidth());
        output_controls->setSizePolicy(sizePolicy);
        output_controls->setOrientation(Qt::Horizontal);
        output_controls->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        output_controls->setCenterButtons(true);

        verticalLayout->addWidget(output_controls);


        horizontalLayout->addWidget(output);


        retranslateUi(Coder);
        QObject::connect(output_controls, SIGNAL(accepted()), Coder, SLOT(accept()));
        QObject::connect(output_controls, SIGNAL(rejected()), Coder, SLOT(reject()));

        QMetaObject::connectSlotsByName(Coder);
    } // setupUi

    void retranslateUi(QDialog *Coder)
    {
        Coder->setWindowTitle(QApplication::translate("Coder", "Coder", Q_NULLPTR));
        input->setTitle(QApplication::translate("Coder", "Input", Q_NULLPTR));
        coding->setTitle(QApplication::translate("Coder", "Coding", Q_NULLPTR));
        output->setTitle(QApplication::translate("Coder", "Output", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Coder: public Ui_Coder {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CODER_H
