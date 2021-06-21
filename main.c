#include "vxWorks.h"
#include "taskLib.h"
#include "stdio.h"
#include "semLib.h"
#include "wdLib.h"
#include "sysLib.h"
#include "dio96.h"
#include "partie_reactive_architecture.h"

//Addresses Capteurs
#define ADDR_C1 0x04
#define ADDR_C2 0x02
#define ADDR_C3 0x01
#define ADDR_C4 0x80
#define ADDR_C5 0x20
#define ADDR_C6 0x10
#define ADDR_C7 0x08
#define ADDR_C8 0x04
#define ADDR_C9 0x80
#define ADDR_C10 0x80
#define ADDR_DEM 0x04
#define ADDR_URG 0x08


//Adresses Actionneurs

#define ADDR_TR1 0x04
#define ADDR_TR2 0x02
#define ADDR_TR3 0x01
#define ADDR_TR4 0x80
#define ADDR_TR9 0x20
#define ADDR_TR10 0x20
#define ADDR_TR11 0x10


#define ADDR_AG1D 0x08
#define ADDR_AG1G 0x08
#define ADDR_AG2D 0x04
#define ADDR_AG2G 0x04



#define cPriot_atelier 150
#define cPriopartie_reactive_reset_architecture 150
#define cPrioTacheA 150
#define cPrioTacheB 100
#define cPrioTacheC 99
#define cPrioTacheDialogue 50
#define STACK_SIZE 20000
#define ValTempo 200 /* 2s*/


int Tidt_atelier;
int TidTacheA ;
int TidTacheB ;
int TidTacheC ;
int TidTacheDialogue ;

typedef enum {false,true} boolean;


//Entrees
UINT8 PPIB_PA;
UINT8 PPIB_PB;
UINT8 PPIC_PC;
UINT8 PPID_PC;

//Sorties
UINT8 PPIA_PA;
UINT8 PPIA_PB;
UINT8 PPIC_PB;
UINT8 PPID_PB;

inC_partie_reactive_architecture entree_cap;
outC_partie_reactive_architecture sortie_act;




char c;
SEM_ID semA;
SEM_ID semB;

WDOG_ID WdId;
SEM_ID EvtFinTempo;

void FonctTempo(void){
 semGive(EvtFinTempo);
}

void TacheA(void) {
 int i;

 for(;;){
 semTake(semA,WAIT_FOREVER);
 for(i=0;i<99;i++){
  printf("a");
 }
 printf("\n");
 }
}

void TacheB(void) {
 int i;

 for(;;){
 semTake(semB,WAIT_FOREVER);
 for(i=0;i<99;i++){
  printf("b");
 }
 printf("\n");
 }
}

void TacheC(void){
 int i;
 for(;;){
  wdStart(WdId,ValTempo, (FUNCPTR) FonctTempo,0);
  semTake(EvtFinTempo, WAIT_FOREVER);
  for(i=0;i<100;i++){
   printf("c");
  }
 }

}

void TacheDialogue(void) {
 char ch;

 for(;;){
  ch= getchar();
  if(ch=='a') {
   semGive(semA);
  } else if(ch=='b') {
   semGive(semB);
  }else {
    break;
  }
 }
}

void lecture_capteurs(inC_partie_reactive_architecture* entree_cap)
{
 boolean C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,DEM,URG;

 lecturePorts(&PPIB_PA,&PPIB_PB,&PPIC_PC,&PPID_PC);


 C1=((PPIB_PB&ADDR_C1)==ADDR_C1);
 C2=((PPIB_PB&ADDR_C2)==ADDR_C2);
 C3=((PPIB_PB&ADDR_C3)==ADDR_C3);
 C4=((PPIB_PA&ADDR_C4)==ADDR_C4);
 C5=((PPIB_PA&ADDR_C5)==ADDR_C5);
 C6=((PPIB_PA&ADDR_C6)==ADDR_C6);
 C7=((PPIB_PA&ADDR_C7)==ADDR_C7);
 C8=((PPIB_PA&ADDR_C8)==ADDR_C8);
 C9=((PPIC_PC&ADDR_C9)==ADDR_C9);
 C10=((PPID_PC&ADDR_C10)==ADDR_C10);
 DEM=((PPIC_PC&ADDR_DEM)==ADDR_DEM);
 URG=((PPID_PC&ADDR_URG)==ADDR_URG);

 printf("PPIB_PA %x\n",PPIB_PA);
 printf("PPIB_PB %x\n",PPIB_PB);
 printf("PPIC_PC %x\n",PPIC_PC);
 printf("PPID_PC %x\n",PPID_PC);

 //Capteurs
 entree_cap->info_capteurs[0]=C1;
 entree_cap->info_capteurs[1]=C2;
 entree_cap->info_capteurs[2]=C3;
 entree_cap->info_capteurs[3]=C4;
 entree_cap->info_capteurs[4]=C5;
 entree_cap->info_capteurs[5]=C6;
 entree_cap->info_capteurs[6]=C7;
 entree_cap->info_capteurs[7]=C8;
 entree_cap->info_capteurs[8]=C9;
 entree_cap->info_capteurs[9]=C10;

 //type_piece
 entree_cap->type_piece[0]=DEM;
 entree_cap->type_piece[1]=URG;


 //a completer

}

