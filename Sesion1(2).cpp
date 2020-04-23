//============================================================================
//----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES ------------
//---------------------------- CURSO 2019/20 ---------------------------------
//--------Autores: Rubén Costa Barriga e Iván Gonzalez Dominguez--------------
//============================================================================


#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "PuertoSerie.h"
#include "ControlFrame.h"
#include "DataFrame.h"
#include "Gestor.h"

#define ESC_KEY (27)
#define RETURN_KEY (8)
#define INTRO_KEY (13)
#define F1 (59)
#define F2 (60)
#define F3 (61)
#define F5 (63)
using namespace std;
int main() {
    //  ControFrame controlF = new ControlFrame();
    HANDLE portCOM;
    char carE = 0;
    char PSerie[5];
    char msg[802] ; //two more characters to the line end
    int colouro;
    int field = 1;
    int isControlFrame;
    int size = 0;
    Gestor g;


    //Header
    printf("============================================================================\n");
    printf("----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES ------------\n");
    printf("---------------------------- CURSO 2019/20 ---------------------------------\n");
    printf("----------------------------- SESION3.CPP ----------------------------------\n");
    printf("--------Autores: Rubén Costa Barriga e Iván Gonzalez Dominguez--------------\n");
    printf("============================================================================\n\n");

    //We will open the port. For it we need to define the following parameters:
    // - Open port name: ("COM1", "COM2", "COM3", ...).
    // - Velocity: (1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200).
    // - Bits number in each sent or received byte: (4, 5, 6, 7, 8).
    // - Parity: (0=sin parity, 1=odd, 2=even, 3=mark, 4=space).
    // - Stop bits: (0=1 bit, 1=1.5 bits, 2=2 bits).

    g.choosePort(PSerie);
    //goto doFinish;
    int choosedVel = g.chooseVel();

    // Here we will open the port choosed by the user
    portCOM = AbrirPuerto(PSerie,choosedVel,8,0,0);

    //Check if the choosed port is valid or not
    if(portCOM == NULL) {
        printf("Error al abrir el port %s\n",PSerie);
        getch();
        return (1); }
    else
        printf("port %s abierto correctamente\n",PSerie);

    //Esc key case to close the program, if esc is not pressed, continue forever
    while(carE != ESC_KEY) {
            g.receiveFrame(field,portCOM,isControlFrame,colouro);


        if (kbhit()) {
            //saving the input
            carE = getch();
            //Esc key case, if it isn't pressed, the switch will be displayed
            if (carE != ESC_KEY) {
                g.send(carE,msg,size,portCOM,colouro);


            }

        }
    }
    //Te port will be closed and the app will return
    //doFinish:
    CerrarPuerto(portCOM);
    return 0; }
