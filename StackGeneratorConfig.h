//
// Created by Frode Sjovatsen on 22/10/2018.
//

#ifndef FINT_STACK_GENERATOR_GUI_STACKGENERATORCONSTANTS_H
#define FINT_STACK_GENERATOR_GUI_STACKGENERATORCONSTANTS_H


#include <QString>
#include <QDir>

class StackGeneratorConfig {

public:
    static QString environmentNone() { return ""; };

    static QString environmentBeta() { return "beta"; }

    static QString environmentApi() { return "api"; };

    static QString felleskomponentDirectory() { return "felleskomponent"; };

    static std::string stackTemplateFile(QString currentDirectory) {
        return QString("%1%2..%3%4").arg(currentDirectory, QDir::separator(), QDir::separator(),
                                         "fint-component-template.yml").toStdString();
    };

    static QString stackFileOutputDirectory(QString configDirectory) {
        return QString("%1%2%3").arg(configDirectory, QDir::separator(), "stackfiles");
    }

    static QString labelSuccess() { return "QLabel { background-color : green; }"; };

    static QString labelError() { return "QLabel { background-color : red; }"; };

    static QString labelWarning() { return "QLabel { background-color : yellow; }"; };


};


#endif //FINT_STACK_GENERATOR_GUI_STACKGENERATORCONSTANTS_H
