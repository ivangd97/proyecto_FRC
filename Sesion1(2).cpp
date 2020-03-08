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
#include "PuertoSerie.h"
#include "ControlFrame.h"
#include "DataFrame.h"


#define ESC_KEY (27)
#define RETURN_KEY (8)
#define INTRO_KEY (13)
#define F1 (59)
#define F2 (60)


//We will define the limit as a variable
int limit = 802;
DataFrame fReceive;
ControlFrame controlReceive;
int i = 0;
HANDLE pantalla;
int colour = 0;

// Simultaneous Read/Write of characters :
int tamanio = 0;

using namespace std;

HANDLE portCOM;

//First of all, we will choose the port that will be opened, we will let the user choose
//from a list of ports and will open it
void choosePort(char PSerie[]) {

    char port;
    bool portFlag = false;
    while(!portFlag) {

        printf("Seleccione el puerto que desea abrir:\n 1. COM1 \n 2. COM2 \n 3. COM3 \n 4. COM4 \n");
        port= getch();
        //Screen will be cleaned
        system("cls");
        switch(port) {

        case '1':
            strcpy (PSerie,"COM1");
            printf("Puerto elegido: %c nombre: COM1 \n ",port);
            portFlag = true;
            break;
        case '2':
            strcpy (PSerie,"COM2");
            printf("Puerto elegido: %c nombre: COM2 \n",port);
            portFlag = true;
            break;
        case '3':
            strcpy (PSerie,"COM3");
            printf("Puerto elegido: %c nombre: COM3 \n",port);
            portFlag = true;
            break;
        case '4':
            strcpy (PSerie,"COM4");
            printf("Puerto elegido: %c nombre: COM4 \n",port);
            portFlag = true;
            break;
        default:
            printf("Puerto elegido no valido...\n");

            break;

        }

    }


}

//Now we will choose the transmission velocity that we will use in the "open port" method,
//there are few possibilities to choose like in the previous switch, the user will decide all
int chooseVel() {

    int velocity;
    char option;
    bool velocityFlag = false;
    while(!velocityFlag) {

        printf("Seleccione la velocidad de transmision :  \n 1. 1400 \n 2. 2400 \n 3. 4800 \n 4. 9600 \n 5. 19200 \n");

        option = getch();
        //Screen will be cleaned
        system("cls");
        switch(option) {

        case '1':
            printf("Velocidad elegida 1: 1400\n");
            velocityFlag = true;
            return velocity = 1400;
            break;
        case '2':
            printf("Velocidad elegida 2: 2400\n");
            velocityFlag = true;
            return velocity = 2400;
            break;
        case '3':
            printf("Velocidad elegida 3: 4800\n");
            velocityFlag = true;
            return velocity = 4800;
            break;
        case '4':
            printf("Velocidad elegida 4: 9600\n");
            velocityFlag = true;
            return velocity = 9600;
            break;
        case '5':
            printf("Velocidad elegida 5: 19200\n");
            velocityFlag = true;
            return velocity = 19200;
            break;

        default:
            printf("Velocidad elegida no valido, saliendo...\n");

            break; }

    }


}



