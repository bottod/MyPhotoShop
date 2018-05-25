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
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>

class MyPhotoShop : public FrameLessWidget
{
    Q_OBJECT

public:
    MyPhotoShop(FrameLessWidget *parent = 0);
    void DefaultSet();              //初始化声明创建变量
    void LayOutSet();               //重新布局
    void BindAction();              //绑定动作到控件
    void GetColor();                //得到颜色信息
    ~MyPhotoShop();
private slots:
    void onOpenActionTrigged();     //打开文件的动作触发
    void onQuitActionTrigged();     //退出程序的动作触发
    void onGrayActionTrigged();     //灰度图动作触发
    void onThreshActionTrigged();   //二值图动作触发
    void onMakeActionTrigged();     //用户需求动作触发

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);//鼠标响应事件
    virtual void mouseMoveEvent(QMouseEvent *event);
private:
    QLabel * m_pImgLabel;               //用于显示图片
    QHBoxLayout * m_H_ContentLayOut;    //内容水平布局
    QVBoxLayout * m_V_ContentLayOut;    //内容竖直布局
    cv::Mat image_raw,image_raw_copy;   //图片(原图)
    cv::Mat image_gray;                 //图片(灰度)
    cv::Mat image_Thresh;               //图片(二值)
    cv::Mat image_Cicle;                //图片（用户需求）
    bool first_in;                      //第一次打开软件
    QToolBar *m_ModifyToolBar;          //快捷功能工具栏

    QAction *m_open_Action;             //打开文件的动作
    QAction *m_close_Action;            //关闭程序的动作
    QAction *m_Gray_Action;             //灰度处理
    QAction *m_Thresh_Action;           //二值化处理(定值)
    QAction * m_Make_Action;            //处理（用户需求）

    QMenuBar * m_pMainMenuBar;          //主菜单栏
    QMenu * m_pFileMenu;                //文件操作菜单
    QMenu * m_pModifyMenu;              //修改图片操作
    QWidget * m_TipBottomWidget;        //底部提示窗口
    QWidget *m_TipColorWidget;          //底部提示颜色信息窗口
    QLabel *m_TipBottomColorLabel;       //底部提示颜色label
    QLabel *m_TipBottomLabel;           //底部提示label
    QVBoxLayout *tiplayout;             //底部窗口布局
    QVBoxLayout *tipColorLayout;        //底部颜色信息窗口布局
    QString m_tipLabelinfo;             //label提示消息
    QString m_tipColorLabelinfo;        //label颜色信息提示

    int TipWigetHeigt;                  //底部提示信息高度
};

#endif // MYPHOTOSHOP_H
