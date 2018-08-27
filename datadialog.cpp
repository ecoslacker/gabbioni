//#include <QVector>
//#include <QStringList>
#include <QDebug>
#include <QString>
#include <QFileInfo>
#include <QFileDialog>
#include <QTableWidgetItem>
#include <QMessageBox>
#include "datadialog.h"
#include "ui_datadialog.h"

const size_t COLUMNS = 2;

DataDialog::DataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataDialog)
{
    ui->setupUi(this);
    connect(ui->openButton,   SIGNAL(pressed()), SLOT(openFile()));
    connect(ui->saveButton,   SIGNAL(pressed()), SLOT(saveFile()));
    connect(ui->addButton,    SIGNAL(pressed()), SLOT(addRow()));
    connect(ui->removeButton, SIGNAL(pressed()), SLOT(removeRow()));

    _fileName = "";
}

DataDialog::~DataDialog()
{
    delete ui;
}

std::vector<std::vector<std::string> > DataDialog::getData()
{
    getDataFromTable();
    return _data;
}

QString DataDialog::getFileName() const
{
    return _fileName;
}

void DataDialog::openFile()
{
    char delimiter = ',';

    // Get the file name from dialog
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Open data File"), QDir::homePath(), tr("Comma separated values (*.csv);;Tab delimited (*.txt)"));

    // Check file name
    QFileInfo testFile(fileName);
    if (!testFile.exists())
        return;

    // Check the extension
    if (testFile.suffix() == "txt") {
        delimiter = '\t';
    }

    // Get the data from the CSV file (this uses libcsvdata)
    if (!CsvData::parseFromFile(fileName.toStdString(), _data, delimiter)) {
        qDebug() << "There was an error openning file: " << fileName << " for reading.";
        return;
    }

    _fileName = fileName;

    setDataToTable();
}

void DataDialog::saveFile()
{
    getDataFromTable();

    // Verify the data
    qDebug() << "Data: " << QString::number(_data.size());
    for (size_t i = 0; i < _data.size(); i++) {
        qDebug() << QString::fromStdString( _data.at(i).at(0) ) << QString::fromStdString( _data.at(i).at(1) );
    }

    std::string delimiter = ",";

    // Get file name from dialog
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, tr("Save data file"), QDir::homePath(), tr("Comma separated values (*.csv);;Tab delimited (*.txt)"));

    // Check file name
    QFileInfo testFile(fileName);

    // Check the extension
    if (testFile.suffix() == "txt") {
        delimiter = '\t';
    }

    qDebug() << fileName << QString::fromStdString(delimiter);
    if (!CsvData::write(fileName.toStdString(), _data, delimiter)) {
        qDebug() << "There was an error openning file: " << fileName << " for writing.";
    }
}

void DataDialog::setDataToTable()
{
    // If data is empty, don't move anything
    if (_data.empty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Incorrect format"));
        msgBox.setText(tr("Not a valid data file, maybe the data is invalid or has an incorrect format."));
        msgBox.setInformativeText(tr("Please provide a valid data text file with tab or comma separated values."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    // Adjust the number of rows to the data
    ui->tableWidget->setRowCount(int(_data.size()));

    // Set the data to the table
    for (size_t i = 0; i < _data.size(); i++) {

        if (_data.at(i).size() != COLUMNS)
            return;

        QTableWidgetItem *itemX = new QTableWidgetItem(QString::fromStdString(_data.at(i).at(0)).trimmed());
        QTableWidgetItem *itemY = new QTableWidgetItem(QString::fromStdString(_data.at(i).at(1)).trimmed());

        itemX->setTextAlignment(Qt::AlignRight);
        itemY->setTextAlignment(Qt::AlignRight);

        ui->tableWidget->setItem(int(i), 0, itemX);
        ui->tableWidget->setItem(int(i), 1, itemY);
    }
}

void DataDialog::getDataFromTable()
{
    /*
     * Set the input data from the table, get the coordinate points from the columns
     * ignoring rows that have al least one empty cell.
     */

    _data.clear();
    for (int i=0; ui->tableWidget->rowCount(); i++) {
        std::vector<std::string> row;
        QTableWidgetItem *itemX = ui->tableWidget->item(i, 0);
        QTableWidgetItem *itemY = ui->tableWidget->item(i, 1);

        // Ignore empty cells
        if (!itemX || !itemY) {
            break;
        }

        row.push_back(itemX->text().toStdString());
        row.push_back(itemY->text().toStdString());
        _data.push_back(row);
    }
}

void DataDialog::addRow()
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);

    QTableWidgetItem *itemX = new QTableWidgetItem();
    QTableWidgetItem *itemY = new QTableWidgetItem();

    itemX->setTextAlignment(Qt::AlignRight);
    itemY->setTextAlignment(Qt::AlignRight);

    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, itemX);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, itemY);
}

void DataDialog::removeRow()
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() - 1);
}
