#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include<QUdpSocket>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include<QtMath>
#include <QThread>
#include <QWebView>



#include <QWebHistory>
#include <QWebHistoryItem>
#include <QWebPage>
#include <QWebView>
#include <QtWebKit/QtWebKit>
#include <QtWebKitWidgets/QtWebKitWidgets> // With Qt >= 4.8
#include<QFile>
#include <QTextStream>
#include <cmath>
#include<QtMath>
#include<math.h>
// zmiennie globalne

QString tekst;			 // tekst znajdujący się w polu tekstowym

QString szukany_tekst;	 	 // tekst, który zamienimy na inny
QString zamiennik;		 // tekst na jaki zostanie zamieniona szukana fraza
QString poprzedniaSciezka;

QString tekst1;
int sterowanie=0;

//551123
QString tab_fuzzy[1030302];
//QString tab_left[552123];
//QString tab_front[552123];
//QString tab_right[552123];
//QString tab_left_engine[552123];
//QString tab_right_engine[552123];
QString lewy_silnik;
QString prawy_silnik;
int fuzzy_left=100;
int fuzzy_front=100;
int fuzzy_right=100;

QString tab_fuzzy_cel[321203];







MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),count(0),count2(0), img(469, 349, QImage::Format_ARGB32),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton->setIcon(QIcon(":/images/stop.png"));
    ui->pushButton_2->setIcon(QIcon(":/images/down_red.png"));
    ui->pushButton_3->setIcon(QIcon(":/images/right_red.png"));
    ui->pushButton_4->setIcon(QIcon(":/images/left_red.png"));
    ui->pushButton_5->setIcon(QIcon(":/images/up_red.png"));




get_addres();
    initSocket();




vListIPaddress();



scene = new QGraphicsScene(this);
/*
QPen pen(Qt::gray, 1);
for(int i=0;i<=340;i=i+20)
{
scene->addLine(0,i,467,i,pen);
}
for(int i=0;i<=450;i=i+20)
{
scene->addLine(i,0,i,340,pen);
}

*/


ui->graphicsView->setScene(scene);

color = qRgb(255, 0, 0);
colorlline= qRgb(128, 128, 128);
colorllinewhite= qRgb(255, 255, 255);
colorrect= qRgb(0, 0, 255);
qDebug() << "1:" ;

drawCross(colorlline);


//QWebView view;
//    view.show();
//    view.load(QUrl("http://google.com"));
//ui->graphicsView_2->setScene();

ui->web2->load(QUrl("http://192.168.137.240:8765"));

fuzylogic();

}

MainWindow::~MainWindow()
{
    delete ui;
}




class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};






void MainWindow::get_addres()
{
    //nPort = 0;
    //IPaddress
    ui->lineEditPort->setText(QString::number(nPort));
    connect(ui->lineEditPort,SIGNAL(textChanged(QString)),this,SLOT(updateUdpPort(QString)));

}

void MainWindow::initSocket()
{
    udpSocket = new QUdpSocket(this);
//    udpSocket->bind(QHostAddress::LocalHost, 33333);
    udpSocket->bind(QHostAddress::Any, nPort);

    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));


}
void MainWindow::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        processTheDatagram(datagram,sender);
    }
}

void MainWindow::processTheDatagram(QByteArray datagram, QHostAddress qha){
    bool ok;
    int n = datagram.toInt(&ok,10);
    QString qsz;
    QString wiadomosc;
    if(ok){
        emit newAngle(n);
        if ( (n < 5) || (n > 70))
        {
            qsz = trUtf8("%1 <font color = red>%2</color>").
                  arg(qha.toString()).
                  arg(datagram.data());
            wiadomosc=datagram.data();
        }
        else
        {
            qsz = trUtf8("%1 <font color = blue>%2</color>").
                  arg(qha.toString()).
                  arg(datagram.data());
            wiadomosc=datagram.data();
        }
    }else
    {
        qsz = trUtf8("%1 <font color = red>%2</color>").
              arg(qha.toString()).
              arg(datagram.data());
        wiadomosc=datagram.data();
    }
    ui->plainTextEdit->appendHtml(qsz);
    odbierzDane(wiadomosc);

}

