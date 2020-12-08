
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	/*Inicializácia dvojsmerného zoznamu*/
	L->First=NULL;
	L->Act=NULL;
	L->Last=NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/

	tDLElemPtr tmp = L->First;
	/*Rušenie všetkých prvkov zoznamu od konečného po prvý*/
	while(L->First!=NULL){
		L->First = L->First->rptr;
 	        free(tmp);
                tmp = L->First;	
	}
	/*Inicializácia dvojsmerného zoznamu*/
	DLInitList(L);
	
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

	/*Alokácia pamäte*/
	tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
	if(!tmp){
		DLError();
		return;
	}
	/*Počiatočné hodnoty:
		*data nastaviť na vkladanú hodnotu
		*ľavý ukazateľ neukazuje nikde, pretože je to prvý prvok
		*pravý ukazateľ ukazuje na pôvodný prvý prvok
	*/
	tmp->data = val;
	tmp->lptr = NULL;
	tmp->rptr = L->First; 
	/*Ak je list prázdny, posledný prvok je zároveň prvý
		ak prvý prvok obsahuje, ľavý pointer nastavím na vkladaný prvok
		a následne L->First nastavým na novo vložený*/
	if(L->First == NULL)
		L->Last = tmp;
	else
		L->First->lptr = tmp;

	L->First = tmp;


}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	/*Alokácia temporary premennej*/
	tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
	/*Ochrana mallocu*/
	if(!tmp){
		DLError();
		return;
	}
	/*Počiatočné hodnoty:
		*data obsahuje vkladaní informácie
		*ľavý pointer ukazuje na pôvodný posledný prvok
		*pravý pointer na následujúci prvok neukazuje nikam pretože 
		vkladaný prvok je posledný
	*/
	tmp->data = val;
	tmp->lptr = L->Last;
	tmp->rptr = NULL;

	/*Ak je zoznam prázdny, vložiť zároveň aj ako prvý
		inak pôvodný posledný pointer nahradiť za vkladaný prvok
		potom prepísať hodnoty posledného prvku vkladaným prvkom
	*/
	if(L->First == NULL)
		L->First = tmp;
	else
		L->Last->rptr = tmp;
	L->Last = tmp;

}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
	L->Act = L->First;
 
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
	L->Act = L->Last;	
 
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	/*Ak je zoznam prázdny chyba, inak priradí hodnotu prvého prvku*/
	if(L->First == NULL){
		DLError();
		return;
	}else{
		*val = L->First->data;
	}
	
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	/*Ak je zoznam prázdny chyba, inak priradí hodnotu posledného prvku*/
	if(L->First == NULL){
		DLError();
		return;
	}else{
		*val = L->Last->data;
	}
	
	
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

	if(L->First == NULL){
		return;
	}else{
		if(L->First == L->Act)		//ak je aktívny zruší aktivitu
			L->Act = NULL;
		if(L->First == L->Last){	//ak je jediný zruší ho
			L->First = L->Last = NULL;
		}else{				//ak je viac prvkov
			L->First = L->First->rptr;
			L->First->lptr = NULL;
		}
	}
	
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 

	if(L->First == NULL){
		return;
	}else{
		if(L->First == L->Act)		//ak je aktívny zruší aktivitu
			L->Act = NULL;
		/*Jediný prvok v zozname*/
		if(L->First == L->Last){	//ak je jediný zruší ho
			L->First = L->Last = NULL;
		}else{				//viac prvkov
			L->Last = L->Last->lptr;
			L->Last->rptr = NULL;
		}
				
	}
	
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	/*Ošetrenie prázfneho zoznamu a posledného prvku*/
	if(L->Act == NULL || L->Act == L->Last){
		return;
	}else{
		if(L->First == L->Last){ 	//ak je jediný
			return;
		}else{				//zrušenie prvku
			tDLElemPtr tmp = L->Act->rptr;
			L->Act->rptr = tmp;
		}
	}	
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/

	if (L->Act && L->Act != L->First){
		/*pomocná premenná*/
        	tDLElemPtr tmp = L->Act->lptr;
		/*left pointer bude rovnaký ako pomocnej premennej*/
                L->Act->lptr = tmp->lptr; 
                if (tmp == L->First) /* prvý prvok*/
                         L->First = L->Act; 
                 else /*ostatné*/
                         tmp->lptr->rptr = L->Act; /*pravý pointer pred pomocným prvkom bude aktívny prvok*/
                 free(tmp);
         }




}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

	
	if(L->Act == NULL)
		return;
	/*vkladaný prvok*/
	tDLElemPtr L2 = (tDLElemPtr) malloc(sizeof(struct tDLElem));
		
	if(!L2){
		DLError();
		return;
	}
	
	/*pomocná premenná*/
	tDLElemPtr tmp = L->Act;

	L2->data = val;
	L2->rptr = NULL;
	L2->lptr = NULL;
	/*ak je act zároveň posledný, stačí vložiť zaňho, a left pointer nastaviť na Act*/	
	if(L->Act == L->Last){
		L->Last->rptr = L2;
		L2->lptr = L->Act;
		L->Last = L2;		//nový Last prvok
	}else{	/*pre ostatné ktoré niesu posledné*/
		L->Act->rptr = L2;
		L2->lptr = L->Act;
		L2->rptr = tmp->rptr;
	}
	/*Uvoľnenie pamäte*/
	free(L2);
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act == NULL)
		return;
	/*vkladaný prvok*/
	tDLElemPtr L2 = (tDLElemPtr) malloc(sizeof(struct tDLElem));
		
	if(!L2){
		DLError();
		return;
	}
	
	/*pomocná premenná*/
	tDLElemPtr tmp = L->Act;

	L2->data = val;
	L2->rptr = NULL;
	L2->lptr = NULL;
	/*ak je act zároveň prvý, stačí vložiť pred neho, a right pointer nastaviť na Act*/	
	if(L->Act == L->First){
		L->First->lptr = L2;
		L2->rptr = L->Act;
		L2->lptr = NULL;
		L->First = L2;		//Nový first prvok
	}else{	/*pre ostatné ktoré niesu posledné*/
		L->Act->rptr = L2;
		L2->lptr = L->Act;
		L2->rptr = tmp->rptr;
	}
	/*Uvoľnenie pamäte*/
	free(L2);
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	/*Ak nieje aktívny výpis chyby, inak priradenie hodnoty parametru val*/
	if(L->Act == NULL){
		DLError();
		return;
	}else{
		*val = L->Act->data;
		return;
	}	
	
	
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	if(L->Act == NULL)
		return;	
	else
		L->Act->data = val;
	
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/

	/*
	 * Ak zoznam nieje aktívny nedeje sa nič, ak je prvok posledný,
	 * tak sa zoznam stane neaktívnym. Inak sa aktivita presunie na následujúci
	 * prvok.
	 * */
	if(L->Act == NULL)
		return;
	else{
		if(L->Act == L->Last)
			L->Act = NULL;
		else
			L->Act = L->Act->rptr;
		
	}	
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	/*
	 * Ak zoznam nieje aktívny nestane sa nič, ak je prvok prvý,
	 * tak sa zoznam stane neaktívnym. Inak sa aktivita presunie na predošlý
	 * prvok.
	 * */
	if(L->Act == NULL)
		return;
	else{
		if(L->Act == L->First)
			L->Act = NULL;
		else
			L->Act = L->Act->lptr;
	}	
	
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	/*Ternárny operátor: ak je aktívny prvok vrať 1, inak 0*/	
	return (L->Act != NULL) ? 1:0;
}

/* Konec c206.c*/
