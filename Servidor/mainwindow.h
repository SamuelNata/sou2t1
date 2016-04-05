/*
   ==================================================================================

   Arquivo....: mainwindow.h
   Comentarios: Declaracao da classe da interface

   ==================================================================================

   Autor..............: Samuel Nata de Franca Borges
                        Vitor de Godeiro Marques
                        Instito Metropole Digital
                        Universidade Federal do Rio Grande do Norte
                        Natal, Rio Grande do Norte, Brasil
   Data Criação.......: 02/04/2016
   Ultima Modificação.: 02/04/2016
   Versao.............: 1.0
   Para Compilar......:
   Para execultar.....:

   ==================================================================================
*/

/* ==================================================================================*/
//includes
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include<stdint.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setValorPotenciometro(int32_t value);
    void setValorTemperatura(float value);
    void setValorUmidade(float value);

private:
    Ui::MainWindow *ui;
    double valorPotenciometro;
    int32_t valorTemperatura;
    double valorUmidade;
    void inicializaGraficoTemperatura();
    void inicializaGraficoUmidade();
    QTimer tempoTemperaturaTimer;
    QTimer tempoUmidadeTimer;

private slots:
    void atualizaGraficoTemperatura();
    void atualizaGraficoUmidade();
    void atualizaGrafico(char cor, float temp, float hum, int pot);

};

#endif // MAINWINDOW_H
