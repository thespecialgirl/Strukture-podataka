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
	countryPosition left;
	countryPosition right;
	townPosition townHead;
}Country;
typedef struct Town {
	char name[MAX_LENGTH];
	int population;
	townPosition next;
}Town;

//deklaracije funkcija
countryPosition createCountry(const char* name);
countryPosition insertSortedCountries(countryPosition root, countryPosition newCountry);
townPosition createTown(const char* name, int population);
int insertSortedTowns(townPosition head, townPosition newTown);
countryPosition readCountriesFromFile(const char* fileName);
int readTownsFromFile(const char* fileName, townPosition head);
int printTowns(townPosition head);
int printCountries(countryPosition root);
countryPosition findCountry(countryPosition root, const char* name);
int printBiggerTowns(townPosition head, int limit);
int searchTowns(countryPosition root);
int freeTowns(townPosition head);
int freeCountries(countryPosition root);

int main() {
	countryPosition root = readCountriesFromFile("drzave.txt");
	printCountries(root);
	searchTowns(root);
	freeCountries(root);
	return 0;
}

//1)fja za dodavanje drzave
countryPosition createCountry(const char* name) {
	countryPosition newCountry = NULL;
	newCountry = (countryPosition)malloc(sizeof(Country));
	if (!newCountry) {
		printf("Alokacija za drzavu nije uspijela!\n");
		return NULL;
	}
	strcpy(newCountry->name, name);
	newCountry->left = NULL;
	newCountry->right = NULL;
	newCountry->townHead = (townPosition)malloc(sizeof(Town));
	if (!newCountry->townHead) {
		free(newCountry);
		return NULL;
	}
	newCountry->townHead->next = NULL;
	return newCountry;
}

//2)fja za sortiranje drzava u BST
countryPosition insertSortedCountries(countryPosition root, countryPosition newCountry) {
	if (root == NULL) {
		return newCountry;
	}
	if (strcmp(newCountry->name, root->name) < 0) {
		root->left = insertSortedCountries(root->left, newCountry);
	}
	else if (strcmp(newCountry->name, root->name) > 0) {
		root->right = insertSortedCountries(root->right, newCountry);
	}
	else {
		free(newCountry);
	}
	return root;
}

//3)fja za dodavanje novog grada
townPosition createTown(const char* name, int population) {
	townPosition newTown = NULL;
	newTown = (townPosition)malloc(sizeof(Town));
	if (!newTown) {
		printf("Alokacija za grad nije uspijela!\n");
		return NULL;
	}
	strcpy(newTown->name, name);
	newTown->population = population;
	newTown->next = NULL;
	return newTown;
}

//4) sortiranje liste gradova
int insertSortedTowns(townPosition head, townPosition newTown) {
	townPosition current = head;
	while (current->next != NULL && (current->next->population < newTown->population ||
		(current->next->population == newTown->population && strcmp(current->next->name, newTown->name) < 0))) {
		current = current->next;
	}
	newTown->next = current->next;
	current->next = newTown;
	return 0;
}

//5)fja za citanje datoteke drzave.txt
countryPosition readCountriesFromFile(const char* fileName) {
	FILE* file = fopen(fileName, "r");
	if (!file) {
		printf("Datoteka %s se nije otvorila!\n", fileName);
		return NULL;
	}
	countryPosition root = NULL;
	char countryName[MAX_LENGTH] = { 0 };
	char townFileName[MAX_LENGTH] = { 0 };
	while (fscanf(file, "%s %s", countryName, townFileName) == 2) {
		countryPosition newCountry = createCountry(countryName);
		if (!newCountry) {
			printf("Alokacija za drzavu nije uspijela!\n");
			fclose(file);
			return root;
		}
		root = insertSortedCountries(root, newCountry);
		readTownsFromFile(townFileName, newCountry->townHead);
	}
	fclose(file);
	return root;
}

//6)fja za citanje gradova iz datoteke
int readTownsFromFile(const char* fileName, townPosition head) {
	FILE* file = fopen(fileName, "r");
	if (!file) {
		printf("Datoteka %s se nije otvorila!\n", fileName);
		return FILE_NOT_OPENED;
	}
	char townName[MAX_LENGTH];
	int population = 0;
	while (fscanf(file, " %[^,],%d", townName, &population) == 2) {

		townPosition newTown = createTown(townName, population);
		if (!newTown) {
			printf("Alokacija za grad nije uspijela!\n");
			fclose(file);
			return MALLOC_FAILED;
		}
		insertSortedTowns(head, newTown);
	}

	fclose(file);
	return 0;
}

//7)fja za ispis gradova
int printTowns(townPosition head) {
	townPosition current = head->next;
	while (current != NULL) {
		printf("\t%s,%d \n", current->name, current->population);
		current = current->next;
	}
	return 0;
}

//8)fja za ispis drzava
int printCountries(countryPosition root) {
	if (root == NULL) {
		return 0;
	}
	printCountries(root->left);			//sortirano po nazivu pa treba inorder sort
	printf("Drzava: %s\n", root->name);
	printTowns(root->townHead);

	printCountries(root->right);
	return 0;
}

//9)fja za trazenje drzave u stablu
countryPosition findCountry(countryPosition root, const char* name) {
	if (root == NULL) { return NULL; }
	int cmp = strcmp(name, root->name);
	if (cmp == 0)  return root;
	else if (cmp < 0)  return findCountry(root->left, name);
	else return findCountry(root->right, name);
}

//10)fja za ispis gradova iznad unesenog br st
int printBiggerTowns(townPosition head, int limit) {
	townPosition current = head->next;
	while (current != NULL) {
		if (current->population > limit) {
			printf("\t%s,%d \n", current->name, current->population);
		}
		current = current->next;
	}
	return 0;
}

//11)fja za pretrazivanje korisnika
int searchTowns(countryPosition root) {
	char countryName[MAX_LENGTH];
	int limit = 0;
	printf("\nUnesi ime drzave: ");
	scanf(" %s", countryName);

	countryPosition country = findCountry(root, countryName);
	if (!country) {
		printf("Drzava %s ne postoji u listi!\n", countryName);
		return -1;
	}
	printf("\nUnesi minimalni broj stanovnika: ");
	scanf(" %d", &limit);
	printf("\nGradovi od drzave %s s vise od %d stanovnika: \n", countryName, limit);
	printBiggerTowns(country->townHead, limit);
	return 0;
}

//12)fje za brisanje gradova i drzava
int freeTowns(townPosition head) {
	townPosition current = head;
	townPosition temp = NULL;
	while (current != NULL) {
		temp = current;
		current = current->next;
		free(temp);
	}
	return 0;
}

int freeCountries(countryPosition root) {
	if (root == NULL) {
		return 0;
	}
	freeCountries(root->left);
	freeCountries(root->right);
	freeCountries(root->townHead);
	free(root);
	return 0;
}