void MainWindow::updateUdpPort(QString qsz){
    bool ok;
    QString nowyIP;
    QString nowyPort;

    nowyIP=qsz.section(':',0,0);
    nowyPort=qsz.section(':',1,1);
    qint16 n = nowyPort.toInt(&ok,10);
 qDebug() << "nowyPort " << nowyPort;
  qDebug() << "nowyIP " << nowyIP;
    if(ok){
        disconnect(udpSocket);
        udpSocket->abort();
        delete udpSocket;
        nPort = n;
        IPaddress=nowyIP;
        qDebug() << "nPort " << nPort;
         qDebug() << "IPaddress " << IPaddress;
        initSocket();
    }
}

void MainWindow::vListIPaddress(){
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (/*ipAddressesList.at != QHostAddress::LocalHost &&*/
            ipAddressesList.at(i).protocol() == QAbstractSocket::IPv4Protocol)

            {
                qDebug() << trUtf8("ipAddress : %1").
                        arg(ipAddressesList.at(i).toString());
                //break;
            }
    }
}







void MainWindow::odbierzDane(QString wiadomosc)
{
QString wybierz,info;


wybierz = wiadomosc.section(',', 0, 0);   // str == "1"
info    = wiadomosc.section(',', 1, 1);   // str == "24stopnie"

qDebug() << "wiadomosc  "<<wiadomosc;
qDebug() << "wybierz  "<<wybierz;
qDebug() << "info  "<<info;

bool ok;

switch (wybierz.toInt()) {
case 1:
    ui->lineEdit->setText(info);
    break;
case 2:
    ui->lineEdit_2->setText(info);
    break;
case 3:
    ui->lineEdit_3->setText(info);
    break;
case 4:
    ui->lineEdit_4->setText(info);
    break;
case 5:
    ui->lineEdit_5->setText(info);
    break;
case 6:
    ui->lineEdit_6->setText(info);
    break;
case 7:
    ui->lineEdit_7->setText(info);
    break;
case 8:
    ui->lineEdit_8->setText(info);
    break;
case 9:
    ui->lineEdit_9->setText(info);
    break;
case 10:
    ui->lcdNumber_2->display(info.toInt());
    break;
case 11:
    ui->lcdNumber_3->display(info.toInt());
    break;
case 12:
    ui->lcdNumber_4->display(info.toInt());
    break;
case 13:

    fuzzy_left=info.section('*', 0, 0).toDouble(&ok);   // str == "1"
    fuzzy_front=info.section('*', 1, 1).toDouble(&ok);   // str == "1"
    fuzzy_right=info.section('*', 2, 2).toDouble(&ok);   // str == "1"
    break;
case 14:

    poz_enkoder_left=info.section('*', 0, 0).toDouble(&ok);   // str == "1"
    poz_enkoder_right=info.section('*', 1, 1).toDouble(&ok);   // str == "1"
    break;
case 15:

    xg=info.section('*', 0, 0).toDouble(&ok);   // str == "1"
    yg=info.section('*', 1, 1).toDouble(&ok);   // str == "1"
    break;

default:
    break;
}






}















void MainWindow::on_pushButton_5_clicked()
{


  /*  if(ui->pushButton_5->text() == QString("push up"))
            ui->pushButton_5->setText(tr("up"));
        else
            ui->pushButton_5->setText(tr("push up"));
            */
    QString text = ui->sterowanie->text();
    ui->sterowanie->setText("jedzie do przodu");
   sterowanie=1;
uaktualnij_dane("jedzie do przodu");




}

