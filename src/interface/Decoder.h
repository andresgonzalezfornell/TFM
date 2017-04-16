#ifndef DECODER_H
#define DECODER_H

// C libraries
#include "iostream"
#include "stdio.h"
#include "stdlib.h"
#include "setjmp.h"
#include "string.h"
// Qt libraries
#include "QApplication"
#include "QMainWindow"
// Classes and local files
#include "Encoder.h"
#include "ChannelsList.h"
#include "AudioFile.h"
#include "../tools/Logger.h"
extern "C" {
#include "../sac/sac_decoder.h"
}

/**
 * @brief   SAC decoder parameter upmix type
 */
struct UpmixType {
    enum upmixtype {
        normal = 0,
        blind = 1,
        binaural = 2,
        stereo = 3
    };
};
/**
 * @brief   SAC decoder parameter decoding type
 */
struct DecodingType {
    enum decodingtype {
        low = 0,
        high = 1
    };
};
/**
 * @brief   SAC decoder parameter binaural quality
 */
struct BinauralQuality {
    enum binauralquality {
        parametric = 0,
        filtering = 1
    };
};
/**
 * @brief   SAC decoder parameter HRTF model
 */
struct HRTFModel {
    enum hrtfmodel {
        kemar = 0,
        vast = 1,
        mps_vt = 2
    };
};

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
    const int fs = 44100;                               /**< signal sampling frequency [Hz] */
    explicit Decoder(QWidget *parent = 0);
    ~Decoder();
    void play();
    void pause();
    void mute();
    void unmute();
    void reset();
    void updateControls();
    void setSource(std::string filename);
    void setBitstream(std::string filename);
    void setInput(std::string filename);
    void setUpmixType(UpmixType::upmixtype upmixtype);
    void setDecodingType(DecodingType::decodingtype decodingtype);
    void setBinauralQuality(BinauralQuality::binauralquality binauralquality);
    void setHRTFModel(HRTFModel::hrtfmodel hrtfmodel);
private:
    Ui::Decoder *ui;                                    /**< user interface object */
    ChannelsList *channels_input;                       /**< input channels list */
    AudioFile *source;                                  /**< encoded source file object */
    QFile *bitstream;                                   /**< encoded bit stream file object */
    AudioFile *input;                                   /**< decoded input file object */
    bool buried;                                        /**< SAC decoder parameter */
    DecodingType::decodingtype decodingtype;            /**< SAC decoder parameter */
    UpmixType::upmixtype upmixtype;                     /**< SAC decoder parameter */
    BinauralQuality::binauralquality binauralquality;   /**< SAC decoder parameter */
    HRTFModel::hrtfmodel hrtfmodel;                     /**< SAC decoder parameter */
    bool debuggermode;                                  /**< SAC decoder parameter */
private slots:
    // Input & output
    void setPlayback(bool state);
    void openInfo();
    // Decoder
    void load();
    void encode();
    void decode();
    void setBuried(bool state);
    void toggleUpmixType(QAction *item);
    void toggleDecodingType(QAction *item);
    void toggleBinauralQuality(QAction *item);
    void toggleHRTFModel(QAction *item);
    void setDebuggerMode(bool state);
};

#endif // DECODER_H
