#include "myphotoshop.h"
#include <QGuiApplication>
#include<QApplication>
#include <QDesktopWidget>
#include <QList>
#include <QScreen>
#include <QDebug>

MyPhotoShop::MyPhotoShop(FrameLessWidget *parent)
    : FrameLessWidget(parent)
{
    this->resize(1200,900);
    DefaultSet();
    LayOutSet();
    BindAction();
}

MyPhotoShop::~MyPhotoShop()
{

}

//重写鼠标相应事件，如果在标题栏双击按照父窗口消息处理 否则自行处理
void MyPhotoShop::mouseDoubleClickEvent(QMouseEvent *event)
{
    //由于是双窗口 上面还有一个标题栏窗口 所以相应的控件窗口位置要下移标题栏窗口高度个单位
    QRect m_rect_raw = m_pImgLabel->geometry();             //相对于主窗口 全部
    //相对于子窗口 内容窗口
    QRect m_rect_modify = QRect(m_rect_raw.x(),m_rect_raw.y()+GetTitleHeigit(),m_rect_raw.width(),m_rect_raw.height());
    //如果鼠标点在标题栏下方
    if(this->GetMousePosInWindow().y() > this->GetTitleHeigit() && this->GetMousePosInWindow().y() < (this->height() - TipWigetHeigt))
    {
        //如果是第一次打开程序
        if(first_in)
        {
            QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),".",tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
            //cv::Mat image = cv::imread(fileName.toLatin1().data());//路径只能是英文

            //路径可以包含中文
            QTextCodec *code = QTextCodec::codecForName("gb2312");
            std::string name = code->fromUnicode(fileName).data();
            if(!name.empty())//如果有选择图片则执行
            {
                if(!m_pImgLabel->isVisible())
                    m_pImgLabel->setVisible(true);//显示label
                image_raw = cv::imread(name);
                cv::cvtColor(image_raw,image_raw_copy,CV_BGR2RGB);
                QImage img = QImage((const unsigned char*)(image_raw_copy.data),image_raw_copy.cols,image_raw_copy.rows,image_raw_copy.cols*image_raw_copy.channels(),QImage::Format_RGB888);
                m_pImgLabel->setPixmap(QPixmap::fromImage(img));
                first_in = false;//不再是第一次打开程序

                m_tipLabelinfo = "Open File Success !!! Filename:   " + QString::fromStdString(name);
                m_TipBottomLabel->setText(m_tipLabelinfo);

                image_gray.data=0;
                image_Thresh.data=0;
            }
        }
        else
        {
            //不在图片label里才能响应鼠标事件
            if(!m_rect_modify.contains(this->mapFromGlobal(QCursor::pos())))
            {
                QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),".",tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
                //cv::Mat image = cv::imread(fileName.toLatin1().data());//路径只能是英文

                //如下 路径可以是任何
                QTextCodec *code = QTextCodec::codecForName("gb2312");
                std::string name = code->fromUnicode(fileName).data();
                if(!name.empty())//如果有选择图片则执行
                {
                    image_raw = cv::imread(name);
                    cv::cvtColor(image_raw,image_raw_copy,CV_BGR2RGB);
                    QImage img = QImage((const unsigned char*)(image_raw_copy.data),image_raw_copy.cols,image_raw_copy.rows,image_raw_copy.cols*image_raw_copy.channels(),QImage::Format_RGB888);
                    m_pImgLabel->setPixmap(QPixmap::fromImage(img));

                    m_tipLabelinfo = "Open File Success !!! Filename:   " + QString::fromStdString(name);
                    m_TipBottomLabel->setText(m_tipLabelinfo);

                    image_gray.data=0;
                    image_Thresh.data=0;
                }
            }
        }
    }
    else
    {
        FrameLessWidget::mouseDoubleClickEvent(event);
    }
    return QWidget::mouseDoubleClickEvent(event);
}

