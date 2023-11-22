#include "graphwindow.h"
#include "ui_graphwindow.h"
#include <QMainWindow>



GraphWindow::GraphWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);

}

GraphWindow::~GraphWindow()
{
    delete ui;
}

void GraphWindow::on_addVertPushButton_clicked()
{
    QString vertexName = QInputDialog::getText(this, "Введите название вершины", "Название:");
    if (!vertexName.isEmpty()) {
        // Создать вершину
        QGraphicsEllipseItem* vertex = new QGraphicsEllipseItem(0, 0, 25, 25);
        vertex->setPos(25, 25); // Пример начальной позиции
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

        textvert.append(textItem);

        // Добавить вершину на сцену
        ui->graphicsView->scene()->addItem(vertex);
        vertices.append(vertex);
    }
}
void GraphWindow::on_addEdgePushButton_clicked()
{

    // Подготовка списка названий вершин
    QStringList vertexNames;
    for (QGraphicsEllipseItem* vertex : vertices) {
        QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(vertex->childItems().first());
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
    for (QGraphicsEllipseItem* vertex : vertices) {
        QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(vertex->childItems().first());
        if (textItem && textItem->toPlainText() == selectedVertexName1) {
            selectedVertex1 = vertex;
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
    for (QGraphicsEllipseItem* vertex : vertices) {
        QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(vertex->childItems().first());
        if (textItem && textItem->toPlainText() == selectedVertexName2) {
            selectedVertex2 = vertex;
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
        loop->setFlag(QGraphicsItem::ItemIsSelectable);
        //loop->setRect();

        // Добавление петли на сцену
        ui->graphicsView->scene()->addItem(loop);
        //updateLoopPosition(loop, selectedVertex1);
        loops.append(loop);
        connect(ui->graphicsView->scene(), &QGraphicsScene::changed, this, [=]() {
            updateLoopPosition(loop, selectedVertex1);
        });
        return;
    }

    // Создание ребра
    QGraphicsLineItem* edge = new QGraphicsLineItem;
    //edge->setFlag(QGraphicsItem::ItemIsMovable);
    edge->setFlag(QGraphicsItem::ItemIsSelectable);
    edge->setLine(selectedVertex1->scenePos().x() + 25, selectedVertex1->scenePos().y() + 25,
                  selectedVertex2->scenePos().x() + 25, selectedVertex2->scenePos().y() + 25);

    // Добавление ребра на сцену
    ui->graphicsView->scene()->addItem(edge);
    edges.append(edge);

    // Соединение сигнала изменения сцены с обновлением позиции ребра
    connect(ui->graphicsView->scene(), &QGraphicsScene::changed, this, [=]() {
        updateEdgePosition(edge, selectedVertex1, selectedVertex2);
    });
}


void GraphWindow::updateEdgePosition(QGraphicsLineItem* edge, QGraphicsEllipseItem* vertex1, QGraphicsEllipseItem* vertex2)
{
    // Обновление позиции ребра между двумя вершинами
    edge->setLine(vertex1->scenePos().x() + vertex1->boundingRect().width() / 2,
                  vertex1->scenePos().y() + vertex1->boundingRect().height() / 2,
                  vertex2->scenePos().x() + vertex2->boundingRect().width() / 2,
                  vertex2->scenePos().y() + vertex2->boundingRect().height() / 2);
}

void GraphWindow::updateLoopPosition(QGraphicsEllipseItem* loop, QGraphicsEllipseItem* vertex)
{
    // Устанавливаем центр и размер круга для петли
    loop->setRect(vertex->scenePos().x()-12, vertex->scenePos().y()-12, 25, 25);  // Пример размеров для создания петли
}

