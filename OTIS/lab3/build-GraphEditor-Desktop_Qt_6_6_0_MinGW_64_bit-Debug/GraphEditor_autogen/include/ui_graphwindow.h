/********************************************************************************
** Form generated from reading UI file 'graphwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHWINDOW_H
#define UI_GRAPHWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GraphWindow
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *addVertPushButton;
    QToolButton *colorToolButton;
    QPushButton *addEdgePushButton;
    QPushButton *deletePushButton;
    QGraphicsView *graphicsView;

    void setupUi(QWidget *GraphWindow)
    {
        if (GraphWindow->objectName().isEmpty())
            GraphWindow->setObjectName("GraphWindow");
        GraphWindow->resize(467, 367);
        gridLayout = new QGridLayout(GraphWindow);
        gridLayout->setObjectName("gridLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        addVertPushButton = new QPushButton(GraphWindow);
        addVertPushButton->setObjectName("addVertPushButton");

        verticalLayout->addWidget(addVertPushButton);

        colorToolButton = new QToolButton(GraphWindow);
        colorToolButton->setObjectName("colorToolButton");
        colorToolButton->setAcceptDrops(false);
        colorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
        colorToolButton->setArrowType(Qt::NoArrow);

        verticalLayout->addWidget(colorToolButton);

        addEdgePushButton = new QPushButton(GraphWindow);
        addEdgePushButton->setObjectName("addEdgePushButton");

        verticalLayout->addWidget(addEdgePushButton);

        deletePushButton = new QPushButton(GraphWindow);
        deletePushButton->setObjectName("deletePushButton");

        verticalLayout->addWidget(deletePushButton);


        gridLayout->addLayout(verticalLayout, 0, 1, 1, 1);

        graphicsView = new QGraphicsView(GraphWindow);
        graphicsView->setObjectName("graphicsView");

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);


        retranslateUi(GraphWindow);

        QMetaObject::connectSlotsByName(GraphWindow);
    } // setupUi

    void retranslateUi(QWidget *GraphWindow)
    {
        GraphWindow->setWindowTitle(QCoreApplication::translate("GraphWindow", "Form", nullptr));
        addVertPushButton->setText(QCoreApplication::translate("GraphWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\262\320\265\321\200\321\210\320\270\320\275\321\203", nullptr));
        colorToolButton->setText(QCoreApplication::translate("GraphWindow", "\320\222\321\213\320\261\320\276\321\200 \321\206\320\262\320\265\321\202\320\260", nullptr));
        addEdgePushButton->setText(QCoreApplication::translate("GraphWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\200\320\265\320\261\321\200\320\276", nullptr));
        deletePushButton->setText(QCoreApplication::translate("GraphWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \321\215\320\273\320\265\320\274\320\265\320\275\321\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GraphWindow: public Ui_GraphWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHWINDOW_H
