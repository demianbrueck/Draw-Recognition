#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPoint>
#include <vector>
#include <string.h>
#include <QDebug>
#include <QWheelEvent>
#include <QThread>
#include <fstream>
#include <cstring>


#define RECOGNITION_POINTS_AMOUNT 10
#define LETTER_S 0
#define LETTER_O1 20
#define LETTER_O2 40
#define LETTER_L1 60
#define LETTER_C 80
#define LETTER_L2 100
#define LETTER_I 120
#define LETTER_G 140
#define LETTER_J 160
#define LETTER_M 180
#define LETTER_N 200
#define LETTER_R 220
#define LETTER_U 240
#define LETTER_V 260
#define LETTER_W 280
#define LETTER_Z 300

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    struct Input{

        int pos_x = 99999;
        int pos_y = 99999;

        bool leftButtonDown;
    };

    struct Pos{
        QPoint topLeft;
        QPoint bottomRight;
        QRectF rect;
    };

    QVector<QPoint> m_Line;
    Input mouseInput;
    Pos MaxPos;
    QString f;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QPoint numDegrees;
    QVector<QPoint> recognitionPoints;
    struct Percentage{
        float percentageFromTop;
        float percentageFromLeft;

    };

    //QVector<Percentage>

    QVector<Percentage> recognitionPointsPercentage;
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;


public slots:
    void onTimer();

private:
    QPoint mousePos;
    Ui::MainWindow *ui;
    QPainter * m_pPainter;
    int numberPoints = 0;
    int mouseWheel = 0;
    char color;
    int sizeBrush = 10;
    int i = 0;
    int v  = 0;
    void getRecognitionNumbers();
    int recognitionPointPlace;
    float comparePoints(int);
    QVector<float> databasePoint;
    float totalDifference;
    int databaseLength;
    int j = 0;
    char getMostSimmilarLetter();
};
#endif // MAINWINDOW_H
