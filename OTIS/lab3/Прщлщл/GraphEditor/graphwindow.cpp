#include "graphwindow.h"
#include "ui_graphwindow.h"
#include <QMainWindow>
#include <QMenu>
#include <QDebug>

GraphWindow::GraphWindow(QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);

    // Заполняем комбо-бокс доступными масштабами
    QStringList scales = {"1.0", "0.5", "1.5", "2.0"};
    ui->comboBox->addItems(scales);

    // Устанавливаем масштаб 1.0x при запуске
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    graphName = name;
    bool ok;
    QStringList items;
    items << "Orient" << "Unorient";
    while (!ok){
    key = QInputDialog::getItem(this, "Укажите вид графа", "", items, 0, false, &ok);
    }
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
}

GraphWindow::~GraphWindow()
{
    delete ui;
}

void GraphWindow::on_addVertPushButton_clicked()
{
    QString num = QString::number(verts.count());
    QString vertexName = QInputDialog::getText(this, "Введите название вершины", "Название:",QLineEdit::Normal,num);
    if (!vertexName.isEmpty()) {
    Vertice vert;
        // Создать вершину
        QGraphicsEllipseItem* vertex = new QGraphicsEllipseItem(0, 0, 25, 25);
        vertex->setData(0,"vert");
        vertex->setData(1, verts.count());
        vertex->setPos(rand()%200, rand()%200); // Пример начальной позиции
        vertex->setFlag(QGraphicsItem::ItemIsMovable);
        vertex->setFlag(QGraphicsItem::ItemIsSelectable);
        vertex->setBrush(QBrush(Qt::white));
        vertex->setZValue(1);
        // Создать текст
        QGraphicsTextItem* textItem = new QGraphicsTextItem(vertexName, vertex);

        // Получить границы текста
        QRectF textRect = textItem->boundingRect();

        // Установить позицию текста в центр вершины
        qreal xPos = (vertex->boundingRect().width() - textRect.width()) / 2.0;
        qreal yPos = (vertex->boundingRect().height() - textRect.height()) / 2.0;
        textItem->setPos(xPos, yPos);


        // Добавить вершину на сцену
        ui->graphicsView->scene()->addItem(vertex);
        vert.textvert = textItem;
        vert.vert = vertex;
        verts.append(vert);
    }
}
void GraphWindow::on_addEdgePushButton_clicked()
{

    // Подготовка списка названий вершин
    QStringList vertexNames;
    for (const Vertice& vert : verts) {
        QGraphicsTextItem* textItem = vert.textvert;
        if (textItem) {
            vertexNames.append(textItem->toPlainText());
        }
    }

    // Вывод списка вершин и предложение выбрать первую вершину
    bool ok;
    QString selectedVertexName1 = QInputDialog::getItem(this, "Выбор первой вершины", "Выберите первую вершину для создания ребра:", vertexNames, 0, false, &ok);

    if (!ok) {
        // Пользователь отменил операцию
        return;
    }

    // Поиск соответствующей вершины по выбранному имени
    QGraphicsEllipseItem* selectedVertex1 = nullptr;
    for (const Vertice& vert : verts) {
        QGraphicsTextItem* textItem = vert.textvert;
        if (textItem && textItem->toPlainText() == selectedVertexName1) {
            selectedVertex1 = vert.vert;
            break;
        }
    }

    if (!selectedVertex1) {
        QMessageBox::warning(this, "Ошибка", "Не удалось найти выбранную первую вершину.");
        return;
    }

    // Вывод списка вершин и предложение выбрать вторую вершину
    QString selectedVertexName2 = QInputDialog::getItem(this, "Выбор второй вершины", "Выберите вторую вершину для создания ребра:", vertexNames, 0, false, &ok);

    if (!ok) {
        // Пользователь отменил операцию
        return;
    }

    // Поиск соответствующей вершины по выбранному имени
    QGraphicsEllipseItem* selectedVertex2 = nullptr;
    for (const Vertice& vert : verts) {
        QGraphicsTextItem* textItem = vert.textvert;
        if (textItem && textItem->toPlainText() == selectedVertexName2) {
            selectedVertex2 = vert.vert;
            break;
        }
    }

    if (!selectedVertex2) {
        QMessageBox::warning(this, "Ошибка", "Не удалось найти выбранную вторую вершину.");
        return;
    }

    // Если выбрана одна и та же вершина, создаем петлю
    if (selectedVertex1 == selectedVertex2) {
        QGraphicsEllipseItem* loop = new QGraphicsEllipseItem;
        //loop->setFlag(QGraphicsItem::ItemIsMovable);
        loop->setData(0,"loop");
        loop->setData(1,selectedVertex1->data(1));
        loop->setFlag(QGraphicsItem::ItemIsSelectable);
        //loop->setRect();

        // Добавление петли на сцену
        ui->graphicsView->scene()->addItem(loop);

        Loop loop1;
        loop1.vertex1 = selectedVertex1;
        loop1.loop = loop;

        loops.append(loop1);

        connect(ui->graphicsView->scene(), &QGraphicsScene::changed, this, [=]() {
            updateLoopPosition(loop, selectedVertex1);
        });
        return;
    }

    // Создание ребра
    if (key == "Unorient"){
    QGraphicsLineItem* edge = new QGraphicsLineItem;
    //edge->setFlag(QGraphicsItem::ItemIsMovable);

    edge->setFlag(QGraphicsItem::ItemIsSelectable);
    edge->setLine(selectedVertex1->scenePos().x() + 25, selectedVertex1->scenePos().y() + 25,
                  selectedVertex2->scenePos().x() + 25, selectedVertex2->scenePos().y() + 25);
    edge->setData(0,"edge");
    edge->setData(1,selectedVertex1->data(1));
    edge->setData(2,selectedVertex2->data(1));
    // Добавление ребра на сцену
    ui->graphicsView->scene()->addItem(edge);
    Edge edge1;
    edge1.vertex1 = selectedVertex1;
    edge1.vertex2 = selectedVertex2;
    edge1.edgeItem = edge;
    edges.append(edge1);

    // Соединение сигнала изменения сцены с обновлением позиции ребра
    connect(ui->graphicsView->scene(), &QGraphicsScene::changed, this, [=]() {
        updateEdgePosition();
    });
    } else {
    // Создание ориентированного ребра
    QGraphicsLineItem* directedEdge = new QGraphicsLineItem;
    directedEdge->setFlag(QGraphicsItem::ItemIsSelectable);
    directedEdge->setData(1,selectedVertex1->data(1));
    directedEdge->setData(2,selectedVertex2->data(1));

    directedEdge->setLine(selectedVertex1->scenePos().x() + 25, selectedVertex1->scenePos().y() + 25,
                          selectedVertex2->scenePos().x() + 25, selectedVertex2->scenePos().y() + 25);
    directedEdge->setData(0,"edge");
    // Добавление ориентированного ребра на сцену
    ui->graphicsView->scene()->addItem(directedEdge);

    // Draw an arrowhead at the end of the line
    double arrowSize = 10.0;
    double angle = atan2(directedEdge->line().dy(), directedEdge->line().dx());
    double x1 = directedEdge->line().x2() - arrowSize * cos(angle - M_PI / 6);
    double y1 = directedEdge->line().y2() - arrowSize * sin(angle - M_PI / 6);
    double x2 = directedEdge->line().x2() - arrowSize * cos(angle + M_PI / 6);
    double y2 = directedEdge->line().y2() - arrowSize * sin(angle + M_PI / 6);

    QGraphicsPolygonItem* arrowhead = new QGraphicsPolygonItem(QPolygonF() << directedEdge->line().p2() << QPointF(x1, y1) << QPointF(x2, y2));
    arrowhead->setBrush(Qt::black);
    ui->graphicsView->scene()->addItem(arrowhead);
    OrEdge directedEdge1;
    directedEdge1.vertex1 = selectedVertex1;
    directedEdge1.vertex2 = selectedVertex2;
    directedEdge1.edgeItem = directedEdge;
    directedEdge1.arrowItem = arrowhead;
    or_edges.append(directedEdge1);
    connect(ui->graphicsView->scene(), &QGraphicsScene::changed, this, [=]() {
        updateOrEdgePosition();
    });
    }

}

