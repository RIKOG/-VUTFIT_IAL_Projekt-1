/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
    printf("*ERROR* The program has performed an illegal operation.\n");
    error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
// We allocate space for the struct and check whether the malloc was successful
void DLL_Init( DLList *list ) {
    list = malloc(sizeof(DLList));
    if(list == NULL){
        DLL_Error();
    }
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
/* We go through every node with TempElement, we save position of TempElement into DelElement,
 * move to next Node with TempElement and free DelElement. That way we are always on the next node without losing its position */
void DLL_Dispose( DLList *list ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    DLLElementPtr TempElement = list->firstElement;
    DLLElementPtr DelElement;
    while(TempElement != NULL){
        DelElement = TempElement;
        TempElement = TempElement->nextElement;
        free(DelElement);
    }
    list->activeElement = NULL;
    list->firstElement = NULL;
    list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    // A situation where there wasn't a Node in the list yet
    if (list->firstElement == NULL){
        // We check if malloc was successful
        DLLElementPtr TempElement = malloc(sizeof(struct DLLElement));
        if(TempElement == NULL){
            DLL_Error();
        }

        // Because there is only one element, its both first and last one in the List
        list->firstElement = TempElement;
        list->activeElement = NULL;
        list->lastElement = TempElement;

        // Because element is both first and last one in the List it has no previous nor next element
        list->firstElement->data = data;
        list->firstElement->previousElement = NULL;
        list->firstElement->nextElement = NULL;
        // If the Node being added is the NEW first element
    } else {
        // We check if malloc was successful
        DLLElementPtr TempElement = malloc(sizeof(struct DLLElement));
        if(TempElement == NULL){
            DLL_Error();
        }

        TempElement->data = data;
        TempElement->previousElement = NULL;

        // We connect new element with the previous first element
        TempElement->nextElement = list->firstElement;
        // We connect previous first element with new element
        list->firstElement->previousElement = TempElement;
        // We make new element the first element
        list->firstElement = TempElement;
    }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    // A situation where there wasn't a Node in the list yet
    if (list->lastElement == NULL){
        // We check if malloc was successful
        DLLElementPtr TempElement = malloc(sizeof(struct DLLElement));
        if(TempElement == NULL){
            DLL_Error();
        }
        // Because there is only one element, its both first and last one in the List
        list->firstElement = TempElement;
        list->activeElement = NULL;
        list->lastElement = TempElement;

        // Because element is both first and last one in the List it has no previous nor next element
        list->lastElement->data = data;
        list->lastElement->previousElement = NULL;
        list->lastElement->nextElement = NULL;

        // If the Node being added is the NEW last element
    } else {
        // We check if malloc was successful
        DLLElementPtr TempElement = malloc(sizeof(struct DLLElement));
        if(TempElement == NULL){
            DLL_Error();
        }

        TempElement->data = data;
        TempElement->nextElement = NULL;

        // Connectiong New last element to previous last element
        TempElement->previousElement = list->lastElement;
        // Connectiong previous last element to New last element
        list->lastElement->nextElement = TempElement;
        // We make new element the last element
        list->lastElement = TempElement;
    }
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    if(list->firstElement == NULL){
        DLL_Error();
    } else {
        *dataPtr = list->firstElement->data;
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    if(list->lastElement == NULL){
        DLL_Error();
    } else {
        *dataPtr = list->lastElement->data;
    }
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    if(list->firstElement != NULL){
        if(list->firstElement->nextElement == NULL){
            // We delete the element
            free(list->lastElement);
            // We set pointers in list to NULL, because its now empty
            list->firstElement = NULL;
            list->lastElement = NULL;
            list->activeElement = NULL;
            return;
        }
        // We save the position of the NEW first element
        DLLElementPtr TempElement = list->firstElement->nextElement;
        // We delete the connection between the old and new first element
        TempElement->previousElement = NULL;
        // If we are deleting element that was also the active element we set active element to NULL
        if (list->firstElement == list->activeElement) {
            list->activeElement = NULL;
        }
        // We delete the element
        free(list->firstElement);

        // We set the firstElement to NEW element
        list->firstElement = TempElement;
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    if(list->lastElement != NULL){
        // If we are deleting the only element left in the list
        if(list->lastElement->previousElement == NULL){
            // We delete the element
            free(list->lastElement);
            // We set pointers in list to NULL, because its now empty
            list->firstElement = NULL;
            list->lastElement = NULL;
            list->activeElement = NULL;
            return;
        }
        // We save the position of the NEW LAST element
        DLLElementPtr TempElement = list->lastElement->previousElement;
        // We delete the connection between the old and new last element
        TempElement->nextElement = NULL;
        // If we are deleting element that was also the active element we set active element to NULL
        if (list->lastElement == list->activeElement){
            list->activeElement = NULL;
        }
        // We delete the element
        free(list->lastElement);

        // We set the lastElement to NEW element
        list->lastElement = TempElement;
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    // If there even is activeElement and the lastElement also isn't the activeElement
    if(list->activeElement != NULL && list->lastElement != list->activeElement){
        // We set DelElement to the next element after activeElement
        DLLElementPtr DelElement = list->activeElement->nextElement;
        /* We save the position of the element after the one we are going to delete,
         (activeElement-ToDeleteElement-TempElement) -> (activeElement-TempElement) */
        DLLElementPtr TempElement = DelElement->nextElement;
        // We connect activeElement with TempElement
        list->activeElement->nextElement = TempElement;
        // If DelElement is the last one in the list
        if(TempElement == NULL){
            // (activeElement-ToDeleteElement-NULL) -> (activeElement) <-> (lastElement)
            list->lastElement = list->activeElement;
            // otherwise we connect TempElement with activeElement
        } else {
            TempElement->previousElement = list->activeElement;
        }
        // We delete DelElement
        free(DelElement);
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    // If there even is activeElement and the firstElement also isn't the activeElement
    if (list->activeElement != NULL && list->firstElement != list->activeElement) {
        // We set DelElement to the previous element after activeElement
        DLLElementPtr DelElement = list->activeElement->previousElement;
        /* We save the position of the element before the one we are going to delete,
         (TempElement-ToDeleteElement-activeElement) -> (TempElement-activeElement) */
        DLLElementPtr TempElement = DelElement->previousElement;
        // We connect activeElement with TempElement
        list->activeElement->previousElement = TempElement;
        // If DelElement is the first one in the list
        if (TempElement == NULL) {
            // (NULL-ToDeleteElement-activeElement) -> (activeElement-NEXT_ELEMENT) */
            list->firstElement = list->activeElement;
            // otherwise we connect TempElement with activeElement
        } else {
            TempElement->nextElement = list->activeElement;
        }
        // We delete DelElement
        free(DelElement);
    }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    if(list->activeElement != NULL){
        // We check if malloc was successful
        DLLElementPtr NewElement = malloc(sizeof(struct DLLElement));
        if(NewElement == NULL){
            DLL_Error();
        }
        // (activeElement-TempElement) -> (activeElement-NewElement-TempElement)
        // We set TempElement to the one after activeElement
        DLLElementPtr TempElement = list->activeElement->nextElement;
        // We put NewElement after activeElement
        list->activeElement->nextElement = NewElement;
        // We connect TempElement with the New Element, in the situation where active element is also the last one, temp element will be NULL
        if(TempElement != NULL){
            TempElement->previousElement = NewElement;
        } else {
            list->lastElement = NewElement;
        }
        // New connections with active and Temp Elements
        NewElement->previousElement = list->activeElement;
        NewElement->nextElement = TempElement;

        NewElement->data = data;
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    if(list->activeElement != NULL){
        // We check if malloc was successful
        DLLElementPtr NewElement = malloc(sizeof(struct DLLElement));
        if(NewElement == NULL){
            DLL_Error();
            return;
        }
        // (TempElement-activeElement) -> (TempElement-NewElement-activeElement)
        // We set TempElement to the one before activeElement
        DLLElementPtr TempElement = list->activeElement->previousElement;
        // We put NewElement before activeElement
        list->activeElement->previousElement = NewElement;
        // We connect TempElement with the New Element, in the situation where active element is also the first one, temp element will be NULL
        if(TempElement != NULL){
            TempElement->nextElement = NewElement;
        } else {
            list->firstElement = NewElement;
        }
        // New connections with active and Temp Elements
        NewElement->nextElement = list->activeElement;
        NewElement->previousElement = TempElement;

        NewElement->data = data;
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    if(list->activeElement != NULL){
        *dataPtr = list->activeElement->data;
    } else {
        DLL_Error ();
    }
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    if(list->activeElement != NULL) {
        list->activeElement->data = data;
    }
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    if(list->activeElement != NULL) {
        list->activeElement = list->activeElement->nextElement;
    }
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
    if(list == NULL){
        DLL_Error();
        return;
    }
    if(list->activeElement != NULL) {
        list->activeElement = list->activeElement->previousElement;
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
    if(list == NULL){
        DLL_Error();
        return 0;
    }
    if(list->activeElement != NULL){
        return 1;
    }
    return 0;
}

/* Konec c206.c */
