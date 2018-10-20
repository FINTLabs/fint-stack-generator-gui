//
// Created by Frode Sjovatsen on 16/10/2018.
//

#ifndef FINT_STACK_GENERATOR_GUI_STACKCONFIG_H
#define FINT_STACK_GENERATOR_GUI_STACKCONFIG_H


#include "ui_dialog.h"
#include <yaml-cpp/yaml.h>
#include <QFileInfo>

class StackConfig {

public:
    StackConfig();
    virtual ~StackConfig();

private:
    static void saveStackConfig(const YAML::Emitter &emitter, QString yamlFilePath);

public:
    static void save(Ui::Dialog *ui, QFileInfo *fileInfo);

    static QString getYamlFile(const QFileInfo *fileInfo, const QString &environment, const QString &stack);
    static bool isStackConfig(YAML::Node &config);
    static YAML::Node load(Ui::Dialog *ui, QFileInfo *fileInfo);
};


#endif //FINT_STACK_GENERATOR_GUI_STACKCONFIG_H
