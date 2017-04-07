#ifndef CODER_H
#define CODER_H

// C libraries
#include "iostream"
#include "stdio.h"
#include "string"
// Qt libraries
#include "QApplication"
#include "QMainWindow"
// Classes and local files
#include "../tools/Logger.h"
#include "AudioChart.h"
#include "channels.h"

namespace Ui {
class Coder;
}

/**
 * @class	Coder
 * @author	Andrés González Fornell
 * @brief	Coder window interface.
 */
class Coder : public QMainWindow
{
    Q_OBJECT
public:
    explicit Coder(QWidget *parent = 0);
    ~Coder();
    void initialize();
private:
    Ui::Coder *ui;     /**< User interface object */
    Channels *channels;       /**< Channels panel object */
    void setfs(int index);
private slots:
    void setfs();
};

#endif // CODER_H
