/*4. Napisati program za zbrajanje i množenje polinoma. Koeficijenti i eksponenti se
čitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINE_MAX 1024
#define SUCCESS 0
#define ERROR_FILE_OPEN -1
#define ERROR_MALLOC -2


typedef struct _Poly* Position;
typedef struct _Poly {
	int num;
	int exp;
	struct _Poly* next; //pokazivač na sljedeći element
}Poly;

int insertSorted(Position* head, int num, int exp);
int readPolyfromLine(const char* row, Position* head);
void printPoly(Position head);
void freePoly(Position* head);
Position addPoly(Position P1, Position P2);
Position multiplyPoly(Position P1, Position P2);

int main() {
	FILE* fp = fopen("polinomi.txt", "r");

	if (!fp) {
		return EXIT_SUCCESS;
	}

	char buffer[LINE_MAX] = { 0 };
	Position P1 = NULL, P2 = NULL;
	if (fgets(buffer, LINE_MAX, fp) != NULL) {

		if (readPolyfromLine(buffer, &P1) != SUCCESS) {
			printf("Greska pri dodavanju prvog polinoma.\n");
			freePoly(&P1);
			fclose(fp);
			return ERROR_MALLOC;
		}
	}

	if (fgets(buffer, LINE_MAX, fp) != NULL) {
		if (readPolyfromLine(buffer, &P2) != SUCCESS) {
			printf("Greska pri dodavanju drugog polinoma.\n");
			freePoly(&P2);
			fclose(fp);
			return ERROR_MALLOC;
		}
	}
	fclose(fp);

	printf("Prvi polinom: ");
	printPoly(P1);
	printf("Drugi polinom: ");
	printPoly(P2);
	Position suma = addPoly(P1, P2);
	printf("Zbroj: ");
	printPoly(suma);

	Position produkt = multiplyPoly(P1, P2);
	printf("Umnožak: ");
	printPoly(produkt);

	freePoly(&P1);
	freePoly(&P2);
	freePoly(&suma);
	freePoly(&produkt);

	return EXIT_SUCCESS;
}

int insertSorted(Position* head, int num, int exp) {

	if (num == 0)  // ako nema brojeva, ne dodaj ništa u listu
		return SUCCESS;

	Position prev = NULL;
	Position temp = *head;

	// sortirano po opadajućem eksponentu
	while (temp != NULL && temp->exp > exp) {
		prev = temp;
		temp = temp->next;
	}

	// ako postoji isti eksponent da zbroji brojeve
	if (temp != NULL && temp->exp == exp) {
		temp->num += num;

		// ako se ponište, obriši te elemente
		if (temp->num == 0) {
			if (prev)
				prev->next = temp->next;
			else
				*head = temp->next;

			free(temp);
		}
		return SUCCESS;
	}

	// stvori novi element
	Position newEl = (Position)malloc(sizeof(Poly));
	if (!newEl)
		return ERROR_MALLOC;

	newEl->num = num;
	newEl->exp = exp;
	newEl->next = temp;

	if (prev)
		prev->next = newEl;
	else
		*head = newEl;

	return SUCCESS;
}

int readPolyfromLine(const char* row, Position* head) {
	int num = 0, exp = 0;
	const char* temp = row;		//trenutna pozicija u retku

	while (1) {
		while (*temp == ' ' || *temp == '\t')   // preskoči praznine
			temp++;

		if (*temp == '\0' || *temp == '\n')     // ako je kraj stringa, prekid
			break;

		int readCount = sscanf(temp, "%d %d", &num, &exp);     // pokušaj pročitati dva broja
		if (readCount != 2)
			break;

		insertSorted(head, num, exp);       // umetni pročitani član u sortiranu listu

		while (*temp != ' ' && *temp != '\t' && *temp != '\0' && *temp != '\n') // preskoči prvi broj
			temp++;

		while (*temp == ' ' || *temp == '\t')      // preskoči razmake
			temp++;

		while (*temp != ' ' && *temp != '\t' && *temp != '\0' && *temp != '\n') // preskoči drugi broj
			temp++;
	}

	return SUCCESS;
}

void printPoly(Position head) {
	if (head == NULL) {
		printf("Lista je prazna\n");
		return;
	}

	while (head != NULL) {
		printf("%d", head->num);	// ispis broja

		if (head->exp == 1)		// ispis eksponenta
			printf("x");
		else if (head->exp > 1)
			printf("x^%d", head->exp);

		if (head->next != NULL) {		// ako sljedeći član nije negativan, ispiši +
			if (head->next->num >= 0)
				printf(" + ");
			else
				printf(" ");
		}

		head = head->next;
	}

	printf("\n");
}

void freePoly(Position* head) {
	Position temp = NULL;
	Position q = *head;		//pokazuje na prvi čvor liste

	while (q != NULL) {
		temp = q;          //trenutni čvor za brisanje
		q = q->next;       //idi na sljedeći
		temp->next = NULL; //prekini vezu
		free(temp);        //oslobodi memoriju čvora
	}

	*head = NULL;			//početak liste sada ne pokazuje nigdje
}

Position addPoly(Position P1, Position P2) {

	Poly head = { .num = 0, .exp = 0, .next = NULL };		// dummy head
	Position result = &head;								// početak treće liste

	while (P1 != NULL && P2 != NULL) {
		Position newEl = (Position)malloc(sizeof(Poly));
		if (!newEl) return NULL;					// provjera alokacije

		if (P1->exp == P2->exp) {			// isti eksponenti
			newEl->exp = P1->exp;
			newEl->num = P1->num + P2->num;

			P1 = P1->next;
			P2 = P2->next;
		}

		else if (P1->exp > P2->exp) {	// veći eksponent iz P1
			newEl->exp = P1->exp;
			newEl->num = P1->num;
			P1 = P1->next;
		}

		else {							// veći eksponent iz P2
			newEl->exp = P2->exp;
			newEl->num = P2->num;
			P2 = P2->next;
		}

		// ako je rezultat nula, preskoči ga
		if (newEl->num == 0) {
			free(newEl);
			continue;
		}

		newEl->next = NULL;
		result->next = newEl;
		result = newEl;
	}

	// ako je ostalo elemenata u jednom polinomu, kopiraj ih
	Position remaining = (P1 != NULL) ? P1 : P2;

	while (remaining != NULL) {
		Position newEl = (Position)malloc(sizeof(Poly));
		if (!newEl) return NULL;

		newEl->num = remaining->num;
		newEl->exp = remaining->exp;
		newEl->next = NULL;

		result->next = newEl;
		result = newEl;

		remaining = remaining->next;
	}

	return head.next; // vraća stvarni početak treće liste
}

Position multiplyPoly(Position P1, Position P2) {
	if (P1 == NULL || P2 == NULL)
		return NULL;

	Position result = NULL;									// rezultat (početno prazan)

	for (Position i = P1; i != NULL; i = i->next) {			// svaki član prvog puta svaki član drugog
		for (Position j = P2; j != NULL; j = j->next) {
			int newNum = i->num * j->num;					 // novi koeficijent
			int newExp = i->exp + j->exp;					 // novi eksponent

			// dodaj taj član u rezultat, sortirano
			insertSorted(&result, newNum, newExp);
		}
	}

	return result;
}

