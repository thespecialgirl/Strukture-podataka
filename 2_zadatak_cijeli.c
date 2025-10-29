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

//Deklaracija funkcije koja dodaje osobu na kraj liste
int addAtTheEnd(Position* headPointer, char* name, char* surname, int birthYear);

//Deklaracija funkcije koja pronalazi osobu po prezimenu
Position findBySurname(Position head, char* surname);

//Deklaracija funkcije koja briše određeni element iz liste
int deleteBySurname(Position* headPointer, char* surname);

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
       /* int status = addAtTheBeginning(&head, name, surname, birthYear);
        if (status != SUCCESS)
            printf("Greska pri dodavanju osobe!\n");*/

        //Dodavanje nove osobe na kraj liste i provjera statusa
        int statusEnd = addAtTheEnd(&head, name, surname, birthYear);
        if (statusEnd != SUCCESS)
            printf("Greska pri dodavanju osobe na kraj liste!\n");
    }

    // Ispis liste
    printf("\nIspis liste:\n");
    int totalPersons = printList(head);
    if (totalPersons == 0)
        printf("Lista je prazna.\n");

	//kad trazimo broj osoba u listi
    else
	printf("\nUkupno osoba u listi: %d\n", totalPersons);

	// Inicijalizacija varijable za prezime koje se trazi
	char searchSurname[MAX_LINE] = "";

    /*Traženje prezimena*/

	//Unos prezimena koje se trazi
	printf("\nUnesi prezime osobe koju zelis pronaci: ");
	scanf("%s", searchSurname);

	//pozivanje funkcije za pronalazak osobe
	Position foundPerson = findBySurname(head, searchSurname);

	//kad trazimo prezime koje je u listi
	if (foundPerson != NULL) {
		printf("Osoba pronadena: %s %s (%d)\n", foundPerson->name, foundPerson->surname, foundPerson->birthYear);
	}
	//kad trazimo prezime koje nije u listi
	else {
		printf("Osoba s prezimenom '%s' nije pronadena u listi.\n", searchSurname);
	}

	/*Brisanje određene osobe iz liste*/

	// Inicijalizacija varijable za prezime koje se brise
	char deleteSurname[MAX_LINE] = "";

	// Unos prezimena koje se brise
	printf("\nUnesi prezime osobe koju zelis izbrisati iz liste: ");
	scanf("%s", deleteSurname);

	// Pozivanje funkcije za brisanje osobe
	int deleteStatus = deleteBySurname(&head, deleteSurname);

	// Provjera statusa brisanja
	if (deleteStatus == SUCCESS) {
		printf("Osoba s prezimenom '%s' uspjesno izbrisana iz liste.\n", deleteSurname);
	}
	else {
		printf("Osoba s prezimenom '%s' nije pronadena u listi. Brisanje nije uspjelo.\n", deleteSurname);
	}
	// Ispis liste nakon brisanja
	printf("\nIspis liste nakon brisanja:\n");

	totalPersons = printList(head);
	if (totalPersons == 0)
		printf("Lista je prazna.\n");
	else
		printf("\nUkupno osoba u listi nakon brisanja: %d\n", totalPersons);

    // Oslobađanje memorije
    freeEntireList(&head);
    printf("\nSva memorija uspjesno oslobodena.\n");

    return SUCCESS;
}

/* 1. Funkcija: dodaje osobu na početak liste */
/*int addAtTheBeginning(Position* headPointer, char* name, char* surname, int birthYear)
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
}*/

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

/* 4. Funkcija: dodaje osobu na kraj liste */
int addAtTheEnd(Position* headPointer, char* name, char* surname, int birthYear)
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
    newPerson->next = NULL;               // Novi čvor će biti na kraju liste

    // Ako je lista prazna, novi čvor postaje head
    if (*headPointer == NULL) {
        *headPointer = newPerson;
        return SUCCESS;
    }
    else
    {                          // Inače, pronađi kraj liste i dodaj novi čvor tamo

        Position current = *headPointer;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newPerson;
    }
    return SUCCESS;
}
/* 5. Funkcija: pronalazi osobu po prezimenu */
Position findBySurname(Position head, char* surname)
{
    Position current = head;
    // Prolazak kroz listu i traženje prezimena
    while (current != NULL) {
        if (strcmp(current->surname, surname) == 0) {
            return current;                  // Prezime pronađeno
        }
        current = current->next;
    }
    return NULL;                            // Prezime nije pronađeno
}

/* 6. Funkcija: briše određeni element iz liste */
int deleteBySurname(Position* headPointer, char* surname)
{
	// Provjera validnosti pokazivača
    if (headPointer == NULL || *headPointer == NULL)
        return ERROR_INVALID_ARGUMENT;

	// Inicijalizacija pokazivača za trenutni i prethodni čvor
    Position current = *headPointer;
    Position previous = NULL;

    // Prolazak kroz listu i traženje prezimena
    while (current != NULL) {

		// Usporedba prezimena
        if (strcmp(current->surname, surname) == 0) {
            // Prezime pronađeno, briši čvor
            if (previous == NULL) {
                // Brišemo head čvor
                *headPointer = current->next;
            }
            else {
                // Brišemo čvor koji nije head
                previous->next = current->next;
            }
            free(current);
            return SUCCESS;                  // Uspješno izbrisano
        }
        previous = current;
        current = current->next;
    }
    return ERROR_INVALID_ARGUMENT;          // Prezime nije pronađeno
}