void MainWindow::on_pushButton_clicked()
{

    QString text = ui->sterowanie->text();
    ui->sterowanie->setText("stop");
    sterowanie=0;
uaktualnij_dane("stop");

}

void MainWindow::on_pushButton_4_clicked()
{
    QString text = ui->sterowanie->text();
    ui->sterowanie->setText("jedzie w lewo");
    sterowanie=3;
    uaktualnij_dane("jedzie w lewo");

}

void MainWindow::on_pushButton_3_clicked()
{
    QString text = ui->sterowanie->text();
    ui->sterowanie->setText("jedzie w prawo");
    sterowanie=4;
    uaktualnij_dane("jedzie w prawo");
}

void MainWindow::on_pushButton_2_clicked()
{
    QString text = ui->sterowanie->text();
    ui->sterowanie->setText("jedzie do tyłu");
    sterowanie=2;
    uaktualnij_dane("jedzie do tyłu");
}

void MainWindow::on_verticalSlider_sliderMoved(int position)
{
    //QString text = ui->lcdNumber->text();
    ui->lcdNumber->display(position);


    QString tmp = QString("%1")
                         .arg(position);

    uaktualnij_dane(tmp);
}


void MainWindow::on_pushButton_6_clicked()
{

    //MyUDP sender;
    MainWindow receiver;
  //  QObject::connect(&my, SIGNAL(wyslij_do_mainwindow(QString,QString,QString)), &receiver, SLOT(odczytDanych(QString,QString,QString)));
    //my.uaktualnij_dane("Polaczono z robotem");
  //  my.readyRead();
   // my.fireSignal();


//ui->lineEditPolacz->setText(tekst1);


}











void MainWindow::on_actionOtw_rz_triggered()
{
    // QFileDialog::getOpenFileName([rodzic (parent)],[tytuł nagłówka],
    // [ścieżka od której rozpoczyna się wyszukiwanie pliku], [rozszerzenia wyszukiwanych plików])
    QString fileName = QFileDialog::getOpenFileName(this,tr("Otwórz..."), "/home/", tr("Pliki txt (*.txt )"));
    QFile plik(fileName);

        // możemy tylko czytać dane, oraz wczytujemy je jako tekst:
        if(!plik.open(QIODevice::ReadOnly | QIODevice::Text))
            return;			 // jeżeli nie udało się otworzyć pliku: przerwij wczytywanie pliku

        // czyścimy wcześniej zapełnioną zmienną tekstową
        tekst.clear();

        QTextStream stream(&plik);

        // czytamy wszystkie dane
        tekst = stream.readAll();

        // umieszczamy je wewnątrz text boxa
        ui->textEdit_2->setText(tekst);

        plik.close();

        // na końcu "ciała" funkcji void MainWindow::otworz_plik()
        poprzedniaSciezka = fileName;
}


void MainWindow::zapisz_plik(QString fileName)
{
    tekst = ui->textEdit_2->toPlainText();

    if(fileName.isEmpty())
        fileName = QFileDialog::getSaveFileName
                (this,tr("Zapisz plik jako..."),tr("/home/"),tr("Pliki tekstowe (*.txt)"));

    if(fileName.isEmpty())
        return;

    QFile plik(fileName);
    plik.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

    QTextStream stream(&plik);
    stream << tekst;

    poprzedniaSciezka = fileName;

    plik.close();
}



void MainWindow::zapisz_plik_jako( )
{

    QString fileName = QFileDialog::getSaveFileName(this,tr("Otwórz..."), "/home/", tr("Pliki txt (*.txt)"));


    tekst = ui->textEdit_2->toPlainText();

    if(fileName.isEmpty())
        fileName = QFileDialog::getSaveFileName
                (this,tr("Zapisz plik jako..."),tr("/home/"),tr("Pliki tekstowe (*.txt)"));

    if(fileName.isEmpty())
        return;

    QFile plik(fileName);
    plik.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

    QTextStream stream(&plik);
    stream << tekst;

    poprzedniaSciezka = fileName;

    plik.close();
}

