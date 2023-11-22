#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QToolButton>
#include <QPixmap>
#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QInputDialog>
#include <cmath>
#include <QMessageBox>
#include <QList>
#include <QGraphicsPathItem>>


namespace Ui {
class GraphWindow;
}

class GraphWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow();

private slots:
    void on_addVertPushButton_clicked();
    void on_addEdgePushButton_clicked();
    void updateEdgePosition(QGraphicsLineItem* edge, QGraphicsEllipseItem* vertex1, QGraphicsEllipseItem* vertex2);
    void updateLoopPosition(QGraphicsEllipseItem* loop, QGraphicsEllipseItem* vertex);

private:
    Ui::GraphWindow *ui;
    QGraphicsScene *scene;
    QList<QGraphicsEllipseItem*> vertices;
    QList<QGraphicsTextItem*> textvert;
    QList<QGraphicsLineItem*> edges;
    QList<QGraphicsEllipseItem*> loops;
};



#endif // GRAPHWINDOW_H
