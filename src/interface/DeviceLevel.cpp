/**
 * @name	DeviceLevel.cpp
 * @author	Andrés González Fornell
 * @brief	Input audio volumeter class.
 */

// Class libraries
#include "device.h"
#include "ui_mainwindow.h"
// System libraries
#include "math.h"
#include "stdlib.h"

/**
 * @brief	DeviceLevel constructor.
 * @param   *element    user interface element pointer to place the volumeter
 */
DeviceLevel::DeviceLevel(QWidget *parent)
    : QWidget(parent)
{
    width = parent->frameGeometry().width();
    level = 0;
    consolelog("Device",progress,"DeviceLevel object is created");
}

/**
 * @brief	It fills the volumeter.
 */
void DeviceLevel::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    // Size
    int x = painter.viewport().left();
    int y = painter.viewport().top();
    int width = this->width;
    int height = painter.viewport().bottom();
    // Fill color
    QLinearGradient gradient = QLinearGradient(0,0,width,0);
    gradient.setColorAt(0, QColor::fromRgbF(0.2941, 0.6706, 0.1882, 1));    // Low level color
    gradient.setColorAt(1, QColor::fromRgbF(0.8314, 0.1373, 0.0863, 1));    // High level color
    QBrush color_fill = QBrush(gradient);
    // Contour color
    QColor color_contour = QColor::fromRgbF(0,0,0,0);
    painter.setPen(QPen(color_contour));
    painter.drawRect(QRect(x,y,width,height));
    if (level != 0) {
        painter.fillRect(x,y,width*level,height,color_fill);
    }
}

/**
 * @brief	It sets the new level value to the volumeter
 * @param   value
 */
void DeviceLevel::setLevel(qreal value)
{
    level = value;
    update();
}
