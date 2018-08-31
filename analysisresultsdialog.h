#ifndef ANALYSISRESULTSDIALOG_H
#define ANALYSISRESULTSDIALOG_H

#include <QDialog>
#include <QStandardItem>
#include <QStandardItemModel>

namespace Ui {
class AnalysisResultsDialog;
}

class AnalysisResultsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AnalysisResultsDialog(QString str="", QString title="Default", QWidget *parent = nullptr);
    ~AnalysisResultsDialog();

private:
    Ui::AnalysisResultsDialog *ui;

    QStandardItemModel *model;

private slots:
    void setModelFromText(QString *text);
};

#endif // ANALYSISRESULTSDIALOG_H
