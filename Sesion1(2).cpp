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
int main()
{
    char carE, carR = 0;
    char PSerie[5];
    char msg[limit] ; //two more characters to the line end
    //Encabezado
    printf("============================================================================\n");
    printf("----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES ------------\n");
    printf("---------------------------- CURSO 2019/20 ---------------------------------\n");
    printf("----------------------------- SESION1.CPP ----------------------------------\n");
    printf("============================================================================\n\n");

    //Abrimos el puerto. Para ello necesitamos indicar los siguientes parámetros:
    // - Nombre del port a abrir: ("COM1", "COM2", "COM3", ...).
    // - Velocidad: (1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200).
    // - Número de bits en cada byte enviado o recibido: (4, 5, 6, 7, 8).
    // - Paridad: (0=sin paridad, 1=impar, 2=par, 3=marca, 4=espacio).
    // - Bits de stop: (0=1 bit, 1=1.5 bits, 2=2 bits).

    choosePort(PSerie);
    printf(PSerie);
    int choosedVel = chooseVel();

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
    while(carE != 27)
    {
        carR = RecibirCaracter(portCOM);
        if (carR != 0)
            printf("%c",carR);
        if (kbhit())
        {
            carE = getch();
            if (carE != 27)
            {

                switch (carE)
                {

                case '\0':

                    switch (getch())
                    {
                    case 59:
                        msg[0]=NULL;
                        msg[tamanio+1] = '\n';
                        msg[tamanio+2] = '\0';

                        EnviarCadena(portCOM, msg, tamanio+2);
                        printf("\n");
                        tamanio = 0;
                        break;

                    }



                    break;

                // Backspace to erase the last character
                case 13:

                        msg[tamanio+1] = '\n';
                                            tamanio += 1 ;


                    printf ("\n");
                   break;

                case 8:

                    if (tamanio > 0)
                    {
                        printf("\b \b");
                        tamanio = tamanio - 1;
                    }
                    break;

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

    CerrarPuerto(portCOM);
    return 0;
}
