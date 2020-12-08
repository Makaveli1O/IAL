/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvořil: Petr Přikryl, listopad 1994
**                                         Úpravy: Andrea Němcová, prosinec 1995
**                                                      Petr Přikryl, duben 1996
**                                                   Petr Přikryl, listopad 1997
**                                  Převod do jazyka C: Martin Tuček, říjen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                                 Karel Masařík, říjen 2013
**                                                 Radek Hranický 2014-2018
**
** Implementujte rekurzivním způsobem operace nad binárním vyhledávacím
** stromem (BVS; v angličtině BST - Binary Search Tree).
**
** Klíčem uzlu stromu je jeden znak (obecně jím může být cokoliv, podle
** čeho se vyhledává). Užitečným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíčem leží vlevo, uzly s větším klíčem leží ve stromu
** vpravo. Využijte dynamického přidělování paměti.
** Rekurzivním způsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován kořenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíč (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Přesnou definici typů
** naleznete v souboru c401.h.
**
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c401.h"
int solved;

void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**
** Ověřit, zda byl již strom předaný přes RootPtr inicializován, nelze,
** protože před první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor využívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to před vlastní prací s BVS. Provedení
** inicializace nad neprázdným stromem by totiž mohlo vést ke ztrátě přístupu
** k dynamicky alokované paměti (tzv. "memory leak").
**
** Všimněte si, že se v hlavičce objevuje typ ukazatel na ukazatel.
** Proto je třeba při přiřazení přes RootPtr použít dereferenční operátor *.
** Ten bude použit i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/
    
    if(*RootPtr == NULL) return;    //ošetrenie volania viac krát init
    else *RootPtr = NULL;

}


//BSTSearch nefunguje tak ako má pri testoch nenachádza A,B





