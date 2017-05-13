#include "EffectsMonitor.h"

const std::string folder = ":/imports/effects/"; // folder path to effects template files
const std::string prefix = "effect_params_"; // prefix of elements object name

/**
 * @brief   EffectsMonitor constructor.
 * @param	framework			user interface framework
 */
EffectsMonitor::EffectsMonitor(QWidget *framework) {
    this->framework = framework;
    this->layout = framework->findChild<QFormLayout *>("effect_monitor_layout");
    this->loadFiles();
    consolelog("EffectsMonitor", LogType::progress,
               "EffectsMonitor object is created");
}

/**
 * @brief   EffectsMonitor constructor.
 * @param	framework			user interface framework
 * @param   effect              selected effect to be load
 */
EffectsMonitor::EffectsMonitor(QWidget *framework, Effect *effect) :
    EffectsMonitor::EffectsMonitor(framework) {
    this->setEffect(effect);
}

/**
 * @brief   EffectsMonitor destructor.
 */
EffectsMonitor::~EffectsMonitor() {
    consolelog("EffectsMonitor", LogType::progress,
               "EffectsMonitor object is deleted");
}

/**
 * @brief   It selects an effect.
 * @param   effect              selected effect
 */
void EffectsMonitor::setEffect(Effect *effect) {
    this->effect = effect;
    this->loadTemplate();
    this->effect->setParams(this->parameters);
    this->plotChart();
    consolelog("EffectsMonitor", LogType::progress, "effect \"" + effect->effect.second + "\" has been loaded");
}

/**
 * @brief   It clears the user interface framework.
 */
void EffectsMonitor::clear() {
    this->parameters.clear();
    this->charts.clear();
    QObjectList elements = this->framework->children();
    for (int index = 0; index < elements.size(); index++) {
        if (elements[index]->objectName().toStdString()
                != this->framework->objectName().toStdString().substr(0,
                                                                      this->framework->objectName().toStdString().find(
                                                                          "_list")) + "_layout") {
            delete elements[index];
        }
    }
}

/**
 * @brief   It loads all imported effects template files from the effects folder as application resource in order to be used by the running program.
 */
void EffectsMonitor::loadFiles() {
    this->effects.clear();
    this->files.clear();
    std::map<Effect::effectID, std::string> effects = Effect::getEffects();
    QStringList files = QDir(QString::fromStdString(folder)).entryList(
                QStringList("*.xml"));
    for (int index = 0; index < files.size(); index++) {
        QFile *file = new QFile(QString::fromStdString(folder) + files[index]);
        if (file->open(QFile::ReadOnly | QFile::Text)) {
            QTextStream *in = new QTextStream(file);
            std::string xml = in->readAll().toStdString();
            file->close();
            if (xml.find("<!DOCTYPE fxtemplate SYSTEM \"") < xml.size()) {
                std::string descriptor = "<!DOCTYPE fxtemplate SYSTEM \"";
                std::string reference = xml.substr(
                            xml.find(descriptor) + descriptor.size());
                reference = reference.substr(0, reference.find("\""));
                while (reference.size() > 0) {
                    std::string effectname = reference.substr(0,
                                                          reference.find(","));
                    if (reference.find(",") < reference.size()) {
                        reference = reference.substr(reference.find(",") + 1);
                    } else {
                        reference.clear();
                    }
                    while (reference[0] == ' ') {
                        reference = reference.substr(1);
                    }
                    Effect::effectID effect = Effect::getEffect(effectname);
                    if (this->effects.find(effect) == this->effects.end()) {
                        this->effects.insert(
                                    std::pair<Effect::effectID, std::string>(effect,
                                                                             effects[effect]));
                        this->files.insert(
                                    std::pair<Effect::effectID, std::string>(effect,
                                                                        file->fileName().toStdString()));
                        consolelog("EffectsMonitor", LogType::info,
                                   "available effect \"" + effectname
                                   + "\" was found in \""
                                   + file->fileName().toStdString()
                                   + "\"");
                    } else {
                        consolelog("EffectsMonitor", LogType::warning,
                                   "available effect \"" + effectname
                                   + "\" was found again in \""
                                   + file->fileName().toStdString()
                                   + "\" so this last reference will be ignored");
                    }
                }
            } else {
                consolelog("EffectsMonitor", LogType::warning,
                           "\"" + file->fileName().toStdString()
                           + "\" does not have a valid template descriptor so it will be ignored");
            }
        } else {
            consolelog("EffectsMonitor", LogType::error,
                       "\"" + file->fileName().toStdString() + "\" is not a file");
        }
    }
}

