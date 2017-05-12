// Class libraries
#include "Chart2D.h"

/**
 * @brief	Chart constructor.
 * @param   framework   user interface framework of chart
 */
Chart2D::Chart2D(QWidget *framework) {
    this->chart = new QChart();
    this->view = new QChartView(chart, framework);
    this->view->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    this->view->setMinimumSize(framework->size());
    this->series = new QLineSeries();
    this->chart->addSeries(series);
    this->xlabel = "x";
    this->ylabel = "y";
    this->setOptions(0);
    consolelog("Chart", LogType::progress, "Chart object is created");
}

/**
 * @brief	Chart constructor.
 * @param   framework   user interface framework of chart
 * @param   range       axes range matrix (range[0][0] = x_min, range[0][1] = x_max, range[1][0] = y_min, range[1][1] = y_max)
 * @param   title       chart title (it will be impress on the chart)
 * @param   xlabel      label for horizontal (x) axis
 * @param   ylabel      label for vertical (y) axis
 * @param   options
 * @ref     ChartOptions
 */
Chart2D::Chart2D(QWidget *framework, double range[2][2], std::string title,
std::string xlabel, std::string ylabel, int options) :
    Chart2D::Chart2D(framework) {
    this->setRange(range);
    this->setTitle(title);
    this->xlabel = xlabel;
    this->ylabel = ylabel;
    this->setOptions(options);
}

/**
 * @brief	Chart destructor.
 */
Chart2D::~Chart2D() {
    consolelog("Chart", LogType::progress, "Chart object is deleted");
}

/**
 * @brief	It sets the points to the chart serie.
 * @param   points
 */
void Chart2D::setPoints(QVector<QPointF> points) {
    this->series->replace(points);
}

/**
 * @brief	It gets the points from the chart serie.
 * @return  points
 */
QVector<QPointF> Chart2D::getPoints() {
    return series->pointsVector();
}

/**
 * @brief   It sets the axis range.
 * @param   range       axis range matrix (range[0][0] = x_min, range[0][1] = x_max, range[1][0] = y_min, range[1][1] = y_max)
 */
void Chart2D::setRange(double range[2][2]) {
    this->chart->axisX(this->series)->setRange(range[0][0], range[0][1]);
    this->chart->axisY(this->series)->setRange(range[1][0], range[1][1]);
}

/**
 * @brief   It sets chart title.
 * @param   title
 */
void Chart2D::setTitle(std::string title) {
    this->chart->setTitle(QString::fromStdString(title));
}

/**
 * @brief   It sets chart options.
 * @param   options
 */
void Chart2D::setOptions(int options) {
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
void Chart2D::clear() {
    this->series->clear();
}
