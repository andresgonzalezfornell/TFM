#include "AudioInfo.h"
#include "ui_AudioInfo.h"

/**
 * @brief   AudioInfo constructor.
 */
AudioInfo::AudioInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AudioInfo),
    file(false)
{
    ui->setupUi(this);
    consolelog("AudioInfo",LogType::progress,"AudioInfo object is created");
}

/**
 * @brief   AudioInfo destructor.
 */
AudioInfo::~AudioInfo()
{
    delete ui;
    consolelog("AudioInfo",LogType::progress,"AudioInfo object is deleted");

}

void AudioInfo::setFile(WAVFile *file) {
    QTableWidget *table = ui->infotable;
    QStringList labels;
    int columns = 2;
    int rows = 13;
    labels << "Chunk ID" << "Chunk size" << "Format";
    labels << "Subchunk 1 ID" << "Subchunk 1 size" << "Audio format" << "Number of channels" << "Sample rate" << "Byte rate" << "Block align" << "Bits per sample";
    labels << "Subchunk 2 ID" << "Subchunk 2 size";
    table->setColumnCount(columns);
    table->setRowCount(rows);
    table->setColumnWidth(0,150);
    QVector<std::string> items_columns = QVector<std::string>(columns,"");
    QVector<QVector<std::string>> items = QVector<QVector<std::string>>(rows,items_columns);
    // Items
    items[0][0] ="Chunk ID";
    items[0][1] = file->header.chunkID;
    items[1][0] ="Chunk size";
    items[1][1] = std::to_string(file->header.chunksize);
    items[2][0] ="Format";
    items[2][1] = file->header.format;
    items[3][0] ="Subchunk 1 ID";
    items[3][1] = file->header.subchunk1ID;
    items[4][0] ="Subchunk 1 size";
    items[4][1] = std::to_string(file->header.subchunk1size);
    items[5][0] ="Audio format";
    items[5][1] = std::to_string(file->header.audioformat);
    items[6][0] ="Number of channels";
    items[6][1] = std::to_string(file->header.numchannels);
    items[7][0] ="Sample rate";
    items[7][1] = std::to_string(file->header.samplerate);
    items[8][0] ="Byte rate";
    items[8][1] = std::to_string(file->header.byterate);
    items[9][0] ="Block align";
    items[9][1] = std::to_string(file->header.blockalign);
    items[10][0] ="Bits per sample";
    items[10][1] = std::to_string(file->header.bitspersample);
    items[11][0] ="Subchunk 2 ID";
    items[11][1] = file->header.subchunk2ID;
    items[12][0] ="Subchunk 2 size";
    items[12][1] = std::to_string(file->header.subchunk2size);
    for (int row = 0; row < rows; row++) {
        for(int column = 0; column < columns; column++) {
            QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(items[row][column]));
            table->setItem(row,column,item);
        }
    }
}
