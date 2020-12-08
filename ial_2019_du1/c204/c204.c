
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu 
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"
int solved;

/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků output. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr outputLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/

void untilLeftPar ( tStack* s, char* output, unsigned* outputLen ) {
	char c;							/*pomocná premenná*/
	do{
		stackTop(s, &c);			/*uloží znak z vrcholu zásobníka do "c"*/
		stackPop(s);				/*odstráni znak zo zásobníka*/

		if(c == '(')			/*Ak je odoberaný znak ľavá zátvorka, koniec*/
			break;

		output[(*outputLen)++]=c;	/*zápis do výstupu(outputLen ukazuje na voľné miesto)*/


	}while(!stackEmpty(s));		/*podmienka pre cyklus(pokiaľ nieje zásobník prázdny)*/
}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného 
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat, 
** představuje parametr outputLen, výstupním polem znaků je opět output.
*/
void doOperation ( tStack* s, char c, char* output, unsigned* outputLen ) {
	char c2; 					/*pomocná premenná*/
	
	if(stackEmpty(s)!=0){		/*ak je zásobník prázdny, vloží operátor na vrch zásobníku*/
		stackPush(s, c);
		return;
	}else{
		stackTop(s, &c2);		/*uloženie znaku z vrchu zásobníka*/
		if(c2 == '('){			/*ak je uložený znak ľavá zátvorka, vloží na vrch zásobníka*/
			stackPush(s, c);
			return;
		}
	/*"c"(vrchol zásobníku) je operátor s nižšiou prioritou ako "c2"*/
	if ((c2 == '-' || c2 == '+') && (c == '*' || c == '/')){ 
		stackPush(s, c); 
		return;
	}

	output[(*outputLen)++] = c2;	/*vloženie znaku zo zásobníka do reťazca*/
	stackPop(s); 					/*odstránenie znaku z vrchu zásobníka*/

		
	/*funckia volá samú seba pokiaľ nevloží operátor na vrchol zásobníku*/
	doOperation(s, c, output, outputLen);
	}
}

/* 
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen 
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202. 
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char* infix2postfix (const char* infExpr) {
	/*Alokovanie památe pre output string*/
	char *output = (char *) malloc(MAX_LEN * sizeof(char));
	if(output == NULL)
		return NULL;

	/*Alokovanie pamäte pre zásobník*/
	tStack *stack = (tStack *) malloc(sizeof(tStack));
	/*Pri zlyhaní alokovania, uvoľní output*/
	if(stack == NULL){
		free(output);
		return NULL;
	}

	/*Inicializácia stacku z c202.c*/
	stackInit(stack);

	/*dĺžka výstupného reťazca*/
	unsigned int outputLen = 0;
	char c = *infExpr;
	/*Cyklus cez string*/
	while(c != '\0'){
		c = *infExpr;
		/*Zistenie operandu a priradenie znaku na koniec reťazca*/
		if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
			output[outputLen++] = c; 
		}
		else if (c == '(')		/*ak je znak zátvorka, vloží ihned znak do zásobníka*/
			stackPush(stack, c); 
		else if (c == '+' || c == '-' || c == '*' || c == '/')  /*nájdenie operátoru + zavolanie funkcie doOperation pre spracovanie*/
			doOperation(stack, c, output, &outputLen); 
		else if (c == ')') 		/*po nájdení pravej zátvorky, vyprázdni zásobník až po ľavú zátvorku*/
			untilLeftPar(stack, output, &outputLen); 
		else if (c == '=') 		/*pri nájdení '=' prejde cez zásobník pokiaľ nieje prázdny a uloží znaky do outputu*/
		{
			while (stackEmpty(stack)==0)
			{
				/*uloží znak do outputu*/
				stackTop(stack, &(output[outputLen++]));
				/*odstránenie znaku z vrchu zásobníka*/
				stackPop(stack); 
			}
			/*pridá na koniec reťazca znak '='*/
			output[outputLen++] = '='; 
			break; 
		}

		c = *(++infExpr);
	}
	/*ukončenie reťazca znakom '\0'*/
	output[outputLen++] = '\0'; 
	/*uvoľnenie reťazca*/
	free(stack); 

	return output;

}

/* Konec c204.c */
