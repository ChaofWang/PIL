#include "homomorphicfilter.h"
#include "ui_homomorphicfilter.h"
#include <Qimage>
#include <qlabel.h>
#include <opencv2/opencv.hpp>
#include <opencv2/legacy/compat.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <QString>
#include <QTextCodec>
#include<QMessageBox>
HomomorphicFilter::HomomorphicFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomomorphicFilter)
{
    ui->setupUi(this);
    this->image = new QImage();
    ui->orig_pic_label->setScaledContents(true);
    ui->gray_label->setScaledContents(true);
    ui->hom_label->setScaledContents(true);
}

HomomorphicFilter::~HomomorphicFilter()
{
    delete image;
    delete ui;
}

void HomomorphicFilter::on_Select_Pic_clicked()
{
    float temp;

    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"),"",tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();   //转换编码方式
    if(filename !="" )
    {
     if(image->load(filename))
     {
           img = imread(name);
/*---------------------------------显示原图像------------------------------------------------*/
           temp = (float)image->width()/(float)image->height();
           ui->orig_pic_label->setPixmap(QPixmap::fromImage(*image));
           /*自适应框体*/
           if(image->width()>= image->height())
           {
               ui->orig_pic_label->resize(ui->orig_pic_widget->width(),(int)(ui->orig_pic_widget->height()/temp));
           }
           else
           {
               ui->orig_pic_label->resize((int)(ui->orig_pic_widget->width()*temp),ui->orig_pic_widget->height());
           }

      }

   }
}
void HomomorphicFilter::my_HomoFilter(Mat srcImg, Mat &dst){
    QImage *homomo_image_ptr;
    float temp;
    homomo_image_ptr = new QImage();
    srcImg.convertTo(srcImg, CV_64FC1);
    dst.convertTo(dst, CV_64FC1);
    //1. ln
    for (int i = 0; i < srcImg.rows; i++)
    {
        double* srcdata = srcImg.ptr<double>(i);
        double* logdata = srcImg.ptr<double>(i);
        for (int j = 0; j < srcImg.cols; j++)
        {
            logdata[j] = log(srcdata[j]+0.0001);
        }
    }

    //spectrum
    //2. dct
    Mat mat_dct = Mat::zeros(srcImg.rows, srcImg.cols, CV_64FC1);
    dct(srcImg, mat_dct);

    //3. linear filter
    Mat H_u_v;
    double gammaH = 1.3;
    double gammaL = 0.6;
    double C = 1;
    double d0 = (srcImg.rows/2)*(srcImg.rows/2) + (srcImg.cols/2)*(srcImg.cols/2);
    double d2 = 0;
    H_u_v = Mat::zeros(srcImg.rows, srcImg.cols, CV_64FC1);

    double totalWeight = 0.0;
    for (int i = 0; i < srcImg.rows; i++)
    {
        double * dataH_u_v = H_u_v.ptr<double>(i);
        for (int j = 0; j < srcImg.cols; j++)
        {
            d2 = pow((i), 2.0) + pow((j), 2.0);
            dataH_u_v[j] =  (gammaH - gammaL)*(1 - exp(-C*d2/d0)) + gammaL;
            totalWeight += dataH_u_v[j];
        }
    }
    H_u_v.ptr<double>(0)[0] = 1.1;

    mat_dct = mat_dct.mul(H_u_v);
    //4. idct
    idct(mat_dct, dst);

    //5. exp
    for (int i = 0; i < srcImg.rows; i++)
    {
        double* srcdata = dst.ptr<double>(i);
        double* dstdata = dst.ptr<double>(i);
        for (int j = 0; j < srcImg.cols; j++)
        {
            dstdata[j] = exp(srcdata[j]);
        }
    }
    dst.convertTo(dst, CV_8UC1);
    /*---------------------------------显示图像------------------------------------------------*/
    *homomo_image_ptr = QImage((const unsigned char*)(dst.data),dst.cols,dst.rows, dst.cols*dst.channels(),  QImage::Format_Grayscale8);
    temp = (float)homomo_image_ptr->width()/(float)homomo_image_ptr->height();
    ui->hom_label->setPixmap(QPixmap::fromImage(*homomo_image_ptr));
    /*自适应框体*/
    if(homomo_image_ptr->width()>= homomo_image_ptr->height())
    {
        ui->hom_label->resize(ui->hom_widget->width(),(int)(ui->hom_widget->height()/temp));
    }
    else
    {
        ui->hom_label->resize((int)(ui->hom_widget->width()*temp),ui->hom_widget->height());
    }

    delete homomo_image_ptr;

}
void HomomorphicFilter::on_homomo_clicked()
{
    if(img.empty())
      {
          QMessageBox::information(this,tr("警告"),tr("请先打开图像！"));
          return ;
      }
    else
     {
        Mat gray_img;
        float temp;
        QImage *gray_image_ptr;
        gray_image_ptr = new QImage();        
        cvtColor(img,gray_img,CV_BGR2GRAY);
        Mat dst(gray_img.rows, gray_img.cols, gray_img.type());
        my_HomoFilter(gray_img, dst);
        /*---------------------------------显示灰度图像------------------------------------------------*/
        *gray_image_ptr = QImage((const unsigned char*)(gray_img.data),gray_img.cols,gray_img.rows, gray_img.cols*gray_img.channels(),  QImage::Format_Grayscale8);
        temp = (float)gray_image_ptr->width()/(float)gray_image_ptr->height();
        ui->gray_label->setPixmap(QPixmap::fromImage(*gray_image_ptr));
        /*自适应框体*/
        if(gray_image_ptr->width()>= gray_image_ptr->height())
        {
            ui->gray_label->resize(ui->gray_widget->width(),(int)(ui->gray_widget->height()/temp));
        }
        else
        {
            ui->gray_label->resize((int)(ui->gray_widget->width()*temp),ui->gray_widget->height());
        }
        delete  gray_image_ptr;
     }
}