void GraphWindow::updateEdgePosition()
{
    for (const Edge& edge : edges) {
        QGraphicsEllipseItem* vertex1 = edge.vertex1;
        QGraphicsEllipseItem* vertex2 = edge.vertex2;
        QGraphicsLineItem* edgeItem = edge.edgeItem;

        if (vertex1 && vertex2 && edgeItem) {
            edgeItem->setLine(vertex1->scenePos().x() + vertex1->boundingRect().width() / 2,
                          vertex1->scenePos().y() + vertex1->boundingRect().height() / 2,
                          vertex2->scenePos().x() + vertex2->boundingRect().width() / 2,
                          vertex2->scenePos().y() + vertex2->boundingRect().height() / 2);
        }
    }
}

void GraphWindow::updateOrEdgePosition() {
    for (OrEdge& directedEdge1 : or_edges) {
        QGraphicsEllipseItem* vertex1 = directedEdge1.vertex1;
        QGraphicsEllipseItem* vertex2 = directedEdge1.vertex2;
        QGraphicsLineItem* edgeItem = directedEdge1.edgeItem;
        QGraphicsPolygonItem* arrowItem = directedEdge1.arrowItem;

        if (vertex1 && vertex2 && edgeItem && arrowItem) {
            // Update the position of the edge
            edgeItem->setLine(vertex1->scenePos().x() + vertex1->boundingRect().width() / 2,
                              vertex1->scenePos().y() + vertex1->boundingRect().height() / 2,
                              vertex2->scenePos().x() + vertex2->boundingRect().width() / 2,
                              vertex2->scenePos().y() + vertex2->boundingRect().height() / 2);

            // Update the position and shape of the arrowhead
            double arrowSize = 10.0;
            double angle = atan2(edgeItem->line().dy(), edgeItem->line().dx());
            double x1 = edgeItem->line().x2() - arrowSize * cos(angle);
            double y1 = edgeItem->line().y2() - arrowSize * sin(angle);

            // Set the position of the arrowhead
            arrowItem->setPos(QPointF(x1, y1));

            // Calculate the coordinates for the other two points of the triangle
            double x2 = x1 - arrowSize * cos(angle - M_PI / 6);
            double y2 = y1 - arrowSize * sin(angle - M_PI / 6);
            double x3 = x1 - arrowSize * cos(angle + M_PI / 6);
            double y3 = y1 - arrowSize * sin(angle + M_PI / 6);

            // Update the shape of the arrowhead
            QPolygonF arrowheadPolygon(QPolygonF() << QPointF(0, 0) << QPointF(x2 - x1, y2 - y1) << QPointF(x3 - x1, y3 - y1));
            arrowItem->setPolygon(arrowheadPolygon);
        }
    }
}

