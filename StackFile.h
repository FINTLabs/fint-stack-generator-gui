//
// Created by Frode Sjovatsen on 17/10/2018.
//

#ifndef FINT_STACK_GENERATOR_GUI_STACKFILE_H
#define FINT_STACK_GENERATOR_GUI_STACKFILE_H

#include <yaml-cpp/yaml.h>
#include <QString>


class StackFile {

public:
    explicit StackFile(const YAML::Node &config) : config(config) {}
    StackFile() = default;

public:
    QString generate(std::string templateFile, QString outputDirectory);

private:
    YAML::Node config;

private:
    void consumerConfig(YAML::Node &config);
    void providerConfig(YAML::Node &config);
    void healthAdapterConfig(YAML::Node &config);

    std::string value(YAML::Node node, QString xxxx);
    std::string value(YAML::Node node, QString xxxx, QString yyyy);

private:
    QString environment() { return QString(this->config["environment"].as<std::string>().data());};
    QString stack() { return QString(this->config["stack"].as<std::string>().data());};
    QString uri() { return QString(this->config["uri"].as<std::string>().data());};
    QString repository() { return QString(this->config["repository"].as<std::string>().data());};
    QString version() { return QString(this->config["version"].as<std::string>().data());};
    QString port() { return this->config["port"].as<std::string>().data();};
    QString provider() { return QString(this->config["provider"].as<std::string>().data());};
    QString resources() { return QString(this->config["resources"].as<std::string>().data());};



};


#endif //FINT_STACK_GENERATOR_GUI_STACKFILE_H
