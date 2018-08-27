#ifndef CUSTOMTABLENOPASTE_H
#define CUSTOMTABLENOPASTE_H
#include <QTableWidget>
#include <QKeyEvent>

class CustomTableNoPaste : public QTableWidget
{
    Q_OBJECT
public:
    explicit CustomTableNoPaste(QWidget *parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // CUSTOMTABLENOPASTE_H
