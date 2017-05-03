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
#include <QButtonGroup>

//init the GUI widgets
Game::Game(Setting *s,QWidget *parent) : QMainWindow(parent),p(parent)
  ,pSetting(s)
{
    SetUpData();
    resize(400,320);
    InitGUIWidgets();
    setGUIProperty();
    setLayout();
    GameControl(parent);

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


void Game::GameControl(QWidget *parent)
{
    //设置倒计时
    connect(timeLine, SIGNAL(frameChanged(int)), progressBar, SLOT(setValue(int)));
    //timeout的时候显示答案
    connect(timeLine,&QTimeLine::finished,this,
            static_cast<void (Game::*)()>(&Game::showAnswers));
    //点击答案的时候，显示答案同时倒计时停止
    connect(group,static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),this,
            static_cast<void (Game::*)(int)>(&Game::showAnswers));
    //点击next的时候，显示问题倒计时开始
    connect(next, &QPushButton::clicked,this,&Game::showQuestion);
    //back to main menu
    connect(returnToMain,&QPushButton::clicked,parent,[=](){
        this->close();
        parent->show();
    });
    //TODO yinbiao.size()==0弹出QMessageBox
    connect(this,&Game::yinbiaoSizeChange,this,&Game::GameOver);
    /****
     *
     * 新版本
    一个新的private成员vioce
    点击vioce会发出声音(下一题的时候会hide这个button)
    *****/
}

void Game::setLayout()
{
    //set layout
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

    QWidget *widget =new QWidget(this);
    widget->setLayout(mainlayout);
    setCentralWidget(widget);
}

void Game::setGUIProperty()
{
    questionNum->setText(u8"还剩下");
    correctNum->setText(u8"正确次数");
    errorNum->setText(u8"错误次数");
    question->setText(u8"点击开始进行测试\n在倒计时结束之前点击答案。");
    qNum->setDigitCount(2);
    qNum->display(yinbiao.size());


    cNum->setDigitCount(2);
    cNum->display(0);

    eNum->setDigitCount(2);
    eNum->display(0);

    returnToMain->setText(u8"返回");
    progressBar->setValue(100);
    progressBar->setRange(0,100);
    progressBar->setTextVisible(false);

    group->addButton(answer1,0);
    group->addButton(answer2,1);
    group->addButton(answer3,2);
    timeLine->setFrameRange(0, 100);
    timeLine->setCurveShape(QTimeLine::LinearCurve);

    timeLine->setDirection(QTimeLine::Backward);
    next->setText(u8"开始");

}

void Game::InitGUIWidgets()
{
    question =new QLabel(this);
    answer1 = new QPushButton(this);
    answer2= new QPushButton(this);
    answer3= new QPushButton(this);
    questionNum = new QLabel(this);

    correctNum = new QLabel(this);
    errorNum = new QLabel(this);
    group = new QButtonGroup(this);

    qNum = new QLCDNumber(this);
    cNum = new QLCDNumber(this);
    eNum = new QLCDNumber(this);
    returnToMain = new QPushButton(this);

    progressBar = new QProgressBar(this);
    timeLine = new QTimeLine(pSetting->getLineSecond()*1000, this);
    next = new QPushButton(this);

}

void Game::showQuestion()
{

    //show first questions
    if(timeLine->state()!=QTimeLine::Running )
    {
    qsrand(QTime::currentTime().msec());
    trueAns=qrand()%yinbiao.size();
    question->setText(QString(u8"\"%1\"的罗马音").arg(yinbiao[trueAns].second));
    trueButtonId =qrand()%3;
    auto buttonlist = group->buttons();
    int lastError =-1;
    //show answers
    for(auto i:buttonlist)
    {

        if(trueButtonId ==group->id(i))
            i->setText(yinbiao[trueAns].first);
        else
        {
            //yinbiao.size()>2,不允许出现相同（A==B==C）的选项
            int size=yinbiao.size();
            int errorAns=qrand()%size;
            while(size>2 && (errorAns==trueAns || errorAns==lastError) )
                    errorAns=qrand()%size;//A!=B A!=C
            lastError =errorAns;//B!=C
           i->setText(yinbiao[errorAns].first);
        }
    }
    }
    timeLine->start();
}

void Game::showAnswers(int id)
{
    qDebug()<<"Now State:"<<timeLine->state();
    if(timeLine->state()!=QTimeLine::Paused)
    {
    question->setText(QString(u8"正确答案是%1").arg(yinbiao[trueAns].first));
    qDebug()<<"Delete Index:"<<trueAns;
    yinbiao.remove(trueAns);
    qNum->display(yinbiao.size());
    qDebug()<<"id"<<id;
    if(id!=trueButtonId)
        eNum->display(++error);
    else
        cNum->display(++correct);
    }
    timeLine->setPaused(true);
    next->setText(u8"下一题");
    emit yinbiaoSizeChange(yinbiao.size());
}

void Game::showAnswers()
{
    showAnswers(-1);
}

void Game::GameOver(int size)
{
    qDebug()<<"size:"<<size;
    if(size==0)
    {
        auto res =QMessageBox::information(this,"finished Game",u8"您已经完成所有联系！\n你想再来一遍吗？",
                                 QMessageBox::Yes|QMessageBox::No);
        if(res==QMessageBox::No)
            close();
        else
        {
            SetUpData();
            setGUIProperty();
        }
    }
}


