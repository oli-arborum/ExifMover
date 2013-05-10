#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDir>
#include <QSettings>
#include <libexif/exif-data.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_settings = new QSettings( "o_baum", "ExifMover" );
    QString dir = m_settings->value( "directory", "" ).toString();
    if( dir != "" ) ui->leDirName->setText( dir );
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_settings;
}

void MainWindow::on_pbSelectDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory") ); // ,
                                                     // "/home",
                                                     // QFileDialog::ShowDirsOnly
                                                     // | QFileDialog::DontResolveSymlinks);
    ui->leDirName->setText( dir );
    m_settings->setValue( "directory", dir );
}

void MainWindow::on_actionTest_triggered()
{
    QDir *dir = new QDir( ui->leDirName->text() );
    QStringList filters;
    filters << "*.jpg" << "*.jpeg";
    dir->setNameFilters( filters );
    dir->setFilter( QDir::Files );

    QStringList headers;
    headers << tr("File name") << tr("EXIF date");

    ui->tableWidget->setRowCount( dir->count() );
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels( headers );

    QFileInfoList fil = dir->entryInfoList();
    for( int i=0; i<fil.size(); ++i ) {
        QTableWidgetItem *itemFileName = new QTableWidgetItem( fil.at(i).fileName() );
        itemFileName->setFlags( Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
        ui->tableWidget->setItem( i, 0, itemFileName );
        ///// @TODO write C++ wrapper (or function) for that...
        ExifData* exif_data = exif_data_new_from_file( fil.at(i).absoluteFilePath().toAscii() );
        ExifEntry* exif_time = exif_data_get_entry( exif_data, EXIF_TAG_DATE_TIME );
        // @TODO read also EXIF_TAG_DATE_TIME_ORIGINAL and EXIF_TAG_DATE_TIME_DIGITIZED, use if EXIF_TAG_DATE_TIME is invalid or not plausible
        QString fileDate;
        if( exif_time == NULL ) {
            // TODO try file timestamp
            fileDate = "error!";
        } else {
            char buf[1024];
            exif_entry_get_value(exif_time, buf, 1024);
            struct tm tm;
            if( strptime(buf, "%Y:%m:%d %H:%M:%S", &tm) == NULL ) {
               fileDate = "parse error!";
            } else {
                fileDate = buf;
            }
        }
        //exif_entry_unref( exif_time );
        exif_data_unref( exif_data );
        QTableWidgetItem *itemFileDate = new QTableWidgetItem( fileDate );
        itemFileDate->setFlags( Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
        ui->tableWidget->setItem( i, 1, itemFileDate );
        /////
    }
    delete dir;
}
