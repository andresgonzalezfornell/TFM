/**
 * @name	AudioChart.cpp
 * @author	Andrés González Fornell
 * @brief	Chart class for audio signals.
 */

// Class libraries
#include "audiochart.h"

/**
 * @brief	AudioChart constructor.
 * @param   framework   user interface framework of chart
 * @param   title       chart title (it will be impress on the chart)
 */
AudioChart::AudioChart(QWidget *framework, string title) {
    chart = new QChart();
    view = new QChartView(chart,framework);
    view->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
    view->setMinimumSize(250,200);
    series = new QLineSeries();
    chart->addSeries(series);
    this->range = range;
    // Axes
    QValueAxis *x_axis = new QValueAxis();
    x_axis->setRange(0,AudioSignal::fs());
    x_axis->setLabelFormat("%g");
    x_axis->setTitleText("Frequency");
    chart->setAxisX(x_axis,series);
    QValueAxis *y_axis = new QValueAxis();
    y_axis->setRange(0,1);
    y_axis->hide();
    y_axis->setLabelFormat("%g");
    y_axis->setTitleText("Audio level");
    chart->setAxisY(y_axis,series);
    chart->legend()->hide();
    chart->setTitle(QString::fromStdString(title));
    consolelog("AudioChart",progress,"AudioChart object is created");
}

/**
 * @brief	AudioChart destructor.
 */
AudioChart::~AudioChart() {
    consolelog("AudioChart",progress,"AudioChart object is deleted");
}

/**
 * @brief	It sets the points to the chart serie.
 * @param   points
 */
void AudioChart::setPoints(QVector<QPointF> *points) {
    series->replace(*points);
}

/**
 * @brief	It gets the points from the chart serie.
 * @return  points
 */
QVector<QPointF> AudioChart::getPoints() {
    return series->pointsVector();
}
