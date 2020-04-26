#include "Gestor.h"

#define ESC_KEY (27)
#define RETURN_KEY (8)
#define INTRO_KEY (13)
#define F1 (59)
#define F2 (60)
#define F3 (61)
#define F5 (63)
#define F6 (64)

ifstream inStream;
ofstream outStream;
ofstream logStream,mStream,eStream;


Gestor::Gestor() {
    sondeo = false;
    cont = 0;
    limit = 802;
    isFile = false;
    endOfFile = false;
    log = false;
    field=1;
    i = 0;
    flag = false;
    size = 0;
    screen = GetStdHandle(STD_OUTPUT_HANDLE);
    protocolo = false;
    line = 0;

}



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

void Gestor::imprimirFrame(){
                SetConsoleTextAttribute (screen, 12);

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

int Gestor::receiveFrame() {
    int tipoTrama = 0;
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
                 if(protocolo==false){
                    SetConsoleTextAttribute (screen, colouro);
                    printf("Fichero Recibido. \n");


                if(log) {
                    logStream << "Fichero Recibido. \n"; }

                    }
            } if(carR == 'E'){
                rolEsclavo();
            }else if (carR == 'M'){

                rolMaestro();
            }
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

                if(protocolo == true){

                    controlReceive.imprimirTramaControl(2,eStream);
                    controlReceive.imprimirTramaControl2(2,mStream);
                    tipoTrama = (int) controlReceive.getC();

                }else{
                    imprimirFrame();
                }
                isControlFrame = 0;
 }
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

                    if(!sondeo){
                    fReceive.writeFile(outStream,screen,log,logStream,colouro,eStream,line);
                    }else{
                    fReceive.writeFile(outStream,screen,log,logStream,colouro,mStream,line);

                    }



                    if(protocolo){

                        if(i<3){

                        SetConsoleTextAttribute (screen, 12);
                        printf("R %c STX %c %d %d\n", fReceive.getD(),fReceive.getNT(), fReceive.getBCE(),fReceive.calculateBCE());

						controlSend.setNT(fReceive.getNT());
						controlSend.setD(fReceive.getD());
                        controlSend.setC(06);
                        controlSend.sendControl(portCOM);
                        if(!sondeo){
                            eStream<<"R "<<fReceive.getD()<<" STX "<<fReceive.getNT()<<" "<<(unsigned int)fReceive.getBCE()<<" "<<(unsigned int)fReceive.calculateBCE()<<"\n";
                            controlSend.imprimirTramaControl(1,eStream);

                        }else{
                            mStream<<"R "<<fReceive.getD()<<" STX "<<fReceive.getNT()<<" "<<(unsigned int)fReceive.getBCE()<<" "<<(unsigned int)fReceive.calculateBCE()<<"\n";
                            controlSend.imprimirTramaControl(1,mStream);

                        }
                        i++;



                        }else{
                        SetConsoleTextAttribute (screen, 2);
                        printf("R %c STX %c %d %d\n", fReceive.getD(),fReceive.getNT(), fReceive.getBCE(),fReceive.calculateBCE());

						controlSend.setNT(fReceive.getNT());
						controlSend.setD(fReceive.getD());
                        controlSend.setC(06);
                        controlSend.sendControl(portCOM);


                        if(!sondeo){
                            eStream<<"R "<<fReceive.getD()<<" STX "<<fReceive.getNT()<<" "<<(unsigned int)fReceive.getBCE()<<" "<<(unsigned int)fReceive.calculateBCE()<<"\n";
                            controlSend.imprimirTramaControl(1,eStream);

                        }else{
                            mStream<<"R "<<fReceive.getD()<<" STX "<<fReceive.getNT()<<" "<<(unsigned int)fReceive.getBCE()<<" "<<(unsigned int)fReceive.calculateBCE()<<"\n";
                            controlSend.imprimirTramaControl(1,mStream);

                        }
                        }
                    }


                }else if (endOfFile) {
                    i =0;
                    line = 0;
                    //If the file process is end, we will send the size of the document

                    SetConsoleTextAttribute (screen, colouro);

                    printf("El fichero recibido tiene un tamanio de %s bytes.\n", fReceive.getData());

                    if(log) {
                    logStream <<"El fichero recibido tiene un tamanio de "<<fReceive.getData()<<" bytes.\n";
                    }
                    endOfFile = false;

                     if(protocolo){
                        SetConsoleTextAttribute (screen, 8);
                        printf("R %c STX %c %d %d\n", fReceive.getD(),fReceive.getNT(), fReceive.getBCE(),fReceive.calculateBCE());
						controlSend.setNT(fReceive.getNT());
						controlSend.setD(fReceive.getD());
                        controlSend.setC(06);
                        controlSend.sendControl(portCOM);
                          if(!sondeo){
                            eStream<<"El fichero recibido tiene un tamanio de "<<fReceive.getData()<<" bytes.\n";
                            eStream<<"R "<<fReceive.getD()<<" STX "<<fReceive.getNT()<<" "<<(unsigned int)fReceive.getBCE()<<" "<<(unsigned int)fReceive.calculateBCE()<<"\n";
                            controlSend.imprimirTramaControl(1,eStream);

                        }else{
                            mStream<<"El fichero recibido tiene un tamanio de "<<fReceive.getData()<<" bytes.\n";
                            mStream<<"R "<<fReceive.getD()<<" STX "<<fReceive.getNT()<<" "<<(unsigned int)fReceive.getBCE()<<" "<<(unsigned int)fReceive.calculateBCE()<<"\n";
                            controlSend.imprimirTramaControl(1,mStream);

                        }

                        SetConsoleTextAttribute (screen, 11);
                        }



                }else {
                    fReceive.showData(screen);
                    if(log) {
                        logStream.write(fReceive.getData(),fReceive.getL()); }
                }


            }else {

                if(protocolo==true){
                    SetConsoleTextAttribute (screen, 12);
                    printf("R %c STX %c %d %d \n",fReceive.getD(),fReceive.getNT(),fReceive.getBCE(),fReceive.calculateBCE());
                    controlSend.setNT(fReceive.getNT());
                    controlSend.setD(fReceive.getD());
                    controlSend.setC(21);
                    controlSend.sendControl(portCOM);
                      if(!sondeo){
                            eStream<<"R "<<fReceive.getD()<<" STX "<<fReceive.getNT()<<" "<<(unsigned int)fReceive.getBCE()<<" "<<(unsigned int)fReceive.calculateBCE()<<"\n";
                            controlSend.imprimirTramaControl(1,eStream);

                        }else{
                            mStream<<"R "<<fReceive.getD()<<" STX "<<fReceive.getNT()<<" "<<(unsigned int)fReceive.getBCE()<<" "<<(unsigned int)fReceive.calculateBCE()<<"\n";
                            controlSend.imprimirTramaControl(1,mStream);

                        }


                    printf("ERROR\n");



                }if(isFile) {
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

    return tipoTrama;

}



void Gestor::processFile() {
    int numCar;
    int cont=0;
    int tamF = 0;
    char key;
    bool exit = false;
    char numCar2[200];
    char autoress[802];


    //First of all, we must open the original file

    inStream.open("EProtoc.txt");
    //If the opening is correct, program will follow the execution
    if (inStream.is_open()) {
        //Character which will tell the receiver that the following frames will be about files
        EnviarCaracter(portCOM, '{');

        while (!inStream.eof() && !exit) {
        if(cont == 0 ) {
            inStream.getline(autoress,254,'\n');
            fSend.setL(strlen(autoress));
            fSend.setBCE(fSend.calcularBCE_2(autoress));
            fSend.sendDataFrame2(portCOM,autoress);
            cont++;

              if(protocolo){
                SetConsoleTextAttribute (screen, 12);
                printf("E %c STX %c %d\n", fSend.getD(), fSend.getNT(),fSend.getBCE());
                if (!sondeo){
                mStream<<"E "<<fSend.getD()<<" STX " <<fSend.getNT()<<" "<<(unsigned int)fSend.getBCE()<<"\n";
                }else{
                eStream<<"E "<<fSend.getD()<<" STX " <<fSend.getNT()<<" "<<(unsigned int)fSend.getBCE()<<"\n";
                }
            while (receiveFrame()!= 06){

                }
                fSend.changeNT();

            }
        }



        //Color and background
        if(cont ==1) {
            inStream.getline(stringAux,254,'\n');
            strcpy(cadcolour,stringAux);
            fSend.setL(strlen(stringAux));
            fSend.setBCE(fSend.calcularBCE_2(stringAux));
            fSend.sendDataFrame2(portCOM,stringAux);

            cont++;
         if(protocolo){
                SetConsoleTextAttribute (screen, 12);
                printf("E %c STX %c %d\n", fSend.getD(), fSend.getNT(),fSend.getBCE());
               if (!sondeo){
                mStream<<"E "<<fSend.getD()<<" STX " <<fSend.getNT()<<" "<<(unsigned int)fSend.getBCE()<<"\n";
                }else{
                eStream<<"E "<<fSend.getD()<<" STX " <<fSend.getNT()<<" "<<(unsigned int)fSend.getBCE()<<"\n";
                }
                while (receiveFrame()!= 06){

                }
                fSend.changeNT();

            }

        }

        //Name of the receiver file
        if(cont ==2) {
            inStream.getline(stringAux,254,'\n');
            fSend.setL(strlen(stringAux));
            fSend.setBCE(fSend.calcularBCE_2(stringAux));
            fSend.sendDataFrame2(portCOM,stringAux);
            cont++;
             if(protocolo){
                SetConsoleTextAttribute (screen, 12);
                printf("E %c STX %c %d\n", fSend.getD(), fSend.getNT(),fSend.getBCE());
                if (!sondeo){

                mStream<<"E "<<fSend.getD()<<" STX " <<fSend.getNT()<<" "<<(unsigned int)fSend.getBCE()<<"\n";
                mStream<<"Enviando fichero por "<< autoress <<". \n";

                }else{

                eStream<<"E "<<fSend.getD()<<" STX " <<fSend.getNT()<<" "<<(unsigned int)fSend.getBCE()<<"\n";

                eStream<<"Enviando fichero por "<< autoress <<". \n";
        }

                while (receiveFrame()!= 06){

                }
                fSend.changeNT();

            }
                SetConsoleTextAttribute (screen,atoi(cadcolour));
                printf("Enviando fichero por %s. \n",autoress);
                if(log){

                    logStream<<"Enviando fichero por "<< autoress <<". \n";
                }

        }

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
                }

                if(protocolo && inStream.gcount()>0){
                SetConsoleTextAttribute (screen,2);
                printf("E %c STX %c %d\n", fSend.getD(), fSend.getNT(),fSend.getBCE());
                if (!sondeo){

                mStream<<"E "<<fSend.getD()<<" STX " <<fSend.getNT()<<" "<<(unsigned int)fSend.getBCE()<<"\n";
                }else{

                eStream<<"E "<<fSend.getD()<<" STX " <<fSend.getNT()<<" "<<(unsigned int)fSend.getBCE()<<"\n";
                }

                while (receiveFrame()!= 06){

                }
                fSend.changeNT();

                }
            }



            //ESC key case to cancel the process
            if (kbhit()) {
                key = getch();
                if (key == 27) {
                    exit = true; } }
            receiveFrame();
        }
        sprintf(numCar2,"%d",tamF);
        inStream.close();

        //Send the character to tell the receiver that the file process is ending
        EnviarCaracter(portCOM, '}');
      //  manageFrame(portCOM,numCar2,strlen(numCar2),field,isControlFrame,colouro,fSend);
        fSend.setL(strlen(numCar2));
        fSend.setData(numCar2);
        fSend.setBCE(fSend.calculateBCE());
        fSend.sendDataFrame2(portCOM,numCar2);
        SetConsoleTextAttribute (screen,atoi(cadcolour));
        printf("El fichero enviado tiene un tamanio de %s bytes.\n", fSend.getData());
        if(protocolo ){
                SetConsoleTextAttribute (screen, 8);
                printf("E %c STX %c %d\n", fSend.getD(), fSend.getNT(),fSend.getBCE());
                if (!sondeo){
                mStream<<"El fichero enviado tiene un tamanio de "<<fSend.getData() <<" bytes.\n";

                mStream<<"E "<<fSend.getD()<<" STX " <<fSend.getNT()<<" "<<(unsigned int)fSend.getBCE()<<"\n";
                }else{
                eStream<<"El fichero enviado tiene un tamanio de "<< fSend.getData()<< " bytes.\n";

                eStream<<"E "<<fSend.getD()<<" STX " <<fSend.getNT()<<" "<<(unsigned int)fSend.getBCE()<<"\n";
                }
                while (receiveFrame()!= 06){

                }
                fSend.changeNT();

                }
        //All works correctly and the file is sent
        colouro = atoi(cadcolour) + 0*16;
        SetConsoleTextAttribute (screen, colouro);


        if(log == true) {
             logStream <<"El fichero enviado tiene un tamanio de "<<fSend.getData()<<" bytes.\n"; }

    }else {
        colouro = atoi(cadcolour) + 0*16;
        SetConsoleTextAttribute (screen, colouro);
        printf("Fichero no encontrado. \n");
        if(log == true) {
            logStream <<"Fichero no encontrado. \n";        }

    }

}
//This topic will divide the message in a little ones which its length is 254

