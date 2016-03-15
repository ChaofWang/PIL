#ifndef FILTER_H
#define FILTER_H

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
class filter;
}

class filter : public QDialog
{
    Q_OBJECT

public:
    explicit filter(QWidget *parent = 0);

    ~filter();

private slots:
    void on_Select_Pic_clicked();

    void on_filter_comboBox_activated(const QString &arg1);


    void on_sharp_comboBox_activated(const QString &arg1);

private:
    Ui::filter *ui;
    Mat img;
    Mat src;
    Mat dst;
    Mat soomth_img;
    QImage *filter_image;
    QImage *image;
};

#endif // FILTER_H
