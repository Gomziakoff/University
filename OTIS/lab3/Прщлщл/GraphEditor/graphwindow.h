#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include "ui_graphwindow.h"
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
#include <QGraphicsPathItem>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QMimeData>
#include <QClipboard>
#include <QIODevice>

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWindow(QString name, QWidget *parent = nullptr);
    ~GraphWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override
    {
        if (event->matches(QKeySequence::Copy)) {
            copyObjects();
        } else if (event->matches(QKeySequence::Paste)) {
            pasteObjects();
        }
    }

private slots:
    void on_addVertPushButton_clicked();
    void on_addEdgePushButton_clicked();
    void updateEdgePosition();
    void updateOrEdgePosition();
    void updateLoopPosition(QGraphicsEllipseItem* loop, QGraphicsEllipseItem* vertex);
    void setColor(const QColor &color);
    void addActionWithColor(QMenu* menu, const QString& colorName, const QColor& color);
    void on_renamePushButton_clicked();
    void on_deletePushButton_clicked();
    void on_colorPushButton_clicked();
    void on_comboBox_currentIndexChanged(int index);
private:
    Ui::GraphWindow *ui;
    QGraphicsScene *scene;



    void copyObjects() {
        QJsonArray copiedArray;

        for (QGraphicsItem* selectedItem : scene->selectedItems()) {
            if (selectedItem->data(0).toString() == "vert") {
                QJsonObject vertObj = serializeVert(selectedItem);
                copiedArray.append(vertObj);
            } else if (selectedItem->data(0).toString() == "edge") {
                QJsonObject edgeObj = serializeEdge(selectedItem);
                copiedArray.append(edgeObj);
            } // Add similar blocks for orEdges and loops
        }

        QClipboard* clipboard = QGuiApplication::clipboard();
        QJsonDocument doc(copiedArray);
        clipboard->setText(doc.toJson());
    }



    void pasteObjects() {
        QClipboard* clipboard = QGuiApplication::clipboard();
        QString clipboardText = clipboard->text();

        QJsonDocument doc = QJsonDocument::fromJson(clipboardText.toUtf8());
        QJsonArray copiedArray = doc.array();

        for (const QJsonValue& value : copiedArray) {
            QJsonObject obj = value.toObject();
            QString type = obj["type"].toString();

            if (type == "vert") {
                deserializeAndAddVert(obj);
            } else if (type == "edge") {
                deserializeAndAddEdge(obj);
            } // Add similar blocks for orEdges and loops
        }
    }

    struct Edge {
        QGraphicsEllipseItem* vertex1;
        QGraphicsEllipseItem* vertex2;
        QGraphicsLineItem* edgeItem;
        bool operator==(const Edge& other) const {
            return vertex1 == other.vertex1 && vertex2 == other.vertex2 && edgeItem == other.edgeItem;
        }

    };

    struct OrEdge {
        QGraphicsEllipseItem* vertex1;
        QGraphicsEllipseItem* vertex2;
        QGraphicsLineItem* edgeItem;
        QGraphicsPolygonItem* arrowItem;
        bool operator==(const OrEdge& other) const {
            return vertex1 == other.vertex1 && vertex2 == other.vertex2 && edgeItem == other.edgeItem && arrowItem==other.arrowItem;
        }

    };
    struct Loop {
        QGraphicsEllipseItem* vertex1;
        QGraphicsEllipseItem* loop;
        bool operator==(const Loop& other) const {
            return vertex1 == other.vertex1 && loop == other.loop;
        }

    };

    struct Vertice{
        QGraphicsEllipseItem* vert;
        QGraphicsTextItem* textvert;
    };

    QString graphName;
    QList<Vertice> verts;
    QList<Edge> edges;
    QList<Loop> loops;
    QList<OrEdge> or_edges;
    QString key;

    QList<Vertice> copiedVert;
    QList<Edge> copiedEdge;
    QList<OrEdge> copiedOrEdge;
    QList<Loop> copiedLoops;

    QJsonObject serializeVert(QGraphicsItem* item) {
        QJsonObject obj;
        obj["type"] = "vert";
        obj["text"] = item->data(1).toString(); // Assuming the text is stored in data(1)
        obj["x"] = item->scenePos().x();
        obj["y"] = item->scenePos().y();
        // Add other properties as needed
        return obj;
    }

    void deserializeAndAddVert(const QJsonObject& obj) {
        QGraphicsEllipseItem* newVertexItem = new QGraphicsEllipseItem(0, 0, 25, 25);
        QGraphicsTextItem* newTextItem = new QGraphicsTextItem(obj["text"].toString(), newVertexItem);

        qreal x = obj["x"].toDouble();
        qreal y = obj["y"].toDouble();
        newVertexItem->setPos(x, y);

        ui->graphicsView->scene()->addItem(newVertexItem);

        Vertice vert;
        vert.textvert = newTextItem;
        vert.vert = newVertexItem;
        verts.append(vert);
    }

    QJsonObject serializeObject(const OrEdge& orEdge) {
        QJsonObject obj;
        obj["type"] = "orEdge";
        // Add other properties as needed
        return obj;
    }

    QJsonObject serializeObject(const Loop& loop) {
        QJsonObject obj;
        obj["type"] = "loop";
        // Add other properties as needed
        return obj;
    }

    Vertice deserializeVert(const QJsonObject& obj) {
        QGraphicsEllipseItem* newVertexItem = new QGraphicsEllipseItem(0, 0, 25, 25);
        QGraphicsTextItem* newTextItem = new QGraphicsTextItem(obj["text"].toString(), newVertexItem);

        // Set other properties based on your actual data structure
        // For example, you might need to set the position of the newVertexItem:
        qreal x = obj["x"].toDouble(); // Assuming x is stored in the JSON object
        qreal y = obj["y"].toDouble(); // Assuming y is stored in the JSON object
        newVertexItem->setPos(x, y);

        // Additional properties...

        // Add the new vertex to the scene or the list of items
        ui->graphicsView->scene()->addItem(newVertexItem);

        Vertice vert;
        vert.textvert = newTextItem;
        vert.vert = newVertexItem;

        return vert;
    }
};



#endif // GRAPHWINDOW_H
