// Class libraries
#include "AudioChart.h"

/**
 * @brief	AudioChart constructor.
 * @param   framework   user interface framework of chart
 * @param   range       axis range matrix (range[0][0] = x_min, range[0][1] = x_max, range[1][0] = y_min, range[1][1] = y_max)
 * @param   title       chart title (it will be impress on the chart)
 * @param   options
 * @ref     ChartOptions
 */
AudioChart::AudioChart(QWidget *framework, float range[2][2], std::string title, int options) {
    this->chart = new QChart();
    this->view = new QChartView(chart,framework);
    this->view->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
    this->view->setMinimumSize(250,200);
    this->series = new QLineSeries();
    this->chart->addSeries(series);
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
    chart->setAxisX(x_axis,this->series);
    chart->setAxisY(y_axis,this->series);
    if (!(options & ChartOptions::legend)) {
        this->chart->legend()->hide();
    }
    this->chart->setTitle(QString::fromStdString(title));
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

/**
 * @brief   It sets the axis range.
 * @param   range       axis range matrix (range[0][0] = x_min, range[0][1] = x_max, range[1][0] = y_min, range[1][1] = y_max)
 */
void AudioChart::setRange(float range[2][2]) {
    this->chart->axisX(this->series)->setRange(range[0][0],range[0][1]);
    this->chart->axisY(this->series)->setRange(range[1][0],range[1][1]);
}
