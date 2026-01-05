/*6. Napisati program koji čita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini račun. Na početku svake datoteke je zapisan datum u kojem vremenu je
račun izdat u formatu YYYY-MM-DD. Svaki sljedeći red u datoteci predstavlja artikl u formatu
naziv, količina, cijena. Potrebno je formirati vezanu listu računa sortiranu po datumu. Svaki čvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omogućiti upit kojim će korisnik saznati koliko je novaca sveukupno potrošeno na specifični
artikl u određenom vremenskom razdoblju i u kojoj je količini isti kupljen.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 50
#define FILE_NOT_OPENED -1
#define MALLOC_FAILED -2
#define NOT_DELETED -3


typedef struct Artical {
	char name[MAX_LENGTH];
	int quantity;
	double price;
	struct Artical* next;				//pokazivac na sljedeci artikal u listi
}Artical;

typedef struct Bill {
	char date[MAX_LENGTH];
	Artical* firstArtical;							//pokazivac na prvi artikl racuna
	struct Bill* next;					//pokazivac na sljedeci racun
}Bill;

typedef struct Result {
	int totalQuantity;
	double totalPrice;
}Result;

//deklarcije
int ReadFileOfBills(const char* fileName, Bill** head);			//funkcija za citanje datoteke racuni.txt
Bill* ReadBill(const char*fileName);				//funkcija za citanje racuna
int ReadArticals(FILE* fp, Bill*bill);				//funkcija za citanje artikala u racunu
int InsertSortedArticals(Bill*bill,Artical*newArtical);	//funkcija za sortiranje artikala
int InsertSortedBill(Bill**head,Bill*NewBill);			//funkcija za sortiranje racuna
int DateRange(const char* date,const char* startDate, const char* endDate);	//funkcija za provjeru raspona datuma
int QuantityOfArtical(Bill*bill,const char* articalName);		//funkcija za racunanje kolicine nekog artikla
double MoneyCount(Bill* bill, const char* articalName);			//funkcija za racunanje ukupne cijene za neki artikal
Result TotalResult(Bill* head,const char*articalName,const char* startDate, const char* endDate);	//funkcija za racunanje korisnikova upita
int DeleteArticals(Artical*first);		//funkcije za brisanje
int DeleteBills(Bill**head);

int main() {
	Bill* head = NULL;				//pokazivac na pocetak liste racuna

	/*if (ReadFileOfBills("racuni.txt") == FILE_NOT_OPENED) {
	printf("Greska pri čitanju datoteke racuni.txt!\n");
	return FILE_NOT_OPENED;}-bilo za provjeru ispisa na početku*/

	ReadFileOfBills("racuni.txt",&head);

	char articalName[MAX_LENGTH];
	char startDate[MAX_LENGTH];
	char endDate[MAX_LENGTH];
	printf("Unesi naziv artikla kojeg trazis:");
	scanf("%s",articalName);
	printf("\nUnesi pocetni datum (YYYY-MM-DD):");
	scanf("%s", startDate);
	printf("\nUnesi krajnji datum (YYYY-MM-DD):");
	scanf("%s", endDate);
	Result result;
	result = TotalResult(head,articalName,startDate,endDate);
	printf("\nTrazeni artikal: %s",articalName);
	printf("\nRazdoblje: %s - %s",startDate,endDate);
	printf("\nUkupno potroseno novca: %lf KM \n",result.totalPrice);
	printf("\nUkupno kupljeno artikla: %d \n", result.totalQuantity);

	if (DeleteBills(&head) == NOT_DELETED) {
		printf("Greska pri brisanju memorije!\n");
	}
	else {
		printf("Memorija uspijesno oslobodena!\n");
	}
	head = NULL;
	return 0;
}

//1)funkcija za  citanje datoteke racuni.txt
int ReadFileOfBills(const char*fileName,Bill**head) {
	char buffer[MAX_LENGTH];
	FILE* fp = fopen(fileName,"r");
	if (!fp)
		return FILE_NOT_OPENED;
	while (fgets(buffer,MAX_LENGTH,fp)!=NULL) {
		buffer[strcspn(buffer, "\n")] = 0;					//brise \n
		//printf("Procitan racun: %s\n",buffer); -samo za ispis za provjeru

		Bill* NewBill = ReadBill(buffer);		//za svako ime zove funkciju 2) koja cita taj racun
		if (NewBill!=NULL) {
		//printf("Procitani racun s datumom: %s\n",NewBill->date);	-bilo za provjeru ispisa racuna sa datumom
		//free(NewBill);
			InsertSortedBill(head,NewBill);
		}
	}
	fclose(fp);
	return 0;
}

