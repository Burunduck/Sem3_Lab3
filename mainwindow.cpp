#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "buisnesslogic.h"
#include <fstream>
#include <iostream>

#include <QMessageBox>
#include <QString>
#include <QTabWidget>
#include <QFileDialog>
#include <QtCharts>

using namespace QtCharts;
// спросить про утечки памяти, узнать почему таблица может вырастать до двух


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->btnLoadTable->setDisabled(true);
    ui->btnCountMetrics->setDisabled(true);
    QChart *chart = new QChart();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnLoadFile_clicked()
{
    this->filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                      "/home",
                                                      tr("(*.csv)"));
    ui->lineEditFilePath->setText(this->filePath);
    if (ui->lineEditFilePath->text() != QString(""))
    {
        ui->btnLoadTable->setDisabled(false);
    }
}


void MainWindow::initTableWidget()
{
    sourceData source;
    source.path = ui->lineEditFilePath->text().toStdString();
    source.region = ui->lineEditChooseRegion->text().toStdString();
    resultData result = command(getTable, &source);
    string** fromTableStr = result.tableDataByRegion.tableInMatrix;
    if(result.tableDataByRegion.countOfLines > 2)
    {
        ui->tableWidget->setRowCount(result.tableDataByRegion.countOfLines);
        ui->tableWidget->setColumnCount(result.tableDataByRegion.countOfColumns);
        for(int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            for(int j = 0; j < ui->tableWidget->columnCount(); j++)
            {
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setText(QString :: fromStdString(fromTableStr[i][j]));
                ui->tableWidget->setItem(i, j, item);
            }
        }
        ui->btnCountMetrics->setDisabled(false);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Sorry, we don`t have this region in our table data");
        msgBox.exec();
    }
}

void MainWindow::on_btnLoadTable_clicked()
{
    initTableWidget();
}

void MainWindow::on_btnCountMetrics_clicked()
{
    sourceData source;
    source.path = ui->lineEditFilePath->text().toStdString();
    source.region = ui->lineEditChooseRegion->text().toStdString();
    source.column = ui->lineEditChooseColumn->text().toInt() - 1;
    resultData result = command(getTable, &source);
    if(result.tableDataByRegion.countOfLines > 2)
    {
        if(ui->lineEditChooseColumn->text().toInt() -1  == 0 || ui->lineEditChooseColumn->text().toInt() - 1 > 1 & ui->lineEditChooseColumn->text().toInt() -1 < 7)
        {
            result = command(calculateMetrics, &source);
            ui->lblMin->setText("Min: " + QString::number(result.metrics.min));
            ui->lblAverage->setText("Average: " + QString::number(result.metrics.average));
            ui->lblMedian->setText("Median: " + QString::number(result.metrics.median));
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Please enter a correct data for column");
            msgBox.exec();
        }
    }
    else
    {
            QMessageBox msgBox;
            msgBox.setText("Sorry, we don`t have this region in our table data");
            msgBox.exec();
    }
}
