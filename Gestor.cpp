#include "Gestor.h"

#define ESC_KEY (27)
#define RETURN_KEY (8)
#define INTRO_KEY (13)
#define F1 (59)
#define F2 (60)
#define F3 (61)
#define F5 (63)

ifstream inStream;
ofstream outStream;
ofstream logStream;

Gestor::Gestor() {
    limit = 802;
    isFile = false;
    endOfFile = false;
    log = false;
    field=1;
    i = 0;
    flag = false;
    size = 0;
    screen = GetStdHandle(STD_OUTPUT_HANDLE); }



void Gestor::choosePort() {


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

void Gestor::chooseVel() {

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
            velocity = 1400;
            break;
        case '2':
            printf("Velocidad elegida 2: 2400\n");
            velocityFlag = true;
            velocity = 2400;
            break;
        case '3':
            printf("Velocidad elegida 3: 4800\n");
            velocityFlag = true;
            velocity = 4800;
            break;
        case '4':
            printf("Velocidad elegida 4: 9600\n");
            velocityFlag = true;
            velocity = 9600;
            break;
        case '5':
            printf("Velocidad elegida 5: 19200\n");
            velocityFlag = true;
            velocity = 19200;
            break;

        default:
            printf("Velocidad elegida no valido, saliendo...\n");
            break;

        }

    }


    portCOM = AbrirPuerto(PSerie,velocity,8,0,0);


    if(portCOM == NULL) {
        printf("Error al abrir el port %s\n",PSerie);
        getch();

    }
    else {
        printf("port %s abierto correctamente\n",PSerie); }


}

HANDLE Gestor::getPortCom() {

    return portCOM; }

void Gestor::receiveFrame(int &field,int &isControlFrame,int &colouro) {
    char carR = RecibirCaracter(portCOM);
    unsigned char bce;
    // If our string have any character, it will be shown
    if (carR != 0) {
        //this switch will save the received attributes of a control frame and will build it
        //it will print a message announcing the type of the control frame received
        //With Data Frame modification, this switch will choose between control and data frame, building it to be sent
        switch(field) {

        case 1:

            if (carR==22) {
                controlReceive.setS(carR);
                fReceive.setS(carR);
                field++;

            } if(carR == '{') {
                //Receiver know that the following frames will be about file process
                isFile = true;

                //Receiver know that the file process is ending
            } if(carR == '}') {
                outStream.close();
                isFile = false;
                endOfFile = true;


                SetConsoleTextAttribute (screen, colouro);
                printf("Fichero Recibido. \n");


                if(log) {
                    logStream << "Fichero Recibido. \n"; } }

            break;

        case 2:

            controlReceive.setD(carR);
            fReceive.setD(carR);
            field++;
            break;

        case 3:


            if(carR == 02) {
                fReceive.setC(carR);
                isControlFrame = 0; }
            if(carR !=02) {
                controlReceive.setC(carR);
                isControlFrame = 1; }
            field++;
            break;

        //Case 4 will print the kind of the control frame received (if it's a control frame, else continue)
        case 4:

            if(isControlFrame == 1) {
                controlReceive.setNT(carR);
                field = 1;

                if(controlReceive.getC() == 05) {
                    printf("Se ha recibido una trama ENQ\n");
                    if(log) {
                        logStream <<"Se ha recibido una trama ENQ\n"; } }

                else if (controlReceive.getC()==04) {
                    printf("Se ha recibido una trama EOT\n");
                    if(log) {
                        logStream <<"Se ha recibido una trama EOT\n"; } }
                else if (controlReceive.getC()==06) {
                    printf("Se ha recibido una trama ACK\n");
                    if(log) {
                        logStream <<"Se ha recibido una trama ACK\n"; } }
                else if  (controlReceive.getC()==21) {
                    printf("Se ha recibido una trama NACK\n");
                    if(log) {
                        logStream <<"Se ha recibido una trama NACK\n"; }

                } }
            else {
                fReceive.setNT(carR);
                field++; }
            break;

        case 5:

            fReceive.setL((unsigned char)carR);
            field++;

        case 6:

            RecibirCadena(portCOM,fReceive.getData(),(int)fReceive.getL());
            fReceive.insertData('\0');
            field++;
            break;

        case 7:

            fReceive.setBCE((unsigned char) carR);
            field = 1;

            //Here, bce will be calculated based in the received data
            bce = fReceive.calculateBCE();
            if(bce = fReceive.getBCE()) {
                //If bce is well calculated, the data has been received without issues, show data

                if(isFile) {
                    //if file process is initialized, instead f show data, the file will be written
                    fReceive.writeFile(outStream,screen,log,logStream,colouro);



                }
                else if (endOfFile) {
                    //If the file process is end, we will send the size of the document


                    SetConsoleTextAttribute (screen, colouro);

                    printf("El fichero recibido tiene un tamanio de %s bytes.\n", fReceive.getData());

                    if(log) {
                        logStream <<"El fichero recibido tiene un tamanio de "<<fReceive.getData()<<" bytes.\n"; }
                    endOfFile = false;

                }
                else {
                    fReceive.showData(screen);
                    if(log) {
                        logStream.write(fReceive.getData(),fReceive.getL()); } } }
            else {
                if(isFile) {
                    printf("Error en la recepcion de la trama del fichero. \n");
                    if(log) {
                        logStream <<"Error en la rececpcion del la trama del fichero. \n"; } }
                else {

                    printf("Error en la trama recibida \n");
                    if(log) {
                        logStream <<"Error en la trama recibida \n"; } } }
            break;

        default:

            printf("Trama no recibida correctamente. \n");
            if(log) {
                logStream <<"Trama no recibida correctamente.\n"; }
            break;

        }
    }

}



