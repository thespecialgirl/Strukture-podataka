#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> //treba se koristiti za malloc i free
#define FILE_NOT_OPENED (-1) //datoteka nije otvorena
#define MAX_LINE 1024 //veličina za redke

//struktura za studenta
typedef struct Student {
    char name[50];
    char surname[50];
    int points;
} Student;

int countLines(char* students);

//2.funkcija za čitanje studenata
Student* readStudent(char* fileName,int numberOfStudents) {

    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("Error!");
        return 0;
    }
   //dinamička alokacija
    Student* students = (Student*)malloc(numberOfStudents * sizeof(Student));
    if (students==NULL) {
        printf("Malloc failed!");
        return NULL; //mora biti NULL!!
    }
    //petlja koja čita redak po redak i sprema ih u svaki elem. niza students
    for (int i = 0; i < numberOfStudents; i++) {
        //stavljeno %49s da ne dođe do buffer overflowa
        fscanf(fp,"%49s %49s %d",students[i].name, students[i].surname, &students[i].points);
    }
    fclose(fp);
    return students;

}


int main() {
    char fileName[] = "students.txt"; //pošto u funkcijama kod fopena ne stavljam naziv nego samo fileName

    int numberOfStudents = countLines("students.txt");

    //provjera je li 1. funkcija radi
    if (numberOfStudents==FILE_NOT_OPENED) {
        return -1;
    }
    printf("Number of students in file is: %d \n", numberOfStudents);
    Student* students = readStudent("students.txt", numberOfStudents);
    //provjera
    if (students == NULL) {
        return -1;
    }    
    printf("\nList of students:\n");
    for (int i = 0; i < numberOfStudents;i++) {
        printf("%s %s %d\n",students[i].name, students[i].surname, students[i].points);
    }
    
    //tražimo maksimalan br bodova uz pretpostavku da 1. ima najviše
    int maxPoints = students[0].points;
    for (int i = 0; i < numberOfStudents;i++) {
        if (students[i].points > maxPoints) {
            maxPoints = students[i].points;
        }
    } //ispis imena prezimena bodova relativnih bodova i apsolutnih
    printf("Name\tSurname\tPoints\tRelative Points (%%)\n");

    for (int i = 0; i < numberOfStudents; i++) {
        //formula ta računanje relativnih br. bodova
        double relativePoints = (double)students[i].points / maxPoints * 100;
        printf("%s\t%s\t%d\t%.2f %%\n", students[i].name, students[i].surname, students[i].points, relativePoints);
    }
    
    free(students);//mora bit na kraju!!
    return 0;
}

//funkcija koja broji redove
int countLines(char* fileName) {

    FILE *fp=fopen(fileName,"r");

    //provjera je li se datoteka može otvoriti
    if (fp == NULL) {
        printf("Error!");
        return -1;
    }
	
    char buffer[MAX_LINE];//rezerviraj prostor u memoriji za 1 redak
    int counter = 0;//brojač redaka

	//petlja koja čita i broji redke dok ne dođe do kraja datoteke
    while (fgets(buffer, MAX_LINE, fp) != NULL) {
        counter++;
    }
    fclose(fp);
    return counter;
}
