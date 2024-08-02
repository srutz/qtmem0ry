#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QWidget>
#include <QGraphicsView>

class GameScene : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GameScene(QWidget *parent = nullptr);
    ~GameScene();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QGraphicsScene* scene;

    void initResources();
    void layoutScene();

signals:
};

#endif // GAMESCENE_H
