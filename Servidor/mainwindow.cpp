/*
   ==================================================================================

   Arquivo....: mainwindow.cpp
   Comentarios: Codigo-fonte da interface

   ==================================================================================

   Autor..............: Samuel Nata de Franca Borges
                        Vitor de Godeiro Marques
                        Instito Metropole Digital
                        Universidade Federal do Rio Grande do Norte
                        Natal, Rio Grande do Norte, Brasil
   Data Criação.......: 02/04/2016
   Ultima Modificação.: 05/04/2016
   Versao.............: 1.0
   Para Compilar......:
   Para execultar.....:

   ==================================================================================
*/

/* ==================================================================================*/
//includes

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "updatevalores.h"

#include<iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->valorPotenciometro = 0;
    this->valorTemperatura = 0;
    this->valorUmidade = 0;
    ui->setupUi(this);
    this->setWindowTitle("Visualizador de Recursos");
    this->inicializaGraficoTemperatura();
    this->inicializaGraficoUmidade();

    QThread     *workerThread;
    UpdateValores *worker;

    workerThread = new QThread;
    worker       = new UpdateValores;
    worker->moveToThread(workerThread);

    connect(workerThread, SIGNAL(started()), worker, SLOT(doWork()));
    connect(worker, SIGNAL(finished()), workerThread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));
    connect(worker, SIGNAL(updateCount(char,float,float,int)), this, SLOT(atualizaGrafico(char,float,float,int)));

    workerThread->start();
}


void MainWindow::setValorPotenciometro(int32_t value)
{
    valorPotenciometro = value;
}

void MainWindow::setValorTemperatura(float value)
{
    valorTemperatura = value;
}

void MainWindow::setValorUmidade(float value)
{
    valorUmidade = value;
}

void MainWindow::atualizaGrafico(char cor, float temp, float hum, int pot)
{



    this->setValorPotenciometro(pot);
    this->setValorUmidade(hum);
    this->setValorTemperatura(temp);
    ui->hSlidetPotenciometro->setValue(pot);
    if(cor == 'r' || cor == 'R')
        ui->wgtLed->setColor("red");
    else
        if(cor == 'b' || cor == 'B')
            ui->wgtLed->setColor("blue");
    else
        ui->wgtLed->setColor("green");
}

/**
 * @brief MainWindow::inicializaGraficoTemperatura
 * @descricao   Metodo responsavel por inicialziar o grafico da temperatura,
 *              o qual ira conter todos os valores da temperatura, que sera
 *              obtido via socket.
 * @para        void
 * @return      void
 */
void MainWindow::inicializaGraficoTemperatura()
{
    ui->wgtTemperatura->addGraph();
    ui->wgtTemperatura->graph(0)->setPen(QPen(Qt::red));

    ui->wgtTemperatura->addGraph();
    ui->wgtTemperatura->graph(1)->setPen(QPen(Qt::blue));
    ui->wgtTemperatura->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->wgtTemperatura->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);


    ui->wgtTemperatura->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->wgtTemperatura->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->wgtTemperatura->xAxis->setTickLabelRotation(90);
    ui->wgtTemperatura->xAxis->setAutoTickStep(false);
    ui->wgtTemperatura->xAxis->setTickStep(2);
    ui->wgtTemperatura->axisRect()->setupFullAxesBox();


    ui->wgtTemperatura->graph(0)->setName("Temperaturuca (°C)");
    ui->wgtTemperatura->legend->setVisible(true);
    ui->wgtTemperatura->legend->removeItem(1);
    ui->wgtTemperatura->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    ui->wgtTemperatura->legend->setFont(QFont("Helvetica",9));
    ui->wgtTemperatura->legend->setRowSpacing(-10);


    connect(ui->wgtTemperatura->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->wgtTemperatura->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->wgtTemperatura->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->wgtTemperatura->yAxis2, SLOT(setRange(QCPRange)));

    connect(&tempoTemperaturaTimer, SIGNAL(timeout()), this, SLOT(atualizaGraficoTemperatura()));
    tempoTemperaturaTimer.start(0); // Interval 0 means to refresh as fast as possible
}

