//
// Created by Frode Sjovatsen on 17/10/2018.
//

#include "StackFile.h"
#include <fstream>
#include <QString>
#include <QDebug>
#include <iostream>
#include <QDir>



QString StackFile::generate(std::string &templateFile) {

    YAML::Node stackTemplate;

    try {
        stackTemplate = YAML::LoadFile(templateFile);
    }
    catch (YAML::ParserException &e) {
        return QString(e.what());
    }

    this->providerConfig(stackTemplate);
    this->consumerConfig(stackTemplate);
    this->healthAdapterConfig(stackTemplate);
    std::ofstream fout("/Users/fsjovatsen/test.yml");
    fout << stackTemplate;

    return QString();

}

void StackFile::consumerConfig(YAML::Node &config) {

    config["services"]["provider"]["ports"][0] =
            this->value(config["services"]["provider"]["ports"][0], this->port());

    config["services"]["provider"]["environment"]["server.context-path"] =
            this->value(config["services"]["provider"]["environment"]["server.context-path"], this->uri());

    config["services"]["provider"]["environment"]["fint.audit.mongo.databasename"] =
            this->value(config["services"]["provider"]["environment"]["fint.audit.mongo.databasename"], this->environment());


}



void StackFile::providerConfig(YAML::Node &config) {

    config["services"]["consumer"]["image"] = QString("%1%2%3").arg(this->repository(), QDir::separator(), this->stack()).toStdString();

    config["services"]["consumer"]["ports"][0] =
            this->value(config["services"]["consumer"]["ports"][0], this->port());

    config["services"]["consumer"]["environment"]["server.context-path"] =
            this->value(config["services"]["consumer"]["environment"]["server.context-path"], this->uri());

    config["services"]["consumer"]["environment"]["fint.audit.mongo.databasename"] =
            this->value(config["services"]["consumer"]["environment"]["fint.audit.mongo.databasename"], this->environment());

    config["services"]["consumer"]["environment"]["fint.relations.default-base-url"] =
            this->value(config["services"]["consumer"]["environment"]["fint.relations.default-base-url"], this->environment());



}


void StackFile::healthAdapterConfig(YAML::Node &config) {

    config["services"]["health-adapter"]["environment"]["fint.adapter.sse-endpoint"] =
            this->value(config["services"]["health-adapter"]["environment"]["fint.adapter.sse-endpoint"],
                    this->environment(), this->uri());

    config["services"]["health-adapter"]["environment"]["fint.adapter.status-endpoint"] =
            this->value(config["services"]["health-adapter"]["environment"]["fint.adapter.status-endpoint"],
                        this->environment(), this->uri());

    config["services"]["health-adapter"]["environment"]["fint.adapter.response-endpoint"] =
            this->value(config["services"]["health-adapter"]["environment"]["fint.adapter.response-endpoint"],
                        this->environment(), this->uri());
}



std::string StackFile::value(YAML::Node node, QString xxxx) {
     std::string value = node.as<std::string>();

     return QString(node.as<std::string>().data()).replace(QString("$1"), xxxx).toStdString();

}

std::string StackFile::value(YAML::Node node, QString xxxx, QString yyyy) {
    return QString(node.as<std::string>().data())
    .replace(QString("$1"), xxxx)
    .replace(QString("$2"), yyyy)
    .toStdString();
}