void Gestor::manageFrame(HANDLE &portCOM,char msg[],int tamanio,int &field,int &isControlframe,int &colouro,DataFrame fSend){

    int tamanioAux=0;
	int cutPoint = 0;

	while (tamanio > 0) {
		if (tamanio > 254) {
			tamanio -= 254;
			tamanioAux = 254;
		} else {
			tamanioAux = tamanio;
			tamanio = 0;
		}
		fSend.setL((unsigned char) tamanioAux);
		for (int j = 0; j < tamanioAux; j++) {
			//fSend.getPartialData[j] = msg[j + cutPoint];
			fSend.setPartialData(j,msg[j+cutPoint]);
		}
		if(tamanio!=0){
		cutPoint += 254;
        //Calculate the bce associated to the new little frame
        fSend.setBCE(fSend.calculateBCE()) ;
		//Send the little frame
        fSend.sendDataFrame(portCOM);
        receiveFrame();



		}
	}

    //Last frame char adding
	fSend.setL((unsigned char) tamanioAux);
    fSend.setBCE(fSend.calculateBCE()) ;
	fSend.sendDataFrame(portCOM);
	receiveFrame();



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
            manageFrame(portCOM,msg,size,field,isControlFrame,colouro,fSend);




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

        case F6:
            logStream.close();
            log=false;

            rol();


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
            size = size + 1;


        }
        break; } }


