#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    database.connect();

    if (database.open()){
          loadFirmwareTable();
          loadControllerTable();
    }
    else {
        QMessageBox::information(this, "failed", "Connection failed");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButtonController_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    loadControllerTable();
}

void MainWindow::on_toolButtonFirmware_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    loadFirmwareTable();
}

void MainWindow::loadFirmwareTable(){
    model = new QSqlTableModel();
    model->setTable("firmware");
    model->setSort(0, Qt::AscendingOrder);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("id"));
    model->setHeaderData(1, Qt::Horizontal, tr("name"));
    model->setHeaderData(2, Qt::Horizontal, tr("path"));
    model->setHeaderData(3, Qt::Horizontal, tr("description"));
    ui->tableViewFirmware->setModel(model);
    ui->tableViewFirmware->resizeRowsToContents();
    ui->tableViewFirmware->resizeColumnsToContents();
}

void MainWindow::loadControllerTable(){
    rmodel = new QSqlRelationalTableModel();
    rmodel->setTable("controller");
    rmodel->setRelation(4, QSqlRelation("firmware", "firmware_id", "firmware_name"));
    rmodel->select();

    rmodel->setHeaderData(0, Qt::Horizontal, tr("id"));
    rmodel->setHeaderData(1, Qt::Horizontal, tr("name"));
    rmodel->setHeaderData(2, Qt::Horizontal, tr("ip"));
    rmodel->setHeaderData(3, Qt::Horizontal, tr("description"));
    rmodel->setHeaderData(4, Qt::Horizontal, tr("firmware"));

    ui->tableViewController->setModel(rmodel);
    ui->tableViewController->resizeRowsToContents();
    ui->tableViewController->resizeColumnsToContents();

    qmodel = new QSqlQueryModel();
    qmodel->setQuery("SELECT firmware_name FROM firmware;");
    ui->comboBoxFirmware->setPlaceholderText(QStringLiteral("(firmware)"));
    ui->comboBoxFirmware->setModel(qmodel);
    ui->comboBoxFirmware->setCurrentIndex(-1);
}

void MainWindow::on_toolButtonExit_clicked()
{
    close();
}


void MainWindow::on_tableViewFirmware_pressed(const QModelIndex &index)
{
    ui->lineEditFirmwareName->setText( index.sibling(index.row(),1).data().toString() );
    ui->lineEditFirmwarePath->setText( index.sibling(index.row(),2).data().toString() );
    ui->lineEditFirmwareDescription->setText( index.sibling(index.row(),3).data().toString() );
}


void MainWindow::on_toolButtonFirmwareAdd_clicked()
{
    QString str = "INSERT INTO firmware(firmware_name, firmware_path, firmware_description) VALUES (";
    if (ui->lineEditFirmwareName->displayText() != ""){
        str += "'" + ui->lineEditFirmwareName->displayText() + "', ";
    } else {
        str += "NULL,";
    }

    if (ui->lineEditFirmwarePath->displayText() != ""){
        str += "'" + ui->lineEditFirmwarePath->displayText() + "', ";
    } else {
        str += "NULL, ";
    }

    if (ui->lineEditFirmwareDescription->displayText() != ""){
        str += "'" + ui->lineEditFirmwareDescription->displayText() + "');";
    } else {
        str += "NULL);";
    }

    QSqlQueryModel qmodel;
    qmodel.setQuery(str);

    loadFirmwareTable();
}


void MainWindow::on_toolButtonFirmwareDelete_clicked()
{
    QSqlQuery query;
    query.prepare("DELETE FROM firmware WHERE  firmware_name = :id;");
    query.bindValue(":id", ui->lineEditFirmwareName->displayText());
    query.exec();

    loadFirmwareTable();
}


void MainWindow::on_tableViewController_pressed(const QModelIndex &index)
{
    ui->lineEditControllerName->setText( index.sibling(index.row(),1).data().toString() );
    ui->lineEditControllerIp->setText( index.sibling(index.row(),2).data().toString() );
    ui->lineEditControllerDescription->setText( index.sibling(index.row(),3).data().toString() );
    ui->comboBoxFirmware->setCurrentText( index.sibling(index.row(),4).data().toString() );
}

