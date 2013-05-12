#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDir>
#include <QSettings>
#include <libexif/exif-data.h>
#include <QtDebug>

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
    if( !dir.isEmpty() ) {
        ui->leDirName->setText( dir );
        m_settings->setValue( "directory", dir );
    }
}

void MainWindow::on_actionReadDir_triggered()
{
    QDir *dir = new QDir( ui->leDirName->text() );
    QStringList filters;
    filters << "*.jpg" << "*.jpeg";
    dir->setNameFilters( filters );
    dir->setFilter( QDir::Files );

    QStringList headers;
    headers << tr("File name") << tr("EXIF date") << tr("status");

    // prepare tableWidget...
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount( dir->count() );
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels( headers );

    m_fileTimeMap.clear();

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
                m_fileTimeMap.insert( fil.at(i).fileName(), tm );
            }
        }
        //exif_entry_unref( exif_time );
        exif_data_unref( exif_data );
        QTableWidgetItem *itemFileDate = new QTableWidgetItem( fileDate );
        itemFileDate->setFlags( Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
        ui->tableWidget->setItem( i, 1, itemFileDate );
        QTableWidgetItem *itemStatus = new QTableWidgetItem( tr("new") );
        itemStatus->setFlags( Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
        ui->tableWidget->setItem( i, 2, itemStatus );
    }
    delete dir;
}

void MainWindow::on_actionClearList_triggered()
{
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->setRowCount(0);

    m_fileTimeMap.clear();
}

void MainWindow::on_actionMoveFiles_triggered()
{
    size_t countSuccess = 0;
    size_t countError = 0;
    for( t_fileTimeMap::Iterator it = m_fileTimeMap.begin(); it != m_fileTimeMap.end(); ++it ) {
        QString srcFolderName = ui->leDirName->text();
        char buf[1024];
        struct tm tm = it.value();
        strftime( buf, 1024, "%F", &tm );
        if( srcFolderName.right(1) != "/" )
            srcFolderName += "/";
        QString dstFolderName( srcFolderName );
        dstFolderName.append( buf );
        dstFolderName += "/";
        if( !QDir(dstFolderName).exists() ) {
            QDir().mkdir(dstFolderName);
        }
        if( QDir().rename( srcFolderName + it.key(), dstFolderName + it.key() ) ) {
            ++countSuccess;
        } else {
            qDebug() << "error moving " << it.key() << " to " << dstFolderName;
            ++countError;
        }
    }
    if( countError ) {
        QString msg = QString( tr("Error while moving %1 files.\n%2 files were moved successfully!") ).arg(countError).arg(countSuccess);
        QMessageBox::warning( this, "ExifMover", msg );
    } else {
        QString msg = QString( tr("%1 files were moved successfully!") ).arg(countSuccess);
        QMessageBox::information( this, "ExifMover", msg );
    }
}