void Gestor::rol(){
    protocolo = true;

    SetConsoleTextAttribute (screen, 10);

    printf("Seleccione MAESTRO o ESCLAVO \n");


    printf("Pulse 1 para MAESTRO \n");


    printf("Pulse 2 para ESCLAVO \n");


    char opcion = getch();
    switch(opcion){
        case '1':
            printf("Has elegido MAESTRO \n");

            mStream.open("Prolog-m.txt",ios::app);

            mStream<<"Seleccione Maestro o ESCLAVO \n";
            mStream<<"Pulse 1 para MAESTRO \n";
            mStream<<"Pulse 2 para ESCLAVO \n";
            mStream<<"Has elegido MAESTRO \n";
            mStream.close();


            EnviarCaracter(portCOM,'E');
            rolMaestro();
            break;
        case '2':
            printf("Has elegido ESCLAVO \n");
            eStream.open("Prolog-e.txt",ios::app);

            eStream<<"Seleccione Maestro o ESCLAVO \n";
            eStream<<"Pulse 1 para MAESTRO \n";
            eStream<<"Pulse 2 para ESCLAVO \n";
            eStream<<"Has elegido ESCLAVO \n";
            eStream.close();

            EnviarCaracter(portCOM,'M');
            rolEsclavo();
            break;

        case ESC_KEY:
            printf("Saliendo del protocolo\n");
            mStream<<"Saliendo del protocolo\n";
            break;
        default:
            printf("Entrada no valida. Introduzca de nuevo una opcion \n");
            mStream<<"Entrada no valida. Introduzca de nuevo una opcion \n";
            rol();
            break;
    }

}

