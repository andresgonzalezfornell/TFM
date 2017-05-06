#ifndef AUDIOINFO_H
#define AUDIOINFO_H

// System libraries
#include "stdlib.h"
// Qt libraries
#include "QDialog"
#include "QTableView"
#include "QVector"
// Classes and local files
#include "../process/File.h"
#include "../tools/Logger.h"

namespace Ui {
class AudioInfo;
}

/**
 * @class	AudioInfo
 * @author	Andrés González Fornell
 * @brief	Audio object info dialog class.
 */
class AudioInfo : public QDialog {
    Q_OBJECT
public:
    AudioInfo(QWidget *parent = 0);
    ~AudioInfo();
    void setFile(WAVFile *file);
private:
    Ui::AudioInfo *ui;  /**< user interface object */
    WAVFile file;       /**< audio file object */
};

#endif // AUDIOINFO_H