void MainWindow::on_actionZapisz_triggered()
{
    zapisz_plik(poprzedniaSciezka);
}

void MainWindow::on_actionZapisz_jako_triggered()
{
    zapisz_plik_jako();
}

void MainWindow::on_actionZamknij_triggered()
{

    qApp->exit();
}

















void MainWindow::uaktualnij_dane(const QString& dane)
{
 QByteArray Dane;
 Dane.append(dane);
Sleeper::msleep(2);
//udpSocket->writeDatagram(Dane,QHostAddress("192.168.0.255"),5001);
udpSocket->writeDatagram(Dane,QHostAddress(IPaddress),nPort+1);
qDebug() << "Message from: bufforu " << Dane;


}



void MainWindow::mousePressEvent(QMouseEvent *event)
{

    if(ui->checkBox_2->isChecked())
    {
        rysujTrase(event);
    }
    if(ui->checkBox_3->isChecked())
    {
      //  rysujPrzeszkode(20,20,100,100,colorrect);
    }



}




void MainWindow::drawCross(QRgb colorllinee)
{

    if(ui->checkBox->isChecked())
    {



    for(int i=0;i<=460;i++)
    {
        for(int j=0;j<=350;j=j+ui->spinBox_2->value())
        {
        img.setPixel(i, j,colorllinee );

        }

    }
    for(int i=0;i<=350;i++)
    {
        for(int j=0;j<=460;j=j+ui->spinBox_2->value())
        {
        img.setPixel(j, i,colorllinee );

        }

    }
scene->addPixmap(QPixmap::fromImage(img));
scene->backgroundBrush();

 }

}




void MainWindow::rysujPrzeszkode(int xx,int yy,int szer,int wys,QRgb colorrect)
{
int x,y;
szer=szer+xx;
wys=wys+yy;

   for( x=xx;x<=szer;x++)
    {
        for(y=yy ;y<=wys;y++)
        {
        img.setPixel(x, y,colorrect );
       // img.setPixel(y, x,colorlline );
        }

    }

scene->addPixmap(QPixmap::fromImage(img));
scene->backgroundBrush();
}



void MainWindow::rysujTrase(QMouseEvent *event)
{



    qDebug() << "1aa:" ;
    switch(event->button())
    {
    qDebug() << "2aa:" ;
        case Qt::LeftButton:
            count++;
qDebug() << "2a:" ;
            if(1 == count)
            {
                if(lastpoint.x()>=0 && lastpoint.x()<=469 && lastpoint.y()>=0 && lastpoint.y()<=349)
                {
                p1 = ui->graphicsView->mapFromGlobal(event->globalPos());



                p1=lastpoint;
                if(count2==0)
                {
                 lastpoint=ui->graphicsView->mapFromGlobal(event->globalPos());
                 count2=100;
                 p1=lastpoint;
                }
                qDebug() << "1b:" ;
                }
                else
                {
                p1 = ui->graphicsView->mapFromGlobal(event->globalPos());



                }
                //p1 = ui->graphicsView->mapFromGlobal(event->pos())
                qDebug() << "1a:" ;
            }
            else if(2 == count)
            {
                p2 = ui->graphicsView->mapFromGlobal(event->globalPos());


                int k=1;
                k=ui->spinBox_3->value();

                int xx1=p1.x();
                int yy1=p1.y();
                qDebug() << "2aap2:   " <<xx1;
                qDebug() << "2aap2:   " <<yy1;
                p1.setX(p1.x()-xx1%k);
                p1.setY(p1.y()-yy1%k);
                qDebug() << "3aap2:   " <<p1.x();
                qDebug() << "3aap2:   " <<p1.y();

                pomoc_point.setX(abs(p2.x()-p1.x()));
                pomoc_point.setY( abs(p2.y()-p1.y()));
                if(pomoc_point.x()>=pomoc_point.y())
                {
                    p2.setY(p1.y());

                }
                else
                {
                  p2.setX(p1.x());

                }


                lastpoint=p2;
                //p2 = ui->graphicsView->x();
qDebug() << "3a:" ;
                drawLine();
                reset();
                scene->addPixmap(QPixmap::fromImage(img));
            }

            break;

        default:
            reset();
            img.fill(Qt::white);
    }



}





