#include "hist.h"
#include "ui_hist.h"
#include <Qimage>
#include <qlabel.h>
#include <opencv2/opencv.hpp>
#include <opencv2/legacy/compat.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <QString>
#include <QTextCodec>
#include <QMessageBox>
using namespace std;
using namespace cv;
hist::hist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hist)
{
    ui->setupUi(this);
    this->image = new QImage();
    this->grayimage = new QImage();
    this->eqlizeimg = new QImage();
    ui->label->setScaledContents(true);
    ui->graylabel->setScaledContents(true);
    ui->label_2->setScaledContents(true);
}

hist::~hist()
{
    delete  this->image;
    delete  this->grayimage;
    delete  this->eqlizeimg;
    delete ui;
}

void hist::on_chooseImg_clicked()
{
    float temp;
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"),"",tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();   //转换编码方式
    if(filename !="" )
    {
     if(image->load(filename))
     {
/*---------------------------------显示原图像------------------------------------------------*/
           temp = (float)image->width()/(float)image->height();
           ui->label->setPixmap(QPixmap::fromImage(*image));
           /*自适应框体*/
           if(image->width()>= image->height())
           {
               ui->label->resize(ui->widget->width(),(int)(ui->widget->height()/temp));
           }
           else
           {
               ui->label->resize((int)(ui->widget->width()*temp),ui->widget->height());
           }

 /*---------------------------------显示灰度图------------------------------------------------*/
           img = imread(name);
           cvtColor(img,img,CV_BGR2GRAY);
           *grayimage = QImage((const unsigned char*)(img.data),img.cols,img.rows, img.cols*img.channels(),  QImage::Format_Indexed8);

           temp = (float)grayimage->width()/(float)grayimage->height();
           ui->graylabel->setPixmap(QPixmap::fromImage(*grayimage));

            /*自适应框体*/
           if(grayimage->width()>= grayimage->height())
           {
               ui->graylabel->resize(ui->graywidget->width(),(int)(ui->graywidget->height()/temp));
           }
           else
           {
               ui->graylabel->resize((int)(ui->graywidget->width()*temp),ui->graywidget->height());
           }

        }
    }

}
/*-------------------------------- 显示计算直方图-----------------------------------------------*/
void hist::on_histbuttom_clicked()
{
    if(img.empty())
    {
        QMessageBox::information(this,tr("警告"),tr("请先打开图像！"));
        return ;
    }

    else
    {
        MatND hist;
        myCal_Hist(img,hist);
    }
}
void hist::myCal_Hist(Mat Gray_img,MatND hist)
{
    int bins = 256;
    int hist_size[] = {bins};
    float range[] = { 0, 256 };
    const float* ranges[] = { range};

    int channels[] = {0};
    //计算直方图
    calcHist( &Gray_img, 1, channels, Mat(), // do not use mask
        hist, 1, hist_size, ranges,
        true, // the histogram is uniform
        false );

    //绘制直方图图像
    int hist_height=256;
    //int bins = 256;
    double max_val;  //直方图的最大值
    int scale = 2;   //直方图的宽度
    minMaxLoc(hist, 0, &max_val, 0, 0); //计算直方图最大值

    Mat hist_img = Mat::zeros(hist_height,bins*scale, CV_8UC3); //创建一个直方图图像并初始化为0

    cout<<"max_val = "<<max_val<<endl;
    //在直方图图像中写入直方图数据
    for(int i=0;i<bins;i++)
    {
        float bin_val = hist.at<float>(i); // 第i灰度级上的数
        int intensity = cvRound(bin_val*hist_height/max_val);  //要绘制的高度
        //填充第i灰度级的数据
        rectangle(hist_img,Point(i*scale,hist_height-1),
            Point((i+1)*scale - 1, hist_height - intensity),
            CV_RGB(255,255,255));
    }

    imshow( "Gray Histogram", hist_img );

}
/*-------------------------------------直方图均衡----------------------------------------------*/
void hist::on_histeEqualize_clicked()
{
    if(img.empty())
    {
        QMessageBox::information(this,tr("警告"),tr("请先打开图像！"));
        return ;
    }

    else
    {
    float temp;             //图像width 和height的比例系数用于自适应框体
    Mat* tempimg;           //用于Mat->QImage的转换

    tempimg = new Mat;

    equalizeHist(img,eqlizehist);   //均衡,输出为eqlizehist
    *tempimg = eqlizehist;
    //转换格式
    *eqlizeimg = QImage((const unsigned char*)(tempimg->data),tempimg->cols,tempimg->rows, tempimg->cols*tempimg->channels(),  QImage::Format_Indexed8);

    temp = (float)eqlizeimg->width()/(float)eqlizeimg->height();
    ui->label_2->setPixmap(QPixmap::fromImage(*eqlizeimg));
     /*自适应框体*/
    if(grayimage->width()>= eqlizeimg->height())
    {
        ui->label_2->resize(ui->eqlizewidget->width(),(int)(ui->eqlizewidget->height()/temp));
    }
    else
    {
        ui->label_2->resize((int)(ui->eqlizewidget->width()*temp),ui->eqlizewidget->height());
    }
    delete tempimg;
    }

}

/*-----------------------------------显示均衡后的直方图------------------------------------------*/
void hist::on_showhist_clicked()
{
    if(img.empty())
    {
        QMessageBox::information(this,tr("警告"),tr("请先打开图像！"));
        return ;
    }

    else
    {
        MatND hist;
        myCal_Hist(eqlizehist,hist);
    }
}
