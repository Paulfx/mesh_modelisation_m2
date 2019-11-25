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

    void on_pbPyramid_released();

    void on_pbTetrahedron_released();

    void on_pbOpenFile_released();

    void on_pbCreateQueen_released();

    void on_pushButton_released();

    void on_pbSplitFaceMiddle_released();

    void on_pbAddPoint_released();

    void on_pbDelaunayAdd_released();

    void on_pb2DSquare_released();

    void on_pbFlipEdge_released();

private:
    Ui::MainWindow *ui;

    void updateTextCurrentIndex();
    void updateTextInfos();
};

#endif // MAINWINDOW_H
