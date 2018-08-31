#include "myqgraphicsview.h"

MyQGraphicsView::MyQGraphicsView(QWidget *parent)
{
    // Initialize
}

void MyQGraphicsView::mouseMoveEvent(QMouseEvent *event)
{

//    // WARNING: This will send the coordinates of the mouse over the GraphicsView, NOT FROM THE SCENE!
//    double x_coord = event->pos().x();
//    double y_coord = event->pos().y();

//    //qDebug() << "Coordinates: " << x_coord << ", " << y_coord;
//    update();

//    QGraphicsView::mouseMoveEvent(event);
//    emit coordinatesChanged(x_coord, y_coord);

    // Emit a signal when the mouse moves in the GraphicsView
    QGraphicsView::mouseMoveEvent(event);
    emit coordinatesChanged();
}
