#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_FAIL -1
#define EMPTY_FILE -2



typedef struct el {
	char* name;			//ime svakog elementa
	struct el* sibling;	//lista za sibling elemente
	struct el* child;	//lista za child elemente
	struct el* parent;	//lista za roditelj elemente
} element;

typedef element* directory;		//pointer na strukturu element

int md(directory);				//funkcija za pravljenje novih poddirektorija
directory cd_dir(directory);	//funkcija za prebacivanje u poddirektorij
directory cd(directory);		//funkcija za prebacivanje u roditeljski direktorij
int dir(directory);				//funkcija za ispisivanje direktorija i njegovih poddirektorija
void exitAll(directory node);				//funkcija za brisanje citavog stabla


int md(directory root) {		//1.) funkcija md za novi direktorij
	directory curr = root;		

	char name[15];
	printf("Unesi ime direktorija: \n");
	scanf("%s", name);


	directory newDir = (directory)malloc(sizeof(element));	//novi direktorij

	directory newDir = malloc(sizeof(element));			//provjera za malloc
	if (newDir == NULL) {
		printf("Nemoguće alocirati memoriju!\n");
		return MALLOC_FAIL;
	}

	newDir->child = NULL;
	newDir->sibling = NULL;
	newDir->parent = curr;			//alociramo roditelja ovog direktorija na root
	newDir->name = _strdup(name);



	if (root->child == NULL) {
		root->child = newDir;		//ako roditelj nema druge djece postavljamo novi direktorij kao njegovo dijete

		return EXIT_SUCCESS;
	}
	else {
		directory temp = root->child;
		while (temp->sibling != NULL)		//ako direktorij ima djecu trazimo zadnjeg siblinga 
			temp = temp->sibling;
		temp->sibling = newDir;				//kada pronademo zadnjeg siblinga na njega dodajemo novi direktorij


	}
	return EXIT_SUCCESS;
}

directory cd_dir(directory root) {		//2.) funkcija za prebacivanje u poddirektorij
	char target[20];
	printf("Unesite ime poddriektorija u koji se zelite prebaciti.\n");
	scanf("%s", target); //ime trazenog direktorija


	directory temp = root->child;


	while (temp != NULL && strcmp(target, temp->name)) {	//vrti petlju dok ne nade trazeni direktorij
		temp = temp->sibling;
	}
	if (temp == NULL) {
		printf("Ne postoji trazeni podddirektorij.\n");
		return root;
	}
	return temp;										//ako ga pronade postavlja u mainu na ovaj poddirektorij

}

directory cd(directory child_) {		//3.) funkcija za prebacivanje u roditeljski direktorij
	directory root = child_->parent;	//postavlja direktorij na svog roditelja
	if (root != NULL) {
		printf("Direktorij %s\n", root->name);
		return root;
	}
	else {
		printf("Vec se nalazite u root direktoriju.\n");			//ako nema roditelja znaci da smo vec u rootu
		return child_;
	}

}

int dir(directory root) {				//4.) funkcija za ispisivanje direktorija i njegovih poddirektorija
	printf("\nDirektorij:%s\nPoddirektoriji:\n", root->name);
	directory temp = root->child;
	if (temp == NULL)
		printf("Ne postoje.\n");

	while (temp != NULL) {			//prolazi kroz sve siblinge
		printf("%s\n", temp->name);
		temp = temp->sibling;
	}
	return EXIT_SUCCESS;
}

void exitAll(directory node) {			//5.) funkcija za brisanje čitavog stabla
	if (node == NULL)
		return;

	exitAll(node->child);		 // prvo obriši djecu

	exitAll(node->sibling);		// potom obriši "sibling" listu

	free(node->name);			// oslobodi ime

	free(node);					// na kraju oslobodi sam čvor
}



int main() {

	directory root = (directory)malloc(sizeof(element)); //alokacija i inicijalizacija za pocetni root direktorij

	root->child = NULL;
	root->parent = NULL;
	root->sibling = NULL;
	root->name = _strdup(":C");

	directory current = root; //pokazivac na direktorij u kojem se nalazimo
	int choice = 0;

	do {
		printf("1-md\n2-cd dir\n3-cd\n4-dir\n5-exit\n\n");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			md(current);
			printf("\n\n");
			break;
		case 2:
			current = cd_dir(current);
			if (current != NULL)
				printf("Trenutni direktorij %s.\n\n", current->name);
			break;
		case 3:
			current = cd(current);
			printf("\n\n");
			break;
		case 4:
			dir(current);
			printf("\n\n");
			break;
		case 5:
		case 5:
			exitAll(root);
			printf("Izlazak iz programa!\n");
			exit(EXIT_SUCCESS);

			break;

		default:
			printf("Pogresan unos.\n");
			break;
		}
	} while (choice != 5);


	return EXIT_SUCCESS;
}