void GraphWindow::updateLoopPosition(QGraphicsEllipseItem* loop, QGraphicsEllipseItem* vertex)
{
    // Устанавливаем центр и размер круга для петли
    loop->setRect(vertex->scenePos().x()-12, vertex->scenePos().y()-12, 25, 25);  // Пример размеров для создания петли
}

void GraphWindow::on_renamePushButton_clicked()
{
    // Подготовка списка названий вершин
    QStringList vertexNames;
    for (const Vertice& vert : verts) {
        QGraphicsTextItem* textItem = vert.textvert;
        if (textItem) {
            vertexNames.append(textItem->toPlainText());
        }
    }

    // Вывод списка вершин и предложение выбрать первую вершину
    bool ok;
    QString selectedVertexName1 = QInputDialog::getItem(this, "Выбор вершины для переименования", "Выберите вершину для изменения имени:", vertexNames, 0, false, &ok);

    if (!ok) {
        // Пользователь отменил операцию
        return;
    }

    // Поиск соответствующей вершины по выбранному имени
    QGraphicsEllipseItem* selectedVertex1 = nullptr;
    for (const Vertice& vert : verts) {
        QGraphicsTextItem* textItem = vert.textvert;
        if (textItem && textItem->toPlainText() == selectedVertexName1) {
            selectedVertex1 = vert.vert;
             QString newName = QInputDialog::getText(this,"Введите новое имя вершины","Имя:");
            if (!newName.isEmpty()){
            textItem->setPlainText(newName);
             QRectF textRect = textItem->boundingRect();
            // Установить позицию текста в центр вершины
            qreal xPos = (vert.vert->boundingRect().width() - textRect.width()) / 2.0;
            qreal yPos = (vert.vert->boundingRect().height() - textRect.height()) / 2.0;
            textItem->setPos(xPos, yPos);
            break;
            } else {QMessageBox::warning(this, "Ошибка", "Пустое название");}

        }
    }

    if (!selectedVertex1) {
        QMessageBox::warning(this, "Ошибка", "Не удалось найти выбранную первую вершину.");
        return;
    }
}

