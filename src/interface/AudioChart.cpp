// Class libraries
#include "audiochart.h"

/**
 * @brief	AudioChart constructor.
 * @param   framework   user interface framework of chart
 * @param   range       axis range matrix (range[0][0] = x_min, range[0][1] = x_max, range[1][0] = y_min, range[1][1] = y_max)
 * @param   title       chart title (it will be impress on the chart)
 * @param   options
 * @ref     ChartOptions
 */
AudioChart::AudioChart(QWidget *framework, float range[2][2], string title, int options) {
    chart = new QChart();
    view = new QChartView(chart,framework);
    view->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
    view->setMinimumSize(250,200);
    series = new QLineSeries();
    chart->addSeries(series);
    // Axes
    QAbstractAxis *x_axis;
    QAbstractAxis *y_axis;
    if (options & ChartOptions::logX) {
        x_axis = new QLogValueAxis();
    } else {
        x_axis = new QValueAxis();
    }
    if (options & ChartOptions::logY) {
        y_axis = new QLogValueAxis();
    } else {
        y_axis = new QValueAxis();
    }
    x_axis->setRange(range[0][0],range[0][1]);
    y_axis->setRange(range[1][0],range[1][1]);
    x_axis->setTitleText("Frequency");
    y_axis->setTitleText("Audio level");
    if (!(options & ChartOptions::labelX)) {
        x_axis->hide();
    }
    if (!(options & ChartOptions::labelY)) {
        y_axis->hide();
    }
    chart->setAxisX(x_axis,series);
    chart->setAxisY(y_axis,series);
    if (!(options & ChartOptions::legend)) {
        chart->legend()->hide();
    }
    chart->setTitle(QString::fromStdString(title));
    consolelog("AudioChart",LogType::progress,"AudioChart object is created");
}

/**
 * @brief	AudioChart destructor.
 */
AudioChart::~AudioChart() {
    consolelog("AudioChart",LogType::progress,"AudioChart object is deleted");
}

/**
 * @brief	It sets the points to the chart serie.
 * @param   points
 */
void AudioChart::setPoints(QVector<QPointF> points) {
    this->series->replace(points);
}

/**
 * @brief	It gets the points from the chart serie.
 * @return  points
 */
QVector<QPointF> AudioChart::getPoints() {
    return series->pointsVector();
}
