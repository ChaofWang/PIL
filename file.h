#ifndef FILE_H
#define FILE_H

#include <QMainWindow>

namespace Ui {
class file;
}

class file : public QMainWindow
{
    Q_OBJECT

public:
    explicit file(QWidget *parent = 0);
    ~file();

private slots:
    void on_actionZhifangtu_triggered();

    void on_actionGeo_trans_triggered();

    void on_actionFliter_triggered();

    void on_actionHomomorphicFilter_triggered();

    void on_actionImg_seg_triggered();

private:
    Ui::file *ui;
};

#endif // FILE_H
