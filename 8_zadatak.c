/*8. Napisati program koji omogućava rad s binarnim stablom pretraživanja. Treba
omogućiti unošenje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i
level order), brisanje i pronalaženje nekog elementa.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct tree* treePos;

//deklarcije
treePos insert(treePos root,int value);
treePos deleteNode(treePos root, int value);
treePos findMin(treePos root);
void inorder(treePos root);
void postorder(treePos root);
void preorder(treePos root);
void levelOrder(treePos root);
treePos find(treePos root, int value);
void deleteTree(treePos root);


typedef struct tree {
	int n;
	treePos left;
	treePos right;
}Tree;

int main() {
	treePos root = NULL;
	root = insert(root,10);
	root = insert(root, 7);
	root = insert(root, 12);
	root = insert(root, 4);
	root = insert(root, 5);
	root = insert(root, 3);
	root = deleteNode(root,4);
	printf("Inorder ispis: ");
	inorder(root);
	printf("\n");

	printf("Postrder ispis: ");
	postorder(root);
	printf("\n");

	printf("Preorder ispis: ");
	preorder(root);
	printf("\n");


	printf("Level order ispis: ");
	levelOrder(root);
	printf("\n");

	treePos result = find(root, 7);

	if (result != NULL)
		printf("Element %d je pronadjen u stablu.\n", result->n);
	else
		printf("Element nije pronadjen u stablu.\n");

	deleteTree(root);
	root = NULL;

	return 0;
}

//1. Unos elem
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

if (value < root->n) {
	root->left = insert(root->left, value);
}
if (value > root->n) {
	root->right = insert(root->right, value);
}
return root;
}
//2. Brisanje nekog elem
treePos deleteNode(treePos root, int value) {
	if (root == NULL)
		return NULL;
	if (value < root->n) {
		root->left = deleteNode(root->left, value);
	}
	else if (value > root->n) {
		root->right = deleteNode(root->right, value);
	}
	else {		//našli cvor za brisanje

		if (root->left == NULL) {
			treePos temp = root->right;
			free(root);
			return temp;
		}

		else if (root->right == NULL) {
			treePos temp = root->left;
			free(root);
			return temp;
		}

		treePos temp = findMin(root->right);	// Slučaj 3: dva djeteta
		root->n = temp->n;
		root->right = deleteNode(root->right, temp->n);
	}

	return root;
}

treePos findMin(treePos root) {		//za brisanje kad ima dvoje djece
	if (root == NULL)
		return NULL;

	while (root->left != NULL)
		root = root->left;

	return root;
}

//3.inorder ispis
void inorder(treePos root) {
	if (root == NULL)
		return;
	inorder(root->left);
	printf("%d ", root->n);
	inorder(root->right);
}

//4.postorder ispis
void postorder(treePos root) {
	if (root == NULL)
		return;

	postorder(root->left);
	postorder(root->right);
	printf("%d ", root->n);
}

//5.preorder ispis
void preorder(treePos root) {
	if (root == NULL)
		return;

	printf("%d ", root->n);
	preorder(root->left);
	preorder(root->right);
}

//6. level order
void levelOrder(treePos root) {
	if (root == NULL)
		return;

	treePos queue[100];
	int front = 0, rear = 0;

	queue[rear++] = root;

	while (front < rear) {
		treePos current = queue[front++];
		printf("%d ", current->n);

		if (current->left != NULL)
			queue[rear++] = current->left;

		if (current->right != NULL)
			queue[rear++] = current->right;
	}
}

//7. Traženje nekog elem.
treePos find(treePos root, int value) {

	if (root == NULL)
		return NULL;

	if (value == root->n)
		return root;

	if (value < root->n)
		return find(root->left, value);
	else
		return find(root->right, value);
}

//8. Brisanje stabla
void deleteTree(treePos root) {
	if (root == NULL)
		return;		//za prekid funkc.

	deleteTree(root->left);
	deleteTree(root->right);

	free(root);
}
