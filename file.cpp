#include "file.h"
#include "ui_file.h"
#include "hist.h"
#include "geo_trans.h"
#include "filter.h"
#include "homomorphicfilter.h"
#include "image_segmentation.h"
file::file(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::file)
{
    ui->setupUi(this);
}

file::~file()
{

    delete ui;
}

void file::on_actionZhifangtu_triggered()
{
    hist *HistWindow = new hist();
    HistWindow->show();
    HistWindow->exec();
    delete HistWindow;
}

void file::on_actionGeo_trans_triggered()
{
    geo_trans *GeoWindow = new geo_trans();
    GeoWindow->show();
    GeoWindow->exec();
    delete GeoWindow;
}

void file::on_actionFliter_triggered()
{
    filter *FilterWindow = new filter();
    FilterWindow->show();
    FilterWindow->exec();
    delete FilterWindow;
}

void file::on_actionHomomorphicFilter_triggered()
{
    HomomorphicFilter *homoWindow = new HomomorphicFilter();
    homoWindow->show();
    homoWindow->exec();
    delete homoWindow;
}

void file::on_actionImg_seg_triggered()
{
    image_segmentation *img_segWindow = new image_segmentation();
    img_segWindow->show();
    img_segWindow->exec();
    delete img_segWindow;
}
