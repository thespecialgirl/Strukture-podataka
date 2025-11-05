/*5. Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa
rezultat. Stog je potrebno realizirati preko vezane liste.
*/
#define _CRT_SECURE_NO_WARNINGS
#define EXIT_SUCCESS 0
#define EXIT_FAIL 1
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


typedef struct _node {
    double number;			//vrijednost u čvoru tj. broj na stogu
    struct _node* next;		//pokazivač na idući čvor u stogu tj. broj
}Node;

typedef Node* Position;	//pokazivac na čvor tj. broj na stogu

int Push(Position* P, double number);		//funkcija za stavljanje broja na stog
int Pop(Position* P, double* number);		//funkcija za skidanje broja sa stoga
int EvaluatePostfix(Position* top);          //funkcija za evaluaciju postfiks izraza iz datoteke


int main() {

    Position top = NULL;	//pokazivač na vrh stoga
    EvaluatePostfix(&top);

    return EXIT_SUCCESS;
}

int Push(Position* P, double number) {

    Position newNode =malloc(sizeof(Node));	//alokacija memorije za novi čvor
    if (!newNode) {
        printf("Greska pri alokaciji memorije!\n");
        return EXIT_FAIL;
    }
    newNode->number = number;		//postavljanje vrijednosti u novi čvor
    newNode->next = *P;			//postavljanje pokazivača novog čvora na trenutni vrh stoga
    *P = newNode;				//postavljanje vrha stoga na novi čvor
    return EXIT_SUCCESS;
}
int Pop(Position* P, double* number) {
    Position temp = NULL;		//pokazivač na privremeni čvor
    if (*P == NULL) {			//provjera da li je stog prazan
        printf("Stog je prazan!\n");
        return EXIT_FAIL;
    }
    *number = (*P)->number;		//dohvaćanje vrijednosti sa vrha stoga
    temp = *P;					//postavljanje privremenog pokazivača na vrh stoga
    *P = (*P)->next;			//postavljanje vrha stoga na idući čvor
    free(temp);					//oslobađanje memorije za privremeni čvor
    return EXIT_SUCCESS;
}

int EvaluatePostfix(Position* top) {
    char buffer[50];        // spremnik za trenutno pročitani element izraza
    double a = 0, b = 0, result = 0;

    printf("Unesi postfix izraz i ctrl+z:\n");

    while (scanf("%s", buffer) == 1) {                  // čitaj dok ima riječi
        // ako počinje brojkom ili minusom ispred broja → broj
        if (isdigit(buffer[0]) || (buffer[0] == '-' && isdigit(buffer[1]))) {
            double value = atof(buffer);   // konverzija iz stringa u double
            Push(top, value);              // ubaci broj na stog
        }
        // ako ima samo jedan znak, pretpostavi da je operator
        else if (strlen(buffer) == 1) {
            char op = buffer[0];

            if (Pop(top, &b) == EXIT_FAIL || Pop(top, &a) == EXIT_FAIL) {
                printf("Greška kod unosa na stog!\n");
                return EXIT_FAIL;
            }

            switch (op) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/':
                if (b == 0) {
                    printf("Greška: dijeljenje s nulom!\n");
                    return EXIT_FAIL;
                }
                result = a / b;
                break;
            default:
                printf("Nepoznat operator: %c\n", op);
                return EXIT_FAIL;
            }

            Push(top, result);  // rezultat stavi natrag na stog
        }
        else {
            printf("Nepoznat unos: %s\n", buffer);
            return EXIT_FAIL;
        }
    }

    if (*top == NULL) {
        printf("Greška: stog je prazan – nema rezultata!\n");
        return EXIT_FAIL;
    }

    if ((*top)->next != NULL) {        // na kraju mora ostati točno jedan broj na stogu
        printf("Greška: višak brojeva na stogu!\n");
        return EXIT_FAIL;
    }

    printf("Rezultat = %.2f\n", (*top)->number);
    Position temp = *top;
    free(temp);                   // oslobađanje memorije za zadnji čvor
    *top = NULL;                // postavljanje vrha stoga na NULL
    return EXIT_SUCCESS;
}
