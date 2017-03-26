#ifndef AUDIOCHART_H
#define AUDIOCHART_H

// System libraries
#include "math.h"
#include "stdlib.h"
// Qt libraries
#include "QWidget"
#include "QObject"
#include "QtCharts"
#include "QPointF"
// Classes and local files
#include "../tools/Logger.h"
#include "../module/AudioSignal.h"

using namespace QtCharts;

/**
 * @class	AudioChart
 * @author	Andrés González Fornell
 * @brief	Chart class for audio signals.
 */
class AudioChart : public QWidget {
    Q_OBJECT
public:
    AudioChart(QWidget *framework, float range[2][2], string title, int options);
    ~AudioChart();
    void setPoints(QVector<QPointF> points);
    QVector<QPointF> getPoints();
    /**
     * @brief   It defines some features of the chart.
     */
    struct ChartOptions {
        enum Options{
            logX = 0x00001,     /**< it configures the x axis as logarithm scale */
            logY = 0x00010,     /**< it configures the y axis as logarithm scale  */
            logXY = 0x00011,    /**< it configures the x and y axes as logarithm scale  */
            labelX = 0x00100,   /**< it shows the x axis description on the chart  */
            labelY = 0x01000,   /**< it shows the y axis description on the chart  */
            labelXY = 0x01100,  /**< it shows the x and y axes description on the chart  */
            legend = 0x10000    /**< it shows the legend on the chart  */
        };
    };
private:
    QChart *chart;              /**< chart object */
    QChartView *view;           /**< chart view object from chart */
    QLineSeries *series;        /**< series object from chart */
};

#endif // AUDIOCHART_H
