/*
 * File:   Xboard.cpp
 * Author: Flip4it chess
 *
 * Created on March 12, 2010, 9:31 PM
 */

#include "Xboard.h"
#include "Game.h"
#include "stuff.h"


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>


extern Game joc;
extern FILE* debug_file;

Xboard::Xboard() {



}


int Xboard::giveCommandToXboard(char *command){

      //  int nr_char=strlen(command);

	//char *s;
	//s=(char*)malloc(sizeof(char)*nr_char+1);
	// se copiaza comanda in s si se adauga sfarsit de linie
	//strcpy(s,command);
	//s[nr_char]='\n';
	//toate comenzile catre Xboard se termina cu \n
    
    fputs(command,stdout);
    
        // se trimite comanda catre Xboard
	//write(1,s,nr_char+1);

        //se elibereaza memoria alocata
	//free(s);

        return 0;
}



void Xboard::initializeFeatures(){

    // functia este accesata ca raspuns la comanda protover 2 data de Xboard
    // functia seteaza numele engine-lui, sa citeasca si trimita in SAN
	char comm1[100]="feature myname=\"Flip4itEngine\" san=1 usermove=1\n";
	char comm2[20]="feature done=1\n";
    giveCommandToXboard(comm1);

    // aici se bag initializarile premergatoare jocului
//    joc.initialize();

    giveCommandToXboard(comm2);
}
void Xboard::moveCommandXboardToEngine(char* command){

    // TO DO

	//int poz;
	char san[20];

        sscanf(command,"%*s %s\n",san);
        //interpreteaza mutarea si o salveaza in san, din formatul ""
        //sprintf(buffer,"move %s",SAN);
       // Xboard::giveCommandToXboard(buffer);


        // de facut logica pentru  FORCE mode


        // de apelat functie din Game ce actualizeaza tabela de sah
        // dupa care se face o mutare de catre engine
      //  joc.getCurrentBoard().updateBoard(san,joc.getOponentColor());

}

    void Xboard::illegalMoveEngineToXboard(char *command){

    }
   void  Xboard::resignEngineToXboard(){
         
       char comm1[30]="1-0 {BLACK resigns}\n";
       char comm2[30]="0-1 {WHITE resigns}\n";
       if(joc.colorEngine==BLACK) giveCommandToXboard(comm1);
         else
             giveCommandToXboard(comm2);
	//altfel programul da exit unexpectedly;
        // asa ca trebuie o bucla while(1)
	

    }

    void Xboard::newCommandXboardToEngine(char *command){
        /* specificatiile:
         Ce trebuie sa faca comanda:
         * 1.reseteaza tabela de sah la pozitia initiala
         * 2.seteaza ca albul sa mute
         * 3.iese din force mode si face ca enginu-l sa joace cu culoarea neagra
         * 4.asociaza clock-ul enginu-lui cu Black clock-ul oponentului cu White
         * 5.reseteaza clock-urile si time controlurile la inceputul noului joc
         * 6.foloseste "WALL clock" pentru masurarea timpului
         * 7.opreste clock-urile
         * 8.No pondering on move, remove search depth limit (setata inainte)
        */

        joc.gameReset();
        joc.onTheMove=WHITE;
        joc.forceMode=false;
        joc.colorEngine=BLACK;

        // reseteaza tabela de sah
           // iese din force mode
        // pune enginul pe black
         // opoentul pe white



    }
    void Xboard::forceCommandXboardToEngine(char *command){


        // se seteaza force mode
        // moves received in force mode are legal and in proper turn
        // not think ponder or make moves
        joc.forceMode=true;


    }
    void Xboard::goCommandXboardToEngine(char *command){

        // paraseste force mode

        joc.forceMode=false;
        // asociating clocks
        joc.colorEngine=joc.onTheMove;
        


    }
    void Xboard::whiteCommandXBoardToEngine(char* command){
        joc.onTheMove=WHITE;
        joc.colorEngine=BLACK;

    }


    void Xboard::blackCommandXboardToEngine(char *command){
        joc.onTheMove=BLACK;
        joc.colorEngine=WHITE;

    }
    void Xboard::quitCommandXboardToEngine(char *command){
        // chess engine should exit immediately
        // free memory activate distructors for objects...


        exit(0);
    }