void Gestor::rolMaestro(){
    mStream.open("Prolog-m.txt",ios::app);
    SetConsoleTextAttribute (screen, 10);

    protocolo = true;
    printf("Has seleccionado MAESTRO \n");
    mStream<<"Has seleccionado MAESTRO \n";

    printf("Selecciona la operacion a realizar \n");
    mStream<<"Seleccciona la operacion a realizar \n";

    printf("1.SELECCION \n");
    mStream<<"1.SELECCION \n";

    printf("2.SONDEO \n");
    mStream<<"2.SONDEO \n";

    char opcion = getch();
    switch(opcion){
    case '1':
        printf("Has elegido la operacion SELECCION \n");
        mStream<<"Has elegido la operacion SELECCION \n";
        mStream.close();

        seleccionMaestro();
        break;
    case '2':
        printf("Has elegido la operacion SONDEO \n");
        mStream<<"Has elegido la operacion SONDEO \n";
        mStream.close();
        sondeo = true;
        sondeoMaestro();
        break;
    case ESC_KEY:
        rol();
        break;
    default:
        printf("No es valida esa opcion, pruebe otra vez \n");
        mStream<<"No es valida esa opcion, pruebe otra vez \n";
        mStream.close();

        rolMaestro();
        break;
    }
    sondeo = false;

}

