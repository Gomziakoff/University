/********************************************************************************
** Form generated from reading UI file 'graph.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPH_H
#define UI_GRAPH_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_graph
{
public:
    QAction *save;
    QAction *load;
    QAction *addE;
    QAction *renameV;
    QAction *deleteV;
    QAction *information;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QGraphicsView *graphicsView;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *graph)
    {
        if (graph->objectName().isEmpty())
            graph->setObjectName("graph");
        graph->resize(800, 600);
        save = new QAction(graph);
        save->setObjectName("save");
        load = new QAction(graph);
        load->setObjectName("load");
        addE = new QAction(graph);
        addE->setObjectName("addE");
        renameV = new QAction(graph);
        renameV->setObjectName("renameV");
        deleteV = new QAction(graph);
        deleteV->setObjectName("deleteV");
        information = new QAction(graph);
        information->setObjectName("information");
        centralwidget = new QWidget(graph);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");

        verticalLayout->addWidget(graphicsView);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        graph->setCentralWidget(centralwidget);
        menubar = new QMenuBar(graph);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName("menu_2");
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName("menu_3");
        graph->setMenuBar(menubar);
        statusbar = new QStatusBar(graph);
        statusbar->setObjectName("statusbar");
        graph->setStatusBar(statusbar);

        menubar->addAction(menu_3->menuAction());
        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menu->addAction(addE);
        menu->addAction(renameV);
        menu->addSeparator();
        menu->addAction(deleteV);
        menu_2->addAction(information);
        menu_3->addAction(save);
        menu_3->addAction(load);

        retranslateUi(graph);

        QMetaObject::connectSlotsByName(graph);
    } // setupUi

    void retranslateUi(QMainWindow *graph)
    {
        graph->setWindowTitle(QCoreApplication::translate("graph", "MainWindow", nullptr));
        save->setText(QCoreApplication::translate("graph", "\321\201\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        load->setText(QCoreApplication::translate("graph", "\320\267\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214", nullptr));
        addE->setText(QCoreApplication::translate("graph", "\320\264\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\200\320\265\320\261\321\200\320\276", nullptr));
        renameV->setText(QCoreApplication::translate("graph", "\320\237\320\265\321\200\320\265\320\270\320\274\320\265\320\275\320\276\320\262\320\260\321\202\321\214 \320\262\320\265\321\200\321\210\320\270\320\275\321\203", nullptr));
        deleteV->setText(QCoreApplication::translate("graph", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\262\320\265\321\200\321\210\320\270\320\275\321\203", nullptr));
        information->setText(QCoreApplication::translate("graph", "\320\230\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\217 \320\276 \320\263\321\200\320\260\321\204\320\265", nullptr));
        label->setText(QCoreApplication::translate("graph", "\320\224\320\273\321\217 \320\264\320\276\320\261\320\260\320\262\320\273\320\265\320\275\320\270\321\217 \320\262\320\265\321\200\321\210\320\270\320\275\321\213 \320\272\320\273\320\270\320\272\320\275\320\270\321\202\320\265 \320\277\320\276 \321\200\320\260\320\261\320\276\321\207\320\265\320\271 \320\276\320\261\320\273\320\260\321\201\321\202\320\270. \320\237\321\200\320\260\320\262\321\213\320\271 \320\272\320\273\320\270\320\272 \320\277\320\276 \320\262\321\213\320\264\320\265\320\273\320\265\320\275\320\276\320\271 \320\262\320\265\321\200\321\210\320\270\320\275\320\265 \320\264\320\273\321\217 \320\270\320\267\320\274\320\265\320\275\320\265\320\275\320\270\321\217 \321\206\320\262\320\265\321\202\320\260", nullptr));
        menu->setTitle(QCoreApplication::translate("graph", "\320\230\320\267\320\274\320\265\320\275\320\265\320\275\320\270\320\265 \320\263\321\200\320\260\321\204\320\260", nullptr));
        menu_2->setTitle(QCoreApplication::translate("graph", "\320\220\320\273\320\263\320\276\321\200\320\270\321\202\320\274\321\213", nullptr));
        menu_3->setTitle(QCoreApplication::translate("graph", "\320\244\320\260\320\271\320\273", nullptr));
    } // retranslateUi

};

namespace Ui {
    class graph: public Ui_graph {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPH_H