void MainWindow::drawLine()
{

drawCross(colorlline);
    int pom=0;
    //I assume we have a coordinate system with the origin in the upper left corner
    //with the positive axes down and to the right, this is set up in the ctor



    int k=1;
    k=ui->spinBox_3->value();


    qDebug() << "1aap2:   " <<k;
    int xx=p2.x();
    int yy=p2.y();
    qDebug() << "2aap2:   " <<xx;
    qDebug() << "2aap2:   " <<yy;
    p2.setX(p2.x()-xx%k);
    p2.setY(p2.y()-yy%k);
    qDebug() << "3aap2:   " <<p2.x();
    qDebug() << "3aap2:   " <<p2.y();



    //first point
    QPoint f = p1.toPoint();
    //last point
    QPoint l = p2.toPoint();
qDebug() << "4:" ;
    //vertical line
    if(f.x() == l.x())
    {
        qDebug() << "vertical line";

        if(f.y() > l.y())
        {
            qSwap(f, l);
        }

        for(int y=f.y(); y<=l.y(); y++)
        {

            if(pom%ui->spinBox->value()==0)
            {
               qDebug() << "x:" << f.x() << "y:" << y;
               QString tmp = QString("%1,%2,%3,%4").arg("x").arg(f.x()).arg("y").arg(y);
               uaktualnij_dane(tmp);
              // Sleeper::usleep(10);
              // Sleeper::msleep(100);
              // Sleeper::sleep(10);

            }
            pom++;
            img.setPixel(f.x(), y, color);
            img.setPixel(f.x()+1, y+1, color);
            img.setPixel(f.x()-1, y-1, color);
        }

        return;
    }

    //slope
    qreal m = (p2.y() - p1.y())/(p2.x() - p1.x());
    qDebug() << "Slope:" << m;

    //in order to get better effects
    //I should render the line iterating through the longer side
    //the if condition translates into: the rise is higher than the run
    if(m > 1.0 || m < -1.0)
    {
        qDebug() << "the rise is higher than the run";

        m = 1/m;

        //draw the line from the top to bottom
        if(f.y() > l.y())
        {
            qSwap(f, l);
        }

        for(int y=f.y(); y<=l.y(); y++)
        {
            int x = round(m*(y-f.y()) + f.x());

            if(pom%ui->spinBox->value()==0)
            {
               qDebug() << "x:" << x << "y:" << y;
               QString tmp = QString("%1,%2,%3,%4").arg("x").arg(x).arg("y").arg(y);
               uaktualnij_dane(tmp);
              // Sleeper::msleep(100);
            }
            pom++;

            img.setPixel(x, y, color);
            img.setPixel(x+1, y+1, color);
            img.setPixel(x-1, y-1, color);
        }
    }
    else{
        //if the line was drawn right to left, make it so I draw left to right
        if(f.x() > l.x())
        {
            qSwap(f, l);
        }

        for(int x=f.x(); x<=l.x(); x++)
        {
            int y = round(m*(x-f.x()) + f.y());


            if(pom%ui->spinBox->value()==0)
            {
               qDebug() << "x:" << x << "y:" << y;
               QString tmp = QString("%1,%2,%3,%4").arg("x").arg(x).arg("y").arg(y);
               uaktualnij_dane(tmp);
             //  Sleeper::msleep(100);
            }
            pom++;


            img.setPixel(x, y,color );
            img.setPixel(x+1, y+1,color );
            img.setPixel(x-1, y-1,color );

        }
    }

}

