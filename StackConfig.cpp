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
    } else {
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

YAML::Node StackConfig::load(Ui::Dialog *ui, QFileInfo *fileInfo) {

    QString environment;
    QString resources;
    QString port;
    QString stack;
    QString provider;
    QString repository;
    QString uri;
    QString version;
    YAML::Node config;


    try {
        config = YAML::LoadFile(fileInfo->absoluteFilePath().toStdString());
    }
    catch (YAML::ParserException &e) {
        ui->messageLabel->setStyleSheet("QLabel { background-color : red; }");
        ui->messageLabel->setText("Unable to parse yaml file.");
        ui->saveButton->setDisabled(true);
        return YAML::Node();
    }


    ui->messageLabel->setText("");

    if (!StackConfig::isStackConfig(config)) {
        ui->messageLabel->setStyleSheet("QLabel { background-color : yellow; }");
        ui->messageLabel->setText("This is not at FINTLabs stack config file!");
        ui->saveButton->setDisabled(true);
        return YAML::Node();
    }

    ui->saveButton->setDisabled(false);

    if (config["environment"]) {
        environment = QString::fromStdString(config["environment"].as<std::string>());
    }
    if (config["resources"]) {
        resources = QString::fromStdString(config["resources"].as<std::string>());
    }
    if (config["port"]) {
        port = QString::fromStdString(config["port"].as<std::string>());
    }
    if (config["stack"]) {
        stack = QString::fromStdString(config["stack"].as<std::string>());
    }
    if (config["provider"]) {
        provider = QString::fromStdString(config["provider"].as<std::string>());
    }
    if (config["repository"]) {
        repository = QString::fromStdString(config["repository"].as<std::string>());
    }
    if (config["uri"]) {
        uri = QString::fromStdString(config["uri"].as<std::string>());
    }
    if (config["version"]) {
        version = QString::fromStdString(config["version"].as<std::string>());
    }

    ui->environmentCombo->setCurrentText(environment);
    ui->assetEdit->setText(resources);
    ui->portEdit->setText(port);
    ui->stackEdit->setText(stack);
    ui->providerEdit->setText(provider);
    ui->repositoryEdit->setText(repository);
    ui->uriEdit->setText(uri);
    ui->versionEdit->setText(version);

    return config;
}
