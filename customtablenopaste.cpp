#include "customtablenopaste.h"
#include <QDebug>
#include <QClipboard>
#include <QApplication>
#include <QTableWidgetItem>

CustomTableNoPaste::CustomTableNoPaste(QWidget *parent)
{

}

void CustomTableNoPaste::keyPressEvent(QKeyEvent *event)
{
    // // Override the copy key sequence of this event
    if (event->matches(QKeySequence::Copy)) {
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
