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

//#include "libtsp.h"
//#include "libtsp/AFpar.h"

//#include "sac_dec.h"
//#include "lib/SAC/sac_dec/include/sac_polyphase.h"
//#include "spatial_filereader.h"
//#include "spatial_bdreader.h"
//#include "spatial_hrtfreader.h"
//#include "sac_parallelReverb.h"

#include "spatial_frontend.c"

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
    void decode();
};

#endif // DECODER_H
