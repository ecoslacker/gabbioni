#ifndef DIMENSIONDIALOG_H
#define DIMENSIONDIALOG_H

#include <layer.h>
#include <QDialog>
#include <QVector>

namespace Ui {
class DimensionDialog;
}

class DimensionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DimensionDialog(QWidget *parent = nullptr);
    ~DimensionDialog();

    QVector<Layer> *getLayers() const;
    void setLayers(QVector<Layer> *value);

    void configureTable();

private:
    Ui::DimensionDialog *ui;
    QVector<Layer> *layers;
};

#endif // DIMENSIONDIALOG_H
