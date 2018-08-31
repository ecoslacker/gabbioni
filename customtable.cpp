#include "customtable.h"
#include <QDebug>
#include <QClipboard>
#include <QApplication>
#include <QTableWidgetItem>

CustomTable::CustomTable(QWidget *parent)
{

}

void CustomTable::keyPressEvent(QKeyEvent *event)
{
    // Override the paste key sequence of this event
    if (event->matches(QKeySequence::Paste)) {

        // Get the data from clipboard, separate rows by '\n' and columns by '\t'
        QClipboard *clipboard = QGuiApplication::clipboard();
        QString text = clipboard->text();
        QStringList rows = text.split('\n', QString::SkipEmptyParts);

        // Adjust the rows to match the data from clipboard
        this->setRowCount(rows.count());

        // Paste only the first two columns of data into the table
        for (int i=0; i < rows.count(); i++) {
            QStringList columns = rows.at(i).split('\t');
            if (columns.count() == 1) {
                // Set value in case there is only one column. WARNING! User should provide the other value!
                QTableWidgetItem *item = new QTableWidgetItem(columns.at(0));
                item->setTextAlignment(Qt::AlignRight);
                this->setItem(i, 0, item);
            } else if (columns.count() > 1) {
                // Set values for two columns
                QTableWidgetItem *item1 = new QTableWidgetItem(columns.at(0));
                QTableWidgetItem *item2 = new QTableWidgetItem(columns.at(1));

                item1->setTextAlignment(Qt::AlignRight);
                item2->setTextAlignment(Qt::AlignRight);

                this->setItem(i, 0, item1);
                this->setItem(i, 1, item2);
            }
        }
    } else if (event->matches(QKeySequence::Copy)) {
        // Override the copy key sequence of this event
        QString text;
        QTableWidgetSelectionRange range = selectedRanges().first();

        // Separate rows by '\n' and columns by '\t'
        for (int i = 0; i < range.rowCount(); i++) {
            if (i > 0)
                text += "\n";
            for (int j = 0; j < range.columnCount(); j++) {
                if (j > 0)
                    text += "\t";
                QTableWidgetItem *item = this->item(range.topRow() + i, range.leftColumn() + j);
                text += item->text().trimmed();
            }
        }

        qDebug() << "Copied text:" << text;

        // Set the data from table (selected range) to clipboard
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->clear();
        clipboard->setText(text);

    } else {
        // Pass the key event to the parent to use its original functionality (move, insert, delete, etc.)
        QTableWidget::keyPressEvent(event);
    }
}
