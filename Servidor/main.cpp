/*
   ==================================================================================

   Arquivo....: main.cpp
   Comentarios: Codigo-fonte principal

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
#include "mainwindow.h"
#include <QApplication>

/* ==================================================================================*/


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();



}
