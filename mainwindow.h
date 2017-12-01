#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void slotPrintText();
    void slotPrintTextExact();
    void warning(QString &warn,int time);
    double getDeparture();
    double getLanding();
    int getMinQueue();
    int getMaxQueue();
    int getTime();
    int getQueue();
    void appendToText(QString &text);
    void appendToExactText(QString &text);
    void appendToDetailText(QString &text);
    void appendToDetailExactText(QString &text);
    void slotPlot();

    bool isNotExactWay();
    bool isExactWay();
    bool isTwoRunWay();
    bool isTwoPriorWay();
    bool isThreeWay();
    bool isNewOneWay();

    void updateFuelNum(int state);
    int getFuelNum();
    void getLandVector(QVector<int>& toland);
    void getDepartVector(QVector<int>& todepart);
    void turnText();


private:
    Ui::MainWindow *ui;
    QHBoxLayout *layout;
    QLabel *label;
    QSpinBox *spinBox;

};

#endif // MAINWINDOW_H
