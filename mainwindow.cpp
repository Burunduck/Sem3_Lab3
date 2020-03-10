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
    ui->btnDrawGraphic->setDisabled(true);
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
        ui->btnDrawGraphic->setDisabled(false);
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
        if(ui->lineEditChooseColumn->text().toInt() - 1 == 0 || ui->lineEditChooseColumn->text().toInt() - 1 > 1 & ui->lineEditChooseColumn->text().toInt() -1 < 7)
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

void MainWindow::on_btnDrawGraphic_clicked()
{

    QChart *chart = new QChart();
    QLineSeries *series = new QLineSeries();

    sourceData source;
    source.path = ui->lineEditFilePath->text().toStdString();
    source.region = ui->lineEditChooseRegion->text().toStdString();
    source.column = ui->lineEditChooseColumn->text().toInt() - 1;
    resultData result = command(getTable, &source);
    if(result.tableDataByRegion.countOfLines > 2)
    {
        if(ui->lineEditChooseColumn->text().toInt() - 1 > 1 & ui->lineEditChooseColumn->text().toInt() -1 < 7 )
        {
            result = command(calculateMetrics, &source);
            for (int i = 1; i < result.tableDataByRegion.countOfLines; i++)
            {
                series->append(atoi(result.tableDataByRegion.tableInMatrix[i][0].c_str()), atof(result.tableDataByRegion.tableInMatrix[i][source.column].c_str()));
            }
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Please enter a correct data for column");
            msgBox.exec();
            return;
        }
    }
    else
    {
            QMessageBox msgBox;
            msgBox.setText("Sorry, we don`t have this region in our table data");
            msgBox.exec();
            return;
    }

    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Simple line chart example");

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Data point");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(series->count());
    axisX->setLabelsAngle(-90);
    axisX->setMinorTickCount(2);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Data point");
    axisY->setRange(result.metrics.min, result.metrics.max);
    axisY->setMinorTickCount(10);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    ui->graphicsView->setChart(chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}
