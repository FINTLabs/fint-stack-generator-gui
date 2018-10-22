#include "dialog.h"
#include "ui_dialog.h"
#include "StackConfig.h"
#include "StackFile.h"
#include "StackGeneratorConfig.h"

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
#include <QWidget>
#include <QMessageBox>


Dialog::Dialog(QString currentDirectory, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Dialog),
        currentDirectory(currentDirectory) {

    ui->setupUi(this);

    this->setWindowTitle(currentDirectory);

    ui->newDirectoryToolButton->setIcon(this->style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->deleteDirFileToolButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogDiscardButton));
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

    ui->environmentCombo->addItem(StackGeneratorConfig::environmentNone());
    ui->environmentCombo->addItem(StackGeneratorConfig::environmentBeta());
    ui->environmentCombo->addItem(StackGeneratorConfig::environmentApi());

    ui->messageLabel->setText("Please select a directory or file!");


}

Dialog::~Dialog() {
    delete ui;
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
        selectedDirectory = selectedFileInfo.absoluteFilePath();
        ui->saveButton->setDisabled(false);
        clearInputFields();
        return;
    } else {
        this->setWindowTitle(selectedFileInfo.canonicalPath());
        selectedDirectory = selectedFileInfo.canonicalPath();
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
                                                  nullptr, &ok);
    QString fullPath = QString("%1%2%3").arg(selectedDirectory, QDir::separator(), directoryName);

    if (!QDir(fullPath).exists()) {
        QDir().mkpath(fullPath);
    }

}

void Dialog::on_deleteDirFileToolButton_clicked() {

    if (selectedFileInfo.isFile()) {
        QFile::remove(selectedFileInfo.absoluteFilePath());
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
        QString result = stackFile.generate(StackGeneratorConfig::stackTemplateFile(currentDirectory),
                                            StackGeneratorConfig::stackFileOutputDirectory(selectedDirectory));

        if (result.isEmpty()) {
            ui->messageLabel->setText("Application stack file is generated successfully!");
            ui->messageLabel->setStyleSheet(StackGeneratorConfig::labelSuccess());
        } else {
            ui->messageLabel->setText(result);
            ui->messageLabel->setStyleSheet(StackGeneratorConfig::labelError());
        }
    }

}


