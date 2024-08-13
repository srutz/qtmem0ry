#include <QMessageBox>
#include <QVBoxLayout>
#include <QToolBar>
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
        AboutDialog dialog(QApplication::activeWindow());
        dialog.exec();
    });
    QObject::connect(ui->actionStart_Game, &QAction::triggered, this, [=] {
        startGame();
    });
    QObject::connect(ui->actionEnd_Game, &QAction::triggered, this, [=] {
        endGame();
    });

    auto toolbar = new QToolBar("Main toolbar", this);
    addToolBar(toolbar);
    auto buttonAction1 = new QAction(QIcon(":/images/play.png"), "Start Game", this);
    connect(buttonAction1, &QAction::triggered, this, [=] () {
        startGame();
    });
    toolbar->addAction(buttonAction1);
    auto buttonAction2 = new QAction(QIcon(":/images/stop.png"), "Stop Game", this);
    connect(buttonAction2, &QAction::triggered, this, [=] () {
        endGame();
    });
    toolbar->addAction(buttonAction2);

    auto memoryPanel = new MemoryPanel(this);
    auto layout = new QVBoxLayout(ui->content);
    ui->content->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(memoryPanel);
    memoryPanel->setupChildren();
    ui->content->setStyleSheet("background-color: #3498db;");


    auto splitter = ui->splitter;
    QList<int> sizes;
    sizes << 240 << 1000;
    splitter->setSizes(sizes);

    QVector<QPair<QString, QString>> data = {
    };
    auto model = new KeyValueTableModel();
    model->setData(data);
    ui->tableView->setModel(model);

    QObject::connect(&memoryPanel->stats(), &Stats::changed, this, [=] (const Stats& stats) {
        updateStats(stats);
    });
    updateStats(memoryPanel->stats());
}

void MainWindow::updateStats(const Stats& stats) {
    auto model = dynamic_cast<KeyValueTableModel*>(ui->tableView->model());
    QVector<QPair<QString, QString>> data = {
        { "Total-Moves", QString::number(stats.moves()) },
        { "Hits", QString::number(stats.hits()) },
        { "Misses", QString::number(stats.misses()) },
        { "Seen-Misses", QString::number(stats.seenMisses()) },
    };
    model->setData(data);
}

void MainWindow::startGame() {
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
}

void MainWindow::endGame() {
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
}

MainWindow::~MainWindow()
{
    delete ui;
}
