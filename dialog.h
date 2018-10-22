#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileSystemModel>

#include <yaml-cpp/yaml.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QString currentDirectory, QWidget *parent = nullptr);
    ~Dialog();

private slots:
    //void on_setDirectoryToolButton_clicked();
    void on_treeView_clicked(const QModelIndex &index);
    void on_saveButton_clicked();
    //void on_newConfigButton_clicked();
    void on_newDirectoryToolButton_clicked();
    void on_deleteDirFileToolButton_clicked();
    void on_renameDirectoryToolButton_clicked();
    void on_generateButton_clicked();


private:
    Ui::Dialog *ui;
    QFileSystemModel *dirModel;
    QString currentDirectory;
    //QString currentFile;
    QString selectedDirectory;
    QFileInfo selectedFileInfo;
    YAML::Node config;
    //bool isCleared = false;
    void writeSettings();
    void readSettings();
    //bool isStackConfig(const YAML::Node conf);
    void clearInputFields();

    //void saveStackConfig(const YAML::Emitter &emitter, QString yamlFilePath) const;
};

#endif // DIALOG_H
