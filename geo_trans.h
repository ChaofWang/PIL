#ifndef GEO_TRANS_H
#define GEO_TRANS_H

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
class geo_trans;
}

class geo_trans : public QDialog
{
    Q_OBJECT

public:
    explicit geo_trans(QWidget *parent = 0);
    Mat img;
    ~geo_trans();

private slots:
    void on_Select_Pic_clicked();

    void on_xuanwo_clicked();


    void on_waiqu_clicked();

    void on_wave_clicked();

private:
    Ui::geo_trans *ui;
    QImage *image;
};

#endif // GEO_TRANS_H
