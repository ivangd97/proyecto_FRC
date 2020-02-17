//============================================================================
// ----------- PRÁCTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES -----------
// ---------------------------- CURSO 2019/20 --------------------------------
// ----------------------------- SESION1.CPP ---------------------------------
//============================================================================


#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include "PuertoSerie.h"


#define ESC_KEY (27)
#define RETURN_KEY (8)
#define INTRO_KEY (13)
#define F1 (59)
#define F2 (60)

static unsigned char ACK = 06;
static unsigned char NACK = 21;
static unsigned char ENQ = 05;
static unsigned char EOT = 04;


struct TramaControl
{
    unsigned char S =22;
    unsigned char D ='T';
    unsigned char C ;
    unsigned char NT = '0';

} ;

//We will define the limit as a variable
int limit = 802;

using namespace std;

HANDLE portCOM;

//First of all, we will choose the port that will be opened, we will let the user choose
//from a list of ports and will open it
void choosePort(char PSerie[])
{

    int port;
    printf("Seleccione el puerto que desea abrir: \n 0. Exit \n 1. COM1 \n 2. COM2 \n 3. COM3 \n 4. COM4 \n");
    cin >> port;
    switch(port)
    {
    case 0:
        printf("Saliendo...\n");
        break;
    case 1:
        strcpy (PSerie,"COM1");
        printf("Puerto elegido: %d\n",port);
        break;
    case 2:
        strcpy (PSerie,"COM2");
        printf("Puerto elegido: %d\n",port);
        break;
    case 3:
        strcpy (PSerie,"COM3");
        printf("Puerto elegido: %d\n",port);
        break;
    case 4:
        strcpy (PSerie,"COM4");
        printf("Puerto elegido: %d\n",port);
        break;
    default:
        printf("Puerto elegido no válido...\n");
        break;
    }

}

//Now we will choose the transmission velocity that we will use in the "open port" method,
//there are few possibilities to choose like in the previous switch, the user will decide all
int chooseVel()
{

    int velocity;
    printf("Seleccione la velocidad de transmisión : \n 0. Exit \n 1. 1400 \n 2. 2400 \n 3. 4800 \n 4. 9600 \n 5. 19200 \n");
    cin >> velocity;
    switch(velocity)
    {
    case 0:
        printf("Saliendo....\n");
        break;
    case 1:
        printf("Velocidad elegida: 1400\n");
        return velocity = 1400;
        break;
    case 2:
        printf("Velocidad elegida: 2400\n");
        return velocity = 2400;
        break;
    case 3:
        printf("Velocidad elegida: 4800\n");
        return velocity = 4800;
        break;
    case 4:
        printf("Velocidad elegida: 9600\n");
        return velocity = 9600;
        break;
    case 5:
        printf("Velocidad elegida: 19200\n");
        return velocity = 19200;
        break;

    default:
        printf("Velocidad elegida no válido, saliendo...\n");

        break;
    }
}

char sendControlFrame()
{
    int controlFrame;
    bool exit = false;

    while(!exit)
    {
          printf("Trama de control a enviar : \n 1: Trama ENQ. \n 2: Trama EOT. \n 3: Trama ACK. \n 4: Trama NACK. \n");
            cin>>controlFrame;

        switch (controlFrame)
        {
        case 1:
            printf("Trama ENQ \n");
            exit = true;
            return ENQ;
            break;

        case 2:
            printf("Trama EOT \n");
            exit = true;

            return EOT;
            break;
        case 3:
            printf("Trama ACK \n");
            exit = true;

            return ACK;
            break;
        case 4:
            printf("Trama NACK \n");
            exit = true;

            return NACK;
            break;
        default:
            printf("Trama incorrecta, seleccione de nuevo.\n");
            break;
        }
}

}
int main()
{
    char carE, carR = 0;
    char PSerie[5];
    char msg[limit] ; //two more characters to the line end
    //Header
    printf("============================================================================\n");
    printf("----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES ------------\n");
    printf("---------------------------- CURSO 2019/20 ---------------------------------\n");
    printf("----------------------------- SESION1.CPP ----------------------------------\n");
    printf("============================================================================\n\n");

    //We will open the port. For it we need to define the following parameters:
    // - Open port name: ("COM1", "COM2", "COM3", ...).
    // - Velocity: (1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200).
    // - Bits number in each sent or received byte: (4, 5, 6, 7, 8).
    // - Parity: (0=sin parity, 1=odd, 2=even, 3=mark, 4=space).
    // - Stop bits: (0=1 bit, 1=1.5 bits, 2=2 bits).

    choosePort(PSerie);
    printf(PSerie);
    int choosedVel = chooseVel();
    char control;

    // Here we will open the port choosed by the user
    portCOM = AbrirPuerto(PSerie,choosedVel,8,0,0);

    //Check if the choosed port is valid or not
    if(portCOM == NULL)
    {
        printf("Error al abrir el port %s\n",PSerie);
        getch();
        return (1);
    }
    else
        printf("port %s abierto correctamente\n",PSerie);


    // Simultaneous Read/Write of characters :
    int tamanio = 0;

    //Esc key case to close the program, if esc is not pressed, continue forever
    while(carE != ESC_KEY)
    {
        carR = RecibirCaracter(portCOM);
        // If our string have any character, it will be shown
        if (carR != 0)
            printf("%c",carR);
        if (kbhit())
        {
            //saving the input
            carE = getch();
            //Esc key case, if it isn't pressed, the switch will be displayed
            if (carE != ESC_KEY)
            {

                switch (carE)
                {
                //if F1 key is pressed, the message will be sent
                case '\0':

                    switch (getch())
                    {
                    case F1:
                        msg[0]=NULL;
                        msg[tamanio+1] = '\n';
                        msg[tamanio+2] = '\0';

                        EnviarCadena(portCOM, msg, tamanio+2);
                        printf("\n");
                        tamanio = 0;
                        break;

                    case F2:
                        control = sendControlFrame();
                        EnviarCaracter(portCOM,control);
                        break;



                    }



                    break;


                // If intro key is pressed, we will show and end line and continue the input in the next one
                case INTRO_KEY:
                    if (tamanio<limit-2)

                        msg[tamanio+1] = '\n';
                    tamanio += 1 ;
                    printf ("\n");
                    break;

                // If backspace key is pressed, we will delete the last character
                case RETURN_KEY:

                    if (tamanio > 0)
                    {
                        printf("\b \b");
                        tamanio = tamanio - 1;
                    }
                    break;

                //default case will prevent the exceeding input in the array
                default:
                    if (tamanio<limit-2)
                    {

                        tamanio = tamanio + 1;
                        printf("%c",carE);
                        msg[tamanio] = carE;


                    }
                    break;
                }
            }
        }
    }
    //Te port will be closed and the app will return
    CerrarPuerto(portCOM);
    return 0;
}
