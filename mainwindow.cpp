#include <QMessageBox>
#include <QVBoxLayout>
#include <keyvaluetablemodel.h>


#include <cstddef>
#include <cmath>
#include <vector>
#include <algorithm>

#include "aboutdialog.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "memorypanel.h"
#include "util.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->action_Quit, &QAction::triggered, this, [] {
        QApplication::exit(0);
    });
    QObject::connect(ui->action_About_QtMem0ry, &QAction::triggered, this, [] {
        AboutDialog dialog;
        dialog.exec();
    });
    QObject::connect(ui->actionStart_Game, &QAction::triggered, this, [=] {
        auto memoryPanel = this->findChild<MemoryPanel*>();
        switch (memoryPanel->gameState()) {
        case STOPPED:
        case OVER:
            memoryPanel->setGameState(GameState::STARTED);
            break;
        case STARTED:
            Util::showMessage("Game is already started");
            break;
        }
    });
    QObject::connect(ui->actionEnd_Game, &QAction::triggered, this, [=] {
        auto memoryPanel = this->findChild<MemoryPanel*>();
        switch (memoryPanel->gameState()) {
        case STOPPED:
        case OVER:
            Util::showMessage("Game is already stopped");
            break;
        case STARTED:
            auto reply = QMessageBox::question(this, "Confirmation", "Are you sure you want to stop the game?", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                memoryPanel->setGameState(GameState::STOPPED);
            }
            break;
        }
    });

    auto memoryPanel = new MemoryPanel(this);
    auto layout = new QVBoxLayout(ui->content);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(memoryPanel);
    memoryPanel->setupChildren();

    auto splitter = ui->splitter;
    QList<int> sizes;
    sizes << 240 << 1000;
    splitter->setSizes(sizes);

    QVector<QPair<QString, QString>> data = {
        {"Key1", "Value1"},
        {"Key2", "Value2"},
        {"Key3", "Value3"},
    };

    auto model = new KeyValueTableModel();
    model->setData(data);
    ui->tableView->setModel(model);

    memoryPanel->stats().connect()
}

MainWindow::~MainWindow()
{
    delete ui;
}