#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainterPath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setMouseTracking(true);
    ui->setupUi(this);

    m_pPainter = new QPainter(this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->start(1);

    std::ifstream databaseIn("database.txt");
    if(databaseIn.is_open()){
        std::string s;
        while(std::getline(databaseIn,s)){
            //qDebug()<< QString::fromStdString(s);

            databasePoint.append(std::stof(s));
        }
    }


    MaxPos.topLeft.setX(9999);
    MaxPos.topLeft.setY(9999);
    MaxPos.bottomRight.setX(-1);
    MaxPos.bottomRight.setY(-1);


}
void MainWindow::keyPressEvent(QKeyEvent * event)
{
    switch( event->key() ){
    case 'R': color = 'R'; break;
    case 'G': color = 'G'; break;
    case 'Y': color = 'Y'; break;
    case 'B': color = 'B'; break;
    case 'D': color = 'D'; break;

    }

    QString s = QString("Key: %1").arg((char)event->key());
    qDebug(s.toLatin1());
    update();

}
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    QPoint p;

    p.setX(event->x());
    p.setY(event->y());

    m_Line.append(p);
    update();



    //qDebug() << "Mouse move event..";
    update();

}
void MainWindow::mousePressEvent(QMouseEvent *event){
    QPoint p;
    p.setX(event->x());
    p.setY(event->y());
    m_Line.append(p);
    update();



}
void MainWindow::mouseReleaseEvent(QMouseEvent *event){

    getRecognitionNumbers();
    //qDebug() << comparePoints(0);
    qDebug() << getMostSimmilarLetter();
    m_Line.clear();
    mousePos.setX(event->x());
    mousePos.setY(event->y());
    recognitionPointsPercentage.clear();

    MaxPos.topLeft.setX(9999);
    MaxPos.topLeft.setY(9999);
    MaxPos.bottomRight.setX(-1);
    MaxPos.bottomRight.setY(-1);

    update();

}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::black);

    //painter.drawPoint(mouseInput.pos_x,mouseInput.pos_y);
    //qDebug() << "Paint Event..";

    QVectorIterator<QPoint> i(m_Line);


    // qDebug() << mousePos;
    while (i.hasNext()){
        QPoint p = i.next();
        if(p.x() < MaxPos.topLeft.x()) MaxPos.topLeft.setX(p.x());
        if(p.y() < MaxPos.topLeft.y()) MaxPos.topLeft.setY(p.y());
        if(p.x() > MaxPos.bottomRight.x()) MaxPos.bottomRight.setX(p.x());
        if(p.y() > MaxPos.bottomRight.y()) MaxPos.bottomRight.setY(p.y());

        painter.drawEllipse(p.x(),p.y(),sizeBrush,sizeBrush);
    }


    MaxPos.rect = QRectF(MaxPos.topLeft.x(),
                         MaxPos.topLeft.y(),
                         MaxPos.bottomRight.x()-MaxPos.topLeft.x(),
                         MaxPos.bottomRight.y()-MaxPos.topLeft.y());

    painter.setBrush(Qt::red);
    //for(int k=0;k<recognitionPoints.size();k++) painter.drawEllipse(recognitionPoints[k].x(),recognitionPoints[k].y(),10,10);


    QPainterPath path;
    path.addRect(MaxPos.rect);
    QPen pen(Qt::red, 2);
    painter.setPen(pen);
    painter.setBrush(QColor(0,0,0,0));
    //painter.drawPath(path);







    painter.end();



}
void MainWindow::wheelEvent(QWheelEvent *event)
{

    /* numDegrees = event->angleDelta();
    if(numDegrees.y() > 0){
        sizeBrush += 2;
        qDebug("fef");
    }
    if(numDegrees.y() < 0 && sizeBrush > 6){
        sizeBrush = sizeBrush - 2;
        qDebug("fe");
    }

    update();*/
}


void MainWindow::onTimer(){

}

void MainWindow::getRecognitionNumbers()
{

    recognitionPoints.clear();
    recognitionPointPlace = 0;
    while(m_Line.size() % RECOGNITION_POINTS_AMOUNT != 0){
        m_Line.pop_back();
    }

    for(int i= 0;i<RECOGNITION_POINTS_AMOUNT;i++){

        recognitionPoints.append(m_Line[recognitionPointPlace]);
        recognitionPointPlace += m_Line.size() /RECOGNITION_POINTS_AMOUNT;
        QPoint f;
        Percentage p;
        float subtotal1;
        float subtotal2;
        f = recognitionPoints[i];
        subtotal1 = f.y()-MaxPos.topLeft.y();
        subtotal2 = subtotal1/MaxPos.rect.height();
        p.percentageFromTop = subtotal2*100;

        subtotal1 = f.x()-MaxPos.topLeft.x();
        subtotal2 = subtotal1/MaxPos.rect.width();
        p.percentageFromLeft =subtotal2*100;
        recognitionPointsPercentage.append(p);
        //p.percentageFromTop = MaxPos.rect.height()/ w;
        //qDebug() << m_Line[i];
        //qDebug() << " ";



        //qDebug() << "line Y:"<< f.y() ;
        //qDebug() << "MaxPos top left:" << MaxPos.topLeft.y();
        //qDebug() << "from left:"<<p.percentageFromLeft;
        //qDebug() << "from Top;" << p.percentageFromTop;
        //qDebug() << p.percentageFromLeft;
        //qDebug() << p.percentageFromTop;

    }

    //qDebug() << "------------";

    //qDebug() << " ";
    //qDebug()<< MaxPos.rect.height();

}