void MainWindow::reset()
{


    count = 0;
    scene->clear();

    //count2=0;
    p1 = p2 = QPoint(0, 0);
    drawCross(colorlline);
    scene->addPixmap(QPixmap::fromImage(img));

}







void MainWindow::on_checkBox_toggled(bool checked)
{
    if(checked==true)
    {
        drawCross(colorlline);
    }
    else
    {
        scene->clear();
      //  drawCross(colorllinewhite);

    }

}

void MainWindow::on_checkBox_3_toggled(bool checked)
{


    if(checked==true)
    {

    }
    else
    {

      //  scene->clear();
      //  rysujPrzeszkode(int xx,int yy,int szer,int wys,colorrect);

    }
}

void MainWindow::on_pushButton_11_clicked()
{

    if(ui->checkBox_3->isChecked())
    {



    int xx,yy,szer,wys;
    xx=ui->spinBox_4->value();
    yy=ui->spinBox_5->value();
    szer=ui->spinBox_6->value();
    wys=ui->spinBox_7->value();

    rysujPrzeszkode( xx, yy, szer, wys,colorrect);

    }
}





void MainWindow::fuzylogic()
{


QFile file("C:/Users/lenovo/Documents/PROJEKTY QT/Najlepszy_v2/Generator2_v1/plik_cel_do_4m_co_1cm_001s.txt");
//QFile file("C:/Users/lenovo/Documents/PROJEKTY QT/Najlepszy_v2/Generator2_v1/plik_do_1m_co_1cm.txt");
if(!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(0, "error", file.errorString());
}


QString line;
//QStringList fields;
//QString tmp;
//QString fleft,ffront,fright,fleft_engine,fright_engine;
QTextStream in(&file);
int i=0;
while(!in.atEnd()) {
     line = in.readLine();
   //  fields = line.split(",");
  //  model->appendRow(fields);
  //  qDebug() <<i << " "<< line;
  //   tmp = QString("%1").arg(line);


//tab_fuzzy[i] = line;   // str == "1"
tab_fuzzy_cel[i]= line;




   //  tab_left[i] = line.section(' ', 0, 0);   // str == "1"
   //  tab_front[i]    = tmp.section(' ', 1, 1);   // str == "24stopnie"
   //  tab_right[i] = tmp.section(' ', 2, 2);   // str == "1"
   //  tab_left_engine[i]    = tmp.section(' ', 3, 3);   // str == "24stopnie"
   //  tab_right_engine[i] = tmp.section(' ', 4, 4);   // str == "1"


   // tab_left[i]=fleft;
   // tab_right[i]=fright;
   // tab_front[i]=ffront;
  //  tab_left_engine[i]=fleft_engine;
  //  tab_right_engine[i]=fright_engine;

//qDebug() << "tab_left " <<tab_left[500] ;
i++;
}

file.close();


qDebug() << "ok gotowe"  ;

}




