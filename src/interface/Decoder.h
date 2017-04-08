#ifndef DECODER_H
#define DECODER_H

// C libraries
#include "iostream"
#include "stdio.h"
#include "string"
// Qt libraries
#include "QApplication"
#include "QMainWindow"
// Classes and local files
#include "Coder.h"
#include "ChannelsList.h"
#include "AudioFile.h"
#include "../tools/Logger.h"

namespace Ui {
class Decoder;
}

/**
 * @class	Decoder
 * @author	Andrés González Fornell
 * @brief	Decoder window interface.
 */
class Decoder : public QMainWindow
{
    Q_OBJECT
public:
    explicit Decoder(QWidget *parent = 0);
    ~Decoder();
    void setfs(int fs);
private:
    Ui::Decoder *ui;        /**< User interface object */
    AudioFile *file;        /**< Coder input file object */
private slots:
    void load();
    void encode();
};

#endif // DECODER_H
