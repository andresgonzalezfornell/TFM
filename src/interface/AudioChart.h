#ifndef AUDIOCHART_H
#define AUDIOCHART_H

// System libraries
#include "cmath"
#include "stdlib.h"
// Qt libraries
#include "QWidget"
#include "QObject"
#include "QtCharts"
#include "QPointF"
// Classes and local files
#include "../tools/Logger.h"

using namespace QtCharts;

/**
 * @class	AudioChart
 * @author	Andrés González Fornell
 * @brief	Chart class for audio signals.
 */
class AudioChart : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief   It defines some features of the chart.
     */
    struct ChartOptions {
        enum Options{
            logX = 0x00001,     /**< it configures the x axis as logarithm scale */
            logY = 0x00010,     /**< it configures the y axis as logarithm scale  */
            labelX = 0x00100,   /**< it shows the x axis description on the chart  */
            labelY = 0x01000,   /**< it shows the y axis description on the chart  */
            legend = 0x10000    /**< it shows the legend on the chart  */
        };
    };
    std::string xlabel;         /**< horizontal (x) axis label */
    std::string ylabel;         /**< vertical (y) axis label */
    AudioChart(QWidget *framework);
    AudioChart(QWidget *framework, float range[2][2], std::string title, std::string xlabel, std::string ylabel, int options);
    ~AudioChart();
    void setPoints(QVector<QPointF> points);
    QVector<QPointF> getPoints();
    void setRange(float range[2][2]);
    void setTitle(std::string title);
    void setOptions(int options);
    void clear();
private:
    QChart *chart;              /**< chart object */
    QChartView *view;           /**< chart view object from chart */
    QLineSeries *series;        /**< series object from chart */
};

#endif // AUDIOCHART_H
