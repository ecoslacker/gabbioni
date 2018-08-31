#ifndef CUSTOMTABLE_H
#define CUSTOMTABLE_H
#include <QTableWidget>
#include <QKeyEvent>

class CustomTable : public QTableWidget
{
    Q_OBJECT
public:
    CustomTable(QWidget *parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // CUSTOMTABLE_H
