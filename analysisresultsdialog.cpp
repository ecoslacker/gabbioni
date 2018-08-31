#include "analysisresultsdialog.h"
#include "ui_analysisresultsdialog.h"

AnalysisResultsDialog::AnalysisResultsDialog(QString str, QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalysisResultsDialog)
{
    ui->setupUi(this);
    setWindowTitle(title);

    model = new QStandardItemModel;

    ui->plainTextEdit->setPlainText(str);
    setModelFromText(&str);
}

AnalysisResultsDialog::~AnalysisResultsDialog()
{
    delete ui;
}

void AnalysisResultsDialog::setModelFromText(QString *text)
{
    model->clear();
    QStringList headers, lines;
    headers << "Property" << "Value" << "Unit" << "Condition";
    lines = text->split("\n");

    if (lines.size() <= 0)
        return;

    // Create rows with two columns each, first is the name and the second is the value
    for (int i = 0; i < lines.size(); i++) {
        QList<QStandardItem*> row;
        int elements = lines.at(i).split("\t").size();

        // Populate the model properly, according the number of data of each line. Data is delimited by tabs (\t).
        // If only one element, set it up in the model as a section title, two elements are values without units,
        // three elements are data values with dimension and four elements are conditions or comparisons.
        if (elements <= 0 || elements > 4) {
            continue;
        } else {
            QStandardItem *itemCol1 = new QStandardItem(lines.at(i).split("\t").at(0));  // Column 1
            QStandardItem *itemCol2 = new QStandardItem("");  // Column 2
            QStandardItem *itemCol3 = new QStandardItem("");  // Column 3
            QStandardItem *itemCol4 = new QStandardItem("");  // Column 4
            if (elements > 1) {
                itemCol2->setText(lines.at(i).split("\t").at(1));
                if (elements > 2) {
                    itemCol3->setText(lines.at(i).split("\t").at(2));
                    if (elements > 3) {
                        itemCol4->setText(lines.at(i).split("\t").at(3));
                    }
                }
            }
            itemCol1->setEditable(false);
            itemCol2->setEditable(false);
            itemCol3->setEditable(false);
            itemCol4->setEditable(false);
            itemCol2->setTextAlignment(Qt::AlignRight);
            row.append(itemCol1);
            row.append(itemCol2);
            row.append(itemCol3);
            row.append(itemCol4);
        }

        // Append the row to the model
        model->appendRow(row);

        ui->treeView->setModel(model);
        ui->treeView->setAlternatingRowColors(true);
        // Adjust the colum size to their content
        ui->treeView->resizeColumnToContents(0);



//        if (elements == 1) {
//            QStandardItem *item = new QStandardItem(lines.at(i));
//            item->setEditable(false);
//            row.append((item));
//        } else if (elements == 2) {
//            QStandardItem *itemCol1 = new QStandardItem(lines.at(i).split("\t").at(0));  // Column 1
//            QStandardItem *itemCol2 = new QStandardItem(lines.at(i).split("\t").at(1));  // Column 2
//            itemCol1->setEditable(false);
//            itemCol2->setEditable(false);
//            row.append(itemCol1);
//            row.append(itemCol2);
//        } else if (elements == 3) {
//            QStandardItem *itemCol1 = new QStandardItem(lines.at(i).split("\t").at(0));  // Column 1
//            QStandardItem *itemCol2 = new QStandardItem(lines.at(i).split("\t").at(1));  // Column 2
//            QStandardItem *itemCol3 = new QStandardItem(lines.at(i).split("\t").at(2));  // Column 3
//            itemCol1->setEditable(false);
//            itemCol2->setEditable(false);
//            itemCol3->setEditable(false);
//            row.append(itemCol1);
//            row.append(itemCol2);
//            row.append(itemCol3);
//        } else if (elements == 4) {
//            QStandardItem *itemCol1 = new QStandardItem(lines.at(i).split("\t").at(0));  // Column 1
//            QStandardItem *itemCol2 = new QStandardItem(lines.at(i).split("\t").at(1));  // Column 2
//            QStandardItem *itemCol3 = new QStandardItem(lines.at(i).split("\t").at(2));  // Column 3
//            QStandardItem *itemCol4 = new QStandardItem(lines.at(i).split("\t").at(3));  // Column 4
//            itemCol1->setEditable(false);
//            itemCol2->setEditable(false);
//            itemCol3->setEditable(false);
//            itemCol4->setEditable(false);
//            row.append(itemCol1);
//            row.append(itemCol2);
//            row.append(itemCol3);
//            row.append(itemCol4);
//        } else {
//            continue;
//        }

    }
    model->setHorizontalHeaderLabels(headers);
}
