/*
 * File:   Book.cpp
 * Author: Flip4it chess
 *
 * Created on March 11, 2010, 7:34 PM
 *
 * Multumiri: Opening Book Module for Chess Programs, written by Ed Schr�der Version 1.00
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Book.h"
#include "ChessBoard.h"

//Diverse constante folosite pentru obtinerea mutarilor
static char BVELD[] = {  1,11,21,31,41,51,61,71,	// MVS format to
		         2,12,22,32,42,52,62,72,	// REBEL format
                         3,13,23,33,43,53,63,73,
                         4,14,24,34,44,54,64,74,
                         5,15,25,35,45,55,65,75,
                         6,16,26,36,46,56,66,76,
                         7,17,27,37,47,57,67,77,
                         8,18,28,38,48,58,68,78 };



 static char TA[] = {                                // ASCII output REBEL moves
        		'X',
                'A','A','A','A','A','A','A','A','X','X',
                'B','B','B','B','B','B','B','B','X','X',
                'C','C','C','C','C','C','C','C','X','X',
                'D','D','D','D','D','D','D','D','X','X',
                'E','E','E','E','E','E','E','E','X','X',
                'F','F','F','F','F','F','F','F','X','X',
                'G','G','G','G','G','G','G','G','X','X',
                'H','H','H','H','H','H','H','H' };

static char TN[] = {			                    // ASCII output REBEL moves
        		'X',
                '1','2','3','4','5','6','7','8','X','X',
                '1','2','3','4','5','6','7','8','X','X',
                '1','2','3','4','5','6','7','8','X','X',
                '1','2','3','4','5','6','7','8','X','X',
                '1','2','3','4','5','6','7','8','X','X',
                '1','2','3','4','5','6','7','8','X','X',
                '1','2','3','4','5','6','7','8','X','X',
                '1','2','3','4','5','6','7','8' };

static char BOEKRND[] = { 8,4,2,1,1,1,1,1,1,1,1,1,1 };

static char BORDPOS[] =
                    {    8,18,28,38,48,58,68,78,
                         7,17,27,37,47,57,67,77,
        		 6,16,26,36,46,56,66,76,
                         5,15,25,35,45,55,65,75,
                         4,14,24,34,44,54,64,74,
                         3,13,23,33,43,53,63,73,
		         2,12,22,32,42,52,62,72,
                         1,11,21,31,41,51,61,71,0 };



extern FILE* debug_file;

//Obtine mutarea urmatoare, pornind de la tabla primita ca parametru;
char* Book::getNextMove (ChessBoard tabla, COLOR culoare)
{
    int i,j,poz;                                                          // DEMONSTRATION
    INITIALIZE();
                                                  // Initialize (one time call)
    if (ERROR)
     { fprintf(debug_file,"Missing obliged file(s) RANDOM1.BIN and/or RANDOM2.BIN"); exit(1); }

    //Construim EPD;
    char EPDTemp[200];
    memset(EPDTemp,' ',64);
    memset(EPD,0,200);

    BITBOARD temp_board[2][6];
    for(i=0;i<=1;i++)
        for(j=0;j<6;j++)
            temp_board[i][j]=tabla.board[i][j];
    
    //punem piesele;
    for(i=0;i<=1;i++)
        for(j=0;j<6;j++)
            for(poz=0;poz<64;poz++)
            {
		int L=poz/8;
		int C=poz%8;
                if(temp_board[i][j]%2==1){    //daca e impar => bitul e 1;
                    if(i==BLACK)    //piese albe
                        switch(j){
                            case PAWN:    EPDTemp[(7-L)*8+C]='p'; break;
                            case KNIGHT:  EPDTemp[(7-L)*8+C]='n'; break;
                            case KING:    EPDTemp[(7-L)*8+C]='k'; break;
                            case BISHOP:  EPDTemp[(7-L)*8+C]='b'; break;
                            case ROOK:    EPDTemp[(7-L)*8+C]='r'; break;
                            case QUEEN:   EPDTemp[(7-L)*8+C]='q'; break;

                        }
                    else
                         switch(j){
                            case PAWN:    EPDTemp[(7-L)*8+C]='P'; break;
                            case KNIGHT:  EPDTemp[(7-L)*8+C]='N'; break;
                            case KING:    EPDTemp[(7-L)*8+C]='K'; break;
                            case BISHOP:  EPDTemp[(7-L)*8+C]='B'; break;
                            case ROOK:    EPDTemp[(7-L)*8+C]='R'; break;
                            case QUEEN:   EPDTemp[(7-L)*8+C]='Q'; break;
                        }
                    }
                    temp_board[i][j]>>=1;

                }
	//Scurtam textul
	int abs=0;
	char k;
	for(i=0;i<8;i++)
	{
	    k=0;
	    for(j=0;j<8;j++)
	    {
		if(EPDTemp[8*i+j]==' ')
		    k++;
		else
		{
		    if(k>0)
			EPD[abs++]=k+'0';
		    EPD[abs++]=EPDTemp[8*i+j];
		    k=0;
		}
	    }
	    if(k>0)
		EPD[abs++]=k+'0';
	    if(i!=7)
		EPD[abs++]='/';
	}

	EPD[abs++]=' ';
	if(culoare==WHITE)
	    EPD[abs++]='w';
	else
	    EPD[abs++]='b';

	strcat(EPD," KQkq -");
	EPD[strlen(EPD)]=0;

	fprintf(debug_file,"%s\n",EPD);
	//EPD[14]='1';

	//Obtinem mutarea in FROM si TO
        FIND_OPENING();
        if (ERROR) fprintf(debug_file,"Something went wrong, error-code %d",ERROR);
	else fprintf(debug_file,"Move: %s-%s\nList:",FROM,TO);

	//Toate posibilitatile:
	for (i=0; i<AZ; i++) fprintf(debug_file,"%c%c-%c%c ",FROM1[i],FROM2[i],TO1[i],TO2[i]);
	    fprintf(debug_file,"\n\n");

	//Nu exista deschidere;
	if(FROM[0]==0)
	    return 0;

	//Obtinem tipul de piesa
	PIECE piesa;
	fprintf(debug_file,"Poz:%d-%d\n",(FROM[1]-'1')*8,FROM[0]-'A');
	switch(EPDTemp[(FROM[1]-'1')*8+FROM[0]-'A'])
	{
	    case 'p': piesa=PAWN; break;
	    case 'r': piesa=ROOK; break;
	    case 'b': piesa=BISHOP; break;
	    case 'k': piesa=KING; break;
	    case 'n': piesa=KNIGHT; break;
	    case 'q': piesa=QUEEN; break;

	    case 'P': piesa=PAWN; break;
	    case 'R': piesa=ROOK; break;
	    case 'B': piesa=BISHOP; break;
	    case 'K': piesa=KING; break;
	    case 'N': piesa=KNIGHT; break;
	    case 'Q': piesa=QUEEN; break;

	}
	//fprintf(debug_file,"-- %lld --\n",(tabla.board[culoare][piesa] & ((U64)1<<((FROM[1]-'1')*8+FROM[0]-'A'))));
	
        fprintf(debug_file,"Piesa:%d\n",piesa);
	if((tabla.board[culoare][piesa] & ((U64)1<<((FROM[1]-'1')*8+FROM[0]-'A'))) == (U64) 0)
		return 0;

        //Analizam rocadele
        if(!strcmp(FROM,"E8") && !strcmp(TO,"G8"))
        {
                fprintf(debug_file,"SAN: O-O");
                return strdup("O-O");
        }
        if(!strcmp(FROM,"E8") && !strcmp(TO,"A8"))
        {
                fprintf(debug_file,"SAN: O-O-O");
                return strdup("O-O-O");
        }
        if(!strcmp(FROM,"E1") && !strcmp(TO,"G1"))
        {
                fprintf(debug_file,"SAN: O-O");
                return strdup("O-O");
        }
        if(!strcmp(FROM,"E1") && !strcmp(TO,"A1"))
        {
                fprintf(debug_file,"SAN: O-O-O");
                return strdup("O-O-O");
        }

	fprintf(debug_file,"SAN:%s\n",tabla.getSANofMove(culoare,(FROM[1]-'1')*8+FROM[0]-'A',(TO[1]-'1')*8+TO[0]-'A',piesa));

	//Intoarcem SAN-ul
	return tabla.getSANofMove(culoare,(FROM[1]-'1')*8+FROM[0]-'A',(TO[1]-'1')*8+TO[0]-'A',piesa);

}



void Book::INITIALIZE()                                               // read hashkeys in memory

{
    strcpy(BOOKFILE,"mainbook.bin");
    strcpy(TOERFILE,"tourbook.bin");
    strcpy(EPD,"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    ERROR=0;
    p_BORD = (long *) _BORD;               // 32 bit redefinition BORD

	int x; FILE *fp1; FILE *fp2;

        ERROR=0;

        fp1=fopen("random1.bin","rb"); if (fp1==NULL) { ERROR=1; return; }
        fp2=fopen("random2.bin","rb"); if (fp2==NULL) { ERROR=1; return; }

        for (x=0; x<1120; x++) { fread(&RANDOM1[x],4,1,fp1); fread(&RANDOM2[x],4,1,fp2); }

        fclose(fp1);
        fclose(fp2);

//        fp1=fopen(TOERFILE,"rb");                             // read tournament book in memory
//        if (fp1==NULL) {                       // no tournament book
//                         _GB[0]=255; _GB[1]=255; return; }
//
//        y=0; while ((x=getc(fp1))!=EOF) { _GB[y]=x; y++; }
//        _GB[y-1]=255; _GB[y-2]=255;
//        fclose(fp1);

}


void Book::FIND_OPENING()                                 // input : BOOKFILE / EPD
                                                    // output: FROM and TO as E2 and E4 and ERROR
{       int xl,px,xx,x,y,rnd,bhk1,bhk2,xd,xe;
        int ch,by,v,kleur,boekfase;
        char db[100],eb[100];
        unsigned char zcd[200],zce[200],vb[100];
        char fentab[]   = "??PNBRQKpnbrqk??";
        char fenkleur[] = "wb??";

        ERROR=0;

        if ((FPB = fopen(BOOKFILE,"rb")) == NULL) { ERROR=2; return; }

        FROM[0]=0; TO[0]=0;
        db[0]=0; eb[0]=0; vb[0]=255;

        RND=clock();                                // initialize randomizer
        
        x=rand()%9999; rnd=RND+x;

        RND=rnd;                                    // debug info

        AZ=0;                                       // number of found book moves

        boekfase=1;                // no tournament book selected -> search main book


//      retrieve EPD
//      ============

        for (x=1; x<=78; x++) if (_BORD[x] > 0) _BORD[x]=1;     // empty board

        x=-1; y=0;
next:   x++; ch=EPD[x];
        if (ch=='/') goto next;
        if (ch >= '1' && ch <= '8') { v=ch-48; y=y+v; goto test; }
        for (v=2; v<=15; v++)
        if (fentab[v]==ch) { _BORD[(int)BORDPOS[y]]=v; y++; break; }
        if (v>13) goto error;
test:   if (y < 64) goto next;

        x++; if (EPD[x] != ' ') goto error;

        x++; if (EPD[x]==fenkleur[0]) { kleur=0; goto done; }   // white to move
             if (EPD[x]==fenkleur[1]) { kleur=1; goto done; }   // black to move

error:  ERROR=3; fclose(FPB); return;


done:   MAKE_HASHKEY(); bhk1=HK1; bhk2=HK2;                     // make hashkey of retrieved EPD



again:  xl=-2; px=-1;

boek10: px++;
boek12: if ((xx = fgetc(FPB)) == EOF) goto boek90;
           zcd[px]=xx; xl=xl+2;
           if ((xx = fgetc(FPB)) == EOF) goto boek90;
           zce[px]=xx; 
        if (zcd[px] < 128) goto boek10;                     // collect moves till end of varition

//      Search the filled stack for possible bookmove
//      =============================================

        x=0; BOEK_BEGIN(); HK1=0x05035c45; HK2=0xf1b92b1f;  // hashkey values start position
boek14: if (bhk1==HK1 && bhk2==HK2) goto boek16;            // hashkeys match, bookmove found
        XD=BVELD[zcd[x] & 0x3f];
        XE=BVELD[zce[x] & 0x3f];
        BOEKIN();                                           // update board and hashkeys
        x++; if (x > px) goto boek20;                       // end of variantion, check for next
        goto boek14;

//      Bookmove found, insert in table
//      ===============================

boek16: if (zce[x]<64) goto boek20;                         // Bad book move, no insert
        if ((x&1) != (kleur&1)) goto boek20;                // colours don't match, no insert
        xd=BVELD[zcd[x]&0x3f]; xe=BVELD[zce[x]&0x3f];       // Bookmove found
        for (v=0; v<AZ; v++)                                // skip possible doubles
         { if (db[v]==xd && eb[v]==xe && vb[v]<64) vb[v]=zce[x];
           if (db[v]==xd && eb[v]==xe) goto boek20; }

        db[AZ]=xd; eb[AZ]=xe; vb[AZ]=zce[x];                // store move in internal format
        FROM1[AZ]=TA[xd]; FROM2[AZ]=TN[xd];                 // store move in ascii format
        TO1[AZ]=TA[xe]; TO2[AZ]=TN[xe]; BOEKSTAT[AZ]=boekfase;
        AZ++;                                               // number of found book moves


//      Spool back till bit-6 becomes zero
//      ==================================

boek20: if (zcd[px] < 192) goto boek12;                         // there is another one !
boek22: px--;
        if (px < 0) goto boek10;
        if (zcd[px] >= 64) goto boek22;
        goto boek12;


//      End of File, decide move to play
//      ================================

boek90: if (boekfase==0 && AZ==0) { boekfase=1; goto again; }   // nothing found in tournament book
                                                                // search now the main book
        if (AZ==0) goto einde;                                  // no book move(s) found

boek92: rnd=rnd & 63; by=-1;
boek94: by++; if (by >= AZ) goto boek92;
        rnd=rnd-BOEKRND[by];
        if (rnd > 0) goto boek94;

        x=db[by]; FROM[0]=TA[x]; FROM[1]=TN[x]; FROM[2]=0;
        x=eb[by]; TO[0]=TA[x]; TO[1]=TN[x]; TO[2]=0;

einde:  fclose(FPB);

}



void Book::BOEK_BEGIN()                                               // start position to BORD

{       p_BORD[0]=0x01020500;           // a0/a1/a2/a3
        p_BORD[1]=0x08010101;           // a4/a5/a6/a7
        p_BORD[2]=0x0300000b;           // a8/a9/a0/b1
        p_BORD[3]=0x01010102;           // b2/b3/b4/b5
        p_BORD[4]=0x00090801;           // b6/b7/b8/b9

        p_BORD[5]=0x01020400;           // b0/c1/c2/c3
        p_BORD[6]=0x08010101;           // c4/c5/c6/c7
        p_BORD[7]=0x0600000a;           // c8/c9/c0/d1
        p_BORD[8]=0x01010102;           // d2/d3/d4/d5
        p_BORD[9]=0x000c0801;           // d6/d7/d8/d9

        p_BORD[10]=0x01020700;          // d0/e1/e2/e3
        p_BORD[11]=0x08010101;          // e4/e5/e6/e7
        p_BORD[12]=0x0400000d;          // e8/e9/e0/f1
        p_BORD[13]=0x01010102;          // f2/f3/f4/f5
        p_BORD[14]=0x000a0801;          // f6/f7/f8/f9

        p_BORD[15]=0x01020300;          // f0/g1/g2/g3
        p_BORD[16]=0x08010101;          // g4/g5/g6/g7
        p_BORD[17]=0x05000009;          // g8/g9/g0/h1
        p_BORD[18]=0x01010102;          // h2/h3/h4/h5
        p_BORD[19]=0x000b0801;          // h6/h7/h8/h9

}



void Book::MAKE_HASHKEY()

{       int x,y,v,sc;

        HK1=0; HK2=0;

        for (x=1; x<=78; x++)
         { if (_BORD[x]<2) continue;
           sc=_BORD[x]; v=sc<<6; y=sc<<4; y=y+v;
           HK1=HK1 ^ RANDOM1[y+x];
           HK2=HK2 ^ RANDOM2[y+x];
         }

}


void Book::BOEKIN()                                                   // update BORD and HASHKEY

{       int sc,z,y0;

        char kntab[] = { 0,0,2,0,0,0,0,1,3,0,0,0,0,1 };

        sc=_BORD[XD]; y0=_BORD[XE];

        z=sc<<6; z=z+(sc<<4);
        HK1=HK1 ^ RANDOM1[z+XD]; HK1=HK1 ^ RANDOM1[z+XE];
        HK2=HK2 ^ RANDOM2[z+XD]; HK2=HK2 ^ RANDOM2[z+XE];

        _BORD[XE]=sc; _BORD[XD]=1;

        if (y0!=1)                                              // remove captured piece from hashkey
         { z=y0<<6; z=z+(y0<<4);
           HK1=HK1 ^ RANDOM1[z+XE];
           HK2=HK2 ^ RANDOM2[z+XE]; }

        if (kntab[sc]==1)                                       // handle castling
         { if (XE-XD==20)  { _BORD[XE+10]=1; _BORD[XE-10]=sc-2; MAKE_HASHKEY(); }
           if (XE-XD==-20) { _BORD[XE-20]=1; _BORD[XE+10]=sc-2; MAKE_HASHKEY(); } }

        if (sc==2 && y0==1 && XE-XD!=1 && XE-XD!=2) { _BORD[XE-1]=1; MAKE_HASHKEY(); }    // ep (white)
        if (sc==8 && y0==1 && XD-XE!=1 && XD-XE!=2) { _BORD[XE+1]=1; MAKE_HASHKEY(); }    // ep (black)

}
//          End of story
