/*3. Prethodnom zadatku dodati funkcije:
A. dinamički dodaje novi element iza određenog elementa,
B. dinamički dodaje novi element ispred određenog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. čita listu iz datoteke.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE (1024)                // definirana maksimalna duljina unosa
#define SUCCESS 0                       // uspješan povratni kod
#define ERROR_MALLOC (-1)               // greška alokacije memorije
#define ERROR_INVALID_ARGUMENT (-3)     // neispravan argument funkcije
#define ERROR_FILE_OPENING (-4)         // greška pri otvaranju datoteke
#define NOT_FOUND (-5)                  // element nije pronađen

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

// Funkcija za dodati nakon nekog prezimena
int addAfter(Position head,char* targetSurname, char* name, char* surname, int birthYear);

// Funkcija za dodati ispred nekog prezimena
int addBefore(Position head, char* targetSurname, char* name, char* surname, int birthYear);

// Funkcija za sortiranje liste po prezimenima
int sortListBySurname(Position head);

// Funkcija za upis liste u datoteku
int writeListToFile(Position head,char* filename);

// Funkcija za čitanje liste iz datoteke
int readListFromFile(Position* headPointer, char* filename);

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

                            /* Dodavanje nove osobe na početak liste i provjera statusa*/
       /* int status = addAtTheBeginning(&head, name, surname, birthYear);
        if (status != SUCCESS)
            printf("Greska pri dodavanju osobe!\n");*/

                            /*Dodavanje nove osobe na kraj liste i provjera statusa*/
        int statusEnd = addAtTheEnd(&head, name, surname, birthYear);
        if (statusEnd != SUCCESS)
            printf("Greska pri dodavanju osobe na kraj liste!\n");
    }

                               /*Ispis liste*/
    printf("\nIspis liste:\n");
    int totalPersons = printList(head);
    if (totalPersons == 0)
        printf("Lista je prazna.\n");

    //kad trazimo broj osoba u listi
    else
        printf("\nUkupno osoba u listi: %d\n", totalPersons);

                     /*Dodavanje ISPRED određenog prezimena*/

	char targetSurnameB[MAX_LINE];
	char newNameB[MAX_LINE];
	char newSurnameB[MAX_LINE];
	int newBirthYearB = 0;
	printf("\n B) Dodaj ISPRED kojeg prezimena zelis nadodati osobu? ");
	scanf("%s", targetSurnameB);
	printf("Unesi novu osobu (ime prezime godina rodenja): ");
	scanf("%s %s %d", newNameB, newSurnameB, &newBirthYearB);
	int statusB = addBefore(head, targetSurnameB, newNameB, newSurnameB, newBirthYearB);
	if (statusB == SUCCESS)
		printf("Nova osoba uspjesno dodana ispred %s!\n", targetSurnameB);
	else if (statusB == NOT_FOUND)
		printf("Osoba s prezimenom %s nije pronadena u listi.\n", targetSurnameB);
	else if (statusB == ERROR_MALLOC)
		printf("Greska pri alokaciji memorije!\n");
	else
		printf("Dogodila se greska pri dodavanju osobe (kod: %d)\n", statusB);
	// Ispis liste nakon dodavanja
	printf("\nLista nakon dodavanja:\n");
	printList(head);

                         /*Dodavanje IZA određenog prezimena*/
char targetSurname[MAX_LINE];
char newName[MAX_LINE];
char newSurname[MAX_LINE];
int newBirthYear = 0;

printf("\n A) Dodaj IZA kojeg prezimena zelis nadodati osobu? ");
scanf("%s", targetSurname);

printf("Unesi novu osobu (ime prezime godina rodenja): ");
scanf("%s %s %d", newName, newSurname, &newBirthYear);

int statusA = addAfter(head, targetSurname, newName, newSurname, newBirthYear);

if (statusA == SUCCESS)
    printf("Nova osoba uspjesno dodana iza %s!\n", targetSurname);
else if (statusA == NOT_FOUND)
    printf("Osoba s prezimenom %s nije pronadena u listi.\n", targetSurname);
else if (statusA == ERROR_MALLOC)
    printf("Greska pri alokaciji memorije!\n");