void GraphWindow::on_deletePushButton_clicked()
{
    // Disconnect signals to avoid unwanted updates during deletion
    disconnect(ui->graphicsView->scene(), &QGraphicsScene::changed, this, nullptr);

    // Collect items to be deleted
    QList<QGraphicsItem *> itemsToDelete;

    QList<QGraphicsItem *> selectedItems = ui->graphicsView->scene()->selectedItems();

    for (QGraphicsItem *item : selectedItems) {
        // Check if the item is an edge (QGraphicsLineItem) and disconnect its update slot
        if (QGraphicsLineItem* edgeItem = dynamic_cast<QGraphicsLineItem*>(item)) {
            edges.erase(std::remove_if(edges.begin(), edges.end(),
                                       [edgeItem](const Edge& edge) { return edge.edgeItem == edgeItem; }),
                        edges.end());

            itemsToDelete.append(edgeItem);

            for (const OrEdge& or_edge : or_edges){
            if (or_edge.edgeItem == edgeItem){
                itemsToDelete.append(or_edge.arrowItem);
            }
            }

            or_edges.erase(std::remove_if(or_edges.begin(), or_edges.end(),
                                       [edgeItem](const OrEdge& edge) { return edge.edgeItem == edgeItem; }),
                        or_edges.end());
        }
        // Check if the item is a vertex (QGraphicsEllipseItem)
        else if (QGraphicsEllipseItem* vertex = dynamic_cast<QGraphicsEllipseItem*>(item)) {
            // Remove connected edges
            QList<QGraphicsLineItem*> connectedEdges;
            for (const Edge& edge : edges) {
                if (edge.vertex1 == vertex || edge.vertex2 == vertex) {
                    connectedEdges.append(edge.edgeItem);
                }
            }
            // Remove connected edges from the list and the scene
            for (QGraphicsLineItem* connectedEdge : connectedEdges) {
                edges.erase(std::remove_if(edges.begin(), edges.end(),
                                           [edgeItem](const Edge& edge) { return edge.edgeItem == edgeItem; }),
                            edges.end());
                ui->graphicsView->scene()->removeItem(connectedEdge);
                itemsToDelete.append(connectedEdge);
            }

            // Remove connected directed edges
            QList<QGraphicsLineItem*> connectedOrEdges;
            QList<QGraphicsPolygonItem*> connectedPolygon;
            for (const OrEdge& edge : or_edges) {
                if (edge.vertex1 == vertex || edge.vertex2 == vertex) {
                    connectedOrEdges.append(edge.edgeItem);
                    connectedPolygon.append(edge.arrowItem);
                }
            }

            // Remove connected edges from the list and the scene
            for (QGraphicsLineItem*& connectedOrEdge : connectedOrEdges) {
                for (const OrEdge& or_edge : or_edges){
                    if (or_edge.edgeItem == connectedOrEdge){
                        itemsToDelete.append(or_edge.arrowItem);
                    }
                }
                or_edges.erase(std::remove_if(or_edges.begin(), or_edges.end(),
                                              [connectedOrEdge](const OrEdge& edge) { return edge.edgeItem == connectedOrEdge; }),
                               or_edges.end());
                ui->graphicsView->scene()->removeItem(connectedOrEdge);
                itemsToDelete.append(connectedOrEdge);
            }

            // Remove connected loops
            QList<QGraphicsEllipseItem*> connectedLoops;
            for (const Loop& connectedLoop : loops) {
                if (connectedLoop.vertex1 == vertex) {
                    connectedLoops.append(connectedLoop.loop);
                }
            }

            // Remove connected loops from the list and the scene
            for (QGraphicsEllipseItem* connectedLoop : connectedLoops) {
                loops.removeAll(Loop{vertex,connectedLoop});
                ui->graphicsView->scene()->removeItem(connectedLoop);
                itemsToDelete.append(connectedLoop);
            }

            // Remove the vertex from the list
            verts.erase(std::remove_if(verts.begin(), verts.end(),
                                          [vertex](const Vertice& vert) { return vert.vert == vertex; }),
                           verts.end());

            // Remove the vertex from the scene
            ui->graphicsView->scene()->removeItem(vertex);
            itemsToDelete.append(vertex);  // Store the item for deletion
        }
        // ... (other item types remain the same)
    }

    // Delete items after the iteration
    for (QGraphicsItem *item : itemsToDelete) {
        delete item;
    }

}