//初始化声明创建变量
void MyPhotoShop::DefaultSet()
{
    TipWigetHeigt = 50;//底部提示信息高度
    m_tipLabelinfo = "Waitting ................";
    m_tipColorLabelinfo = "wait for image opening ......";
    this->setWindowIcon(QIcon(":/Label_Image/V-Team"));
    m_pImgLabel = new QLabel;
    //m_pImgLabel->setScaledContents(true);//禁止缩放防止图片变形
    m_pImgLabel->setVisible(false);
    m_H_ContentLayOut = new QHBoxLayout;
    m_V_ContentLayOut = new QVBoxLayout;
    first_in = true;

    m_ModifyToolBar = new QToolBar("handle");

    m_open_Action = new QAction(QIcon(":/Tool_Image/FileOpen"),tr("Open File .."));//悬浮提示信息
    m_open_Action->setShortcuts(QKeySequence::Open);
    m_close_Action = new QAction(QIcon(":/Tool_Image/Quit"),tr("Quit .."));//悬浮提示信息
    m_close_Action->setShortcuts(QKeySequence::Close);
    m_Gray_Action = new QAction(QIcon(":/Tool_Image/Gray"),tr("Gray .."));//悬浮提示信息
    m_Thresh_Action = new QAction(QIcon(":/Tool_Image/Threshold"),tr("Threshold .."));//悬浮提示信息
    m_Make_Action = new QAction(QIcon(":/Tool_Image/Make"),tr("Make .."));//悬浮提示信息

    m_ModifyToolBar->addAction(m_open_Action);
    m_ModifyToolBar->addAction(m_Gray_Action);
    m_ModifyToolBar->addAction(m_Thresh_Action);
    m_ModifyToolBar->addAction(m_Make_Action);
    m_ModifyToolBar->setOrientation(Qt::Vertical);

    m_pMainMenuBar = new QMenuBar;m_pMainMenuBar->setObjectName("MainMenuBar");
    m_pFileMenu = new QMenu("file");m_pFileMenu->setObjectName("FileMenu");
    m_pModifyMenu = new QMenu("modify");m_pModifyMenu->setObjectName("FileModifyMenu");

    m_TipBottomWidget = new QWidget;m_TipBottomWidget->setObjectName("TipBottomWidget");
    m_TipBottomLabel = new QLabel;m_TipBottomLabel->setObjectName("TipBottomLabel");
    tiplayout = new QVBoxLayout;//底部信息窗口布局
    m_TipBottomLabel->setText(m_tipLabelinfo);

    m_TipColorWidget = new QWidget;m_TipColorWidget->setObjectName("TipColorBottomWidget");
    m_TipBottomColorLabel = new QLabel;m_TipBottomColorLabel->setObjectName("TipColorBottomLabel");
    tipColorLayout = new QVBoxLayout;//底部颜色信息窗口布局
    m_TipBottomColorLabel->setText(m_tipColorLabelinfo);

    //需要顺序设置，少一个都不能在不按下鼠标的情况下触发鼠标移动事件
    this->setMouseTracking(true);
    GetContentWidget()->setMouseTracking(true);
    m_pImgLabel->setMouseTracking(true);
}

void MyPhotoShop::LayOutSet()
{
    //菜单栏添加菜单
    m_pMainMenuBar->addMenu(m_pFileMenu);
    m_pMainMenuBar->addMenu(m_pModifyMenu);
    m_pFileMenu->addAction(m_open_Action);
    m_pFileMenu->addAction(m_close_Action);
    m_pModifyMenu->addAction(m_Gray_Action);
    m_pModifyMenu->addAction(m_Thresh_Action);
    m_pModifyMenu->addAction(m_Make_Action);

    m_pMainMenuBar->setFixedHeight(40);//限高40
    m_V_ContentLayOut->setSpacing(0);
    m_V_ContentLayOut->setMargin(0);

    m_V_ContentLayOut->addWidget(m_pMainMenuBar);

    m_H_ContentLayOut->addWidget(m_ModifyToolBar);
    m_H_ContentLayOut->setAlignment(m_ModifyToolBar,Qt::AlignLeft);     //设置居左显示
    m_H_ContentLayOut->addWidget(m_pImgLabel);
    m_H_ContentLayOut->setAlignment(m_pImgLabel,Qt::AlignCenter);        //设置居中显示

    GetTitleTxtLabel()->setText("MyPhotoShop");                         //标题文字更改
    GetTitleUserLabel()->setText("<a style='color:rgb(153, 255, 102);' href=https://github.com/nicestar>Github</a>");                         //超链接文字更改

    m_V_ContentLayOut->addLayout(m_H_ContentLayOut);

    //底部窗口布局
    m_TipBottomWidget->setFixedHeight(TipWigetHeigt);
    tiplayout->addWidget(m_TipBottomLabel);
    m_TipBottomWidget->setLayout(tiplayout);
    m_V_ContentLayOut->addWidget(m_TipBottomWidget);
    //底部颜色窗口布局
    m_TipColorWidget->setFixedHeight(TipWigetHeigt);
    tipColorLayout->addWidget(m_TipBottomColorLabel);
    m_TipColorWidget->setLayout(tipColorLayout);
    m_V_ContentLayOut->addWidget(m_TipColorWidget);

    GetContentWidget()->setLayout(m_V_ContentLayOut);                 //内容窗口设置布局
}

