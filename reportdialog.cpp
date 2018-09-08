#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>

#include "reportdialog.h"
#include "ui_reportdialog.h"

#include "csv.h"

ReportDialog::ReportDialog(QWidget *parent, QString text) :
    QDialog(parent),
    ui(new Ui::ReportDialog)
{
    ui->setupUi(this);
    ui->plainTextEdit->setPlainText(text);

    connect(ui->saveButton, SIGNAL(pressed()), SLOT(saveReport()));
}

ReportDialog::~ReportDialog()
{
    delete ui;
}

void ReportDialog::saveReport()
{
    QChar delimiter = ',';
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save report"), QDir::homePath(), tr("Comma separated values (*.csv);;Tab delimited (*.txt)"));
    qDebug() << "Saving to: " << fileName;

    // Check file name and extension
    QFileInfo testFile(fileName);
    if (testFile.suffix() == "txt") {
        delimiter = '\t';
    }

    // Save report in CSV or TXT file format
    saveTextReport(fileName, delimiter);
}

void ReportDialog::saveTextReport(QString fileName, QChar delimiter)
{
    QList<QStringList> data;

    // Extract the content from the text field
    QStringList content = ui->plainTextEdit->toPlainText().split('\n');

    // Parse each line of the content
    foreach (QString line, content) {
        QStringList line_data = line.split('\t');
        data.append(line_data);
    }

    // Write data to file name
    CSV::write(data, fileName, "UTF-8", delimiter);
}
