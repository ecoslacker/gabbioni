#include <QFile>
#include <QTextStream>
#include <QTableWidgetItem>
#include <QDebug>

#include "roughnesscoefficientdialog.h"
#include "ui_roughnesscoefficientdialog.h"

const int COLUMNS = 4;
const int IGNORED_LINES = 2;

RoughnessCoefficientDialog::RoughnessCoefficientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoughnessCoefficientDialog)
{
    ui->setupUi(this);
    populateTables();
    connect(ui->tableConduits,  SIGNAL(itemPressed(QTableWidgetItem*)), SLOT(retrieveItemValue(QTableWidgetItem*)));
    connect(ui->tableChannels,  SIGNAL(itemPressed(QTableWidgetItem*)), SLOT(retrieveItemValue(QTableWidgetItem*)));
    connect(ui->tableExcavated, SIGNAL(itemPressed(QTableWidgetItem*)), SLOT(retrieveItemValue(QTableWidgetItem*)));
    connect(ui->tableNatural,   SIGNAL(itemPressed(QTableWidgetItem*)), SLOT(retrieveItemValue(QTableWidgetItem*)));
    connect(ui->lineEdit,       SIGNAL(textEdited(QString)),            SLOT(updateCoefficient()));
    connect(ui->lineEdit,       SIGNAL(textChanged(QString)),           SLOT(updateCoefficient()));
}

RoughnessCoefficientDialog::~RoughnessCoefficientDialog()
{
    delete ui;
}

QString RoughnessCoefficientDialog::coefficient() const
{
    return _coefficient;
}

void RoughnessCoefficientDialog::populateTables()
{
    populateTable(":/help/doc/1_closed_conduits.txt", ui->tableConduits, ui->labelConduits);
    populateTable(":/help/doc/2_channels.txt", ui->tableChannels, ui->labelChannels);
    populateTable(":/help/doc/3_excavated.txt", ui->tableExcavated, ui->labelExcavated);
    populateTable(":/help/doc/4_natural.txt", ui->tableNatural, ui->labelNatural);
}

void RoughnessCoefficientDialog::populateTable(QString txtFile, QTableWidget *table, QLabel *label)
{
    QFile file1(txtFile);
    if (file1.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream1(&file1);
        QStringList content;

        while (!stream1.atEnd()) {
            content.append(stream1.readLine());
        }

        table->setColumnCount(COLUMNS);
        table->setRowCount(content.size() - IGNORED_LINES);
        table->setHorizontalHeaderLabels(content.at(0).split('\t'));
        label->setText(content.at(1).split('\t').at(0));

        int row = 0;
        for (int i = 2; i < content.size(); i++) {

            QTableWidgetItem *item1 = new QTableWidgetItem(content.at(i).split('\t').at(0));
            QTableWidgetItem *item2 = new QTableWidgetItem(content.at(i).split('\t').at(1));
            QTableWidgetItem *item3 = new QTableWidgetItem(content.at(i).split('\t').at(2));
            QTableWidgetItem *item4 = new QTableWidgetItem(content.at(i).split('\t').at(3));

            item1->setTextAlignment(Qt::AlignLeft);
            item2->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item3->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item4->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item1->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            item2->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            item3->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            item4->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

            table->setItem(row, 0, item1);
            table->setItem(row, 1, item2);
            table->setItem(row, 2, item3);
            table->setItem(row, 3, item4);
            table->setWordWrap(true);
            table->resizeColumnsToContents();
            table->setColumnWidth(0, 250);
            table->resizeRowsToContents();

            row++;
        }

    } else {
        return;
    }
}

void RoughnessCoefficientDialog::retrieveItemValue(QTableWidgetItem *item)
{
    QString text = item->text();
    if (text.toDouble() != 0)
        ui->lineEdit->setText(text);
}

void RoughnessCoefficientDialog::updateCoefficient()
{
    _coefficient = ui->lineEdit->text();
}
