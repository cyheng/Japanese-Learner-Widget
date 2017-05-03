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
#include <QProgressBar>
class Setting;
class Game : public QMainWindow
{
    Q_OBJECT
public:
    explicit Game(Setting *s,QWidget *parent = 0);
    void ReadData(const QString &name);
    void SetUpData();
    void LoadSetting(Setting *s);
    void GameControl(QWidget *parent);
    void setLayout();
    void setGUIProperty();
    void InitGUIWidgets();
    void showQuestion();
    void showAnswers(int id);
    void showAnswers();
signals:
    void yinbiaoSizeChange(int);
public slots:
    void GameOver(int size);
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
    QProgressBar *progressBar;
    QTimeLine *timeLine;
    QLabel *questionNum;
    QLabel *correctNum;
    QLabel *errorNum;
    QPushButton *returnToMain;
    Setting *pSetting;
    QButtonGroup *group;
    int trueButtonId;
    int trueAns;
    int correct=0;
    int error=0;
};

#endif // GAME_H
