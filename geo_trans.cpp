#include "geo_trans.h"
#include "ui_geo_trans.h"
#include <Qimage>
#include <qlabel.h>
#include <opencv2/opencv.hpp>
#include <opencv2/legacy/compat.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <QString>
#include <QTextCodec>
#include<QMessageBox>
#define PI 3.1415926
using namespace std;
using namespace cv;
geo_trans::geo_trans(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::geo_trans)
{

    ui->setupUi(this);
    this->image = new QImage();
    ui->horizontalSlider->setRange(0,40);
    ui->horizontalSlider_2->setRange(0,30);
    ui->horizontalSlider_3->setRange(1,25);
    ui->orig_pic_label->setScaledContents(true);
    ui->xuanwo_label->setScaledContents(true);
    ui->waiqu_label->setScaledContents(true);
    ui->wave_label->setScaledContents(true);
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(on_xuanwo_clicked()));
    connect(ui->horizontalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(on_waiqu_clicked()));
    connect(ui->horizontalSlider_3,SIGNAL(valueChanged(int)),this,SLOT(on_wave_clicked()));
}

geo_trans::~geo_trans()
{
    delete image;
    delete ui;
}

void geo_trans::on_Select_Pic_clicked()
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
/*--------------------------------漩涡效果------------------------------------------------*/
void geo_trans::on_xuanwo_clicked()
{
  if(img.empty())
  {
      QMessageBox::information(this,tr("警告"),tr("请先打开图像！"));
      return ;
  }

  else{

    float temp;
    int Para;
    Mat src;
    QImage *xuanwoimage;
    xuanwoimage = new QImage();
    Para = ui->horizontalSlider->value();
    src=img.clone();//深拷贝
    int heigh = src.rows;
    int width = src.cols;
    Point center(width/2, heigh/2);
    Mat IMG;
    src.copyTo(IMG);
    Mat src1u[3];
    split(src,src1u);
    for (int y=0; y<heigh; y++)
         {
             uchar* imgP = IMG.ptr<uchar>(y);
         //  uchar* srcP = src.ptr<uchar>(y);
             for(int x=0; x<width; x++)
             {
                 int R = norm(Point(x,y)-center);
                 double angle = atan2((double)(y-center.y),(double)(x-center.x));
                 double delta=PI*Para/sqrtf(R+1);
                 int newX = R*cos(angle+delta) + center.x;
                 int newY = R*sin(angle+delta) + center.y;

                 if(newX<0) newX=0;
                 if(newX>width-1) newX=width-1;
                 if(newY<0) newY=0;
                 if(newY>heigh-1) newY=heigh-1;

                 imgP[3*x] =   src1u[0].at<uchar>(newY,newX);
                 imgP[3*x+1] = src1u[1].at<uchar>(newY,newX);
                 imgP[3*x+2] = src1u[2].at<uchar>(newY,newX);
             }
         }

   cvtColor(IMG,IMG,CV_BGR2RGB);
   /*---------------------------------显示图像------------------------------------------------*/
   *xuanwoimage = QImage((const unsigned char*)(IMG.data),IMG.cols,IMG.rows, IMG.cols*IMG.channels(),  QImage::Format_RGB888);
   temp = (float)xuanwoimage->width()/(float)xuanwoimage->height();
   ui->xuanwo_label->setPixmap(QPixmap::fromImage(*xuanwoimage));
   /*自适应框体*/
   if(xuanwoimage->width()>= xuanwoimage->height())
   {
       ui->xuanwo_label->resize(ui->xuanwo_widget->width(),(int)(ui->xuanwo_widget->height()/temp));
   }
   else
   {
       ui->xuanwo_label->resize((int)(ui->xuanwo_widget->width()*temp),ui->xuanwo_widget->height());
   }
   delete xuanwoimage;
  }
}
/*--------------------------------歪曲效果------------------------------------------------*/
void geo_trans::on_waiqu_clicked()
{
  if(img.empty())
    {
        QMessageBox::information(this,tr("警告"),tr("请先打开图像！"));
        return ;
    }
  else{

    Mat src;
    Mat IMG;
    QImage *waiquimage;
    waiquimage = new QImage();
    int degree;//chengdu
    float temp;
    src=img.clone();//深拷贝
    int heigh = src.rows;
    int width = src.cols;
    Point center(width/2, heigh/2);
    src.copyTo(IMG);
    Mat src1u[3];
    split(src,src1u);
    degree = ui->horizontalSlider_2->value();

    for (int i=0; i<heigh; i++)
         {
             uchar* imgP = IMG.ptr<uchar>(i);
             for(int j=0; j<width; j++)
             {
                int offsetX=j-center.x;
                int offsetY=center.y-i;
                double x=degree*sin(2*PI* offsetY/128)+offsetX;
                double y=degree*cos(2*PI* offsetX/128)+offsetY;
                 x=x+width/2;
                 y=heigh/2-y;
                 if(x>0 && x<width-1 && y<heigh-1 && y>0)
                 {
                     int x1=floor(x);
                     int y1=floor(y);
                     double p=x-x1;
                    // qDebug("p=%f\n",p);
                     double q=y-y1;
                 imgP[3*j] =   (1-p)*(1-q)*(src1u[0].at<uchar>(y1,x1))+p*(1-q)*(src1u[0].at<uchar>(y1,x1+1))+q*(1-p)*(src1u[0].at<uchar>(y1+1,x1))+p*q*src1u[0].at<uchar>(y1+1,x1+1);
                 imgP[3*j+1] = (1-p)*(1-q)*src1u[1].at<uchar>(y1,x1)+p*(1-q)*src1u[1].at<uchar>(y1,x1+1)+q*(1-p)*src1u[1].at<uchar>(y1+1,x1)+p*q*src1u[1].at<uchar>(y1+1,x1+1);
                 imgP[3*j+2] = (1-p)*(1-q)*src1u[2].at<uchar>(y1,x1)+p*(1-q)*src1u[2].at<uchar>(y1,x1+1)+q*(1-p)*src1u[2].at<uchar>(y1+1,x1)+p*q*src1u[2].at<uchar>(y1+1,x1+1);
                 }
             }
        }
   cvtColor(IMG,IMG,CV_BGR2RGB);
   /*---------------------------------显示图像------------------------------------------------*/
   *waiquimage = QImage((const unsigned char*)(IMG.data),IMG.cols,IMG.rows, IMG.cols*IMG.channels(),  QImage::Format_RGB888);
   temp = (float)waiquimage->width()/(float)waiquimage->height();
   ui->waiqu_label->setPixmap(QPixmap::fromImage(*waiquimage));
   /*自适应框体*/
   if(waiquimage->width()>= waiquimage->height())
   {
       ui->waiqu_label->resize(ui->waiqu_widget->width(),(int)(ui->waiqu_widget->height()/temp));
   }
   else
   {
       ui->waiqu_label->resize((int)(ui->waiqu_widget->width()*temp),ui->waiqu_widget->height());
   }
   delete waiquimage;

  }
}
/*--------------------------------水波效果------------------------------------------------*/
void geo_trans::on_wave_clicked()
{
  if(img.empty())
      {
          QMessageBox::information(this,tr("警告"),tr("请先打开图像！"));
          return ;
      }
  else{
    Mat src;
    Mat IMG;
    float temp;
    int  wavelength = 15 ;
    int phase;
    QImage *waveimage;
    waveimage = new QImage();
    src=img.clone();//深拷贝
    phase = ui->horizontalSlider_3->value();
    int heigh = src.rows;
    int width = src.cols;
    Point center(width/2, heigh/2);

    src.copyTo(IMG);
    Mat src1u[3];
    split(src,src1u);


    int amplitude=10;

    double radius=0;
    if(0 == radius)
    {
        radius=min(heigh,width)/2;
    }

    int icentreX=center.x;
    int icentreY=center.y;
    double radius2=radius*radius;

   for (int ii=0; ii<heigh; ii++)
         {
             uchar* imgP = IMG.ptr<uchar>(ii);
             for(int jj=0; jj<width; jj++)
             {
                double dy=ii-icentreY;
                double dx=jj-icentreX;
                double distance2=dx*dx+dy*dy;
                double x,y;
                if(distance2>radius2)
                {
                    x=jj;
                    y=ii;
                }else
                {
                   double distance=sqrt(distance2);
                   double amount=amplitude*sin(distance/wavelength*2*PI-phase); // 计算改点振幅
                   amount=amount*(radius-distance)/radius;
                   amount=amount*wavelength/(distance+0.0001); // 计算能量损失，
                   x=jj+dx*amount;
                   y=ii+dy*amount; // 得到water ripple 最终迁移位置
                }

                if(x>0 && x<width-1 && y<heigh-1 && y>0)
                {
                   int x1=floor(x);
                   int y1=floor(y);
                    double p=x-x1;
                    double q=y-y1;

                     imgP[3*jj] =   (1-p)*(1-q)*(src1u[0].at<uchar>(y1,x1))+p*(1-q)*(src1u[0].at<uchar>(y1,x1+1))+q*(1-p)*(src1u[0].at<uchar>(y1+1,x1))+p*q*src1u[0].at<uchar>(y1+1,x1+1);
                     imgP[3*jj+1] = (1-p)*(1-q)*src1u[1].at<uchar>(y1,x1)+p*(1-q)*src1u[1].at<uchar>(y1,x1+1)+q*(1-p)*src1u[1].at<uchar>(y1+1,x1)+p*q*src1u[1].at<uchar>(y1+1,x1+1);
                     imgP[3*jj+2] = (1-p)*(1-q)*src1u[2].at<uchar>(y1,x1)+p*(1-q)*src1u[2].at<uchar>(y1,x1+1)+q*(1-p)*src1u[2].at<uchar>(y1+1,x1)+p*q*src1u[2].at<uchar>(y1+1,x1+1);

                }
              }
            }
   cvtColor(IMG,IMG,CV_BGR2RGB);
   /*---------------------------------显示图像------------------------------------------------*/
   *waveimage = QImage((const unsigned char*)(IMG.data),IMG.cols,IMG.rows, IMG.cols*IMG.channels(),  QImage::Format_RGB888);
   temp = (float)waveimage->width()/(float)waveimage->height();
   ui->wave_label->setPixmap(QPixmap::fromImage(*waveimage));
   /*自适应框体*/
   if(waveimage->width()>= waveimage->height())
   {
       ui->wave_label->resize(ui->wave_widget->width(),(int)(ui->wave_widget->height()/temp));
   }
   else
   {
       ui->wave_label->resize((int)(ui->wave_widget->width()*temp),ui->wave_widget->height());
   }
   delete waveimage;
  }
}
