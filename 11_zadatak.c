/*11. Prepraviti zadatak 10 na način da se formira hash tablica država. Tablica ima 11 mjesta, a
funkcija za preslikavanje ključ računa da se zbraja ASCII vrijednost prvih pet slova države zatim
računa ostatak cjelobrojnog dijeljenja te vrijednosti s veličinom tablice. Države s istim ključem se
pohranjuju u vezanu listu sortiranu po nazivu države. Svaki čvor vezane liste sadrži stablo
gradova sortirano po broju stanovnika, zatim po nazivu grada.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 50
#define TABLE_SIZE 11
#define FILE_NOT_OPENED -1
#define MALLOC_FAILED -2

typedef struct Country* countryPos;
typedef struct Town* townPos;

typedef struct Town {
	char name[MAX_LENGTH];
	int population;
	townPos left;
	townPos right;
}Town;

typedef struct Country {
	char name[MAX_LENGTH];
	townPos townRoot;
	countryPos next;
}Country;

//deklaracije funkcija
int hashKey(const char* countryName);
countryPos createCountry(const char* name);
int insertSortedCountries(countryPos head, countryPos newCountry);
townPos insertSortedTowns(townPos root, townPos newTown);
townPos createTown(const char* name, int population);
int readTownsFromFile(const char* fileName, townPos* root);
int readCountriesFromFile(const char* fileName, countryPos table[]);
int printTowns(townPos root);
int printHashTable(countryPos table[]);
countryPos findCountry(countryPos table[], const char* name);
int printTownsBigger(townPos root, int limit);
int freeTowns(townPos root);
int freeCountries(countryPos head);
int freeHashTable(countryPos table[]);
int searchTowns(countryPos table[]);

int main() {
	countryPos table[TABLE_SIZE];
	for (int i = 0; i < TABLE_SIZE; i++) {
		table[i] = createCountry("");	//dummy head
	}
	if (readCountriesFromFile("drzave.txt", table)!=0) {
		printf("Greska pri citanju datoteke drzave.txt!\n");
		freeHashTable(table);
		return FILE_NOT_OPENED;
	}
	printHashTable(table);
	searchTowns(table);
	freeHashTable(table);
	return 0;
}

//1)fja za hash tablicu
int hashKey(const char* countryName) {
	int sum = 0;
	for (int i = 0; i < 5 && countryName[i] !='\0'; i++) {		//ASCII prvih 5 slova
		sum +=(unsigned char)countryName[i];
	}
	return sum%TABLE_SIZE;
}

//2)fja za kreiranje drzave
countryPos createCountry(const char* name) {
	countryPos newCountry = NULL;
	newCountry = (countryPos)malloc(sizeof(Country));
	if (!newCountry) {
		printf("Alokacija za drzavu nije uspijela!\n");
		return NULL;
	}
	strcpy(newCountry->name, name);
	newCountry->next = NULL;
	newCountry->townRoot = NULL;
	return newCountry;
}

//3)fja za sortiranje drzava po imenu
int insertSortedCountries(countryPos head, countryPos newCountry) {
	countryPos current = head;
	while (current->next != NULL && strcmp(current->next->name, newCountry->name) < 0) {
		current = current->next;
	}
	newCountry->next = current->next;
	current->next = newCountry;
	return 0;
}

//4)fja za sortirani unos gradova u stablo
townPos insertSortedTowns(townPos root, townPos newTown) {
	if (root == NULL) {
		return newTown;
	}
	if (newTown->population < root->population) {
		root->left = insertSortedTowns(root->left, newTown);
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

//5)fja za ubacivanje novog grada u stablo
townPos createTown(const char* name, int population) {
	townPos newTown = NULL;
	newTown = (townPos)malloc(sizeof(Town));
	if (!newTown) {
		printf("Alokacija za grad nije uspijela!\n");
		return NULL;
	}
	strcpy(newTown->name, name);
	newTown->population = population;
	newTown->left = NULL;
	newTown->right = NULL;
	return newTown;
}

//6)fja za citanje gradova
int readTownsFromFile(const char* fileName, townPos* root) {
	FILE* file = fopen(fileName, "r");
	if (!file) {
		printf("Datoteka %s se nije otvorila!\n", fileName);
		return FILE_NOT_OPENED;
	}
	char townName[MAX_LENGTH] = { 0 };
	int population = 0;
	while (fscanf(file, " %[^,],%d", townName, &population) == 2) {
		townPos newTown = createTown(townName, population);
		if (!newTown) {
			printf("Alokacija za grad nije uspijela!\n");
			fclose(file);
			return MALLOC_FAILED;
		}
		*root = insertSortedTowns(*root, newTown);
	}

	fclose(file);
	return 0;
}

//7)fja za citanje datoteke drzave.txt
int readCountriesFromFile(const char* fileName, countryPos table[]) {
	FILE* file = fopen(fileName, "r");
	if (!file) {
		printf("Datoteka %s se nije otvorila!\n", fileName);
		return FILE_NOT_OPENED;
	}
	char countryName[MAX_LENGTH] = { 0 };
	char townFileName[MAX_LENGTH] = { 0 };
	while (fscanf(file, "%s %s", countryName, townFileName) == 2) {

		countryPos newCountry = createCountry(countryName);
		if (!newCountry) {
			printf("Alokacija za drzavu nije uspijela!\n");
			fclose(file);
			return MALLOC_FAILED;
		}
		readTownsFromFile(townFileName, &newCountry->townRoot);
		int index = hashKey(countryName);
		insertSortedCountries(table[index], newCountry);
	}
	fclose(file);
	return 0;
}

//8)fja za inorder ispis gradova
int printTowns(townPos root) {
if (root == NULL) {
	return 0;
}
printTowns(root->left);
printf("\t%s,%d \n", root->name, root->population);
printTowns(root->right);
return 0;
}
//9)ispis hash tablice
int printHashTable(countryPos table[]) {

	for (int i = 0; i < TABLE_SIZE; i++) {
		 printf("\n[%d]\n",i);
		 countryPos current = table[i]->next;	//preskacem dummy head
		 while (current != NULL) {
			 printf("Drzava:%s\n",current->name);
			 printTowns(current->townRoot);
			 current = current->next;
		 }
	}
	return 0;
}

//10)fja za pronaci drzavu u listi
countryPos findCountryInList(countryPos head, const char* name) {
	countryPos current = head->next;
	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

//11)fja za pronaci drzavu u hash tablici
countryPos findCountry(countryPos table[], const char* name) {
	int index = hashKey(name);
	return findCountryInList(table[index],name);
}

//12)fja za ispis vecih gradova od onoga sta je korisnik unio
int printTownsBigger(townPos root, int limit) {
	if (root == NULL) {
		return 0;
	}
	printTownsBigger(root->left, limit);
	if (root->population > limit) {
		printf("\t%s,%d\n", root->name, root->population);
	}
	printTownsBigger(root->right, limit);
	return 0;
}

//13)fje free
int freeTowns(townPos root) {
if (root == NULL) {
	return 0;
}
freeTowns(root->left);
freeTowns(root->right);
free(root);
return 0;
}

int freeCountries(countryPos head) {
	countryPos current = head->next;
	countryPos temp = NULL;
	while (current != NULL) {
		temp = current;
		current = current->next;
		freeTowns(temp->townRoot);
		free(temp);
	}
	free(head);		//free od dummy heada
	return 0;
}

int freeHashTable(countryPos table[]) {
	for (int i = 0; i < TABLE_SIZE; i++) {
		freeCountries(table[i]);
		table[i] = NULL;
	}
	return 0;
}

//14)fja za pretragu drzava
int searchTowns(countryPos table[]) {
	char countryName[MAX_LENGTH];
	int limit = 0;
	printf("\nUnesi ime drzave: ");
	scanf(" %s", countryName);

	countryPos country = findCountry(table, countryName);
	if (!country) {
		printf("Drzava %s ne postoji u tablici!\n", countryName);
		return 0;
	}
	printf("\nUnesi minimalni broj stanovnika: ");
	scanf("%d", &limit);
	printf("\nGradovi od drzave %s s vise od %d stanovnika: \n", country->name, limit);
	printTownsBigger(country->townRoot, limit);
	return 0;
}