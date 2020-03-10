#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initTableWidget();

private slots:
    void on_btnLoadFile_clicked();

    void on_btnLoadTable_clicked();

    void on_btnCountMetrics_clicked();

    void on_btnDrawGraphic_clicked();

private:
    Ui::MainWindow *ui;
    QString filePath;
};
#endif // MAINWINDOW_H
