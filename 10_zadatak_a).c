/*10. Napisati program koji čita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država. Uz
ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
države. Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada,
broj_stanovnika.
a) Potrebno je formirati sortiranu vezanu listu država po nazivu. Svaki čvor vezane liste
sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.
b) Potrebno je formirati stablo država sortirano po nazivu. Svaki čvor stabla sadrži vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.
Nakon formiranja podataka potrebno je ispisati države i gradove te omogućiti korisniku putem
tastature pretragu gradova određene države koji imaju broj stanovnika veći od unosa na
tastaturi.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_NOT_OPENED -1
#define MALLOC_FAILED -2
#define MAX_LENGTH 50

typedef struct Country* countryPosition;
typedef struct Town* townPosition;

typedef struct Country {
	char name[MAX_LENGTH];
	struct Country* next;
	townPosition townroot;	//pokazivac na root stabla gradova
}Country;

typedef struct Town {
	char name[MAX_LENGTH];
	int population;
	struct Town* left;		//za stablo gradova
	struct Town* right;
}Town;

//deklaracije funkcija
int readCountriesFromFile(const char* fileName, countryPosition head);
countryPosition createCountry(const char* name);
townPosition createTown(const char* name, int population);
townPosition insertSortedTowns(townPosition root, townPosition newTown);
int readTownsFromFile(const char* fileName, townPosition* root);
int insertSortedCountries(countryPosition head, countryPosition newCountry);
int printTowns(townPosition root);
int printCountries(countryPosition head);
countryPosition findCountryInList(countryPosition head, const char* name);
int printTownsBigger(townPosition root, int limit);
int freeTowns(townPosition root);
int searchTowns(countryPosition head);

int freeCountries(countryPosition head);

int main() {
	Country head;
	head.next = NULL;
	readCountriesFromFile("drzave.txt",&head);
	printCountries(&head);
	searchTowns(&head);
	freeCountries(&head);
	return 0;
}

//1)fja za citanje datoteke drzave.txt
int readCountriesFromFile(const char* fileName,countryPosition head) {
	FILE* file = fopen(fileName,"r");
	if (!file) {
		printf("Datoteka %s se nije otvorila!\n",fileName);
		return FILE_NOT_OPENED;
	}
	char countryName[MAX_LENGTH] = {0};
	char townFileName[MAX_LENGTH] = {0};
	while (fscanf(file, "%s %s", countryName, townFileName) == 2) {

	countryPosition newCountry = createCountry(countryName);	
	if (!newCountry) {
		printf("Alokacija za drzavu nije uspijela!\n");
		fclose(file);
		return MALLOC_FAILED;
	}
	//printf("Ime drzave: %s - Datoteka gradova: %s\n",countryName,townFileName); -bilo na pocetku za provj citanja

		readTownsFromFile(townFileName,&newCountry->townroot);
		insertSortedCountries(head,newCountry);
	}


	fclose(file);
	return 0;
}

//2)fja za ubacivanje nove drzave u listu
countryPosition createCountry(const char* name) {
	countryPosition newCountry = NULL;
	newCountry=(countryPosition)malloc(sizeof(Country));
	if (!newCountry) {
		printf("Alokacija za drzavu nije uspijela!\n");
		return NULL;
	}
	strcpy(newCountry->name,name);
	newCountry->next = NULL;
	newCountry->townroot = NULL;
	return newCountry;
}

//3)fja za ubacivanje novog grada u stablo
townPosition createTown(const char* name,int population) {
	townPosition newTown = NULL;
	newTown = (townPosition)malloc(sizeof(Town));
	if (!newTown) {
		printf("Alokacija za grad nije uspijela!\n");
		return NULL;
	}
	strcpy(newTown->name, name);
	newTown->population = population;
	newTown->left= NULL;
	newTown->right= NULL;
	return newTown;
}

//4)fja za sortirani unos gradova u stablo
townPosition insertSortedTowns(townPosition root, townPosition newTown) {
	if (root == NULL) {
		return newTown;
	}
	if (newTown->population < root->population) {
		root->left = insertSortedTowns(root->left,newTown);
	}
	else if (newTown->population > root->population) {
		root->right = insertSortedTowns(root->right, newTown);
	}
	else {
		if (strcmp(newTown->name, root->name) < 0) {
			root->left = insertSortedTowns(root->left, newTown);
		}
		else {
			root->right = insertSortedTowns(root->right, newTown);
		}
		}
	return root;
}

//5)fja za citanje gradova
int readTownsFromFile(const char* fileName,townPosition*root ) {
	FILE* file = fopen(fileName, "r");
	if (!file) {
		printf("Datoteka %s se nije otvorila!\n", fileName);
		return FILE_NOT_OPENED;
	}
	char townName[MAX_LENGTH] = { 0 };
	int population = 0;
	while (fscanf(file, " %[^,],%d", townName, &population) == 2) {
		townPosition newTown = createTown(townName,population);
		if (!newTown) {
			printf("Alokacija za grad nije uspijela!\n");
			fclose(file);
			return MALLOC_FAILED;
		}
		*root = insertSortedTowns(*root,newTown);
	}

	fclose(file);
	return 0;
}

//6)fja za sortiranje liste drzava po imenu
int insertSortedCountries(countryPosition head,countryPosition newCountry) {
	countryPosition current=head;
	while (current->next!=NULL && strcmp(current->next->name, newCountry->name)<0) {
		current = current->next;
	}
	newCountry->next = current->next;
	current->next = newCountry;
	return 0;
}

//7)fja za ispis gradova
int printTowns(townPosition root) {
	if (root == NULL) {
		return 0;
	}
	printf("\t%s,%d \n",root->name,root->population);
	printTowns(root->left);
	printTowns(root->right);
	return 0;
}

//8)fja za ispis drzava
int printCountries(countryPosition head) {
	countryPosition current = head->next;	//preskoci dummy head
	if (current == NULL) {
		printf("Lista drzava je prazna.\n");
		return 0;
	}
	while (current != NULL) {
		printf("Drzava: %s\n",current->name);
		printTowns(current->townroot);
		current = current->next;
	}
	return 0;
}

//9)fja za pronaci drzavu u listi
countryPosition findCountryInList(countryPosition head, const char* name) {
	countryPosition current = head->next;
	while (current!=NULL) {
		if (strcmp(current->name, name) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

//10)fja za ispis vecih gradova od onoga sta je korisnik unio
int printTownsBigger(townPosition root, int limit) {
	if (root==NULL){
		return 0;
	}
	if (root->population > limit) {
		printf("\t%s,%d\n",root->name,root->population);
	}
	printTownsBigger(root->left,limit);
	printTownsBigger(root->right, limit);
	return 0;
}

//11)fja za pretragu drzava
int searchTowns(countryPosition head) {
	char countryName[MAX_LENGTH] = { 0 };
	int limit = 0;
	printf("\nUnesi ime drzave: ");
	scanf(" %s",countryName);
	countryPosition country = findCountryInList(head,countryName);
	if (!country) {
		printf("Drzava %s ne postoji u listi!\n",countryName);
		return -1;
	}
	printf("\nUnesi minimalni broj stanovnika: ");
	scanf(" %d", &limit);
	printf("\nGradovi od drzave %s s vise od %d stanovnika: \n",countryName,limit);
	printTownsBigger(country->townroot,limit);
	return 0;
}

//12)fje za brisanje
int freeTowns(townPosition root) {
	if (root==NULL) {
		return 0;
	}
	freeTowns(root->left);
	freeTowns(root->right);
	free(root);
	return 0;
}
int freeCountries(countryPosition head) {
	countryPosition current = head->next;
	countryPosition temp = NULL;
	while (current != NULL) {
		freeTowns(current->townroot);
		temp = current;
		current = current->next;
		free(temp);
		return 0;
	}
	head->next = NULL;
	return 0;
}
