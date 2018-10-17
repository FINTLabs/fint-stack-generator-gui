//

#include <yaml-cpp/yaml.h>
#include <QDir>
#include <fstream>
#include "StackConfig.h"

StackConfig::StackConfig() = default;

StackConfig::~StackConfig() {}

void StackConfig::save(Ui::Dialog *ui, QFileInfo *fileInfo) {

    QString environment = ui->environmentCombo->currentText();
    QString stack = ui->stackEdit->text();
    QString uri = ui->uriEdit->text();
    QString repository = ui->repositoryEdit->text();
    QString version = ui->versionEdit->text();
    QString port = ui->portEdit->text();
    QString provider = ui->providerEdit->text();
    QString resources = ui->assetEdit->text();

    YAML::Emitter emitter;
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "environment";
    emitter << YAML::Value << environment.toStdString();
    emitter << YAML::Key << "stack";
    emitter << YAML::Value << stack.toStdString();
    emitter << YAML::Key << "uri";
    emitter << YAML::Value << uri.toStdString();
    emitter << YAML::Key << "repository";
    emitter << YAML::Value << repository.toStdString();
    emitter << YAML::Key << "version";
    emitter << YAML::Value << version.toStdString();
    emitter << YAML::Key << "port";
    emitter << YAML::Value << port.toInt();
    emitter << YAML::Key << "provider";
    emitter << YAML::Value << provider.toStdString();
    emitter << YAML::Key << "resources";
    emitter << YAML::Value << resources.toStdString();
    emitter << YAML::EndMap;

    saveStackConfig(emitter, getYamlFile(fileInfo, environment, stack));

}

QString StackConfig::getYamlFile(const QFileInfo *fileInfo, const QString &environment, const QString &stack) {

    QString yamlFile;
    if (fileInfo->isDir()) {
        yamlFile = QString("%1%2%3-%4.yml").arg(fileInfo->absoluteFilePath(), QDir::separator(), stack,
                                                environment);//currentFile;
    }
    else {
        yamlFile = fileInfo->absoluteFilePath();
    }
    return yamlFile;
}

void StackConfig::saveStackConfig(const YAML::Emitter &emitter, QString yamlFilePath) {
    std::ofstream fout(yamlFilePath.toStdString());
    fout << emitter.c_str();
}

bool StackConfig::isStackConfig(YAML::Node &config) {
    return config["environment"]
           && config["resources"]
           && config["port"]
           && config["stack"]
           && config["provider"]
           && config["repository"]
           && config["uri"]
           && config["version"];
}

void StackConfig::get(Ui::Dialog *ui, QFileInfo *fileInfo) {

}