void Gestor::processFile() {
    int numCar;
    int cont=0;
    int tamF = 0;
    char key;
    bool exit = false;
    char numCar2[200];
    char cadcolour[200];

    //First of all, we must open the original file
    inStream.open("fichero-e.txt");
    //If the opening is correct, program will follow the execution
    if (inStream.is_open()) {
        //Character which will tell the receiver that the following frames will be about files
        EnviarCaracter(portCOM, '{');

        if(cont == 0 ) {
            inStream.getline(stringAux,254,'\n');
            printf("Enviando fichero por %s. \n",stringAux);
            fSend.setL(strlen(stringAux));
            fSend.setBCE(fSend.calcularBCE_2(stringAux));
            fSend.sendDataFrame2(portCOM,stringAux);
            receiveFrame(field,isControlFrame,colouro);






            cont++;
            if(log == true) {
                logStream <<"Enviando fichero por "<< stringAux<< "\n"; } }

        //Color and background
        if(cont ==1) {
            inStream.getline(stringAux,254,'\n');
            strcpy(cadcolour,stringAux);
            fSend.setL(strlen(stringAux));
            fSend.setBCE(fSend.calcularBCE_2(stringAux));
            fSend.sendDataFrame2(portCOM,stringAux);
            receiveFrame(field,isControlFrame,colouro);










            cont++;

        }

        //Name of the receiver file
        if(cont ==2) {
            inStream.getline(stringAux,254,'\n');
            fSend.setL(strlen(stringAux));
            fSend.setBCE(fSend.calcularBCE_2(stringAux));
            fSend.sendDataFrame2(portCOM,stringAux);
            receiveFrame(field,isControlFrame,colouro);




            cont++; }


        while (!inStream.eof() && !exit) {
            //Reading authors of the file (not practice authors necessary)

            //Data of the original file to send
            if(cont==3) {
                inStream.read(stringAux, 254);
                numCar = inStream.gcount();
                stringAux[numCar] = '\0';
                tamF += numCar;
                if(numCar!=0) {
                    fSend.setL(numCar);
                    fSend.setBCE(fSend.calcularBCE_2(stringAux));
                    fSend.sendDataFrame2(portCOM,stringAux);
                    receiveFrame(field,isControlFrame,colouro);




                }




            }


            //ESC key case to cancel the process
            if (kbhit()) {
                key = getch();
                if (key == 27) {
                    exit = true; } }

            receiveFrame(field,isControlFrame,colouro);

        }


        sprintf(numCar2,"%d",tamF);
        inStream.close();
        //Send the character to tell the receiver that the file process is ending
        EnviarCaracter(portCOM, '}');
        fSend.manageFrame(portCOM,numCar2,strlen(numCar2));


        //All works correctly and the file is sent
        colouro = atoi(cadcolour) + 0*16;
        SetConsoleTextAttribute (screen, colouro);
        printf("Fichero enviado. \n");

        if(log == true) {
            logStream << "Fichero enviado. \n"; }

    }
    else {
        colouro = atoi(cadcolour) + 0*16;
        SetConsoleTextAttribute (screen, colouro);
        printf("Fichero no encontrado. \n");
        if(log == true) {
            logStream <<"Fichero no encontrado. \n";        } }

}


void Gestor::send(char &carE,char msg[],int &size,int &colouro) {


    switch (carE) {
    //if F1 key is pressed, the message will be sent
    case '\0':

        switch (getch()) {

        case F1:
            //Manage the Frame which will be sent, this topic will divide the Data if it is too long
            //And send several frames to complete the original message
            msg[size]='\n';
            size++;
            msg[size]='\0';
            fSend.manageFrame(portCOM,msg,size);


            if(log) {
                logStream.write(msg,size); }
            printf("\n");
            if(log) {
                logStream <<"\n"; }

            size = 0;


            break;

        case F2:
            controlSend.sendControlFrame(portCOM,log,logStream,screen);


            break;

        case F3:
            processFile();
            break;

        //F5 case will write in another file
        case F5:
            logStream.open("log.txt",ios::app);
            log = true;
            break;

        }
        break;

    // If intro key is pressed, we will show and end line and continue the input in the next one
    case INTRO_KEY:

        if (size<limit-2) {
            printf ("\n");
            msg[size] = '\n';
            size += 1 ; }
        break;

    // If backspace key is pressed, we will delete the last character
    case RETURN_KEY:

        if (size > 0) {
            SetConsoleTextAttribute (screen, 12);
            printf("\b \b");
            size = size - 1;

        }
        break;

    //default case will prevent the exceeding input in the array
    default:

        if (size<limit-2) {
            SetConsoleTextAttribute (screen, 12);
            msg[size] = carE;
            printf("%c",carE);
            if(log) {
                logStream<<carE; }
            size = size + 1;


        }
        break; } }





Gestor::~Gestor() {
    //dtor
}