//打开文件的动作触发
void MyPhotoShop::onOpenActionTrigged()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),".",tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
    //cv::Mat image = cv::imread(fileName.toLatin1().data());//路径只能是英文

    //路径可以包含中文
    QTextCodec *code = QTextCodec::codecForName("gb2312");
    std::string name = code->fromUnicode(fileName).data();
    if(!name.empty())//如果有选择图片则执行
    {
        if(!m_pImgLabel->isVisible())
            m_pImgLabel->setVisible(true);//显示label
        image_raw = cv::imread(name);
        cv::cvtColor(image_raw,image_raw_copy,CV_BGR2RGB);
        //加上image_raw_copy.cols*image_raw_copy.channels()修复了有些图片显示错位问题 数据对齐的问题
        QImage img = QImage((const unsigned char*)(image_raw_copy.data),image_raw_copy.cols,image_raw_copy.rows,image_raw_copy.cols*image_raw_copy.channels(),QImage::Format_RGB888);
        m_pImgLabel->setPixmap(QPixmap::fromImage(img));
        first_in = false;//不再是第一次打开程序

        m_tipLabelinfo = "Open File Success !!! Filename:   " + QString::fromStdString(name);
        m_TipBottomLabel->setText(m_tipLabelinfo);

        image_gray.data=0;
        image_Thresh.data=0;
    }
}

//退出程序的动作触发
void MyPhotoShop::onQuitActionTrigged()
{
    this->close();
}

//灰度图动作触发
void MyPhotoShop::onGrayActionTrigged()
{
    if(image_raw_copy.data)
    {
        cv::cvtColor(image_raw_copy,image_gray,CV_BGR2GRAY);
        QImage img = QImage((const unsigned char*)(image_gray.data),image_gray.cols,image_gray.rows,image_gray.cols*image_gray.channels(),QImage::Format_Indexed8);
        m_pImgLabel->setPixmap(QPixmap::fromImage(img));
        m_tipLabelinfo = "Grayscale Operation Successfully !!! ";
        m_TipBottomLabel->setText(m_tipLabelinfo);
        image_raw_copy.data = 0;//处理完赋值为0避免下次还保留着上一次的缓存
    }
    else
    {
        m_tipLabelinfo = "Warning !!!  Need A Image First !!! ";
        m_TipBottomLabel->setText(m_tipLabelinfo);
    }
}

//二值图动作触发
void MyPhotoShop::onThreshActionTrigged()
{
    if(image_gray.data)
    {
        cv::threshold(image_gray,image_Thresh,150,255,CV_THRESH_BINARY);
        QImage img = QImage((const unsigned char*)(image_Thresh.data),image_Thresh.cols,image_Thresh.rows,image_Thresh.cols*image_Thresh.channels(),QImage::Format_Grayscale8);
        m_pImgLabel->setPixmap(QPixmap::fromImage(img));
        m_tipLabelinfo = "Threshold Operation Successfully !!! ";
        m_TipBottomLabel->setText(m_tipLabelinfo);
        image_gray.data = 0;
    }
    else
    {
        m_tipLabelinfo = "Warning !!!  Need Grayscale Operation First !!! ";
        m_TipBottomLabel->setText(m_tipLabelinfo);
    }
}

