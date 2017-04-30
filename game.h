#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QString>
#include <QPair>
#include <QVector>
#include <QLCDNumber>
#include <QPushButton>
#include <QLabel>
#include <QTimeLine>
class Setting;
class Game : public QMainWindow
{
    Q_OBJECT
public:
    explicit Game(Setting *s,QWidget *parent = 0);
    void ReadData(const QString &name);
    void SetUpData();
    void LoadSetting(Setting *s);
    void GameOver();
    void GameControl();
private:
    enum {HI_DIR,KA_DIR};
    QVector<QPair<QString,QString>> yinbiao;
    QWidget *p;
    QLabel *question;
    QPushButton *answer1;
    QPushButton *answer2;
    QPushButton *answer3;
    QLCDNumber *qNum;
    QLCDNumber *cNum;
    QLCDNumber *eNum;
    QPushButton *next;
    Setting *pSetting;
};

#endif // GAME_H