void ecriture_act(outC_partie_reactive_architecture* sortie_act)
{
 boolean TR_1,TR_2,TR_3,TR_4,TR_9,TR_10,TR_11,AG1D,AG1G,AG2D,AG2G;


 TR_1=sortie_act->Cde_troncons[0];
 TR_2=sortie_act->Cde_troncons[1];
 TR_3=sortie_act->Cde_troncons[2];
 TR_4=sortie_act->Cde_troncons[3];
 TR_9=sortie_act->Cde_troncons[4];
 TR_10=sortie_act->Cde_troncons[5];
 TR_11=sortie_act->Cde_troncons[6];

 AG1D=sortie_act->Cde_aiguillage[0];
 AG1G=sortie_act->Cde_aiguillage[1];
 AG2D=sortie_act->Cde_aiguillage[2];
 AG2G=sortie_act->Cde_aiguillage[3];


 PPIA_PA=
 ADDR_TR4*TR_4+
 ADDR_TR9*TR_9+
 ADDR_TR10*TR_10+
 ADDR_TR11*TR_11;

 PPIA_PB=
 ADDR_TR1*TR_1+
 ADDR_TR2*TR_2+
 ADDR_TR3*TR_3;

 PPIC_PB=
 ADDR_TR9*TR_4+
 ADDR_TR11*TR_11+
 ADDR_AG1D*AG1D+
 ADDR_AG2D*AG2D;

 PPID_PB=
 ADDR_TR10*TR_10+
 ADDR_AG1G*AG1G+
 ADDR_AG2G*AG2G;

 ecriturePorts(PPIA_PA,PPIA_PB,PPIC_PB,PPID_PB);
}

void t_atelier(void)
{
 for(;;)
 {
  taskDelay(sysClkRateGet()/10);
  //
  lecture_capteurs(&entree_cap);
  //
  partie_reactive_architecture
   (&entree_cap,&sortie_act);

  //
  ecriture_act(&sortie_act);

 }




}

STATUS Debut(void)
{
 //changer sortie_act

 ConfigPPI();
 partie_reactive_reset_architecture(&sortie_act);
 partie_reactive_init_architecture(&sortie_act);

 Tidt_atelier=taskSpawn
  ("t_atelier", cPriot_atelier, 0, STACK_SIZE,
  (FUNCPTR) t_atelier,0,0,0,0,0,0,0,0,0,0);

/* semA = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
 semB = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
 EvtFinTempo = semBCreate(SEM_Q_FIFO,SEM_EMPTY);

 WdId = wdCreate();


 TidTacheDialogue = taskSpawn ("tTacheDialogue", cPrioTacheDialogue, 0,
STACK_SIZE, (FUNCPTR) TacheDialogue,0,0,0,0,0,0,0,0,0,0);
 TidTacheA = taskSpawn ("tTacheA", cPrioTacheA, 0, STACK_SIZE, (FUNCPTR)
TacheA,0,0,0,0,0,0,0,0,0,0);
 TidTacheB = taskSpawn ("tTacheB", cPrioTacheB, 0, STACK_SIZE, (FUNCPTR)
TacheB,0,0,0,0,0,0,0,0,0,0);
 TidTacheC = taskSpawn ("tTacheC", cPrioTacheC, 0, STACK_SIZE, (FUNCPTR)
TacheC,0,0,0,0,0,0,0,0,0,0);*/
}

void Fin(void) {
 taskDelete(TidTacheA);
 taskDelete(TidTacheB);
 taskDelete(TidTacheC);
 semDelete(semA);
 semDelete(semB);
 semDelete(EvtFinTempo);
 wdDelete(WdId);
}