/**
 * @brief   It loads a user interface template from a xml file and loads it as an effect monitor interface.
 */
void EffectsMonitor::loadTemplate() {
    this->clear();
    QFile *file = new QFile(
                QString::fromStdString(this->files[this->effect->effect.first]));
    QTextStream *in = new QTextStream(file);
    if (file->open(QFile::ReadOnly | QFile::Text)) {
        std::string xml = in->readAll().toStdString();
        file->close();
        xml = xml.substr(xml.find(">") + 1); // deleting XML descriptor
        while (xml.find("<") < xml.size()) {
            // Tag
            std::string element = xml.substr(xml.find("<"));
            std::string tag = element.substr(1, element.find(" ") - 1);
            element = element.substr(0,
                                     element.find("</" + tag + ">") + tag.size() + 3);
            this->loadField(element);
            if (xml.find("</" + tag + ">") + tag.size() + 3 < xml.size()) {
                xml = xml.substr(xml.find("</" + tag + ">") + tag.size() + 3);
            } else {
                xml.clear();
            }
        }
        consolelog("EffectsMonitor", LogType::progress,
                   "template file has been loaded");
    } else {
        consolelog("EffectsMonitor", LogType::error,
                   "\"" + file->fileName().toStdString()
                   + "\" could not be opened");
    }
}

/**
 * @brief   It loads a field from a xml line (corresponded to an element).
 * @param   element             string of the xml line (full element)
 */
