#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Mesh informations
    //Font size..
   // ui->labelInfo->setFont();

    updateTextInfos();

    updateTextCurrentIndex();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTextInfos() {
    QString t = "Number of vertices : " + QString::number(ui->widget->vertexNb());
    ui->labelVertexNb->setText(t);

    QString tFace = "Number of faces : " + QString::number(ui->widget->faceNb());
    ui->labelFaceNb->setText(tFace);
}


void MainWindow::updateTextCurrentIndex() {

    QString text = "Current face : " + QString::number(ui->widget->currentFace());
    ui->labelCurrFace->setText(text);

    text = "Current neighboor vertex : " + QString::number(ui->widget->currentVertex());
    ui->labelCurrVertex->setText(text);

    text = "Current local vertex : " + QString::number(ui->widget->currentLocalVertex());
    ui->labelCurrentLocalVertex->setText(text);

    //Start vertex index
    text = QString::number(ui->widget->currentStartVertex());
    ui->lineEditStartVertex->setText(text);
}

void MainWindow::on_pbNextFace_released()
{
    ui->widget->nextFace();
    updateTextCurrentIndex();
}

void MainWindow::on_pbNextVertex_released()
{
    ui->widget->nextVertex(); //Neighboring vertex
    updateTextCurrentIndex();
}

void MainWindow::on_pbTestIterators_released()
{
    ui->widget->testIterators();
}

void MainWindow::on_pbResetV_F_released()
{
    ui->widget->resetVertexFaceIndex();
    updateTextCurrentIndex();
}

void MainWindow::on_pbWireframe_released()
{
    ui->widget->setWireframe();
}

void MainWindow::on_lineEditStartVertex_returnPressed()
{
    unsigned int vs = ui->lineEditStartVertex->text().toInt();
    ui->widget->setVertexStart(vs);
    updateTextCurrentIndex();
}

void MainWindow::on_pbPrevFace_released()
{
    ui->widget->prevFace();
    updateTextCurrentIndex();
}

void MainWindow::on_pbPrevVertex_released()
{
    ui->widget->prevVertex();
    updateTextCurrentIndex();
}

void MainWindow::on_pbPyramid_released()
{
    ui->widget->createPyramid();
    updateTextInfos();
}

void MainWindow::on_pbTetrahedron_released()
{
    ui->widget->createTetrahedron();
    updateTextInfos();
}

void MainWindow::on_pb2DSquare_released()
{
    ui->widget->create2DSquare();
}

void MainWindow::on_pbOpenFile_released()
{
    std::string fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "./", tr("OFF Files (*.off)")).toStdString();

    ui->widget->createFromOFF(fileName);
    updateTextInfos();




}

void MainWindow::on_pbCreateQueen_released()
{
    //ui->widget->createFromOFF("./M2/maillage/Mesh_Computational_Geometry/OFF_files/queen.off");
    ui->widget->createFromOFF("./M2/maillage/Mesh_Computational_Geometry/OFF_files/mushroom.off");
    updateTextInfos();
}

void MainWindow::on_pushButton_released()
{
    ui->widget->setVoronoi();
}

void MainWindow::on_pbSplitFaceMiddle_released()
{
    int index = ui->lineEditFaceIndexSplit->text().toInt();
    ui->lineEditFaceIndexSplit->setText("");
    ui->widget->splitFaceMiddle(index);
}

void MainWindow::on_pbAddPoint_released()
{

    double x = ui->doubleX->value();
    double y = ui->doubleY->value();
    double z = ui->doubleZ->value();
    ui->doubleX->setValue(0);
    ui->doubleY->setValue(0);
    ui->doubleZ->setValue(0);

    ui->widget->addNaivePoint(x,y,z);
}

void MainWindow::on_pbDelaunayAdd_released()
{
    double x = ui->doubleX->value();
    double y = ui->doubleY->value();
    double z = ui->doubleZ->value();
    ui->doubleX->setValue(0);
    ui->doubleY->setValue(0);
    ui->doubleZ->setValue(0);

    ui->widget->addDelaunayPoint(x,y,z);

}

void MainWindow::on_pbFlipEdge_released()
{
    int faceIndex = ui->lineEditFlipEdgeFace->text().toInt();
    int localVertexIndex = ui->lineEditFlipEdgeVertex->text().toInt();

    if (localVertexIndex < 0 || localVertexIndex > 2)
        return;

    ui->widget->flipEdge(faceIndex, localVertexIndex);

}
