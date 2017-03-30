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
    Volumeter(QWidget *parent);
public slots:
    void setLevel(float level);
protected:
    void paintEvent(QPaintEvent *event);
private:
    int width;          /**< Volumeter width */
    qreal level;        /**< Current level */
    QPixmap pixmap;     /**< pixmap */
};

#endif // VOLUMETER_H
