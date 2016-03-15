#ifndef HIST_H
#define HIST_H

#include <QDialog>
#include <opencv2/opencv.hpp>
#include <opencv2/legacy/compat.hpp>
#include <QFileDialog>
#include <QImage>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
namespace Ui {
class hist;
}

class hist : public QDialog
{
    Q_OBJECT

public:
    explicit hist(QWidget *parent = 0);
    Mat img;
    Mat eqlizehist;         //给一个均衡后的Mat类型图像
    void myCal_Hist(Mat Gray_img,MatND hist);
    ~hist();

private slots:
    void on_chooseImg_clicked();

    void on_histbuttom_clicked();

    void on_histeEqualize_clicked();

    void on_showhist_clicked();

private:
    Ui::hist *ui;
    QImage *image;
    QImage *grayimage;
    QImage *eqlizeimg;
};

#endif // HIST_H
