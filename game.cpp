#include "game.h"
#include "settingdialog.h"
#include <QTextStream>
#include <QPushButton>
#include <QProgressBar>
#include <QLCDNumber>
#include <QLabel>
#include <QTimeLine>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <mainwindow.h>
#include <QTime>
#include <QTextCodec>
Game::Game(Setting *s,QWidget *parent) : QMainWindow(parent),p(parent)
  ,pSetting(s),question(new QLabel(this)),answer1( new QPushButton(this))
  ,answer2( new QPushButton(this)),answer3( new QPushButton(this))
{
    SetUpData();
    resize(400,320);
    QLabel *questionNum = new QLabel(u8"还剩下",this);
    QLabel *correctNum = new QLabel(u8"正确",this);
    QLabel *errorNum = new QLabel(u8"错误次数",this);
    qNum = new QLCDNumber(this);
    qNum->setDigitCount(2);
    qNum->display(yinbiao.size());
    cNum = new QLCDNumber(this);
    cNum->setDigitCount(2);
    cNum->display(0);
    eNum = new QLCDNumber(this);
    eNum->setDigitCount(2);
    eNum->display(0);
    QPushButton *returnToMain = new QPushButton(u8"返回",this);
    connect(returnToMain,&QPushButton::clicked,parent,[=](){
        this->close();
        parent->show();
    });



    QProgressBar *progressBar = new QProgressBar(this);
    progressBar->setValue(100);
    progressBar->setRange(0,100);
    progressBar->setTextVisible(false);
    qDebug()<<pSetting->getLineSecond()*1000;
    QTimeLine *timeLine = new QTimeLine(pSetting->getLineSecond()*1000, this);
    timeLine->setFrameRange(0, 100);
    timeLine->setCurveShape(QTimeLine::LinearCurve);
    connect(timeLine, SIGNAL(frameChanged(int)), progressBar, SLOT(setValue(int)));
    timeLine->setDirection(QTimeLine::Backward);

    next = new QPushButton(u8"开始",this);

    question->setText(u8"\"あ\"的罗马音");
    answer1->setText(u8"あ");
    answer2->setText(u8"い");
    answer3->setText(u8"う");

    QGridLayout *mainlayout = new QGridLayout;


    mainlayout->addWidget(question,0,0,3,3);
    mainlayout->addWidget(progressBar,2,0,4,3);
    mainlayout->addWidget(answer1,4,0,2,1);
    mainlayout->addWidget(answer2,4,1,2,1);
    mainlayout->addWidget(answer3,4,2,2,1);
    mainlayout->addWidget(next,6,2);

    mainlayout->addWidget(questionNum,0,3);
    mainlayout->addWidget(qNum,1,3);
    mainlayout->addWidget(correctNum,2,3);
    mainlayout->addWidget(cNum,3,3);
    mainlayout->addWidget(errorNum,4,3);
    mainlayout->addWidget(eNum,5,3);
    mainlayout->addWidget(returnToMain,6,3);

    connect(next, &QPushButton::clicked, timeLine, &QTimeLine::start);
    connect(timeLine,&QTimeLine::finished,this,&Game::GameOver);

    QWidget *widget =new QWidget(this);
    widget->setLayout(mainlayout);
    setCentralWidget(widget);

}

void Game::ReadData(const QString &name)
{
    QFile data(name);
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        QString line;
        QVector<QPair<QString,QString>> wordsvec;
        while(!in.atEnd())
        {
            line =in.readLine();
            QTextStream lines(&line);
            QVector<QString> temp;
            QString word;
            while(!lines.atEnd())
            {
                lines>>word;
                if(word!="")
                 temp.push_back(word);
            }
            wordsvec.push_back(qMakePair(temp[0],temp[1]));
        }
        yinbiao =wordsvec;

    }else
        QMessageBox::critical(this,"error",u8"不能读取文件!");
}

void Game::SetUpData()
{
  if(pSetting->getFront() == HI_DIR)
      ReadData(":/dir/hi_txt");
  else
      ReadData(":/dir/ka_txt");
}

void Game::GameOver()
{
    if(yinbiao.size()==0)
    {
        int res =QMessageBox::information
                (this,"Done!",u8"你已经完成所以训练，你想要再来一次吗？",QMessageBox::Yes| QMessageBox::No);
        if(QMessageBox::Yes==res)
                ;
        else
            {
                close();
                p->show();
             }

    }else
    {
        qsrand(QTime::currentTime().msec());
        qDebug()<<qrand()%yinbiao.size();
        next->setText(u8"下一题");
    }
}

void Game::GameControl()
{
    qsrand(QTime::currentTime().msec());
    int trueAns=qrand()%yinbiao.size();
    //3个以上
    //question会显示正确答案的罗马音
    //一个新的private成员vioce
    //点击vioce会发出声音(下一题的时候会delete这个button)
    //用户点击的button加深阴影
    //正确的button显示绿色
    //2个以下
    //虽然很蠢，不过照旧吧...
}