int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
** Funkce vyhledá uzel v BVS s klíčem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v proměnné Content se
** vrací obsah příslušného uzlu.´Pokud příslušný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah proměnné Content není definován (nic do ní proto
** nepřiřazujte).
**
** Při vyhledávání v binárním stromu bychom typicky použili cyklus ukončený
** testem dosažení listu nebo nalezení uzlu s klíčem K. V tomto případě ale
** problém řešte rekurzivním volání této funkce, přičemž nedeklarujte žádnou
** pomocnou funkci.
**/
    if(RootPtr == NULL){        //Uzol neboj nájdený
        //Content = NULL;
        return FALSE;
    }
        if(RootPtr->Key != K){   //ak vyhľadávaný kľúč nieje korenový uzol
            if(K < RootPtr->Key) return BSTSearch(RootPtr->LPtr, K, Content);   //pokračuj cez ľavý koreň
            
        }
        
    if( K == RootPtr->Key){
            *Content = RootPtr->BSTNodeCont; //nájdený uzol
            return TRUE;
        }
    
    return BSTSearch(RootPtr->RPtr, K, Content);    //pokračuj cez pravý koreň
}


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{
/*   ---------
** Vloží do stromu RootPtr hodnotu Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**
** Funkci implementujte rekurzivně. Nedeklarujte žádnou pomocnou funkci.
**
** Rekurzivní implementace je méně efektivní, protože se při každém
** rekurzivním zanoření ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto případě byla efektivnější jak z hlediska
** rychlosti, tak z hlediska paměťových nároků. Zde jde ale o školní
** příklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/
    
    //pracujeme s ukazateľom na ukazateľ, takže použijem dereferenčný operátor *
    if(*RootPtr == NULL){ //vytvorenie nového uzlu
        *RootPtr = (tBSTNodePtr)(malloc(sizeof(struct tBSTNode)));
        if(!RootPtr) return;    //ošetrenie mallocu
        (*RootPtr)->Key = K;
        (*RootPtr)->BSTNodeCont = Content;
        (*RootPtr)->LPtr = NULL;
        (*RootPtr)->RPtr = NULL;
    }else{
        if(K < (*RootPtr)->Key) BSTInsert(&(*RootPtr)->LPtr, K, Content);    //pokračuj vľavo
        else if(K > (*RootPtr)->Key) BSTInsert(&(*RootPtr)->RPtr, K, Content);   //pokračuj vpravo
        else (*RootPtr)->BSTNodeCont = Content; //prepísanie dát
    }
	
    
	 

}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude přesunuta hodnota
** nejpravějšího uzlu v podstromu, který je určen ukazatelem RootPtr.
** Předpokládá se, že hodnota ukazatele RootPtr nebude NULL (zajistěte to
** testováním před volání této funkce). Tuto funkci implementujte rekurzivně.
**
** Tato pomocná funkce bude použita dále. Než ji začnete implementovat,
** přečtěte si komentář k funkci BSTDelete().
**/
    if((*RootPtr) == NULL || PtrReplaced == NULL) return;  //ošetrenie chybových stavov
    tBSTNodePtr right_most; //dočasný pomocný súbor
    //ak ďaľší pravý pointer nepokračuje, znamená to že je najpravejší(podmienka rekurzie, bude pokračovať pokiaľ nenarazí na tento prípad)
    if (!(*RootPtr)->RPtr){
        right_most = *RootPtr;
        PtrReplaced->BSTNodeCont = right_most->BSTNodeCont;
        PtrReplaced->Key = right_most->Key;
        //v prípade že mal ľavý pointer, zdedí ho.
        if (right_most->LPtr) PtrReplaced->LPtr = right_most->LPtr;

        free(right_most);
        return;
    }
    //pravý pointer nemá následujúci pravý pointer
    if (!(*RootPtr)->RPtr->RPtr){
        right_most = (*RootPtr)->RPtr;

        PtrReplaced->Key = right_most->Key;
        PtrReplaced->BSTNodeCont = right_most->BSTNodeCont;
        //ak nemal ľavý podstrom, nastaviť na null
        (*RootPtr)->RPtr = NULL;
        if (right_most->LPtr) (*RootPtr)->RPtr = right_most->LPtr;
        
        free(right_most);
        return;
    }
        // Rekurzívne volanie, ak ani jeden z predošlých prípadov neplatí(najpravejší nieje
       // ani koreň, ani nasledujúci pravý pointer pravého pointera) tak sa posunie

    ReplaceByRightmost(PtrReplaced, &(*RootPtr)->RPtr);
}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic.
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivně s využitím dříve deklarované
** pomocné funkce ReplaceByRightmost.
**/
        if(*RootPtr == NULL) return; //prázdny uzol

        tBSTNodePtr delete;

        //riešenie pre ľavý podstrom
        if(K < (*RootPtr)->Key){
            if((*RootPtr)->LPtr && (*RootPtr)->LPtr->Key == K){ // kľúč následujúceho ľavého je hľadaný
                delete = (*RootPtr)->LPtr;
                if(delete->LPtr && delete->RPtr){      // má dva podstromy
                    ReplaceByRightmost(delete, &(delete->LPtr));    //nahradenie najpravejším
                    return;
                }//iba ľavý podstrom
                if(delete->LPtr && !delete->RPtr)
                    (*RootPtr)->LPtr = delete->LPtr;
                else if(delete->RPtr && !delete->LPtr) (*RootPtr)->LPtr = delete->RPtr;  // má iba pravý podstrom
                else (*RootPtr)->LPtr = NULL;
                free(delete);
                return;
            }
            BSTDelete(&((*RootPtr)->LPtr), K);      //rekurzívne volanie pre ľavý podstrom
            return;
        }

        //riešenie pre pravý podstrom
        if(K > (*RootPtr)->Key){
            if((*RootPtr)->RPtr && (*RootPtr)->RPtr->Key == K){ //kľúč prvého rptr je hľadaný
                delete = (*RootPtr)->RPtr;
                if(delete->RPtr && delete->LPtr){
                    ReplaceByRightmost(delete, &(delete->LPtr));
                    return;
                }
                if(!delete->RPtr && delete->LPtr) (*RootPtr)->RPtr = delete->LPtr; // má iba ľavý podstrom
                else if(!delete->LPtr && delete->RPtr) (*RootPtr)->RPtr = delete->RPtr; // má iba pravý podstrom
                else (*RootPtr)->RPtr = NULL; // nemá podstrom
                free(delete);
                return;
            }
            BSTDelete(&((*RootPtr)->RPtr), K);  //rekurzívne volanie pre pravý podstrom
            return;
        }
        //hľadaný kľuč je koreň stromu
        delete = *RootPtr;
        if (delete->RPtr && delete->LPtr){
            // nahradenie uzlu najpravejším
            ReplaceByRightmost(delete, &(delete->LPtr));
            return;
        }

        if (delete->LPtr && !delete->RPtr) *RootPtr = delete->LPtr; // iba ľavý podstrom
        else if (!delete->LPtr && delete->RPtr) *RootPtr = delete->RPtr; // ina pravý podstrom
        else *RootPtr = NULL; // žiadny podstrom
        free(delete);
}

void BSTDispose (tBSTNodePtr *RootPtr) {
/*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/
	//rekurzívne zrušenie binárneho stromu
	if(*RootPtr != NULL){
		BSTDispose(&((*RootPtr)->LPtr));
        BSTDispose(&((*RootPtr)->RPtr));
        free(*RootPtr);
	}
    *RootPtr=NULL;      //nakoniec nastaviť ukazateľ na NULL inak bude segmentation fault
}



/* konec c401.c */

