#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>>

//TO DO
//atof not working like it should
//food bit hard-coded, want it to find column name from header
//if debit is blank ignore line
//do ternary to get the line columns lined up

char** string_array;
int total_strings=0;
char* shop_names[3] = { "TESCO STORES","ALDI","CO-OPERATIVE" };
/* I don't this does anything anymore. Looks like it was deprecated*/ 
//char* copy_string(char temp[], int temp_size) {
//	char* new_word= malloc(sizeof(char) * temp_size);
//	strcpy(new_word, temp);
//	return new_word;
//}

void blank_array(char temp[]) {
	temp[0] = '\0';
}

int get_column_count(char* file_name) {
	FILE* file = fopen(file_name, "r");
	char c;
	int comma=0;
	while ((c = getc(file)) != '\n') {
			if (c == ',')
				++comma;
	}
	fclose(file);
	return comma+1;
}

int get_row_count(char* file_name) {
	FILE* file = fopen(file_name, "r");
	char c;
	int newline = 0;
	while ((c = getc(file)) != EOF) {
		if (c == '\n') {
			++newline;
		}
	}
	fclose(file);
	return newline;
}

void load_strings(char* file_name) {
	FILE* file;
	file = fopen(file_name, "r");
	if (file == NULL)
		printf("%s\n", "Null pointer");
	fclose(file);

	total_strings = get_row_count(file_name) * get_column_count(file_name);
	int c;
	char temp[100];
	string_array = calloc(total_strings, sizeof(char*));
	short string_counter = 0;
	short character_counter = 0;	//The amount of characters to copy into the heap memory
	file = fopen(file_name, "r");

	while ((c = getc(file)) != EOF) {
		if (c != ',' && c != '\n') {	//Copy character into temp if it isn't a comma or new line
			temp[character_counter++] = c;
		}
		if (c == ',' || c == '\n') {
			temp[character_counter] = '\0';
			string_array[string_counter] = malloc(character_counter);
			strcpy(string_array[string_counter++], temp);
			//clean up and reset
			blank_array(temp);
			character_counter = 0;
		}
	}
	fclose(file);
}

void show_strings() {
	for (int x = 0; x < total_strings; ++x)
		printf("%s\n", string_array[x]);
}

int convert_to_pennies(char* cp) {

	unsigned int total = 0;
	unsigned int power = 10;
	for (char* p = cp; *p != '\0'; p++) {
		if (isdigit(*p)) {
			total *= power;
			total += *p - '0'; //should this be plus?
		}
		else { ;; }
	}
	return total;
}

char* penny_formatter (int pennies) {
	int digits_no = ((log10(pennies)+1)>2) ? log10(pennies) + 1:3;
	int carr_size = digits_no + 2;
	char* carr = calloc(carr_size, sizeof(char));
	int decimal_insert  = (digits_no - 2>0) ? digits_no-2 : 1;
	int power = digits_no;
	for (int i = 0; i < carr_size; ++i) {
		if (i == digits_no + 1)
			*(carr + i) = '\0';
		else if (i == decimal_insert)
			*(carr + i) = '.';
		else {
			int p = pow(10, power-1);
			int num = (pennies / p)%10;
			*(carr + i) = num + '0';
			--power;
		}
	}
	return carr;
}

void show_food_costs() {
	unsigned int total = 0;
	printf("===================================================================\n");
	for (int i = 4; i < total_strings; i += 8) {
		for (int x = 0; x < sizeof(shop_names) / sizeof(shop_names[0]); ++x) {
			//atoi bit is to deal with blanks
			if (strstr(string_array[i], shop_names[x]) != NULL && (atoi(string_array[i + 1])+1!=1)) {
				printf("%s\t%s\t\t%s\n", string_array[i-4],string_array[i], string_array[i + 1]);
				total += convert_to_pennies(string_array[i + 1]);
			}
			else;
		}
	}
	printf("-------------------------------------------------------------------\n");
	printf("Total: \x9C%s\n", penny_formatter(total));
	printf("===================================================================\n");
}

int main(int argc, char* argv[]) {
	//void load_strings(char*);
	//void show_food_costs();
	//void free(void*);
	int x = 20;
	int y = 010;
	printf("%s\n", penny_formatter(567));
	printf("%s\n", penny_formatter(12345));
	printf("%s\n", penny_formatter(001));
	printf("%s\n", penny_formatter(021));
	return 0;
}
