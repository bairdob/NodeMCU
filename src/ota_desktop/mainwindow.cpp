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

void MainWindow::loadFirmwareTable()
{
    model = new QSqlTableModel();
    model->setTable("firmware");
    model->setSort(0, Qt::AscendingOrder);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("id"));
    model->setHeaderData(1, Qt::Horizontal, tr("name"));
    model->setHeaderData(2, Qt::Horizontal, tr("path"));
    model->setHeaderData(3, Qt::Horizontal, tr("description"));
    ui->tableViewFirmware->setModel(model);
    ui->tableViewFirmware->resizeColumnsToContents();
}

void MainWindow::loadControllerTable()
{
    rmodel = new QSqlRelationalTableModel();
    rmodel->setTable("controller");
    rmodel->setSort(0, Qt::AscendingOrder);
    rmodel->setRelation(4, QSqlRelation("firmware", "firmware_id", "firmware_name"));
    rmodel->select();

    rmodel->setHeaderData(0, Qt::Horizontal, tr("id"));
    rmodel->setHeaderData(1, Qt::Horizontal, tr("name"));
    rmodel->setHeaderData(2, Qt::Horizontal, tr("ip"));
    rmodel->setHeaderData(3, Qt::Horizontal, tr("description"));
    rmodel->setHeaderData(4, Qt::Horizontal, tr("firmware"));

    ui->tableViewController->setModel(rmodel);
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
    selectRow = index.sibling(index.row(),0).data().toString();
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
    query.prepare("DELETE FROM firmware WHERE  firmware_name = :name;");
    query.bindValue(":name", ui->lineEditFirmwareName->displayText());
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


void MainWindow::on_toolButtonControllerDelete_clicked()
{
    QSqlQuery query;
    query.prepare("DELETE FROM controller WHERE  controller_name = :name;");
    query.bindValue(":name", ui->lineEditControllerName->displayText());
    query.exec();

    loadControllerTable();
}


void MainWindow::on_toolButtonControllerAdd_clicked()
{
    QString str = "INSERT INTO controller(controller_name, controller_ip, controller_description, \
                   firmware_id) VALUES (";
    if (ui->lineEditControllerName->displayText() != ""){
        str += "'" + ui->lineEditControllerName->displayText() + "', ";
    } else {
        str += "NULL,";
    }

    if (ui->lineEditControllerIp->displayText() != ""){
        str += "'" + ui->lineEditControllerIp->displayText() + "', ";
    } else {
        str += "NULL, ";
    }

    if (ui->lineEditControllerDescription->displayText() != ""){
        str += "'" + ui->lineEditControllerDescription->displayText() + "', ";
    } else {
        str += "NULL)";
    }

    if (ui->comboBoxFirmware->currentText() != ""){
        QSqlQuery query;
        query.prepare("SELECT * FROM firmware WHERE firmware_name = :name;");
        query.bindValue(":name", ui->comboBoxFirmware->currentText() );
        query.exec();
        query.first();
        QString firmware_id = query.value(0).toString();
        str += firmware_id + ");";
    } else {
        str += "NULL);";
    }

    QSqlQueryModel qmodel;
    qmodel.setQuery(str);

    loadControllerTable();
}


void MainWindow::on_toolButtonContollerLoad_clicked()
{
    QString path = "/Users/bair/Documents/Arduino/NodeMCU/src/ota_desktop/";
    QString  command("python");
    QString ip = ui->lineEditControllerIp->displayText();

    QSqlQuery query;
    query.prepare("SELECT * FROM firmware WHERE firmware_name = :name;");
    query.bindValue(":name", ui->comboBoxFirmware->currentText() );
    query.exec();
    query.first();
    QString firmware = query.value(2).toString();

    QStringList params = QStringList() << path + "espota.py" << "-d" <<"-i"<< ip << "-f" << firmware;

    QProcess *process = new QProcess();
    process->startDetached(command, params);
    process->waitForFinished();
    process->close();
}


void MainWindow::on_toolButtonFirmwareSave_clicked()
{
    QString str = "UPDATE firmware SET ";
    if (ui->lineEditFirmwareName->displayText() != ""){
        str += "firmware_name = '" + ui->lineEditFirmwareName->displayText()+ "', ";
    };

    if (ui->lineEditFirmwarePath->displayText() != ""){
        str += "firmware_path = '" + ui->lineEditFirmwarePath->displayText() + "', ";
    };

    if (ui->lineEditFirmwareDescription->displayText()  != ""){
        str += "firmware_description = '" + ui->lineEditFirmwareDescription->displayText() + "'";
    };

    str += " WHERE firmware_id = " + selectRow + ";";

    qmodel = new QSqlQueryModel();
    qmodel->setQuery(str);

    loadFirmwareTable();
}