void MainWindow::fuzylogic_jedz()
{




bool ok;


  //  int left=100;
  //   int front=99;
  //  int right=100;




    int licznik_left=0,licznik_front=0,licznik_right=0;

    for(int i=0; i<=1030301;i=i+10201)
    {

        int porowanie=tab_fuzzy[i].section(' ', 0, 0).toDouble(&ok);
        //int porowanie=tab_left[i].toDouble(&ok);
       // qDebug() << "tab_left1 " <<tab_left[i] << " por " <<porowanie;
       // qDebug() << "tab_left2 " <<tab_left[i].toDouble(&ok) << " por " <<porowanie;
        if(porowanie<=fuzzy_left)
        {
            licznik_left=licznik_left+10201;
            if(licznik_left>=1030301)
                licznik_left=1030301;

            if(fuzzy_left>=100 && fuzzy_front>=100 && fuzzy_right>=100)
            {
                licznik_left=1030300;
                //qDebug() << "lalalaaaaa ";
            }
        }
        else
        {
            break;
        }

    }

    for(int i=licznik_left-1; i>=0;i--)
    {

        int porowanie=tab_fuzzy[i].section(' ', 1, 1).toDouble(&ok);
        //int porowanie=tab_front[i].toDouble(&ok);
         //qDebug() << "tab_front2 " <<tab_front[i].toDouble(&ok) << " por " <<porowanie;

        if(porowanie>fuzzy_front)
        {
            licznik_front++;
        }
        else
        {
           // licznik_front=licznik_front+80;
            break;

        }

    }

    if(fuzzy_left>=100 && fuzzy_front>=100 && fuzzy_right<=99)
    {
        //licznik_right=licznik_right;//+fuzzy_right;
        qDebug() << "bbbbbb ";
        licznik_front=101;
    }


    for(int i=licznik_front-1; i>=0;i--)
    {
        qDebug() << "aaaaaaa ";


        int porowanie=tab_fuzzy[i].section(' ', 2, 2).toDouble(&ok);
        //int porowanie=tab_right[i].toDouble(&ok);
        if(porowanie>=fuzzy_right)
        {
            licznik_right++;
        }
        else
        {
            break;
        }
    }


    if(fuzzy_left>=100 && fuzzy_front>=100 && fuzzy_right<=99)
    {
        //licznik_right=licznik_right;//+fuzzy_right;
        qDebug() << "bbbbbb ";
        licznik_front=0;
    }


    //int wynik=licznik_left-licznik_front-licznik_right+81;
    int wynik=licznik_left-licznik_front-licznik_right;

   lewy_silnik=tab_fuzzy[wynik].section(' ', 3, 3);
   prawy_silnik=tab_fuzzy[wynik].section(' ', 4, 4);
//lewy_silnik=tab_left_engine[wynik];
//prawy_silnik=tab_right_engine[wynik];
    qDebug() << "fuzzy_left " <<fuzzy_left << "fuzzy_front " <<fuzzy_front << "fuzzy_right "<< fuzzy_right;
    qDebug() << "licznik left " <<licznik_left ;
    qDebug() << "licznik front " <<licznik_front ;
    qDebug() << "licznik right " <<licznik_right ;
    qDebug() << "wynik " <<wynik ;
    qDebug() << "lewy silnik " <<lewy_silnik << " prawy silnik " << prawy_silnik;
    uaktualnij_dane(""+lewy_silnik+","+prawy_silnik);



}