void Gestor::seleccionMaestro(){
    mStream.open("Prolog-m.txt",ios::app);


    //TramaEstablecimiento
    controlSend.setD('R');
    controlSend.setC(05);
    controlSend.setNT('0');
    controlSend.sendControl(portCOM);
    SetConsoleTextAttribute (screen, 1);
    controlSend.imprimirTramaControl(1,mStream);

    //Esperar
    while(receiveFrame()!=06){

    }
    //Fase Transferencia
    fSend.setD('R');
    processFile();

    //Fase de cierre
    controlSend.setC(04);
    controlSend.setNT('0');
    controlSend.sendControl(portCOM);
    SetConsoleTextAttribute (screen, 11);
    controlSend.imprimirTramaControl(1,mStream);

    while(receiveFrame()!=06){

    }

    protocolo = false;
    SetConsoleTextAttribute (screen, 10);

    printf("FIN DE PROTOCOLO \n");
    mStream<<"FIN DE PROTOCOLO \n";

    mStream.close();
    sondeo = false;
}

void Gestor::cierre(){

    SetConsoleTextAttribute (screen, 10);

    controlSend.setC(controlReceive.getC());
    controlSend.setD(controlReceive.getD());
    printf("Liberar el esclavo?\n");
    mStream<<"Liberar el esclavo?\n";
	printf("1-LIBERARLO\n");
	mStream<<"1-LIBERARLO\n";
	printf("2-NO LIBERARLO \n");
	mStream<<"2-NO LIBERARLO \n";
	char opcion = getch();
	switch(opcion){

	case '1':
	    controlSend.setNT(controlReceive.getNT());
	    controlSend.setC(06);
	    controlSend.sendControl(portCOM);
	    controlSend.imprimirTramaControl(1,mStream);
	    break;

    case '2':
        controlSend.setNT(controlReceive.getNT());
	    controlSend.setC(21);
	    controlSend.sendControl(portCOM);
	    controlSend.imprimirTramaControl(1,mStream);
	    while(receiveFrame()!=4){

	    }
        cierre();
        break;

    default:
        printf("Fallo \n");
        cierre();
        break;


	}

	mStream.close();
}

