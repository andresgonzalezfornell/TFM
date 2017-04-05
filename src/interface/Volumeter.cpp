// Class libraries
#include "Volumeter.h"

/**
 * @brief	Volumeter constructor.
 * @param   *parent         user interface element pointer to place the volumeter
 * @param   fs              signal sampling frequency [Hz]
 */
Volumeter::Volumeter(QWidget *parent, int fs)
    : QWidget(parent)
{
    this->width = parent->frameGeometry().width();
    this->level = 0;
    this->fs = fs;
    consolelog("Volumeter",LogType::progress,"Volumeter object is created");
}

/**
 * @brief	It fills the volumeter.
 */
void Volumeter::paintEvent(QPaintEvent * /* event */)
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
        painter.fillRect(x,y,this->level*width,height,color_fill);
    }
}

/**
 * @brief	It sets the new level value to the volumeter
 * @param   level
 */
void Volumeter::setLevel(float level)
{
    this->level = level;
    update();
}

/**
 * @brief   It adds a sample for level calculation.
 * @param   sample
 */
void Volumeter::addSample(float sample) {
    this->energy += pow(sample,2);
    this->samples++;
    if (this->samples >= (this->refreshperiod*this->fs)) {
        this->setLevel(energy/this->samples);
        this->energy = 0;
        this->samples = 0;
    }
}