else
    printf("Dogodila se greska pri dodavanju osobe (kod: %d)\n", statusA);

// Ispis liste nakon dodavanja
printf("\nLista nakon dodavanja:\n");
printList(head);

                                    /*Traženje prezimena*/

// Inicijalizacija varijable za prezime koje se trazi
    char searchSurname[MAX_LINE] = "";

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

	                            /*Sortiranje liste po prezimenima*/
	int sortStatus = sortListBySurname(head);
	if (sortStatus == SUCCESS) {
		printf("\nLista je uspjesno sortirana po prezimenima.\n");
	}
	else {
		printf("\nDogodila se greska pri sortiranju liste (kod: %d)\n", sortStatus);
	}
	// Ispis liste nakon sortiranja
	printf("\nIspis sortirane liste:\n");
	totalPersons = printList(head);
	if (totalPersons == 0)
		printf("Lista je prazna.\n");
	else
		printf("\nUkupno osoba u listi nakon sortiranja: %d\n", totalPersons);

	                    /*Upis liste u datoteku*/

	// Inicijalizacija varijable za naziv datoteke
    char filename[MAX_LINE];

	// Unos naziva datoteke
    printf("\nUnesi naziv datoteke u koju zelis upisati listu (npr. osobe.txt): ");
    scanf("%s", filename);

	// Pozivanje funkcije za upis liste u datoteku
    int fileStatus = writeListToFile(head, filename);

	// Provjera statusa upisa
	if (fileStatus == SUCCESS) {
		printf("\nLista je uspjesno upisana u datoteku.\n");
	}
	else {
		printf("\nDogodila se greska pri upisu liste u datoteku (kod: %d)\n", fileStatus);
	}

    /*Čitanje liste iz datoteke*/
	Position fileHead = NULL;                 // Početak liste iz datoteke
	// Pozivanje funkcije za čitanje liste iz datoteke
	int readStatus = readListFromFile(&fileHead, filename);
	// Provjera statusa čitanja
	if (readStatus == SUCCESS) {
		printf("\nLista je uspjesno procitana iz datoteke:\n");
		printList(fileHead);
	}
	else {
		printf("\nDogodila se greska pri citanju liste iz datoteke (kod: %d)\n", readStatus);
	}
	// Oslobađanje memorije liste iz datoteke
	freeEntireList(&fileHead);

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
/* 7. Funkcija: dodaje novi element iza određenog elementa */
int addAfter(Position head, char* targetSurname, char* name, char* surname, int birthYear)
{
    if (head == NULL || targetSurname==NULL||name==NULL||surname==NULL)              // Provjera validnosti pokazivača
        return ERROR_INVALID_ARGUMENT;

	// Inicijalizacija pokazivača za trenutni čvor
    Position current = head;

    // Prolazak kroz listu i traženje ciljanog prezimena
    while (current != NULL) {
        if (strcmp(current->surname, targetSurname) == 0) {
            // Ciljano prezime pronađeno, dodaj novi čvor iza njega
        
            // Alokacija memorije za novi čvor
            Position newPerson = (Position)malloc(sizeof(Person));
            if (newPerson == NULL)
				return ERROR_MALLOC;

            // Kopiranje podataka u novi čvor
            strcpy(newPerson->name, name);
            strcpy(newPerson->surname, surname);
            newPerson->birthYear = birthYear;

            // Umetanje novog čvora iza trenutnog
            newPerson->next = current->next;
            current->next = newPerson;
            return SUCCESS;                  // Uspješno dodano
        }
        current = current->next;
	}
	return NOT_FOUND;          // Ciljano prezime nije pronađeno
}
/* 8. Funkcija: dodaje novi element ispred određenog elementa */
int addBefore(Position head, char* targetSurname, char* name, char* surname, int birthYear)
{
    if (head == NULL || targetSurname == NULL || name == NULL || surname == NULL)              // Provjera validnosti pokazivača
        return ERROR_INVALID_ARGUMENT;

	//Sve isto kao i u funkciji addAfter, samo što se dodaje ispred
    if (strcmp(head->surname, targetSurname) == 0) {
        // Alokacija memorije za novi čvor
        Position newPerson = (Position)malloc(sizeof(Person));
        if (newPerson == NULL)
            return ERROR_MALLOC;
        // Kopiranje podataka u novi čvor
        strcpy(newPerson->name, name);
        strcpy(newPerson->surname, surname);
        newPerson->birthYear = birthYear;
        // Novi čvor postaje novi head
        newPerson->next = head;
        head = newPerson;
        return SUCCESS;                  // Uspješno dodano
    }
    // Inicijalizacija pokazivača za trenutni i prethodni čvor
    Position current = head;
    Position previous = NULL;
    // Prolazak kroz listu i traženje ciljanog prezimena
    while (current != NULL) {
        if (strcmp(current->surname, targetSurname) == 0) {
            // Ciljano prezime pronađeno, dodaj novi čvor ispred njega
           
            // Alokacija memorije za novi čvor
            Position newPerson = (Position)malloc(sizeof(Person));
            if (newPerson == NULL)
                return ERROR_MALLOC;
            // Kopiranje podataka u novi čvor
            strcpy(newPerson->name, name);
            strcpy(newPerson->surname, surname);
            newPerson->birthYear = birthYear;
            // Umetanje novog čvora ispred trenutnog
            newPerson->next = current;
            if (previous != NULL) {
                previous->next = newPerson;
            }
            return SUCCESS;                  // Uspješno dodano
        }
        previous = current;
        current = current->next;
    }
    return NOT_FOUND;          // Ciljano prezime nije pronađeno
}
/* 9. Funkcija: sortira listu po prezimenima osoba */
int sortListBySurname(Position head) {
    if (head == NULL)                        // Lista prazna
        return ERROR_INVALID_ARGUMENT;
    Position i, j;
    char tempName[MAX_LINE];
    char tempSurname[MAX_LINE];
    int tempBirthYear;
    // Bubble sort algoritam za sortiranje liste po prezimenima
    for (i = head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (strcmp(i->surname, j->surname) > 0) {
                // Zamjena podataka između čvorova
                strcpy(tempName, i->name);
                strcpy(tempSurname, i->surname);
                tempBirthYear = i->birthYear;
                strcpy(i->name, j->name);
                strcpy(i->surname, j->surname);
                i->birthYear = j->birthYear;
                strcpy(j->name, tempName);
                strcpy(j->surname, tempSurname);
                j->birthYear = tempBirthYear;
            }
        }
    }
	return SUCCESS;
}