void EffectsMonitor::loadField(std::string element) {
    const int width = 100;
    std::string tag = element.substr(1, element.find(" ") - 1);
    // Attributes
    std::string id = this->getAttribute(element, "id");
    // Deleting banned characters for id
    std::string newid = "";
    for (int character = 0; character < (int) id.size(); character++) {
        bool valid = false;
        if (character > 0) {
            valid += (0x30 <= id[character] && id[character] <= 0x39); // numbers
        }
        valid += (0x41 <= id[character] && id[character] <= 0x5A); // uppercase letters
        valid += (0x61 <= id[character] && id[character] <= 0x7A); // lowercase letters
        if (valid) {
            newid += id[character];
        } else {
            consolelog("EffectsMonitor", LogType::warning,
                       "character \"" + id.substr(character, 1)
                       + "\" is not allowed as parameter name in \""
                       + id + "\" and it will be removed");
        }
    }
    id = newid;
    std::string text = this->getAttribute(element, "text");
    std::string value = this->getAttribute(element, "value");
    std::string suffix = this->getAttribute(element, "suffix");
    bool readonly = Effect::getBool(this->getAttribute(element, "readonly"));
    double min = atof(this->getAttribute(element, "min").c_str());
    double max = std::atof(this->getAttribute(element, "max").c_str());
    double step = std::atof(this->getAttribute(element, "step").c_str());
    if (step <= 0) {
        step = 1;
    }
    int length = std::atoi(this->getAttribute(element, "length").c_str());
    if (length <= 0 || length > 100) {
        length = 100;
    }
    // Contain
    std::string contain = element.substr(element.find(">") + 1);
    if (contain.rfind("<")==0) {
        contain = "";
    } else {
        contain = contain.substr(0,contain.rfind("<"));
    }
    // User interface
    if (tag == "int") {
        QSpinBox *field = new QSpinBox(this->framework);
        field->setMinimumWidth(width);
        field->setObjectName(QString::fromStdString(prefix + id));
        this->parameters.insert(std::pair<std::string, std::string>(id, value));
        field->setReadOnly(readonly);
        field->setSuffix(QString::fromStdString(suffix));
        field->setMinimum(min);
        field->setMaximum(max);
        field->setValue(std::atoi(value.c_str()));
        field->setSingleStep(step);
        this->layout->addRow(QString::fromStdString(text), field);
        QObject::connect(field,SIGNAL(valueChanged(int)),this,SLOT(updateParameter(int)));
    } else if (tag == "double") {
        QDoubleSpinBox *field = new QDoubleSpinBox(this->framework);
        field->setMinimumWidth(width);
        field->setObjectName(QString::fromStdString(prefix + id));
        this->parameters.insert(std::pair<std::string, std::string>(id, value));
        field->setReadOnly(readonly);
        field->setSuffix(QString::fromStdString(suffix));
        field->setMinimum(min);
        field->setMaximum(max);
        field->setValue(std::atof(value.c_str()));
        field->setSingleStep(step);
        this->layout->addRow(QString::fromStdString(text), field);
        QObject::connect(field,SIGNAL(valueChanged(double)),this,SLOT(updateParameter(double)));
    } else if (tag == "string") {
        QLineEdit *field = new QLineEdit(this->framework);
        field->setMinimumWidth(width);
        field->setObjectName(QString::fromStdString(prefix + id));
        this->parameters.insert(std::pair<std::string, std::string>(id, value));
        field->setText(QString::fromStdString(value));
        field->setReadOnly(readonly);
        field->setMaxLength(length);
        this->layout->addRow(QString::fromStdString(text), field);
        QObject::connect(field, SIGNAL(textChanged(QString)), this,
                         SLOT(updateParameter(QString)));
    } else if (tag == "bool") {
        QCheckBox *field = new QCheckBox(this->framework);
        field->setMinimumWidth(width);
        field->setObjectName(QString::fromStdString(prefix + id));
        this->parameters.insert(std::pair<std::string, std::string>(id, value));
        field->setChecked(
                    value == "true" || value == "True" || value == "TRUE" || value == "1");
        field->setEnabled(!readonly);
        this->layout->addRow(QString::fromStdString(text), field);
        QObject::connect(field,SIGNAL(toggled(bool)),this,SLOT(updateParameter(bool)));
    } else if (tag == "enum") {
        QGroupBox *field = new QGroupBox();
        QVBoxLayout *list = new QVBoxLayout(this->framework);
        field->setMinimumWidth(width);
        field->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        field->setObjectName(QString::fromStdString(prefix + id));
        list->setObjectName(QString::fromStdString(prefix + id + "_layout"));
        field->setLayout(list);
        this->parameters.insert(std::pair<std::string, std::string>(id, value));
        this->layout->addRow(QString::fromStdString(text), field);
        while (contain.find("<option ") < contain.size()) {
            std::string option = contain.substr(contain.find("<option"));
            if (readonly) {
                option = option.substr(0, 8) + "readonly=\"true\" " + option.substr(8);
            }
            option = option.substr(0, option.find("</option>") + 9);
            this->loadField(option);
            if (contain.find("<option>") + option.size() < contain.size()) {
                contain = contain.substr(contain.find("</option>") + 9);
            } else {
                contain = "";
            }
        }
        this->framework->findChild<QRadioButton *>(QString::fromStdString(prefix + id + "_" + value))->setChecked(true);
    } else if (tag == "option") {
        QGroupBox *group = this->framework->findChild<QGroupBox *>(
                    QString::fromStdString(prefix + id));
        if (group == NULL) {
            consolelog("EffectsMonitor", LogType::error,
                       "option tag is not appended from any enum element");
        } else {
            QRadioButton *option = new QRadioButton(group);
            option->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            option->setObjectName(QString::fromStdString(prefix + id + "_" + value));
            option->setText(QString::fromStdString(text));
            option->setEnabled(!readonly);
            group->layout()->addWidget(option);
            QObject::connect(option,SIGNAL(toggled(bool)),this,SLOT(updateParameter(bool)));
        }
    } else if (tag == "label") {
        QLabel *field = new QLabel(this->framework);
        field->setObjectName(QString::fromStdString(prefix + id));
        field->setText(QString::fromStdString(text));
        this->layout->addRow(field);
    } else if (tag == "chart") {
        QWidget *field = new QWidget(this->framework);
        field->setMinimumSize(300, 225);
        field->setObjectName(QString::fromStdString(prefix + id));
        double range[2][2] = {{Effect::getDouble(this->getAttribute(element, "xmin")),
                             Effect::getDouble(this->getAttribute(element, "xmax"))},{
                             Effect::getDouble(this->getAttribute(element, "ymin")),
                             Effect::getDouble(this->getAttribute(element, "ymax"))}};
        std::string xlabel = this->getAttribute(element, "xlabel");
        std::string ylabel = this->getAttribute(element, "ylabel");
        int options = 0;
        if (xlabel != "") {
            options |= Chart2D::ChartOptions::labelX;
        }
        if (ylabel != "") {
            options |= Chart2D::ChartOptions::labelY;
        }
        if (this->getAttribute(element, "xscale") == "log") {
            options |= Chart2D::ChartOptions::logX;
        }
        if (this->getAttribute(element, "ylabel") == "log") {
            options |= Chart2D::ChartOptions::logY;
        }
        Chart2D *chart = new Chart2D(field, range, text, xlabel, ylabel, options);
        charts.insert(std::pair<std::string, Chart2D *>(id, chart));
        this->layout->addRow(field);
    } else {
        consolelog("EffectsMonitor", LogType::error,
                   "tag \"" + tag + "\" is not recognised as a valid tag");
    }
    consolelog("EffectsMonitor", LogType::progress, "element \"" + id + "\" of type \"" + tag + "\" was created");
}

