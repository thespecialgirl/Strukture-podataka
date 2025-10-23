/*2. Definirati strukturu osoba (ime, prezime, godina rođenja) i napisati program koji:
A. dinamički dodaje novi element na početak liste,
B. ispisuje listu,
C. dinamički dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE (1024)                // definirana maksimalna duljina unosa
#define SUCCESS 0                       // uspješan povratni kod
#define ERROR_MALLOC (-1)               // greška alokacije memorije
#define ERROR_INVALID_ARGUMENT (-3)     // neispravan argument funkcije

// Deklaracija strukture na početku
struct _Person;

// Definiran pokazivač na strukturu Person
typedef struct _Person* Position;

// Struktura za osobe
typedef struct _Person
{
    char name[MAX_LINE];
    char surname[MAX_LINE];
    int birthYear;
    Position next;
} Person;

// Deklaracija funkcije koja dodaje osobu na početak liste
int addAtTheBeginning(Position* headPointer, char* name, char* surname, int birthYear);

// Deklaracija funkcije koja ispisuje listu i vraća broj ispisanih elemenata
int printList(Position head);

// Deklaracija funkcije koja oslobađa cijelu listu
int freeEntireList(Position* headPointer);


int main(void)
{
    Position head = NULL;                     // Početak liste

    // Varijable za unos podataka
    char name[MAX_LINE] = "";
    char surname[MAX_LINE] = "";
    int birthYear = 0;

    printf("Upisi podatke o osobi. Ako si gotov, upisi 'end' kao ime.\n");

    while (1) {                               // Beskonačna petlja do break-a
        printf("\nUpisi ime: ");
        scanf("%s", name);

        if (strcmp(name, "end") == 0)        // Ako je ime 'end', prekini unos
            break;

        printf("Upisi prezime: ");
        scanf("%s", surname);

        printf("Upisi godinu rodenja: ");
        scanf("%d", &birthYear);

        // Dodavanje nove osobe na početak liste i provjera statusa
        int status = addAtTheBeginning(&head, name, surname, birthYear);
        if (status != SUCCESS)
            printf("Greska pri dodavanju osobe!\n");
    }

    // Ispis liste
    printf("\nIspis liste:\n");
    int totalPersons = printList(head);
    if (totalPersons == 0)
        printf("Lista je prazna.\n");

    // Oslobađanje memorije
    freeEntireList(&head);
    printf("\nSva memorija uspjesno oslobodena.\n");

    return SUCCESS;
}

/* 1. Funkcija: dodaje osobu na početak liste */
int addAtTheBeginning(Position* headPointer, char* name, char* surname, int birthYear)
{
    if (headPointer == NULL)                 // Provjera validnosti pokazivača
        return ERROR_INVALID_ARGUMENT;

    // Alokacija memorije za novi čvor
    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL)
        return ERROR_MALLOC;

    // Kopiranje podataka u novi čvor
    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;

    // Novi čvor pokazuje na trenutni head
    newPerson->next = *headPointer;

    // Novi čvor postaje novi head liste
    *headPointer = newPerson;

    return SUCCESS;
}

/* 2. Funkcija: ispisuje sve osobe u listi */
int printList(Position head)
{
    if (head == NULL)                        // Lista prazna
        return 0;

    int count = 0;
    Position current = head;

    // Prolazak kroz cijelu listu i ispis
    while (current != NULL) {
        printf("%s %s (%d)\n", current->name, current->surname, current->birthYear);
        current = current->next;
        count++;
    }

    return count;
}

/* 3. Funkcija: oslobađa memoriju cijele liste */
int freeEntireList(Position* headPointer)
{
    if (headPointer == NULL)
        return ERROR_INVALID_ARGUMENT;

    Position temp = NULL;

    // Prolazak kroz listu i oslobađanje svakog čvora
    while (*headPointer != NULL) {
        temp = *headPointer;
        *headPointer = (*headPointer)->next;
        free(temp);
    }

    return SUCCESS;
}