/**
 * @brief       MainWindow::inicializaGraficoUmidade
 * @descricao   Metodo responsavel por inicializar o grafico da umidade,
 *              o qual sera atualizado a partir dos dados passado via socket.
 * @param       void
 * @return      void
 */
void MainWindow::inicializaGraficoUmidade()
{
    ui->wgtUmidade->addGraph();
    ui->wgtUmidade->graph(0)->setPen(QPen(Qt::red));

    ui->wgtUmidade->addGraph();
    ui->wgtUmidade->graph(1)->setPen(QPen(Qt::blue));
    ui->wgtUmidade->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->wgtUmidade->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);


    ui->wgtUmidade->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->wgtUmidade->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->wgtUmidade->xAxis->setTickLabelRotation(90);
    ui->wgtUmidade->xAxis->setAutoTickStep(false);
    ui->wgtUmidade->xAxis->setTickStep(2);
    ui->wgtUmidade->axisRect()->setupFullAxesBox();


    ui->wgtUmidade->graph(0)->setName("Umidade (%)");
    ui->wgtUmidade->legend->setVisible(true);
    ui->wgtUmidade->legend->removeItem(1);
    ui->wgtUmidade->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    ui->wgtUmidade->legend->setFont(QFont("Helvetica",9));
    ui->wgtUmidade->legend->setRowSpacing(-10);


    connect(ui->wgtUmidade->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->wgtUmidade->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->wgtUmidade->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->wgtUmidade->yAxis2, SLOT(setRange(QCPRange)));

    connect(&tempoUmidadeTimer, SIGNAL(timeout()), this, SLOT(atualizaGraficoUmidade()));
    tempoUmidadeTimer.start(0); // Interval 0 means to refresh as fast as possible
}

/**
 * @brief   MainWindow::atualizaGraficoTemperatura
 *          Metodo responsavel por atualizar o grafico da temperatura,
 *          de acordo com o valor que se encontra na variavel do objeto
 *          valorTemperatura.
 * @param   void
 * @return  void
 */
void MainWindow::atualizaGraficoTemperatura()
{
     double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
     // add data to lines:
     ui->wgtTemperatura->graph(0)->addData(key, this->valorTemperatura);

     // set data of dots:
     ui->wgtTemperatura->graph(1)->clearData();
     ui->wgtTemperatura->graph(1)->addData(key, this->valorTemperatura);

     ui->wgtTemperatura->graph(0)->rescaleValueAxis();

     // make key axis range scroll with the data (at a constant range size of 8):
     ui->wgtTemperatura->xAxis->setRange(key+0.25,60, Qt::AlignRight);
     ui->wgtTemperatura->yAxis->setRange(40,40.25, Qt::AlignRight);
     ui->wgtTemperatura->replot();
}

/**
 * @brief   MainWindow::atualizaGraficoUmidade
 *          Metodo responsavel por atualizar o grafico da umidade,
 *          de acordo com o valor que se encontra na variavel do
 *          objeto valorUmidade
 * @param   void
 * @return  void
 */
void MainWindow::atualizaGraficoUmidade()
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    // add data to lines:
    ui->wgtUmidade->graph(0)->addData(key, this->valorUmidade);

    // set data of dots:
    ui->wgtUmidade->graph(1)->clearData();
    ui->wgtUmidade->graph(1)->addData(key, this->valorUmidade);

    ui->wgtUmidade->graph(0)->rescaleValueAxis();

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->wgtUmidade->xAxis->setRange(key+0.25,60, Qt::AlignRight);
    ui->wgtUmidade->yAxis->setRange(100,100.25, Qt::AlignRight);
    ui->wgtUmidade->replot();

}

MainWindow::~MainWindow()
{
    delete ui;
}
