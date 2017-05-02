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
    consolelog("EffectsMonitor", LogType::progress, "EffectsMonitor object is created");
}

/**
 * @brief   EffectsMonitor constructor.
 * @param	framework			user interface framework
 * @param   effect              selected effect to be load
 */
EffectsMonitor::EffectsMonitor(QWidget *framework, std::string effect) : EffectsMonitor::EffectsMonitor(framework) {
    this->setEffect(effect);
}

/**
 * @brief   EffectsMonitor destructor.
 */
EffectsMonitor::~EffectsMonitor() {
    consolelog("EffectsMonitor", LogType::progress, "EffectsMonitor object is deleted");
}

/**
 * @brief   It selects an effect.
 * @param   effect              selected effect
 */
void EffectsMonitor::setEffect(std::string effect) {
    if (this->effects.find(effect) != this->effects.end()) {
        this->effect = std::pair<std::string, Effect::effectID>(effect, this->effects[effect]);
        consolelog("EffectsMonitor", LogType::progress, "effect \"" + effect + "\" has been loaded");
    } else {
        this->effect = std::pair<std::string, Effect::effectID>(this->effects.begin()->first, this->effects.begin()->second);
        consolelog("EffectsMonitor", LogType::error, "effect \"" + effect + "\" is not available so the default effect has been loaded instead");
    }
    this->loadTemplate();
}

/**
 * @brief   It clears the user interface framework.
 */
