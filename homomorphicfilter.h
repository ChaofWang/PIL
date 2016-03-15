#ifndef HOMOMORPHICFILTER_H
#define HOMOMORPHICFILTER_H

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
class HomomorphicFilter;
}

class HomomorphicFilter : public QDialog
{
    Q_OBJECT

public:
    explicit HomomorphicFilter(QWidget *parent = 0);
    ~HomomorphicFilter();
    void my_HomoFilter(Mat srcImg, Mat &dst);
private slots:
    void on_Select_Pic_clicked();

    void on_homomo_clicked();

private:
    Mat img;
    QImage *image;
    Ui::HomomorphicFilter *ui;
};

#endif // HOMOMORPHICFILTER_H
