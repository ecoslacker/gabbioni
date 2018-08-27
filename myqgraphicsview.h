#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QPointF>

class MyQGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyQGraphicsView(QWidget *parent = nullptr);

signals:
    //void coordinatesChanged(double x, double y);
    void coordinatesChanged();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // MYQGRAPHICSVIEW_H
