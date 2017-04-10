#ifndef CODER_H
#define CODER_H

// C libraries
#include "stdio.h"
#include "string"
#include "stdlib.h"
#include "math.h"
// Qt libraries
#include "QApplication"
#include "QMainWindow"
// Classes and local files
#include "ui_Coder.h"
#include "AudioChart.h"
#include "ChannelsConfiguration.h"
#include "../tools/Logger.h"

namespace Ui {
class Coder;
}

/**
 * @class	Coder
 * @author	Andrés González Fornell
 * @brief	Coder window interface.
 */
class Coder : public QDialog
{
    Q_OBJECT
public:
    WAVFile *input;                     /**< input file object */
    AudioFile *output;                  /**< output file object */
    Coder(QWidget *parent = 0);
    ~Coder();
    void setFilename(std::string filepath, QLineEdit *framework);
private:
    Ui::Coder *ui;                      /**< user interface object */
    ChannelsConfiguration *channels;    /**< channels panel object */
    int fs;                             /**< signal sampling frequency */
private slots:
    // Input
    void loadInput();
    // Coding
    void reset();
    void apply();
    // Output
    void cancel();
    void submit();
};

#endif // CODER_H