//We receive the frame and manage it
void receiveFrame(int &campo,HANDLE &portCOM,int &isControlFrame) {
    char carR = RecibirCaracter(portCOM);

    unsigned char bce;
    // If our string have any character, it will be shown
    if (carR != 0) {
        //this switch will save the received attributes of a control frame and will build it
        //it will print a message announcing the type of the control frame received
        //With Data Frame modification, this switch will choose between control and data frame, building it to be sent
        switch(campo) {

        case 1:

            if (carR==22) {
                controlReceive.setS(carR);
                fReceive.setS(carR);
                campo++; }
            break;

        case 2:

            controlReceive.setD(carR);
            fReceive.setD(carR);
            campo++;
            break;

        case 3:


            if(carR == 02) {

                fReceive.setC(carR);
                isControlFrame = 0;
            }
            if(carR !=02) {

                controlReceive.setC(carR);
                isControlFrame = 1;
            }

            campo++;
            break;

        //Case 4 will print the kind of the control frame received (if it's a control frame, else continue)
        case 4:
            if(isControlFrame == 1) {

                controlReceive.setNT(carR);
                campo = 1;

                if(controlReceive.getC() == 05) {
                    printf("Se ha recibido una trama ENQ\n"); }
                else if (controlReceive.getC()==04) {
                    printf("Se ha recibido una trama EOT\n"); }
                else if (controlReceive.getC()==06) {
                    printf("Se ha recibido una trama ACK\n"); }
                else if  (controlReceive.getC()==21) {
                    printf("Se ha recibido una trama NACK\n"); }


            }
            else {
                fReceive.setNT(carR);
                campo++;
                 }
            break;

        case 5:
            fReceive.setL((unsigned char)carR);
            campo++;

        case 6:
            fReceive.insertData(i,carR);
             if(i < (int)fReceive.getL()-1){
                i++;
            }else{
            fReceive.insertData(i+1,'\0');
            i=0;
            campo++;
            }
            break;

        case 7:
            fReceive.setBCE((unsigned char) carR);
            campo = 1;

            //Here, bce will be calculated based in the received data
            bce = fReceive.calculateBCE();
            if(bce = fReceive.getBCE()) {
            //If bce is well calculated, the data has been received without issues, show data
            fReceive.showData(pantalla,colour);
            }else{
            printf("Error al comprobar BCE. \n");
            }
            break;

        default:
            printf("Trama no recibida correctamente. \n");
            break;
        }
    }
}

//Send topic, it will send a message msg with size tamanio through the port portCOM
void send(char carE,char msg[],int &tamanio,HANDLE &portCOM) {
    switch (carE) {
    //if F1 key is pressed, the message will be sent
    case '\0':

        switch (getch()) {

        case F1:
            //Manage the Frame which will be sent, this topic will divide the Data if it is too long
            //And send several frames to complete the original message

            msg[tamanio]='\n';
            tamanio++;
            msg[tamanio]='\0';
            fReceive.manageFrame(portCOM,msg,tamanio);
            printf("\n");
            tamanio = 0;

            break;

        case F2:
            controlReceive.sendControlFrame(portCOM);
            break; }
        break;
    // If intro key is pressed, we will show and end line and continue the input in the next one
    case INTRO_KEY:
        if (tamanio<limit-2){
            printf ("\n");
            msg[tamanio] = '\n';
            tamanio += 1 ;
            }
        break;

    // If backspace key is pressed, we will delete the last character
    case RETURN_KEY:

        if (tamanio > 0) {
            colour =0 + 0*16;
            pantalla = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute (pantalla, colour);
            printf("\b \b");
            tamanio = tamanio - 1;
        }
        break;

    //default case will prevent the exceeding input in the array
    default:
        if (tamanio<limit-2) {
                colour =12 + 0*16;
                pantalla = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute (pantalla, colour);
                msg[tamanio] = carE;
                printf("%c",carE);
                tamanio = tamanio + 1;

        }
        break;
    }

}

int main() {
    //  ControFrame controlF = new ControlFrame();
    char carE = 0;
    char PSerie[5];
    char msg[limit] ; //two more characters to the line end
    int campo=1;
    int isControlFrame;
    //ControlFrame *fReceive = new ControlFrame();

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

    choosePort(PSerie);
    //goto doFinish;
    int choosedVel = chooseVel();

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
            receiveFrame(campo,portCOM,isControlFrame);


        if (kbhit()) {
            //saving the input
            carE = getch();
            //Esc key case, if it isn't pressed, the switch will be displayed
            if (carE != ESC_KEY) {
                send(carE,msg,tamanio,portCOM);
            }
        }
    }
    //Te port will be closed and the app will return
    //doFinish:
    CerrarPuerto(portCOM);
    return 0; }
