#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
class Setting;
class Game;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    void initSize();
    void showStartMenu();
    void optionSetting();
    void test();
    void startGame();
private:
    Setting* settingDialog;
    std::shared_ptr<Game> gameDialog;
};

#endif // MAINWINDOW_H