void   Xboard::sendMoveToXboard(char* san){

        char move[50];
        // creeaza comanda move san
        sprintf(move,"move %s\n",san);
        giveCommandToXboard(move);

}

char* Xboard::getFirstMove(){
    int move=0;
    char command[100];
    
    char mv[20];
    int time_aux;
    int go=0;
    int computer=0;
	char* san=NULL;

    while(move==0 && go==0){

        fgets(command,100,stdin);


// in functie de tipul comenzii sunt luate diversele actiuni
		if((strstr(command,"usermove")!= NULL) && (command[0]=='u') ){

                     sscanf(command,"%*s %s\n",mv);


                     san=strdup(mv);
			move=1;
                

                     break;


                }
                if(strcmp("force\n",command)==0){
                    forceCommandXboardToEngine(command);

                }
                if(strcmp("computer\n",command)==0){
                    computer=1;
                 
                }
                if(strcmp("white\n",command)==0){
                    whiteCommandXBoardToEngine(command);


                }
                if(strcmp("black\n",command)==0){
                    blackCommandXboardToEngine(command);

                }
                if(strcmp("new\n",command)==0){
                    newCommandXboardToEngine(command);
                }

                if(strcmp("quit\n",command)==0){
                    quitCommandXboardToEngine(command);
                }
                if(strcmp("go\n",command)==0){
                    goCommandXboardToEngine(command);
                   
                    go=1;break;
                }
                if(strstr("time",command)!=NULL){
                   sscanf(command,"%*s %d\n",&time_aux);
				   fprintf(debug_file,"Time Xboard %d\n",time_aux);
                   time=time_aux;
                }
                if(strstr("otime",command)!=NULL){
                    sscanf(command,"%*s %d\n",&time_aux);
                    otime=time_aux;
                }


    }
       // fputs("decizie\n",stdout);
    if(computer==1 && move==1)
    {//fputs("e BLACK multi engine mode",stdout);
   // fputs(san,stdout);
        return san;

    }

        if(computer==0 && move==1){
           //  fputs("e BLACK single engine mode ",stdout);
          //   fputs(san,stdout);
            return san;

    }
       // fputs("e WHITE multi engine mode",stdout);

        return NULL;




}
char*  Xboard::getMoveFromXboard(){

    int move=0;
    char command[100];
 
    char mv[20];
    int time_aux;
    char* san;
    while(move==0){
    // se citeste comanda pana cand se intalneste o comanda usermove SAN dupa
    // care se returneaza mutarea
    fgets(command,100,stdin);


// in functie de tipul comenzii sunt luate diversele actiuni
		if((strstr(command,"usermove")!= NULL) && (command[0]=='u')){

                     sscanf(command,"%*s %s\n",mv);
                     move=1;
			san=strdup(mv);
                     return san;


                }
           

                if(strcmp("force\n",command)==0){
                    forceCommandXboardToEngine(command);

                }
                if(strcmp("white\n",command)==0){
                    whiteCommandXBoardToEngine(command);


                }
                if(strcmp("black\n",command)==0){
                    blackCommandXboardToEngine(command);

                }
                if(strcmp("new\n",command)==0){
                    newCommandXboardToEngine(command);
                }

                if(strcmp("quit\n",command)==0){
                    quitCommandXboardToEngine(command);
                }
             if(strcmp("go\n",command)==0){
                    goCommandXboardToEngine(command);
                }
             if(strstr(command,"time")!=NULL){
                   sscanf(command,"%*s %d\n",&time_aux);
				   fprintf(debug_file,"Time Xboard: %d",time_aux);
                   joc.time=time_aux;
                }
             if(strstr(command,"otime")!=NULL){
                    sscanf(command,"%*s %d\n",&time_aux);
                    joc.otime=time_aux;
                }

               // totul este ok

    }
    return NULL;
}

int   Xboard::initialize(){


    char command[40];

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr,NULL);
    // setare buffere null
    while(1){




             fgets(command,40,stdin);
        //     fputs(command,stdout);
          if(strcmp("protover 2\n",command)==0){
             // fputs("da initializat",stdout);

              initializeFeatures();
              // daca initializarile s-au facut ok, return 1
              return 1;
          }




    }

    //daca ceva nu este ok return 0
       return 0;
    }