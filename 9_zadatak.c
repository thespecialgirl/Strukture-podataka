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
dijela zadatka.*/#define _CRT_SECURE_NO_WARNINGS #include <stdio.h>#include <stdlib.h>#define MAX_LENGTH 10typedef struct tree {
	int n;
	treePos left;
	treePos right;
}Tree;

typedef struct tree* treePos;

//deklarcije
treePos insert(treePos root, int value);int main() {	treePos root = NULL;	int array[MAX_LENGTH] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };	for (int i = 0; i < MAX_LENGTH; i++) {		root = insert(root, array[i]);	}	return 0;}//1. Unos elem
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