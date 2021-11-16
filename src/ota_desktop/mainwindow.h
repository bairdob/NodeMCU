#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <database.h>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void loadFirmwareTable();
    void loadControllerTable();
    ~MainWindow();

private slots:
    void on_toolButtonFirmware_clicked();
    void on_toolButtonController_clicked();
    void on_toolButtonExit_clicked();

    void on_toolButtonFirmwareAdd_clicked();
    void on_toolButtonFirmwareDelete_clicked();

    void on_tableViewController_pressed(const QModelIndex &index);
    void on_tableViewFirmware_pressed(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    Database database;
    QSqlTableModel *model;
    QSqlQueryModel *qmodel;
    QSqlRelationalTableModel * rmodel;
};
#endif // MAINWINDOW_H
