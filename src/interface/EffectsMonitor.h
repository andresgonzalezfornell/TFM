#ifndef EFFECTSMONITOR_H
#define EFFECTSMONITOR_H

// System libraries
#include "stdlib.h"
#include "vector"
#include "map"
// Qt libraries
#include "QObject"
#include "QDir"
#include "QFile"
#include "QTextStream"
#include "QWidget"
#include "QLayout"
#include "QFormLayout"
#include "QLabel"
#include "QSpinBox"
#include "QLineEdit"
#include "QCheckBox"
#include "QGroupBox"
#include "QRadioButton"
// Classes and local files
#include "Chart2D.h"
#include "../effects/Effect.h"
#include "../process/File.h"
#include "../tools/Logger.h"

/**
 * @class	EffectsMonitor
 * @author	Andrés González Fornell
 * @brief	Class for managing effects parameters.
 */
class EffectsMonitor: public QObject {
	Q_OBJECT
public:
    Effect *effect; /**< pointer to current selected effect */
    std::map<Effect::effectID, std::string> effects;    /**< list of all available effects */
    std::map<Effect::effectID, std::string> files;           /**< list of all available effects template files */
    std::map<std::string, std::string> parameters; /**< list of the current effect parameters and their values */
    std::map<std::string, Chart2D *> charts; /**< list of charts of effect monitoring */
	EffectsMonitor(QWidget *framework);
    EffectsMonitor(QWidget *framework, Effect *effect);
	~EffectsMonitor();
    void setEffect(Effect *effect);
    void clear();
    void setParameter(std::string key, std::string value);
    void plotChart();
public slots:
	void updateParameter(int value); // int
	void updateParameter(double value); // double
	void updateParameter(QString value); // string
	void updateParameter(bool value); // bool & enum
private:
	QWidget *framework; /**< effects monitor framework */
	QFormLayout *layout; /**< form layout of effect parameters */
	void loadFiles();
	void loadTemplate();
	std::string getAttribute(std::string element, std::string attribute);
	void loadField(std::string element);
};

#endif // EFFECTSMONITOR_H
