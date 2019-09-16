#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pbNextFace_released();

    void on_pbNextVertex_released();

    void on_pbTestIterators_released();

    void on_pbResetV_F_released();

    void on_pbWireframe_released();

    void on_lineEditStartVertex_returnPressed();

    void on_pbPrevFace_released();

    void on_pbPrevVertex_released();

private:
    Ui::MainWindow *ui;

    void updateTextCurrentIndex();
};

#endif // MAINWINDOW_H
