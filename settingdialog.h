#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H


#include <QDialog>

class Setting : public QDialog
{

public:
    Setting(QWidget *parent=0);
public:
    enum {HI_DIR,KA_DIR};
    int getLineSecond()const;
    int getFront()const;
private:
    int timeLineSecond =4;
    int defaultFront =HI_DIR;
};

#endif // SETTINGDIALOG_H
