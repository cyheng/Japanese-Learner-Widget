#include "settingdialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QWidget>
#include <QSpinBox>
#include <mainwindow.h>
#include <QLabel>
#include <QDebug>
Setting::Setting(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle("Setting");
    QRadioButton *pingButton = new QRadioButton(u8"平假名",this);
    QRadioButton *pianButton = new QRadioButton(u8"片假名",this);
    pingButton->setChecked(true);

    QButtonGroup *buttonGroup = new QButtonGroup;
    buttonGroup->setExclusive(true);//互斥
    buttonGroup->addButton(pingButton);
    buttonGroup->addButton(pianButton);
    buttonGroup->setId(pingButton,HI_DIR);
    buttonGroup->setId(pianButton,KA_DIR);

    QHBoxLayout *radioLayout = new QHBoxLayout;
    radioLayout->addWidget(pingButton);
    radioLayout->addWidget(pianButton);

    QLabel *timeSetText = new QLabel(u8"每道问题的倒计时秒数",this);
    QSpinBox *timeSet = new QSpinBox(this);
    timeSet->setRange(0, 10);  // 范围
    timeSet->setSingleStep(1);  // 步长
    timeSet->setSuffix("s");  // 前缀
    timeSet->setValue(4);  // 当前值
    QHBoxLayout *timeLayout = new QHBoxLayout;
    timeLayout->addWidget(timeSetText);
    timeLayout->addWidget(timeSet);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(radioLayout);
    mainLayout->addLayout(timeLayout);

    connect(timeSet,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,[=](int value)
    {
        timeLineSecond=value;
    });
    connect(buttonGroup,static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),this,
            [=](int id){
        defaultFront =id;
    });
}

int Setting::getLineSecond() const
{
    return timeLineSecond;
}

int Setting::getFront() const
{
    return defaultFront;
}
