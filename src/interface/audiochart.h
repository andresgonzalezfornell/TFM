#ifndef AUDIOCHART_H
#define AUDIOCHART_H

// System libraries
#include "math.h"
#include "stdlib.h"
// Qt libraries
#include "QWidget"
#include "QObject"
#include "QtWidgets"
#include "QtCharts"
#include "QPointF"
// Classes and local files
#include "../tools/Logger.h"
#include "../module/AudioSignal.h"

using namespace QtCharts;

class AudioChart : public QWidget {
    Q_OBJECT
public:
    AudioChart(QWidget *framework, string title);
    ~AudioChart();
    void setPoints(QVector<QPointF> *points);
    QVector<QPointF> getPoints();
    void addPoint(QPointF point);
private:
    QChart *chart;
    QChartView *view;
    QLineSeries *series;
    int range;
};

#endif // AUDIOCHART_H