//2)funkciija za citanje racuna
Bill* ReadBill(const char* fileName) {
	FILE* fp = fopen(fileName, "r");
	if (!fp) {
		return NULL;
	}
	Bill* bill = (Bill*)malloc(sizeof(Bill));
	if (!bill) {
		fclose(fp);
		return NULL;
	}
		fgets(bill->date, MAX_LENGTH, fp);
		bill->date[strcspn(bill->date, "\n")] = 0;
		bill->firstArtical = NULL;
		bill->next = NULL;

		if (ReadArticals(fp, bill) != 0) {		//pozivanje funkcije za citanje artikala
			fclose(fp);
			return 0;
		}

		fclose(fp);
		return bill;
}

//3)funkcija za citanje artikala u racunu
int ReadArticals(FILE* fp, Bill* bill) {
	char buffer[MAX_LENGTH];
	while (fgets(buffer, MAX_LENGTH, fp) != NULL) {
		Artical* newArtical = (Artical*)malloc(sizeof(Artical));
		if (!newArtical)
			return MALLOC_FAILED;
		sscanf(buffer, "%[^,],%d,%lf", newArtical->name, &newArtical->quantity, &newArtical->price);
		InsertSortedArticals(bill,newArtical);
		
	}
	return 0;
}

//4)funkcija za sortiranje artikala po nazivu
int InsertSortedArticals(Bill* bill, Artical* newArtical) {
	Artical* current = bill->firstArtical;
	//1.slucaj:prazna lista ili novi ide ispred prvog
	if (current==NULL || strcmp(newArtical->name,current->name) <0) {
		newArtical->next = current;
		bill->firstArtical = newArtical;
		return 0;
	}
	//2.slucaj:trazi se mjesto nekog artikla
	while (current->next!=NULL && strcmp(newArtical->name, current->next->name)>0) {
		current = current->next;
	}
	newArtical->next = current->next;
	current->next = newArtical;
	return 0;
}

//5)funkcija za sortiranje racuna po datumu
int InsertSortedBill(Bill** head, Bill* NewBill) {
	Bill* current = *head;
	//1.slucaj:prazna lista ili novi ide ispred prvog
	if (current == NULL || strcmp(NewBill->date, current->date) < 0) {
		NewBill->next = current;
		*head=NewBill;
		return 0;
	}
	//2.slucaj:trazi se mjesto nekog racuna
	while (current->next!=NULL && strcmp(NewBill->date, current->next->date)>0) {
		current = current->next;
	}
	NewBill->next = current->next;
	current->next = NewBill;
	return 0;
}

//6) funkc za provjeru je li racun u tom rasponu datuma
int DateRange(const char* date, const char* startDate, const char* endDate) {

	return strcmp(date,startDate)>=0 && strcmp(date, endDate) <= 0;
}

//7)funkcija za racunanje kolicine nekog artikla
int QuantityOfArtical(Bill* bill, const char* articalName) {
	int quantity = 0;
	Artical* current = bill->firstArtical;
	while (current!=NULL) {
		if (strcmp(current->name, articalName) == 0) {
			quantity +=current->quantity;
		}
		current =current->next ;
	}
	return quantity;
}

//8)//funkcija za racunanje ukupne potrošnje za neki artikal
double MoneyCount(Bill* bill, const char* articalName) {
	double money = 0.0;
	Artical* current = bill->firstArtical;
	while (current != NULL) {
		if (strcmp(current->name, articalName) == 0) {
			money += current->quantity * current->price;
		}
		current = current->next;
	}
	return money;
}
//9)funkcija za racunanje upita od korisnika
Result TotalResult(Bill* head, const char* articalName, const char* startDate, const char* endDate) {
	Result result;
	result.totalPrice = 0.0;
	result.totalQuantity = 0;
	Bill* current = head;
	while (current != NULL) {
		if (DateRange(current->date, startDate,endDate)) {
			result.totalPrice += MoneyCount(current,articalName);
			result.totalQuantity += QuantityOfArtical(current, articalName);
		}
		current = current->next;
	}

	return result;
}
//10)funkcije za brisanje za kraj
int DeleteArticals(Artical* first) {
	Artical* temp;
	while (first!=NULL) {
		temp=first;
		first = first->next;
		free(temp);
	}
	return 0;
}
int DeleteBills(Bill** head) {
	Bill* temp;
	if (!head) {
		return NOT_DELETED;
	}
	while(*head!=NULL){
		temp = *head;
		DeleteArticals(temp->firstArtical);
		*head = temp->next;
		free(temp);
	}
	return 0;
}