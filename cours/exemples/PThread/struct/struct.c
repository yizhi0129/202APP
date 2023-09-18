#include <stdio.h>

typedef struct {
	double a; /*sizeof(double) = 8*/ 
	int b; /*sizeof(int) = 4*/
	char c; /*sizeof(char) = 1*/
	/*Padding pour aligner sur 8 octets*/
} s_a;

typedef struct {
        double a; /*sizeof(double) = 8*/
        int b; /*sizeof(int) = 4*/
        char c; /*sizeof(char) = 1*/
	char d; /*sizeof(char) = 1*/
        /*Padding pour aligner sur 8 octets*/
} s_b;

typedef struct {
        double a; /*sizeof(double) = 8*/
        int b; /*sizeof(int) = 4*/
        char c; /*sizeof(char) = 1*/
        char d; /*sizeof(char) = 1*/
	/*Padding pour aligner sur 4 octets*/
	int e; /*sizeof(int) = 4*/
        /*Padding pour aligner sur 8 octets*/
} s_c;

typedef struct {
        double a; /*sizeof(double) = 8*/
        int b; /*sizeof(int) = 4*/
	/*Padding pour gérer le faux partage*/
	char dummy[4];


        char c; /*sizeof(char) = 1*/
        char d; /*sizeof(char) = 1*/
        /*Padding pour aligner sur 4 octets*/
        int e; /*sizeof(int) = 4*/
        /*Padding pour aligner sur 8 octets*/
} s_d;


int main(){
	printf("sizeof s_a %lu\n",sizeof(s_a));
	printf("sizeof s_b %lu\n",sizeof(s_b));
	printf("sizeof s_c %lu\n",sizeof(s_c));
	printf("sizeof s_d %lu\n",sizeof(s_d));
	return 0; 
}
