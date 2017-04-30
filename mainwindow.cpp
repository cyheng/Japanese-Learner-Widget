#include "mainwindow.h"
#include "game.h"
#include <QApplication>
#include <QPushButton>
#include <QProgressBar>
#include <QTimeLine>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>

#include <settingdialog.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),settingDialog(new Setting(this))
{
    setWindowTitle(u8"50音日语学习");
    initSize();
    showStartMenu();

}

MainWindow::~MainWindow()
{

}

void MainWindow::initSize()
{
    resize(400,320);

}
void MainWindow::showStartMenu()
{
    QLabel *title=new QLabel(u8"50音日语学习",this);
    title->setStyleSheet("font-size:30px;");
    QPushButton *start=new QPushButton("Start",this);
    QPushButton *option=new QPushButton("Option",this);
    QPushButton *exit=new QPushButton("Exit",this);

    QVBoxLayout *mainlayout=new QVBoxLayout;
    QHBoxLayout *buttonlayout=new QHBoxLayout;
    mainlayout->addWidget(title,0,Qt::AlignCenter);//居中
    buttonlayout->addStretch();
    buttonlayout->addWidget(start);
    buttonlayout->addStretch();
    buttonlayout->addWidget(option);
    buttonlayout->addStretch();
    buttonlayout->addWidget(exit);
    buttonlayout->addStretch();
    buttonlayout->setContentsMargins(0,0,0,35);
    mainlayout->addLayout(buttonlayout);

    QWidget* widget = new QWidget(this);
    widget->setLayout(mainlayout);
    setCentralWidget(widget);

    connect(start,&QPushButton::clicked,this,&MainWindow::startGame);
    connect(exit,&QPushButton::clicked,this,&QWidget::close);
    connect(option,&QPushButton::clicked,this,&MainWindow::optionSetting);
}

void MainWindow::optionSetting()
{
    settingDialog->exec();
}

void MainWindow::test()
{

    QWidget *temp = new QWidget;
    temp->resize(400,320);
    QPushButton *button=new QPushButton(u8"测试",temp);
    QProgressBar *progressBar = new QProgressBar(temp);
    progressBar->setRange(0, 100);
    progressBar->setTextVisible(false);
    // 构造帧范围为 0 - 100，持续时间为 1000 毫秒（1 秒）的 timeline
    QTimeLine *timeLine = new QTimeLine(10000, temp);
    timeLine->setFrameRange(0, 100);
    timeLine->setCurveShape(QTimeLine::LinearCurve);
    QObject::connect(timeLine, SIGNAL(frameChanged(int)), progressBar, SLOT(setValue(int)));
    timeLine->setDirection(QTimeLine::Backward);
    // 输出当前帧数
    QObject::connect(timeLine, &QTimeLine::frameChanged, [=](int value) {
            qDebug() << value;
        });

    // 启动进度条动画
    QPushButton *startButton = new QPushButton(temp);
    startButton->setText(u8"开始");
    QObject::connect(startButton, SIGNAL(clicked()), timeLine, SLOT(start()));
    QObject::connect(timeLine,&QTimeLine::finished,[=](){
        button->setText(u8"再次测试");
        startButton->setText(u8"下一题");
        timeLine->stop();
    });
    QPushButton *backtostart = new QPushButton(u8"返回",temp);
    QVBoxLayout *layout = new QVBoxLayout(temp);
    layout->addWidget(button);
    layout->addWidget(progressBar);
    layout->addWidget(startButton);
    layout->addWidget(backtostart);
    temp->setLayout(layout);
    hide();
    connect(backtostart,&QPushButton::clicked,this,[=](){
        temp->close();
        this->show();
    });
    temp->show();
}

void MainWindow::startGame()
{
    gameDialog = std::make_shared<Game>(settingDialog,this);
    hide();
    gameDialog->show();
}

