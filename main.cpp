#include "dialog.h"
#include "StackGeneratorConfig.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QString currentDirectory = QString("%1%2%3").arg(QDir::currentPath(), QDir::separator(), StackGeneratorConfig::felleskomponentDirectory());
    if (!QDir().exists(currentDirectory)) {
        QMessageBox::warning(nullptr,
                             QApplication::applicationDisplayName(),
                             "The directory 'felleskomponent' does not exist.\n\nThe application will self destruct.",
                             QMessageBox::Ok);
        return -1;
    }
    Dialog w(currentDirectory);
    w.show();

    return a.exec();
}
