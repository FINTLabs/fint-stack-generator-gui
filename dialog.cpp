#include "dialog.h"
#include "ui_dialog.h"
#include "StackConfig.h"

#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QDebug>
#include <QFile>
#include <QDataStream>

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <fstream>
#include <QInputDialog>
#include <QStyle>


Dialog::Dialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Dialog) {
    ui->setupUi(this);

    readSettings();

    this->setWindowTitle(currentDirectory);

    ui->newDirectoryToolButton->setIcon(this->style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->deleteDirFileToolButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogDiscardButton));
    ui->setDirectoryToolButton->setIcon(this->style()->standardIcon(QStyle::SP_DesktopIcon));
    ui->renameDirectoryToolButton->setIcon(this->style()->standardIcon(QStyle::SP_DirIcon));

    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot |
                        QDir::Files | QDir::AllDirs);
    dirModel->setNameFilters(QStringList() << "*.yml");
    dirModel->setNameFilterDisables(false);
    ui->treeView->setModel(dirModel);
    ui->treeView->setRootIndex(dirModel->setRootPath(currentDirectory));
    ui->treeView->setColumnHidden(1, true);
    ui->treeView->setColumnHidden(2, true);
    ui->treeView->setColumnHidden(3, true);

    //ui->treeView->setColumnWidth(0, 400);

    ui->environmentCombo->addItem("");
    ui->environmentCombo->addItem("beta");
    ui->environmentCombo->addItem("api");

    ui->messageLabel->setText("Please select a directory or file!");


}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::writeSettings() {
    QSettings settings("FINTLabs", "Stack Generator");

    settings.beginGroup("stack/path");
    settings.setValue("path", currentDirectory);
    settings.endGroup();


}

void Dialog::readSettings() {
    QSettings settings("FINTLabs", "Stack Generator");

    settings.beginGroup("stack/path");
    currentDirectory = settings.value("path").toString();
    settings.endGroup();
}

void Dialog::on_setDirectoryToolButton_clicked() {
    QString
            homeLocation = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
    currentDirectory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         homeLocation,
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);

    if (!currentDirectory.isEmpty()) {
        ui->treeView->setRootIndex(dirModel->setRootPath(currentDirectory));
        writeSettings();
    }

}


void Dialog::on_treeView_clicked(const QModelIndex &index) {
    QFileInfo localFileInfo = dirModel->fileInfo(index);
    QString
            environment;
    QString
            resources;
    QString
            port;
    QString
            stack;
    QString
            provider;
    QString
            repository;
    QString
            uri;
    QString
            version;
    YAML::Node config;

    selectedFileInfo = dirModel->fileInfo(index);

    QString
            file = localFileInfo.absoluteFilePath();
    //currentFile = localFileInfo.absoluteFilePath();


    ui->newDirectoryToolButton->setDisabled(selectedFileInfo.isFile());
    ui->renameDirectoryToolButton->setDisabled(selectedFileInfo.isFile());
    ui->deleteDirFileToolButton->setDisabled(false);
    ui->saveButton->setDisabled(false);
    //ui->newConfigButton->setDisabled(false);

    if (localFileInfo.isDir()) {
        this->setWindowTitle(localFileInfo.absoluteFilePath());
        selectDirectory = localFileInfo.absoluteFilePath();
        clearInputFields();
    } else {
        this->setWindowTitle(localFileInfo.canonicalPath());
        selectDirectory = localFileInfo.canonicalPath();
    }

    if (localFileInfo.isDir()) {
        return;
    }

    try {
        config = YAML::LoadFile(file.toStdString());
    }
    catch (YAML::ParserException &e) {
        ui->messageLabel->setText("Unable to parse yaml file.");
        ui->saveButton->setDisabled(true);
        return;
    }


    ui->messageLabel->setText("");

    if (!StackConfig::isStackConfig(config)) {
        ui->messageLabel->setText("This is not at FINTLabs stack config file!");
        ui->saveButton->setDisabled(!StackConfig::isStackConfig(config));
        clearInputFields();
        return;
    }

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

}

void Dialog::on_saveButton_clicked() {

    StackConfig::save(ui, &selectedFileInfo);
}


void Dialog::on_newDirectoryToolButton_clicked() {

    bool ok;
    QString directoryName = QInputDialog::getText(this, "Directory name",
                                                  "Directory name:", QLineEdit::Normal,
                                                  NULL, &ok);
    QString fullPath = QString("%1%2%3").arg(selectDirectory, QDir::separator(), directoryName);

    if (!QDir(fullPath).exists()) {
        QDir().mkpath(fullPath);
    }

}

void Dialog::on_deleteDirFileToolButton_clicked() {

    if (selectedFileInfo.isFile()) {
        QFile().remove(selectedFileInfo.absoluteFilePath());
    }
    if (selectedFileInfo.isDir()) {
        QDir(selectedFileInfo.absoluteFilePath()).removeRecursively();
    }
}

void Dialog::clearInputFields() {
    ui->environmentCombo->setCurrentText("");
    ui->stackEdit->setText("");
    ui->uriEdit->setText("");
    ui->repositoryEdit->setText("");
    ui->versionEdit->setText("");
    ui->portEdit->setText("");
    ui->providerEdit->setText("");
    ui->assetEdit->setText("");
}

void Dialog::on_renameDirectoryToolButton_clicked() {
    bool ok;

    QString directoryName = QInputDialog::getText(this, "Rename",
                                                  "New directory name:", QLineEdit::Normal,
                                                  selectedFileInfo.baseName(), &ok);
    QString oldPath = selectedFileInfo.absoluteFilePath();
    QString newPath = QString("%1%2%3").arg(selectedFileInfo.canonicalPath(), QDir::separator(), directoryName);

    if (ok && QDir(oldPath).exists() && !QDir(newPath).exists()) {
        QDir().rename(oldPath, newPath);
    }
}