/* 10. Funkcija: upisuje listu u datoteku */
int writeListToFile(Position head, char* filename) {
	if (filename == NULL)     //Ne ruši se ako je head NULL, samo ne upisuje ništa!!!
        return ERROR_INVALID_ARGUMENT;
    FILE* file = fopen(filename, "w");       // Otvaranje datoteke za pisanje
    if (file == NULL)
        return ERROR_FILE_OPENING;
    Position current = head;
    // Prolazak kroz listu i upis podataka u datoteku
    while (current != NULL) {
        fprintf(file, "%s %s %d\n", current->name, current->surname, current->birthYear);
        current = current->next;
    }
    fclose(file);                            // Zatvaranje datoteke
    return SUCCESS;
}

/* 11. Funkcija: čita listu iz datoteke */
int readListFromFile(Position* headPointer, char* filename) {
	// Provjera validnosti pokazivača
    if (headPointer == NULL || filename == NULL)
        return ERROR_INVALID_ARGUMENT;

    FILE* file = fopen(filename, "r");       // Otvaranje datoteke za čitanje
    if (file == NULL)
        return ERROR_FILE_OPENING;

	// Inicijalizacija varijabli za čitanje podataka
    char name[MAX_LINE];
    char surname[MAX_LINE];
    int birthYear;
    // Čitanje podataka iz datoteke i dodavanje u listu
    while (fscanf(file, "%s %s %d", name, surname, &birthYear) == 3) {
		// Dodavanje nove osobe na kraj liste i provjera statusa
        int status = addAtTheEnd(headPointer, name, surname, birthYear);
        if (status != SUCCESS) {
            fclose(file);
            return status;                   // Vraćanje greške ako dodavanje nije uspjelo
        }
    }
    fclose(file);                            // Zatvaranje datoteke

    return SUCCESS;
}