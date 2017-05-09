// Class libraries
#include "AudioChart.h"

/**
 * @brief	AudioChart constructor.
 * @param   framework   user interface framework of chart
 */
AudioChart::AudioChart(QWidget *framework) {
    this->chart = new QChart();
    this->view = new QChartView(chart, framework);
    this->view->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    this->view->setMinimumSize(250, 180);
    this->series = new QLineSeries();
    this->chart->addSeries(series);
    this->xlabel = "x";
    this->ylabel = "y";
    this->setOptions(0);
    consolelog("AudioChart", LogType::progress, "AudioChart object is created");
}

/**
 * @brief	AudioChart constructor.
 * @param   framework   user interface framework of chart
 * @param   range       axes range matrix (range[0][0] = x_min, range[0][1] = x_max, range[1][0] = y_min, range[1][1] = y_max)
 * @param   title       chart title (it will be impress on the chart)
 * @param   xlabel      label for horizontal (x) axis
 * @param   ylabel      label for vertical (y) axis
 * @param   options
 * @ref     ChartOptions
 */
AudioChart::AudioChart(QWidget *framework, float range[2][2], std::string title,
std::string xlabel, std::string ylabel, int options) :
    AudioChart::AudioChart(framework) {
    this->setRange(range);
    this->setTitle(title);
    this->xlabel = xlabel;
    this->ylabel = ylabel;
    this->setOptions(options);
}

/**
 * @brief	AudioChart destructor.
 */
AudioChart::~AudioChart() {
    consolelog("AudioChart", LogType::progress, "AudioChart object is deleted");
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
    this->chart->axisX(this->series)->setRange(range[0][0], range[0][1]);
    this->chart->axisY(this->series)->setRange(range[1][0], range[1][1]);
}

/**
 * @brief   It sets chart title.
 * @param   title
 */
void AudioChart::setTitle(std::string title) {
    this->chart->setTitle(QString::fromStdString(title));
}

/**
 * @brief   It sets chart options.
 * @param   options
 */
void AudioChart::setOptions(int options) {
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
    x_axis->setTitleText(QString::fromStdString(this->xlabel));
    y_axis->setTitleText(QString::fromStdString(this->ylabel));
    if (!(options & ChartOptions::labelX)) {
        x_axis->hide();
    }
    if (!(options & ChartOptions::labelY)) {
        y_axis->hide();
    }
    chart->setAxisX(x_axis, this->series);
    chart->setAxisY(y_axis, this->series);
    if (!(options & ChartOptions::legend)) {
        this->chart->legend()->hide();
    }
}

/**
 * @brief   It clears the chart.
 */
void AudioChart::clear() {
    this->series->clear();
}
