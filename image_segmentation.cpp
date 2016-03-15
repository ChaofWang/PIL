#include "image_segmentation.h"
#include "ui_image_segmentation.h"
#include <opencv2/highgui/highgui.hpp>
#include <Qimage>
#include <qlabel.h>
#include <opencv2/opencv.hpp>
#include <opencv2/legacy/compat.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QString>
#include <QTextCodec>
#include <opencv2/highgui/highgui.hpp>
#include<QMessageBox>
using namespace cv;
image_segmentation::image_segmentation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::image_segmentation)
{
    ui->setupUi(this);
    this->image = new QImage();
    ui->img_seg_label->setScaledContents(true);
    ui->orig_pic_label->setScaledContents(true);
}

image_segmentation::~image_segmentation()
{
    delete image;
    delete ui;
}

void image_segmentation::on_Select_Pic_clicked()
{
    float temp;
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"),"",tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();                      //转换编码方式
    if(filename !="" )
    {
     if(image->load(filename))                                                  //QT打开图像
     {
           img = imread(name);                                                  //opencv打开图像
/*---------------------------------显示原图像------------------------------------------------*/
           temp = (float)image->width()/(float)image->height();
           ui->orig_pic_label->setPixmap(QPixmap::fromImage(*image));
           /*自适应框体*/
           if(image->width()>= image->height())
           {
               ui->orig_pic_label->resize(ui->img_seg_widget->width(),(int)(ui->img_seg_widget->height()/temp));
           }
           else
           {
               ui->orig_pic_label->resize((int)(ui->img_seg_widget->width()*temp),ui->img_seg_widget->height());
           }

       }
    }
}

void image_segmentation::on_Img_Seq_clicked()
{
    if(img.empty())
      {
          QMessageBox::information(this,tr("警告"),tr("请先打开图像！"));
          return ;
      }
    else
     {
        /*---------------------------------图像分割------------------------------------------------*/
           IplImage *src = NULL;
           IplImage *dst = NULL;
           CvSeq* comp=NULL;
           float temp;
           int level = 0;
           int tempwidth;
           int tempheight;


           QImage *qImg;
           src = new IplImage();
           qImg = new QImage();
           *src = img;                                                          //mat转Iplimage 浅拷贝
           if(src->width%2 != 0 && src->height%2 != 0)
             {
                 QMessageBox::information(this,tr("警告"),tr("确认图像长宽是2的倍数！"));
                 return ;
             }
           else{
           tempwidth = src->width;
           tempheight = src->height;
           while(tempwidth%2 == 0&&tempheight%2 == 0)
           {

                   tempwidth = tempwidth/2;
                   tempheight = tempheight/2;
                   level++;

           }
           CvMemStorage * storage = cvCreateMemStorage(0) ;
           dst = cvCreateImage(cvGetSize(src), src->depth,src->nChannels);

           cvPyrSegmentation(src, dst, storage,&comp, level, 200, 40 );


//           cvNamedWindow("PrySegmentation",CV_WINDOW_AUTOSIZE);
//           cvShowImage("PrySegmentation", dst);
           cvCvtColor(dst,dst,CV_BGR2RGB);

           uchar* imgData = (uchar*)(dst->imageData);
           qImg=new QImage(imgData,dst->width,dst->height,QImage::Format_RGB888);
           temp = (float)qImg->width()/(float)qImg->height();
           ui->img_seg_label->setPixmap(QPixmap::fromImage(*qImg));

            /*自适应框体*/
           if(qImg->width()>= qImg->height())
            {
              ui->img_seg_label->resize(ui->img_seg_widget->width(),(int)(ui->img_seg_widget->height()/temp));
            }
           else
            {
              ui->img_seg_label->resize((int)(ui->img_seg_widget->width()*temp),ui->img_seg_widget->height());
            }

           delete src;
           delete qImg;
           }
      }
}
