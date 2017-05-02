#ifndef CODER_H
#define CODER_H

// System libraries
#include "stdlib.h"
#include "cmath"
// Qt libraries
#include "QApplication"
#include "QMainWindow"
// Classes and local files
#include "ui_Encoder.h"
#include "AudioChart.h"
#include "../process/File.h"
#include "../tools/Logger.h"
extern "C" {
#include "../sac/sac_encoder.h"
}

namespace Ui {
class Encoder;
}

/**
 * @class	Encoder
 * @author	Andrés González Fornell
 * @brief	Encoder window interface.
 */
class Encoder : public QDialog
{
    Q_OBJECT
public:
    int fs;                             /**< signal sampling frequency [Hz] */
    WAVFile *input;                     /**< input file object */
    WAVFile *output;                    /**< output file object */
    File *bitstream;                    /**< output bit stream file object */
    Encoder(QWidget *parent = 0);
    ~Encoder();
    void setInput(std::string filename);
    void setOutput(std::string filename);
    void setTree(int tree);
private:
    Ui::Encoder *ui;                    /**< user interface object */
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
