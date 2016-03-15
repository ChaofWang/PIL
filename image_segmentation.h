#ifndef IMAGE_SEGMENTATION_H
#define IMAGE_SEGMENTATION_H

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
class image_segmentation;
}

class image_segmentation : public QDialog
{
    Q_OBJECT

public:
    explicit image_segmentation(QWidget *parent = 0);
    ~image_segmentation();

private slots:
    void on_Select_Pic_clicked();

    void on_Img_Seq_clicked();

private:
    Mat img;
    QImage *image;
    Ui::image_segmentation *ui;
};

#endif // IMAGE_SEGMENTATION_H