void GraphWindow::setColor(const QColor &color)
{
    // Получаем список всех выделенных элементов
    QList<QGraphicsItem*> selectedItems = scene->selectedItems();
    // Идентификация и изменение цвета для каждого выделенного элемента
    for (QGraphicsItem *item : selectedItems) {
        QString ItemType = item->data(0).toString();
        // Проверка, является ли элемент вершиной
        QGraphicsEllipseItem *vertexItem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
        if (vertexItem) {
            if (ItemType == "vert"){
                vertexItem->setBrush(QBrush(color));
            } else if (ItemType == "loop"){
                vertexItem->setPen(QPen(color, 2));
            }
        }

        // Проверка, является ли элемент ребром
        QGraphicsLineItem *edgeItem = qgraphicsitem_cast<QGraphicsLineItem*>(item);
        if (edgeItem) {
            edgeItem->setPen(QPen(color, 2)); // Пример установки цвета для ребра
        }
    }
}

void GraphWindow::on_colorPushButton_clicked()
{
QMenu* colorMenu = new QMenu(this);

    // Добавляем цветные квадраты как иконки к действиям
    addActionWithColor(colorMenu, "Красный", Qt::red);
    addActionWithColor(colorMenu, "Зеленый", Qt::green);
    addActionWithColor(colorMenu, "Синий", Qt::blue);
    addActionWithColor(colorMenu, "Желтый", Qt::yellow);
    addActionWithColor(colorMenu, "Фиолетовый", Qt::magenta);
    addActionWithColor(colorMenu, "Голубой", Qt::cyan);
    addActionWithColor(colorMenu, "Серый", Qt::gray);
    addActionWithColor(colorMenu, "Черный", Qt::black);
    addActionWithColor(colorMenu, "Белый", Qt::white);

    // Отображаем меню относительно глобальной позиции кнопки
    colorMenu->exec(ui->colorPushButton->mapToGlobal(QPoint(0, ui->colorPushButton->height())));
}

void GraphWindow::addActionWithColor(QMenu* menu, const QString& colorName, const QColor& color)
{
    QPixmap colorIcon(20, 20);
    colorIcon.fill(color);
    QAction* action = menu->addAction(QIcon(colorIcon), colorName);

    connect(action, &QAction::triggered, this, [this, color]() {
        setColor(color);
    });
}

void GraphWindow::on_comboBox_currentIndexChanged(int index)
{
    qreal scaleFactor = ui->comboBox->itemText(index).toDouble();
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, false);
    ui->graphicsView->resetTransform();
    ui->graphicsView->scale(scaleFactor, scaleFactor);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