void EffectsMonitor::clear() {
    QObjectList elements = this->framework->children();
    for (int index = 0; index < elements.size(); index++) {
        if (elements[index]->objectName().toStdString() != this->framework->objectName().toStdString().substr(0,this->framework->objectName().toStdString().find("_list")) + "_layout") {
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
    std::map<std::string, Effect::effectID> effects = Effect::getEffects();
    QStringList files = QDir(QString::fromStdString(folder)).entryList(QStringList("*.xml"));
    for (int index = 0; index < files.size(); index++) {
        QFile *file = new QFile(QString::fromStdString(folder) + files[index]);
        if(file->open(QFile::ReadOnly | QFile::Text)) {
            QTextStream *in = new QTextStream(file);
            std::string xml = in->readAll().toStdString();
            file->close();
            if(xml.find("<!DOCTYPE fxtemplate SYSTEM \"") < xml.size()) {
                std::string descriptor = "<!DOCTYPE fxtemplate SYSTEM \"";
                std::string reference = xml.substr(xml.find(descriptor) + descriptor.size());
                reference = reference.substr(0,reference.find("\""));
                while(reference.size() > 0) {
                    std::string effect = reference.substr(0, reference.find(","));
                    if (reference.find(",") < reference.size()) {
                        reference = reference.substr(reference.find(",") + 1);
                    } else {
                        reference.clear();
                    }
                    while(reference[0] == ' ') {
                        reference = reference.substr(1);
                    }
                    if (this->effects.find(effect) == this->effects.end()) {
                        this->effects.insert(std::pair<std::string,Effect::effectID>(effect,effects[effect]));
                        this->files.insert(std::pair<std::string,std::string>(effect,file->fileName().toStdString()));
                        consolelog("EffectsMonitor",LogType::info, "available effect \"" + effect + "\" was found in \"" + file->fileName().toStdString() + "\"");
                    } else {
                        consolelog("EffectsMonitor", LogType::warning, "available effect \"" + effect + "\" was found again in \"" + file->fileName().toStdString() + "\" so this last reference will be ignored");
                    }
                }
            } else {
                consolelog("EffectsMonitor", LogType::warning, "\"" + file->fileName().toStdString() + "\" does not have a valid template descriptor so it will be ignored");
            }
        } else {
            consolelog("EffectsMonitor",LogType::error,"\"" + file->fileName().toStdString() + "\" is not a file");
        }
    }
}

/**
 * @brief   It loads a user interface template from a xml file and loads it as an effect monitor interface.
 */
void EffectsMonitor::loadTemplate() {
    this->clear();
    QFile *file = new QFile(QString::fromStdString(this->files[this->effect.first]));
    QTextStream *in = new QTextStream(file);
    if(file->open(QFile::ReadOnly | QFile::Text)) {
        std::string xml = in->readAll().toStdString();
        file->close();
        xml = xml.substr(xml.find(">")+1); // deleting XML descriptor
        while(xml.find("<") < xml.size()) {
            // Tag
            std::string element = xml.substr(xml.find("<"));
            std::string tag = element.substr(1,element.find(" ")-1);
            element = element.substr(0,element.find("</" + tag + ">") + tag.size() + 3);
            this->loadField(element);
            if (xml.find("</" + tag + ">") + tag.size() + 3 < xml.size()) {
                xml = xml.substr(xml.find("</" + tag + ">") + tag.size() + 3);
            } else {
                xml.clear();
            }
        }
        consolelog("EffectsMonitor", LogType::progress, "template file has been loaded");
    } else {
        consolelog("EffectsMonitor",LogType::error,"\"" + file->fileName().toStdString() + "\" could not be opened");
    }
}

/**
 * @brief   It loads a field from a xml line (corresponded to an element).
 * @param   element             string of the xml line (full element)
 */
void EffectsMonitor::loadField(std::string element) {
    const int width = 100;
    std::string tag = element.substr(1,element.find(" ")-1);
    // Attributes
    std::string param = this->getAttribute(element,"param");
    std::string text = this->getAttribute(element,"text");
    std::string value = this->getAttribute(element,"value");
    std::string suffix = this->getAttribute(element,"suffix");
    double min = atof(this->getAttribute(element,"min").c_str());
    double max = std::atof(this->getAttribute(element,"max").c_str());
    double step = std::atof(this->getAttribute(element,"step").c_str());
    if (step <= 0) {
        step = 1;
    }
    int length = std::atoi(this->getAttribute(element,"length").c_str());
    if (length <= 0 || length > 100) {
        length = 100;
    }
    // User interface
    if(tag == "double") {
        QDoubleSpinBox *field = new QDoubleSpinBox();
        field->setMinimumWidth(width);
        field->setObjectName(QString::fromStdString(prefix + param));
        field->setSuffix(QString::fromStdString(suffix));
        field->setMinimum(min);
        field->setMaximum(max);
        field->setValue(std::atof(value.c_str()));
        field->setSingleStep(step);
        this->layout->addRow(QString::fromStdString(text),field);
        QObject::connect(field,SIGNAL(valueChanged(double)),this,SLOT(updateParameters()));
    } else if(tag == "int") {
        QSpinBox *field = new QSpinBox();
        field->setMinimumWidth(width);
        field->setObjectName(QString::fromStdString(prefix + param));
        field->setSuffix(QString::fromStdString(suffix));
        field->setMinimum(min);
        field->setMaximum(max);
        field->setValue(std::atoi(value.c_str()));
        field->setSingleStep(step);
        this->layout->addRow(QString::fromStdString(text),field);
        QObject::connect(field,SIGNAL(valueChanged(int)),this,SLOT(updateParameters()));
    } else if(tag == "string") {
        QLineEdit *field = new QLineEdit();
        field->setMinimumWidth(width);
        field->setObjectName(QString::fromStdString(prefix + param));
        field->setText(QString::fromStdString(value));
        field->setMaxLength(length);
        this->layout->addRow(QString::fromStdString(text),field);
        QObject::connect(field,SIGNAL(textChanged(QString)),this,SLOT(updateParameters()));
    } else if(tag == "bool") {
        QCheckBox *field = new QCheckBox();
        field->setMinimumWidth(width);
        field->setObjectName(QString::fromStdString(prefix + param));
        field->setChecked(value=="true" || value=="True" || value=="TRUE" || value=="1");
        this->layout->addRow(QString::fromStdString(text),field);
        QObject::connect(field,SIGNAL(toggled(bool)),this,SLOT(updateParameters()));
    } else if(tag == "enum") {
        QGroupBox *field = new QGroupBox();
        QVBoxLayout *list = new QVBoxLayout();
        field->setMinimumWidth(width);
        field->setObjectName(QString::fromStdString(prefix + param));
        list->setObjectName(QString::fromStdString(prefix + param + "_layout"));
        field->setLayout(list);
        this->layout->addRow(QString::fromStdString(text),field);
        std::string options = element.substr(element.find(">")+1);
        options = options.substr(0,options.find("</" + tag + ">"));
        while(options.find("<option ") < options.size()) {
            std::string option = options.substr(options.find("<option"));
            option = option.substr(0,option.find("</option>") + 9);
            this->loadField(option);
            if (options.find("<option>") + option.size() < options.size()) {
                options = options.substr(options.find("</option>") + 9);
            } else {
                options = "";
            }
        }
        this->framework->findChild<QRadioButton *>(QString::fromStdString(prefix + param + "_" + value))->setChecked(true);
    } else if(tag == "option") {
        QGroupBox *group = this->framework->findChild<QGroupBox *>(QString::fromStdString(prefix + param));
        if (group==NULL) {
            consolelog("EffectsMonitor", LogType::error, "option tag is not appended from any enum element");
        } else {
            QRadioButton *option = new QRadioButton();
            option->setObjectName(QString::fromStdString(prefix + param + "_" + value));
            option->setText(QString::fromStdString(text));
            group->layout()->addWidget(option);
            QObject::connect(option,SIGNAL(toggled(bool)),this,SLOT(updateParameters()));
        }
    } else {
        consolelog("EffectsMonitor", LogType::error, "tag \"" + tag + "\" is not recognised as a valid tag");
    }
}

/**
 * @brief   It gets an attribute from a xml line (correponded to a xml element).
 * @param   element             string of the xml line (full element)
 * @param   attribute           name of the attribute
 * @return  value of the attribute
 */
std::string EffectsMonitor::getAttribute(std::string element, std::string attribute) {
    std::string value = element;
    value = value.substr(value.find(attribute) + attribute.size());
    value = value.substr(value.find("=") + 1);
    value = value.substr(value.find("\"") + 1);
    value = value.substr(0,value.find("\""));
    return value;
}

void EffectsMonitor::updateParameters() {
    consolelog("EffectsMonitor",LogType::interaction, QObject::sender()->objectName().toStdString() + " has been changed");
}
