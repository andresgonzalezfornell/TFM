#ifndef CODER_H
#define CODER_H

// System libraries
#include "stdio.h"
#include "string.h"
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
extern "C" {
#include "../sac/sac_encoder.h"
}

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
    void setInput(std::string filepath);
    void setOutput(std::string filepath);
    void setTree(int tree);
private:
    Ui::Coder *ui;                      /**< user interface object */
    ChannelsConfiguration *channels;    /**< channels panel object */
    int fs;                             /**< signal sampling frequency */
    int tree;                           /**< SAC encoder parameter */
private slots:
    // Configuration
    void setBitstream();
    void setBuried(bool state);
    void toggleTree();
    // Buttons
    void load();
    void reset();
    void apply();
    void cancel();
    void submit();
};

#endif // CODER_H