void MainWindow::fuzylogic_cel()
{




   // kSetEncoders(ref,0,0);
  //  while(1)
  //  {
        //poz_enkoder_left=10;  // odczytanie pozycji enkodera lewego
        //poz_enkoder_right=10;
        cl=poz_enkoder_left; // ustawienie pozycja enkodera lewego
        cr=poz_enkoder_right;
        if ((cl_old != cl) | (cr_old != cr))  // jeśli został wykoanny ruch
        {
                dl = cl - cl_old;
                dr = cr - cr_old;
                cl_old = cl;
                cr_old = cr;

                if (dl != dr) // jeśli koła obróćiły się o różną odległość
                {
                          angle = (dr - dl)/DR*MPP; // kąt obrotu robota
                          radius = (DR/2)*(dl + dr)/(dr - dl); // odległość promienia skrętu R (ICC) od śrdoka robota
                          forward = radius*sin(angle); // jazda na przód ale w tym wypadku po łuku
                          lateral = radius*(1.0 - cos(angle)); // jazda w bok
                }
                else
                {

                          angle = 0.0; // jeżeli obroty kół były takie same to kąt 0
                          forward = dl*MPP; // jazda na przód
                          lateral = 0.0; // jazda w bok
                }

                        dx = cos(kat_theta);
                        dy = sin(kat_theta);
                        x = x + forward*dx - lateral*dy; //  obliczanie pozycji x
                        y = y + forward*dy + lateral*dx; //  obliczanie pozycji y
                        kat_theta =(kat_theta+ angle);   // zmiana pozycji kąta

           }
                            if (kat_theta >= 2*pi)  // jeżeli kąt większy niż 2pi to odejmnij 2pi
                            {
                             kat_theta =kat_theta - 2*pi;
                            }


                        if (kat_theta <= -2*pi)
                        {
                         kat_theta = kat_theta+2*pi; // jeżeli kąt większy niż 2pi to dodaj 2pi
                        }

                    psi=kat_theta-(atan2((yg-y),(xg-x))); // oblicz obrót od celu
                    z=sqrt(pow((xg-x),2)+pow((yg-y),2)); // oblicz odległość od celu
                    if (z<60)   // jeśli odległość mniejsza niż 60 to zatrzymaj robota

                    {
                   // ksetspeed(ref,0,0);
                   // kSetEncoders(ref,0,0);
                        lewy_silnik= QString("%1").arg(0);
                        prawy_silnik= QString("%1").arg(0);
                        poz_enkoder_left=0;
                        poz_enkoder_right=0;
                //    break;
                    }





                    bool ok;


                      //  int left=100;
                      //   int front=99;
                      //  int right=100;




                        int licznik_z=0,licznik_psi=0;

                        for(int i=0; i<=321200;i=i+1)
                        {

                            int porowanie=tab_fuzzy_cel[i].section(' ', 0, 0).toDouble(&ok);
                            //int porowanie=tab_left[i].toDouble(&ok);
                           // qDebug() << "tab_left1 " <<tab_left[i] << " por " <<porowanie;
                           // qDebug() << "tab_left2 " <<tab_left[i].toDouble(&ok) << " por " <<porowanie;
                            if(porowanie<=z)
                            {
                                licznik_z=licznik_z+1;
                                if(licznik_z>=321200)
                                    licznik_z=321200;

                                if(z>=400 && psi>=4 )
                                {
                                    licznik_z=321199;
                                    //qDebug() << "lalalaaaaa ";
                                }
                            }
                            else
                            {
                                break;
                            }

                        }

                        for(int i=licznik_z-1; i>=0;i--)
                        {

                            double porowanie=tab_fuzzy_cel[i].section(' ', 1, 1).toDouble(&ok);
                            //int porowanie=tab_front[i].toDouble(&ok);
                             qDebug() << "tab_front2 " <<tab_fuzzy_cel[i].toDouble(&ok) << " por " <<porowanie;

                            if(porowanie>psi)
                            {
                                licznik_psi++;
                            }
                            else
                            {
                               // licznik_front=licznik_front+80;
                                break;

                            }

                        }

licznik_psi++;


                        //int wynik=licznik_left-licznik_front-licznik_right+81;
                        int wynik=licznik_z-licznik_psi;





                    lewy_silnik=tab_fuzzy_cel[wynik].section(' ', 2, 2);
                    prawy_silnik=tab_fuzzy_cel[wynik].section(' ', 3, 3);
                   // v = evalfis([z, psi], K);
                    //ksetspeed(ref,v(1),v(2));

                    qDebug() << "licznik z " <<licznik_z ;
                    qDebug() << "licznik psi " <<licznik_psi ;
                    qDebug() << "wynik " <<wynik ;
                   qDebug() << " Z " <<z << " psi  " << psi*180/pi << " rad " << psi;
                   qDebug() << "lewy silnik " <<lewy_silnik << " prawy silnik " << prawy_silnik;
                    // [z psi*180/pi]


  //  }


}






void MainWindow::Readfile(QString filename)
{

}

void MainWindow::on_pushButton_10_clicked()
{


//fuzylogic_jedz();
fuzylogic_cel();

}




void MainWindow::on_pushButton_7_clicked()
{
   // fuzylogic();
}
