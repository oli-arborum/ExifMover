#include <QtGui/QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTextCodec>
#include <QMessageBox>
#include <QtDebug>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name();

    // QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTranslator translator;
    if( !translator.load(QString("ExifMover-") + locale) ) {
        qDebug() << "failed to load translation! locale=" << locale;
    }
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