/**
 * @brief   It gets an attribute from a xml line (correponded to a xml element).
 * @param   element             string of the xml line (full element)
 * @param   attribute           name of the attribute
 * @return  value of the attribute
 */
std::string EffectsMonitor::getAttribute(std::string element,
                                         std::string attribute) {
    if (element.find(attribute) < element.size()) {
        std::string value = element;
        value = value.substr(value.find(attribute) + attribute.size());
        value = value.substr(value.find("=") + 1);
        value = value.substr(value.find("\"") + 1);
        value = value.substr(0, value.find("\""));
        return value;
    } else {
        return "";
    }
}

/**
 * @brief   It sets a parameter from the parameter user interface object.
 * @param   parameter           parameter name
 * @param   value               new parameter value
 */
void EffectsMonitor::setParameter(std::string parameter, std::string value) {
    if (this->parameters.find(parameter) != this->parameters.end()) {
        this->parameters[parameter] = value;
        consolelog("EffectsMonitor", LogType::interaction,
                   "parameter \"" + parameter + "\" has been changed to \"" + value + "\"");
    } else {
        consolelog("EffectsMonitor", LogType::error,
                   "parameter \"" + parameter + "\" was not found in the parameters list");
    }
    this->effect->setParams(this->parameters);
    this->plotChart();
}

/**
 * @brief   It plots every chart on the effects monitor.
 */
void EffectsMonitor::plotChart() {
    for (std::map<std::string, Chart2D *>::iterator chart = this->charts.begin(); chart != this->charts.end(); chart++) {
        std::vector<std::vector<double>> vector = this->effect->plot(chart->first);
        QVector<QPointF> points;
        for (int n = 0; n < (int)vector[0].size(); n++) {
            points.push_back(QPointF(vector[0][n], vector[1][n]));
        }
        chart->second->setPoints(points);
    }
}

/**
 * @name    Parameters slots
 * @brief   User interface functions for effect parameters control.
 * @{
 */

/**
 * @brief   Slot for updating parameters parameters of type int when one of them is changed.
 * @param   value               changed value
 */
void EffectsMonitor::updateParameter(int value) {
    this->setParameter(
                QObject::sender()->objectName().toStdString().substr(prefix.size()),
                std::to_string(value));
}

/**
 * @brief   Slot for updating parameters parameters of type double when one of them is changed.
 * @param   value               changed value
 */
void EffectsMonitor::updateParameter(double value) {
    this->setParameter(
                QObject::sender()->objectName().toStdString().substr(prefix.size()),
                std::to_string(value));
}

/**
 * @brief   Slot for updating parameters of type string when one of them is changed.
 * @param   value               changed value
 */
void EffectsMonitor::updateParameter(QString value) {
    this->setParameter(
                QObject::sender()->objectName().toStdString().substr(prefix.size()),
                value.toStdString());
}

/**
 * @brief   Slot for updating parameters parameters of type bool and enum when one of them is changed.
 * @param   value               changed value
 */
void EffectsMonitor::updateParameter(bool value) {
    std::string element = QObject::sender()->objectName().toStdString().substr(
                prefix.size());
    if (element.find("_") == element.size()) { // bool
        std::string text = "";
        if (value) {
            text = "true";
        } else {
            text = "false";
        }
        this->setParameter(element, text);
    } else { // enum
        if (value) {
            std::string parameter = element.substr(0, element.find("_"));
            std::string option = element.substr(element.find("_") + 1);
            this->setParameter(parameter, option);
        }
    }
}

/** @} */
