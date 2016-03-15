#include "filter.h"
#include "ui_filter.h"
#include <Qimage>
#include <qlabel.h>
#include <opencv2/opencv.hpp>
#include <opencv2/legacy/compat.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <QString>
#include <QTextCodec>
#include <QMessageBox>
using namespace cv;
filter::filter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::filter)
{
    ui->setupUi(this);
    this->image = new QImage();
    ui->orig_pic_label->setScaledContents(true);
    ui->guass_label->setScaledContents(true);
    ui->sharp_label->setScaledContents(true);
}

filter::~filter()
{
    delete image;
    delete ui;
}

void filter::on_Select_Pic_clicked()
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


void filter::on_filter_comboBox_activated(const QString &arg1)
{
    if(img.empty())
    {
        QMessageBox::information(this,tr("警告"),tr("请先打开图像！"));
        return ;
    }

    else
    {
    float temp;
    filter_image = new QImage();
    src = img;

    if(0 == ui->filter_comboBox->currentIndex())
    {
        GaussianBlur(src,dst,Size(5,5),0,0);        //高斯
        soomth_img =dst;
    }
    if(1 == ui->filter_comboBox->currentIndex())
    {
        //中值滤波对脉冲噪声有良好的滤除作用，特别是在滤除噪声的同时，能够保护信号的边缘，使之不被模
        //中值滤波法是一种非线性平滑技术，它将每一像素点的灰度值设置为该点某邻域窗口内的所有像素点灰度值的中值
        medianBlur(src,dst,3);                      //中值
        soomth_img =dst;
    }
    if(2 == ui->filter_comboBox->currentIndex())
    {
        //双边滤波器的好处是可以做边缘保存（edge preserving）
        //第一个参数，InputArray类型的src，输入图像
        //第二个参数，OutputArray类型的dst，即目标图像
        //第三个参数，int类型的d，表示在过滤过程中每个像素邻域的直径
        //第四个参数，double类型的sigmaColor，颜色空间滤波器的sigma值。
        //这个参数的值越大，就表明该像素邻域内有更宽广的颜色会被混合到一起，产生较大的半相等颜色区域。
        //第五个参数，double类型的sigmaSpace坐标空间中滤波器的sigma值，坐标空间的标注方差。他的数值越大，
        //以为着越远的像素会相互影响，从而使更大的区域足够相似的颜色获取相同的颜色。当d>0，d指定了邻域大小且与sigmaSpace无关。否则，d正比于sigmaSpace
        //第六个参数，int类型的borderType，用于推断图像外部像素的某种边界模式。注意它有默认值BORDER_DEFAULT。
        bilateralFilter(src,dst,6,150.0,2.0);        //双边
        soomth_img =dst;
    }
    if(3 == ui->filter_comboBox->currentIndex())
    {
        blur(src,dst,Size(5,5),Point(-1,-1));       //均值
        soomth_img =dst;
    }
        cvtColor(dst,dst,CV_BGR2RGB);
        /*---------------------------------显示图像------------------------------------------------*/
        *filter_image = QImage((const unsigned char*)(dst.data),dst.cols,dst.rows, dst.cols*dst.channels(),  QImage::Format_RGB888);
        temp = (float)filter_image->width()/(float)filter_image->height();
        ui->guass_label->setPixmap(QPixmap::fromImage(*filter_image));
        /*自适应框体*/
        if(filter_image->width()>= filter_image->height())
        {
            ui->guass_label->resize(ui->guass_widget->width(),(int)(ui->guass_widget->height()/temp));
        }
        else
        {
            ui->guass_label->resize((int)(ui->guass_widget->width()*temp),ui->guass_widget->height());
        }

   }
      delete filter_image;
}



void filter::on_sharp_comboBox_activated(const QString &arg1)
{

    float temp;
    Mat sharp_image;
    QImage *sharp_image_ptr;
    sharp_image_ptr = new QImage();
    Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    if(img.empty())
    {
        QMessageBox::information(this,tr("警告"),tr("请先打开图像！"));
        return ;
    }
    else
    {
        if(0 == ui->sharp_comboBox->currentIndex())
        {
            filter2D(img, sharp_image, img.depth(), kernel);
            cvtColor(sharp_image,sharp_image,CV_BGR2RGB);
        }
        if(1 == ui->sharp_comboBox->currentIndex())
        {
            if(soomth_img.empty())
            {
                QMessageBox::information(this,tr("警告"),tr("请先对图像进行平滑处理！"));
                return ;
            }
            else{
                filter2D(soomth_img, sharp_image, soomth_img.depth(), kernel);
            }
        }

        /*---------------------------------显示图像------------------------------------------------*/
        *sharp_image_ptr = QImage((const unsigned char*)(sharp_image.data),sharp_image.cols,sharp_image.rows, sharp_image.cols*sharp_image.channels(),  QImage::Format_RGB888);
        temp = (float)sharp_image_ptr->width()/(float)sharp_image_ptr->height();
        ui->sharp_label->setPixmap(QPixmap::fromImage(*sharp_image_ptr));
        /*自适应框体*/
        if(sharp_image_ptr->width()>= sharp_image_ptr->height())
        {
            ui->sharp_label->resize(ui->sharp_widget->width(),(int)(ui->sharp_widget->height()/temp));
        }
        else
        {
            ui->sharp_label->resize((int)(ui->sharp_widget->width()*temp),ui->sharp_widget->height());
        }
    }

    delete sharp_image_ptr;
}
