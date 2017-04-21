#ifndef VOLUMETER_H
#define VOLUMETER_H

// System libraries
#include "math.h"
#include "stdlib.h"
// Qt libraries
#include "QWidget"
#include "QPainter"
// Classes and local files
#include "../tools/Logger.h"

/**
 * @class	Volumeter
 * @author	Andrés González Fornell
 * @brief	Input audio volumeter class.
 */
class Volumeter : public QWidget
{
    Q_OBJECT
public:
    Volumeter(QWidget *parent, int fs);
    void setLevel(float level);
    void addSample(float sample);
protected:
    void paintEvent(QPaintEvent *event);
private:
    int width;                          /**< volumeter width */
    float level;                        /**< current volumeter level */
    float energy;                       /**< current energy of given samples */
    int samples;                        /**< current number of given samples for the current level */
    QPixmap pixmap;                     /**< pixmap */
    int fs;                             /**< signal sampling frequency [Hz] */
    const float refreshperiod = 0.2;    /**< period of volumeter refreshing [s] */
};

#endif // VOLUMETER_H