float MainWindow::comparePoints(int j)
{
    totalDifference = 0;

    //for(int i = 0;i<1;i++){

        float differenceX = 0;
        float differenceY = 0;
        totalDifference = 0;
        int v = j;
        int RPAT4 = RECOGNITION_POINTS_AMOUNT*2;
        Percentage p;
        //qDebug() << "v:" << v;
        int f=0;
        for(;j<v+RPAT4;j++){
            f++;
            //qDebug() << v+RPAT2;
            //qDebug() << j;
            p = recognitionPointsPercentage[f];
            if(databasePoint[j]<p.percentageFromLeft)differenceX = p.percentageFromLeft - databasePoint[j];
            if(databasePoint[j]>p.percentageFromLeft) differenceX = databasePoint[j] - p.percentageFromLeft;
            else if(databasePoint[j]==p.percentageFromLeft) differenceX=0;
            //qDebug() << databasePoint[j] << p.percentageFromLeft<< "="<< differenceX << "/" << j;
            j++;
            if(databasePoint[j]<p.percentageFromTop)differenceY = p.percentageFromTop - databasePoint[j];
            if(databasePoint[j]>p.percentageFromTop) differenceY = databasePoint[j] - p.percentageFromTop;
            else if(databasePoint[j]==p.percentageFromTop) differenceY=0;
            //qDebug() << databasePoint[j] << p.percentageFromTop<< "="<< differenceY << "/" << j;
            //qDebug() << "X:" << differenceX;
            //qDebug() << "Y:" << differenceY;
            //qDebug() << differenceX;
            //qDebug() << differenceY;
            totalDifference += differenceX +differenceY;


            differenceX = 0;
            differenceY = 0;
            //qDebug() << totalDifference;
        }

        //qDebug() <<"total diff:" << totalDifference;
        differenceX = 0;
        differenceY = 0;

    //}


    j = 0;
    return totalDifference;

}

char MainWindow::getMostSimmilarLetter()
{

    float lowestDifference = 99999;
    char mostSimmilarChar = ' ';


    //qDebug() << comparePoints(LETTER_S);
    if(comparePoints(LETTER_S)<lowestDifference){

        lowestDifference = comparePoints(LETTER_S);
        mostSimmilarChar = 'S';
    }
    if(comparePoints(LETTER_O1)<lowestDifference){
        lowestDifference = comparePoints(LETTER_O1);
        mostSimmilarChar = 'O';
    }
    if(comparePoints(LETTER_O2)<lowestDifference){
        lowestDifference = comparePoints(LETTER_O2);
        mostSimmilarChar = 'O';
    }
    if(comparePoints(LETTER_L1)<lowestDifference){
        lowestDifference = comparePoints(LETTER_L1);
        mostSimmilarChar = 'L';
    }
    if(comparePoints(LETTER_C)<lowestDifference){
        lowestDifference = comparePoints(LETTER_C);
        mostSimmilarChar = 'C';
    }
    if(comparePoints(LETTER_G)<lowestDifference){
        lowestDifference = comparePoints(LETTER_G);
        mostSimmilarChar = 'G';
    }
    if(comparePoints(LETTER_I)<lowestDifference){
        lowestDifference = comparePoints(LETTER_I);
        mostSimmilarChar = 'I';
    }
    if(comparePoints(LETTER_J)<lowestDifference){
        lowestDifference = comparePoints(LETTER_J);
        mostSimmilarChar = 'J';
    }
    if(comparePoints(LETTER_L2)<lowestDifference){
        lowestDifference = comparePoints(LETTER_L2);
        mostSimmilarChar = 'L';
    }
    if(comparePoints(LETTER_M)<lowestDifference){
        lowestDifference = comparePoints(LETTER_M);
        mostSimmilarChar = 'M';
    }
    if(comparePoints(LETTER_N)<lowestDifference){
        lowestDifference = comparePoints(LETTER_N);
        mostSimmilarChar = 'N';
    }
    if(comparePoints(LETTER_R)<lowestDifference){
        lowestDifference = comparePoints(LETTER_R);
        mostSimmilarChar = 'R';
    }
    if(comparePoints(LETTER_U)<lowestDifference){
        lowestDifference = comparePoints(LETTER_U);
        mostSimmilarChar = 'U';
    }
    if(comparePoints(LETTER_V)<lowestDifference){
        lowestDifference = comparePoints(LETTER_V);
        mostSimmilarChar = 'V';
    }
    if(comparePoints(LETTER_W)<lowestDifference){
        lowestDifference = comparePoints(LETTER_W);
        mostSimmilarChar = 'W';
    }
    if(comparePoints(LETTER_Z)<lowestDifference){
        lowestDifference = comparePoints(LETTER_Z);
        mostSimmilarChar = 'Z';
    }
    //else qDebug() << "SOMETHING WENT WRONG!";
    return mostSimmilarChar;
}



MainWindow::~MainWindow()
{
    delete ui;
}
