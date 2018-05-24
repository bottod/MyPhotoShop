#ifndef MYPHOTOSHOP_H
#define MYPHOTOSHOP_H

#include "framelesswidget.h"
#include <QString>
#include <QTextCodec>
#include <QImage>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "opencv/highgui.h"
#include <QFileDialog>
#include <QMenu>

class MyPhotoShop : public FrameLessWidget
{
    Q_OBJECT

public:
    MyPhotoShop(FrameLessWidget *parent = 0);
    void DefaultSet();              //初始化声明创建变量
    void LayOutSet();               //重新布局
    ~MyPhotoShop();
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);//鼠标响应事件
private:
    QLabel * m_pImgLabel;               //用于显示图片
    QHBoxLayout * m_ContentLayOut;      //内容布局
    cv::Mat image_raw,image_raw_copy;   //图片(原图)
    bool first_in;                      //第一次打开软件
};

#endif // MYPHOTOSHOP_H
