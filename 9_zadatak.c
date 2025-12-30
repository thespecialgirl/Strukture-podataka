/*9. Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u čvorove binarnog stabla.
a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
sve element zadanog niza brojeva stvori stablo sa lijeve strane veći,a sa desne strane manji brojevi. Funkcija vraća
pokazivač na korijen stabla.
b) Napisati funkciju replace koja će svaki element stabla zamijeniti sumom elemenata u
njegovom lijevom i desnom podstablu(tj.sumom svih potomaka prije zamjene
vrijednosti u tim potomcima).Npr.stablo sa slike Slika 1 transformirat će se u stablo na
slici Slika 2.
c) Prepraviti program na način da umjesto predefiniranog cjelobrojnog polja korištenjem
funkcije rand() generira slučajne brojeve u rasponu <10, 90>.Također, potrebno je
upisati u datoteku sve brojeve u inorder prolasku nakon korištenja funkcije iz a), zatim b)
dijela zadatka.*/
#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#define MAX_LENGTH 10
#define FILE_NOT_OPENED -1
#include <time.h>

typedef struct tree* treePos;

typedef struct tree {
	int n;
	treePos left;
	treePos right;
}Tree;


//deklarcije
treePos insert(treePos root, int value);
int replace(treePos root);
void deleteTree(treePos root);
void inorderToFile(treePos root,FILE*f);


int main() {
	treePos root = NULL;
	int array[MAX_LENGTH] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };
	for (int i = 0; i < MAX_LENGTH; i++) {
		root = insert(root, array[i]);
	}
	replace(root);
	deleteTree(root);
	root = NULL;

	//c)srand
	srand((unsigned)time(NULL));
	treePos root2 = NULL;
	int min = 10;
	int max = 90;

	for (int i = 0; i < MAX_LENGTH; i++) {
		int value = rand() % (max - min + 1) + min;
		root2 = insert(root2, value);
	}
	FILE* fp = fopen("inorder_ispis.txt","w");
	if (!fp)
		return FILE_NOT_OPENED;
	fprintf(fp,"Inorder ispis nakon insert funkcije:\n");
	inorderToFile(root2,fp);
	replace(root2);
	fprintf(fp, "\nInorder ispis nakon replace funkcije:\n");
	inorderToFile(root2, fp);
	fclose(fp);
	deleteTree(root2);
	root2 = NULL;
	return 0;
}
//a) Unos elem.
treePos insert(treePos root, int value) {
	if (root == NULL) {
		root = (treePos)malloc(sizeof(Tree));
		if (!root)
			return NULL;
		root->n = value;
		root->left = NULL;
		root->right = NULL;
		return root;
	}

	if (value > root->n) {
		root->left = insert(root->left, value);
	}
	else if (value < root->n) {
		root->right = insert(root->right, value);
	}
	return root;
}
//b)
int replace(treePos root){
if (root == NULL)
return 0;						//prazno stablo, suma=0
int prevValue = root->n;		//pamti vrijednost do tada u cvoru
root->n = replace(root->left) + replace(root->right);		//suma sve djece bez roota
return root->n + prevValue;
}
//brisanje stabla rekurzivno
void deleteTree(treePos root) {
	if (root == NULL)
		return;		//za prekid funkc.

	deleteTree(root->left);		//prvo brisemo djecu
	deleteTree(root->right);

	free(root);				//zadnje brisemo roditelja
}
//c) za inorder ispis u datoteku
void inorderToFile(treePos root, FILE* f) {
	if (root!=NULL) {
		inorderToFile(root->left,f);
		fprintf(f,"%d ",root->n);
		inorderToFile(root->right, f);
	}
}