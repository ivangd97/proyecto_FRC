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

using namespace std;

HANDLE PuertoCOM;
void choosePort(char PSerie[])
{

    int puerto;
    printf("Seleccione el puerto que desea abrir: \n 0. Exit \n 1. COM1 \n 2. COM2 \n 3. COM3 \n 4. COM4 \n");
    cin >> puerto;
    switch(puerto)
    {
    case 0:
        printf("Saliendo...\n");
        break;
    case 1:
        strcpy (PSerie,"COM1");
        printf("Puerto elegido: %d\n",puerto);
        break;
    case 2:
        strcpy (PSerie,"COM2");
        printf("Puerto elegido: %d\n",puerto);
        break;
    case 3:
        strcpy (PSerie,"COM3");
        printf("Puerto elegido: %d\n",puerto);
        break;
    case 4:
        strcpy (PSerie,"COM4");
        printf("Puerto elegido: %d\n",puerto);
        break;
    default:
        printf("Puerto elegido no válido...\n");
        break;
    }

}
int chooseVel()
{

    int velocidad;
    printf("Seleccione el puerto que desea abrir: \n 0. Exit \n 1. 1400 \n 2. 2400 \n 3. 4800 \n 4. 9600 \n 5. 19200 \n");
    cin >> velocidad;
    switch(velocidad)
    {
    case 0:
        printf("Saliendo....\n");
        break;
    case 1:
        printf("Velocidad elegida: 1400\n");
        return velocidad = 1400;
        break;
    case 2:
        printf("Velocidad elegida: 2400\n");
        return velocidad = 2400;
        break;
    case 3:
        printf("Velocidad elegida: 4800\n");
        return velocidad = 4800;
        break;
    case 4:
        printf("Velocidad elegida: 9600\n");
        return velocidad = 9600;
        break;
    case 5:
        printf("Velocidad elegida: 19200\n");
        return velocidad = 19200;
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
    char cadena[802]; //dos caracteres mas por el salto de linea \0
    //Encabezado
    printf("============================================================================\n");
    printf("----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES ------------\n");
    printf("---------------------------- CURSO 2019/20 ---------------------------------\n");
    printf("----------------------------- SESION1.CPP ----------------------------------\n");
    printf("============================================================================\n\n");

    //Abrimos el puerto. Para ello necesitamos indicar los siguientes parámetros:
    // - Nombre del puerto a abrir: ("COM1", "COM2", "COM3", ...).
    // - Velocidad: (1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200).
    // - Número de bits en cada byte enviado o recibido: (4, 5, 6, 7, 8).
    // - Paridad: (0=sin paridad, 1=impar, 2=par, 3=marca, 4=espacio).
    // - Bits de stop: (0=1 bit, 1=1.5 bits, 2=2 bits).

    choosePort(PSerie);
    printf(PSerie);
    int velocidadElegida = chooseVel();

    PuertoCOM = AbrirPuerto(PSerie,velocidadElegida,8,0,0);

    // strcpy (PSerie,"COM2");
    //PuertoCOM = AbrirPuerto(PSerie, 9600, 8, 0, 0);
    //Abrimos el puerto COM1 (en la sala siempre abrimos el COM1)
    if(PuertoCOM == NULL)
    {
        printf("Error al abrir el puerto %s\n",PSerie);
        getch();
        return (1);
    }
    else
        printf("Puerto %s abierto correctamente\n",PSerie);


    // Lectura y escritura simultánea de caracteres:
    int tamanio = 0;

    while(carE != 27)
    {
        carR = RecibirCaracter(PuertoCOM);
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
                       cadena[tamanio+1] = '\n';
                       cadena[tamanio+2] = '\0';

                        EnviarCadena(PuertoCOM, cadena, tamanio);
                        printf("\n");
                        tamanio = 0;
                        break;

                    }



                    break;

                // Backspace to erase the last character
                case 13:

                    printf ("\n");
                    //cadena[tamanio] = '\n';

                case 8:

                    if (tamanio > 0)
                    {
                        printf("\b \b");
                        //fflush(stdout);
                        tamanio = tamanio - 1;
                    }
                    break;

                default:
                    if (tamanio<800)
                    {
                        printf("%c",carE);
                        cadena[tamanio] = carE;
                        tamanio = tamanio + 1;


                    }
                    break;
                }
            }
        }
    }

    // We're done, cleanup and exit
    CerrarPuerto(PuertoCOM);
    return 0;
}
