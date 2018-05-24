#include "myphotoshop.h"
#include <QDebug>

MyPhotoShop::MyPhotoShop(FrameLessWidget *parent)
    : FrameLessWidget(parent)
{
    this->resize(800,600);
    DefaultSet();
    LayOutSet();
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
    if(this->GetMousePosInWindow().y() > this->GetTitleHeigit())
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
                QImage img = QImage((const unsigned char*)(image_raw_copy.data),image_raw_copy.cols,image_raw_copy.rows,QImage::Format_RGB888);
                m_pImgLabel->setPixmap(QPixmap::fromImage(img));
                first_in = false;//不再是第一次打开程序
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
                    QImage img = QImage((const unsigned char*)(image_raw_copy.data),image_raw_copy.cols,image_raw_copy.rows,QImage::Format_RGB888);
                    m_pImgLabel->setPixmap(QPixmap::fromImage(img));
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
    this->setWindowIcon(QIcon(":/Label_Image/V-Team"));
    m_pImgLabel = new QLabel;
    m_pImgLabel->setVisible(false);
    m_ContentLayOut = new QHBoxLayout;
    first_in = true;

}

void MyPhotoShop::LayOutSet()
{
    m_ContentLayOut->addWidget(m_pImgLabel);
    m_ContentLayOut->setAlignment(m_pImgLabel,Qt::AlignCenter);     //设置居中显示
    GetTitleTxtLabel()->setText("MyPhotoShop");                     //标题文字更改
    GetTitleUserLabel()->setText("<a style='color:rgb(153, 255, 102);' href=https://github.com/nicestar>Github</a>");                         //超链接文字更改
    GetContentWidget()->setLayout(m_ContentLayOut);                 //内容窗口设置布局
}


