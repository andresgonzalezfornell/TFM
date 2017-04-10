// Class libraries
#include "Decoder.h"
#include "ui_Decoder.h"

int ChannelsList::fs;

/**
 * @brief	Decoder constructor.
 */
Decoder::Decoder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Decoder)
{
    ui->setupUi(this);
    this->file = NULL;
    QObject::connect(ui->menu_file_load,SIGNAL(triggered(bool)),this,SLOT(load()));
    QObject::connect(ui->menu_file_encode,SIGNAL(triggered(bool)),this,SLOT(encode()));
    QObject::connect(ui->input_start,SIGNAL(released()),this,SLOT(decode()));
    // Channels
    ChannelsList::fs = 44100;
    consolelog("Decoder", LogType::progress, "Decoder object is created");
}

/**
 * @brief	Decoder destructor.
 *
 */
Decoder::~Decoder()
{
    delete ui;
    consolelog("Decoder", LogType::progress, "Decoder object is deleted");
}

/**
 * @brief   It sets the signal sampling frequency.
 */
void Decoder::setfs(int fs) {
    ChannelsList::fs = fs;
}

/**
 * @brief   It loads a file as decoder input.
 */
void Decoder::load() {
    std::string filepath = QFileDialog::getOpenFileName(NULL, "Load audio file","","*.wav").toStdString();
    if(filepath=="") {
        consolelog("Decoder",LogType::interaction,"canceling source file selection for decoder");
    } else {
        consolelog("Decoder",LogType::interaction,"selected \"" + filepath + "\" as source file for decoder");
        this->file = new AudioFile(filepath);
    }
}

/**
 * @brief   It encodes a new audio through Coder.
 */
void Decoder::encode() {
    Coder *coder = new Coder();
    coder->exec();
    this->file = coder->output;
}

/**
 * @brief   It decodes the audio input.
 */
void Decoder::decode() {
//    spatialDec* ourDec;
//    long int nSamples;
//    long int nChannels;
//    double sampleFreq;
//    int qmfBands = 0;
//    int nTimeSlots;

//    AFILE* pcmIn = 0;
//    HANDLE_FILE_READER fileReader = NULL;
//    HANDLE_BURIED_DATA_READER bdReader = NULL;
//    BD_INPUT_BUFFER bdBuffer;
//    HANDLE_BYTE_READER bitstream = NULL;
//  #ifdef HRTF_DYNAMIC_UPDATE
//    HANDLE_HRTF_READER hrtfReader = NULL;
//    HANDLE_HRTF_SOURCE hrtfSource = NULL;
//  #endif

//    int done = 0;
//    int samplesToReadPerCall;
//    int samplesRead;
//    float* inSamples;
//    float* inSamplesDeinterleaved;

//    int i;
//    int j;
//    int channel;
//    int ts;
//    long int offset=0;

//    SAC_POLYPHASE_ANA_FILTERBANK *filterbank[MAX_INPUT_CHANNELS] = {0};
//    float** qmfInReal[MAX_INPUT_CHANNELS];
//    float** qmfInImag[MAX_INPUT_CHANNELS];

//    float** inPointers[2*MAX_INPUT_CHANNELS];


//    PRINT(SE,"***** MPEG Surround Decoder - RefSoft Version 1.10 (PCM downmix) *****\n");
//    PRINT(SE,"29 January 2009\n");
//    PRINT(SE,"\n");


    char * argv[1];
    spatial_frontend(1, argv);

}
