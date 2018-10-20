#include "dialog.h"
#include "ui_dialog.h"
#include "StackConfig.h"
#include "StackFile.h"

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

    //readSettings();
    currentDirectory = QDir::currentPath();

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

    //settings.beginGroup("stack/path");
    settings.setValue("path", currentDirectory);
    //settings.endGroup();


}

void Dialog::readSettings() {
    QSettings settings("FINTLabs", "Stack Generator");

    //settings.beginGroup("stack/path");
    currentDirectory = settings.value("path", QDir::currentPath()).toString();
    //settings.endGroup();
}

void Dialog::on_setDirectoryToolButton_clicked() {
    QString homeLocation = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
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


    ui->messageLabel->setStyleSheet("");
    ui->messageLabel->setText("");

    selectedFileInfo = dirModel->fileInfo(index);

    ui->newDirectoryToolButton->setDisabled(!selectedFileInfo.isDir());
    ui->renameDirectoryToolButton->setDisabled(!selectedFileInfo.isDir());
    ui->deleteDirFileToolButton->setDisabled(false);
    ui->generateButton->setDisabled(true);


    if (selectedFileInfo.isDir()) {
        this->setWindowTitle(selectedFileInfo.absoluteFilePath());
        selectDirectory = selectedFileInfo.absoluteFilePath();
        clearInputFields();
        return;
    } else {
        this->setWindowTitle(selectedFileInfo.canonicalPath());
        selectDirectory = selectedFileInfo.canonicalPath();
    }

    config = StackConfig::load(ui, &selectedFileInfo);
    if (config.IsNull()) {
        clearInputFields();
    }

    ui->generateButton->setDisabled(!StackConfig::isStackConfig(config));

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

void Dialog::on_generateButton_clicked() {


    if (StackConfig::isStackConfig(config)) {

        StackFile stackFile(config);

        std::string stackTemplate = QString("%1%2%3").arg(currentDirectory, QDir::separator(), "fint-component-template.yml").toStdString();

        QString result = stackFile.generate(stackTemplate);

        if (result.isEmpty()) {
            ui->messageLabel->setText("Application stack file is generated successfully!");
            ui->messageLabel->setStyleSheet("QLabel { background-color : green; }");
        } else {
            ui->messageLabel->setText(result);
            ui->messageLabel->setStyleSheet("QLabel { background-color : red; }");
        }
    }


}