//用户需求动作触发
void MyPhotoShop::onMakeActionTrigged()
{
    //记录值 0为黑色 255为白色

    //改变后 0 black 1 white

    //x=40+r*cosa;
    //y=40-r*sina;
    if(image_Thresh.data)
    {
        image_Cicle = image_Thresh;
        QFile thepart("./output/ThePart");
        thepart.open(QFile::ReadWrite);
        QTextStream thepartstream(&thepart);
        cv::resize(image_Cicle,image_Cicle,cv::Size(80,80));
        int tempnum = 0;
        for(double rad = 0;rad<360;rad++)//角度
        {
            //y的取值一直是40 x在41到80;(起始位置)根据起始位置变换
            for(int step = 0;step<40;step++)
            {
                int valuex = static_cast<int>(40 + step * cos(rad/180.0*3.1415926));
                int valuey = static_cast<int>(40 - step * sin(rad/180.0*3.1415926));
                if(image_Cicle.at<uchar>(valuex,valuey) == 255)
                    tempnum = 1;
                if(image_Cicle.at<uchar>(valuex,valuey) == 0)
                    tempnum = 0;
                thepartstream<<tempnum;
            }
            thepartstream<<endl;
        }
        cv::Mat imgres;
        cv::resize(image_Cicle,imgres,cv::Size(image_raw.cols,image_raw.rows));//size函数 cols>rows
        QImage img = QImage((const unsigned char*)(imgres.data),imgres.cols,imgres.rows,imgres.cols*imgres.channels(),QImage::Format_Grayscale8);
        m_pImgLabel->setPixmap(QPixmap::fromImage(img));
        thepart.close();
        m_tipLabelinfo = "Make Operation Successfully !!! ";
        m_TipBottomLabel->setText(m_tipLabelinfo);
        image_Thresh.data = 0;
    }
    else
    {
        m_tipLabelinfo = "Warning !!!  Need Threshold Operation First !!! ";
        m_TipBottomLabel->setText(m_tipLabelinfo);
    }
}

//得到图片颜色信息
void MyPhotoShop::GetColor()
{
    //先判断m_pImgLabel窗口是否是显示的
    if(m_pImgLabel->isVisible())
    {
        QRect m_rect_raw = m_pImgLabel->geometry();             //相对于主窗口 全部
        //相对于子窗口 内容窗口
        QRect m_rect_modify = QRect(m_rect_raw.x(),m_rect_raw.y()+GetTitleHeigit(),m_rect_raw.width(),m_rect_raw.height());

        //在图片里移动时
        if(m_rect_modify.contains(this->mapFromGlobal(QCursor::pos())))
        {
            QPoint point = this->mapFromGlobal(QCursor::pos());
            QPoint point_Modify = QPoint(point.x()-m_rect_modify.x(),point.y()-m_rect_modify.y());
            QPixmap pixmap;
            pixmap = *(m_pImgLabel->pixmap());
            QColor color = QColor(pixmap.toImage().pixel(point_Modify));
            m_tipColorLabelinfo = QString("R: %1, G: %2, B: %3, || X: %4, Y: %5").arg(color.red()).arg(color.green()).arg(color.blue()).arg(point_Modify.x()).arg(point_Modify.y());
            m_TipBottomColorLabel->setText(m_tipColorLabelinfo);
        }
    }
}

void MyPhotoShop::mouseMoveEvent(QMouseEvent *event)
{
    GetColor();
    return FrameLessWidget::mouseMoveEvent(event);
}

//绑定动作到控件
void MyPhotoShop::BindAction()
{
    connect(m_open_Action,&QAction::triggered,this,&MyPhotoShop::onOpenActionTrigged);
    connect(m_close_Action,&QAction::triggered,this,&MyPhotoShop::onQuitActionTrigged);
    connect(m_Gray_Action,&QAction::triggered,this,&MyPhotoShop::onGrayActionTrigged);
    connect(m_Thresh_Action,&QAction::triggered,this,&MyPhotoShop::onThreshActionTrigged);
    connect(m_Make_Action,&QAction::triggered,this,&MyPhotoShop::onMakeActionTrigged);
}