void Gestor::sondeoMaestro(){
    sondeo= true;
    mStream.open("Prolog-m.txt",ios::app);



    //Fase de establecimiento
    controlSend.setD('T');
    controlSend.setC(05);
    controlSend.setNT('0');
    controlSend.sendControl(portCOM);
    SetConsoleTextAttribute (screen, 1);
    controlSend.imprimirTramaControl(1,mStream);

    while (receiveFrame()!= 06){

    }

    //Fase de trasnferencia
    while (receiveFrame()!=04){

    }

    //Fase de cierre
    cierre();
    while(receiveFrame() !=04){
    }
    protocolo = false;


    SetConsoleTextAttribute (screen, 10);
    printf("FIN DE PROTOCOLO \n");
    mStream<<"FIN DE PROTOCOLO \n";
    mStream.close();
    sondeo = false;



}
void Gestor::rolEsclavo(){

    eStream.open("Prolog-e.txt",ios::app);
        SetConsoleTextAttribute (screen, 13);

    protocolo = true;

    printf("Has seleccionado ESCLAVO \n");
    eStream<<"Has seleccionado ESCLAVO \n";

    SetConsoleTextAttribute (screen, 1);

    while(receiveFrame()!=05){

    }


    switch(controlReceive.getD()){

    case 'R':
        eStream.close();
        seleccionEsclavo();
        break;
    case 'T':
        eStream.close();
        sondeoEsclavo();
        break;
    default:
        printf("Trama incorrecta \n");
        eStream<<"Trama incorrecta \n";
        break;
    }
    protocolo = false;
    eStream.close();
}

void Gestor::sondeoEsclavo(){
    sondeo = true;
    eStream.open("Prolog-e.txt",ios::app);

    controlSend.setD(controlReceive.getD());
    controlSend.setC(06);
    controlSend.setNT(controlReceive.getNT());
    controlSend.sendControl(portCOM);
    SetConsoleTextAttribute (screen, 1);
    controlSend.imprimirTramaControl(1,eStream);




 //Fase de transferencia
    fSend.setD('T');
    fSend.setNT('0');
    processFile();

    //Fase de cierre
	controlSend.setC(04);
	controlSend.setD('T');
	controlSend.setNT('0');
	controlReceive.setC(21);
	int resultado = 0;

	while (controlReceive.getC() != 06) {
        SetConsoleTextAttribute (screen, 11);
        controlSend.sendControl(portCOM);
        controlSend.imprimirTramaControl(1,eStream);
		controlSend.changeNT();
		resultado = 0;
		while (resultado != 6 && resultado != 21) {
			resultado = receiveFrame();

		}

	}
	if(controlReceive.getC()==06){
        SetConsoleTextAttribute (screen, 11);
        controlSend.sendControl(portCOM);
        controlSend.imprimirTramaControl(1,eStream);
		controlSend.changeNT();
    }

     SetConsoleTextAttribute (screen, 13);
     printf("FIN DE PROTOCOLO \n");
     eStream<<"FIN DE PROTOCOLO \n";
     eStream.close();
     sondeo = false;
}
void Gestor::seleccionEsclavo(){

    eStream.open("Prolog-e.txt",ios::app);

    controlSend.setD(controlReceive.getD());
    controlSend.setC(06);
    controlSend.setNT(controlReceive.getNT());
    controlSend.sendControl(portCOM);
    SetConsoleTextAttribute (screen, 1);
    controlSend.imprimirTramaControl(1,eStream);

        SetConsoleTextAttribute (screen, 1);


 //Transferencia

 while(receiveFrame()!= 04){

 }

 //Fase de cierre

 controlSend.setC(06);
 controlSend.setD(controlReceive.getD());
 controlSend.setNT(controlReceive.getNT());
 controlSend.sendControl(portCOM);
 SetConsoleTextAttribute (screen, 11);
 controlSend.imprimirTramaControl(1,eStream);



    SetConsoleTextAttribute (screen, 13);
    printf("FIN DE PROTOCOLO \n");
    eStream<<"FIN DE PROTOCOLO \n";
    eStream.close();


}


Gestor::~Gestor() {
    //dtor
}
