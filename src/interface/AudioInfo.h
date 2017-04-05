#ifndef AUDIOINFO_H
#define AUDIOINFO_H

// System libraries
#include "stdlib.h"
// Qt libraries
#include "QDialog"
#include "QTableView"
#include "QVector"
// Classes and local files
#include "AudioFile.h"
#include "../tools/Logger.h"

namespace Ui {
class AudioInfo;
}

/**
 * @class	AudioInfo
 * @author	Andrés González Fornell
 * @brief	Audio object info dialog class.
 */
class AudioInfo : public QDialog
{
    Q_OBJECT
public:
    explicit AudioInfo(QWidget *parent = 0);
    ~AudioInfo();
    void setFile(AudioFile *file);
private:
    Ui::AudioInfo *ui;
    AudioFile file;
};

#endif // AUDIOINFO_H
