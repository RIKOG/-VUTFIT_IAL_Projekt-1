
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
    // Set variable char * to non random value
    char ch = '?';
    // Get the length of the postfixExpression
    unsigned j = *postfixExpressionLength;
    // If stack isn't empty, get the character on top of the stack, otherwise *ch = '?' and next while cycle will not proceed because stack is empty
     if(Stack_IsEmpty( stack ) == 0){
         Stack_Top(stack, &ch);
     }
     // We take everything from the stack until left parenthesis and write it into postfixExpression
     while(Stack_IsEmpty( stack ) == 0 && ch != '('){
         postfixExpression[j++] = ch;
         Stack_Pop(stack);
         // After popping from stack we check if stack isn't empty, if stack was empty and we didnt check, Stack_Top would return an error
         if(Stack_IsEmpty( stack ) == 0){
             Stack_Top(stack, &ch);
         }
     }
     // We found '(' or stack is empty, either way we can pop the parenthesis, or if we pop empty stack nothing will happen
    Stack_Pop(stack);
     // We save our new length
    *postfixExpressionLength = j;
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
    // Set variable char * to non random value
    char ch = '?';

    int a, b;
    unsigned j = *postfixExpressionLength;

    // If stack isn't empty, get the character on top of the stack, otherwise *ch = '?'
    if(Stack_IsEmpty( stack ) == 0){
        Stack_Top(stack, &ch);
    }
    // char c contains current character from infixExpression comparing it with what we found on top of the stack
    // IF stack is empty, we are comparing char c with '?', in this case a = 1, b = -1 and while cycle will not proceed
    // IF stack isn't empty, we compare and decide if we want to write to postfixExpression based on precedence
    // IF we find anything else other than " +-*/^ ", for example '(' while cycle will not proceed
    if(c == '+' || c == '-')    {a = 1;} else if(c == '*' || c == '/')    {a = 2;} else if (c == '^')   {a = 3;} else {a = -1;}
    if(ch == '+' || ch == '-'){b = 1;} else if(ch == '*' || ch == '/'){b = 2;} else if (ch == '^') {b = 3;} else {b = -1;}

    while (Stack_IsEmpty( stack ) == 0 && a <= b) {
        postfixExpression[j++] = ch;
        Stack_Pop(stack);
        // After popping from stack we check if stack isn't empty, if stack was empty and we didn't check, Stack_Top would return an error
        if(Stack_IsEmpty( stack ) == 0){
            // Find out the value of next character from stack, if we find '(' while cycle will end
            Stack_Top(stack, &ch);
            if(ch == '+' || ch == '-'){b = 1;} else if(ch == '*' || ch == '/'){b = 2;} else if (ch == '^') {b = 3;} else {b = -1;}
        }
    }
    // We push our newly found operator
    Stack_Push(stack, c);
    // We save our new length
    *postfixExpressionLength = j;
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression Znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns Znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
    Stack *stack = (Stack*) malloc(sizeof(Stack));
    char *postfixExpression = (char *) malloc(sizeof(char)*MAX_LEN+1);
    unsigned *postfixExpressionLength = (unsigned *) malloc(sizeof(unsigned)*500);
    // Checking if none of the mallocs failed
    if(stack == NULL || postfixExpression == NULL || postfixExpressionLength == NULL){
        return NULL;
    }
    // Setting length of infixExpression to 0
    *postfixExpressionLength = 0;
    // Initialize stack, implementation in c202
    Stack_Init(stack);

    char ch;
    unsigned i = 0, j = 0;
    // While cycle goes through whole infix expression
    while (infixExpression[i] != '\0'){

        char c = infixExpression[i];
        // If we found infixExpression[i] is not an operator we write into postfixExpression
        if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')){
            j = *postfixExpressionLength;
            postfixExpression[j] = c;
            *postfixExpressionLength += 1;
        }

        // If we found left parenthesis we just push it to stack
        else if (infixExpression[i] == '(') {
            Stack_Push(stack, '(');
        }

        // If we found right parenthesis, we have to empty stack into postfixExpression until we find left parenthesis
        else if (infixExpression[i] == ')') {
            untilLeftPar( stack, postfixExpression, postfixExpressionLength );

        // If we found the end of the infixExpression
        } else if(infixExpression[i] == '='){
            // We empty what is left in stack into our postfixExpression
            while (Stack_IsEmpty( stack ) == 0){
                Stack_Top(stack, &ch);

                j = *postfixExpressionLength;
                postfixExpression[j] = ch;
                *postfixExpressionLength += 1;

                Stack_Pop(stack);
            }
            // We put character '=' and '\0' to last two indexes
            j = *postfixExpressionLength;
            postfixExpression[j++] = '=';
            postfixExpression[j] = '\0';
            *postfixExpressionLength += 2;
            // We return our postfixExpression
            free(postfixExpressionLength); free(stack);
            return postfixExpression;

        // Operator encountered
        } else {
            c = infixExpression[i];
            // Based on precedence we write what is on stack to postfixExpression, then we push found operator to stack as last thing in the function
            doOperation( stack, c, postfixExpression, postfixExpressionLength );
        }
        i++;
    }
    free(postfixExpression); free(postfixExpressionLength); free(stack);
    return NULL;
}

/* Konec c204.c */
