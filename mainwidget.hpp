#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();



private:
    Ui::MainWidget *ui;

public slots:
    void onTimeout();
    void paintEvent(QPaintEvent* event);
    void keyPressEvent(QKeyEvent* event);

};

#endif // MAINWIDGET_HPP
