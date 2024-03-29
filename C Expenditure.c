#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define MAX_LENGTH 1000
#define TRUE 1
#define FALSE 0

//TO DO
//food bit hard-coded, want it to find column name from header
//do ternary to get the line columns lined up


struct word_array {
	char** vals;
	short count;
};

const short CLMN_DATE = 0;
const short CLMN_DESCRIPTION = 4;
const short CLMN_DEBIT = 5;
const short ROW_LENGTH = 8;
int total_strings = 0;

char** string_array;

struct word_array filter_words = {
	.vals = NULL,
	.count = 0
};

struct word_array display_words = {
	.vals = NULL,
	.count = 0
};

struct word_array* filt_words = &filter_words;
struct word_array* disp_words = &display_words;

void blank_array(char temp[]) {
	temp[0] = '\0';
}

int get_column_count(char* file_name) {
	FILE* file = fopen(file_name, "r");
	int c;
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
	if (file == NULL) {
		printf("No such file exists!\n- %s -\n",file_name);
		return;
	}

	total_strings = get_row_count(file_name) * get_column_count(file_name);
	int c;
	char temp[MAX_LENGTH];
	string_array = calloc(total_strings, sizeof(char*));
	short string_counter = 0;
	short character_counter = 0;	//The amount of characters to copy into the heap memory

	while ((c = getc(file)) != EOF) {
		if (c != ',' && c != '\n')	//Copy character into temp if it isn't a comma or new line
			temp[character_counter++] = c;

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

unsigned short does_charp_contain_decimal_point(char* cp) {

	unsigned int b = FALSE;

	for (char* p = cp; *p != '\0'; p++)
		if (*p == '.')
			b = TRUE;

	return b;
}

int convert_to_pennies(char* cp) {
	const unsigned int no_decimal_point = 100;
	unsigned int total = 0;
	unsigned int power = 10;
	for (char* p = cp; *p != '\0'; p++) {
		if (isdigit(*p)) {
			total *= power;
			total += *p - '0';
		}
		else { ;; }
	}
	if (!does_charp_contain_decimal_point(cp))
		total *= no_decimal_point;
	return total;
}

	//should copy carr into a stack var and free it
char* penny_formatter(int pennies) {
	int digits_no = ((log10(pennies) + 1) > 2) ? log10(pennies) + 1 : 3;
	int carr_size = digits_no + 2;
	char* carr = calloc(carr_size, sizeof(char));
	int decimal_insert = (digits_no - 2 > 0) ? digits_no - 2 : 1;
	int power = digits_no;
	for (int i = 0; i < carr_size; ++i) {
		if (i == digits_no + 1)
			*(carr + i) = '\0';
		else if (i == decimal_insert)
			*(carr + i) = '.';
		else {
			int p = pow(10, power - 1);
			int num = (pennies / p) % 10;
			*(carr + i) = num + '0';
			--power;
		}
	}
	return carr;
}

void load_filter_struct_with_list_of_filter_words(char* name) {
	FILE* f;
	int c,count=0;
	char buff [100];

	c = count;

	f = fopen(name, "r");

	if (f == NULL) {
		printf("File not found!\n- %s -\n", name);
		return;
	}

	filt_words->count=get_column_count(name);
	filt_words->vals = malloc(sizeof(char*) * filt_words->count);

	while ((c = getc(f)) != EOF)
		if (c != ',' && c!='\n') {
			buff[count] = c;
			++count;
		}
		else {
			buff[count] = '\0';
			*(filt_words->vals) = malloc(sizeof(char) * count);
			strcpy(*(filt_words->vals++), buff);
			count = 0;
			buff[0] = '\0';
		}
	//reset filter pointer
	filt_words->vals = filt_words->vals - filt_words->count;
	fclose(f);
}

void load_costs_array() {
	//char* array, make tot the one at the end
	unsigned int total = 0;
	char* temp[MAX_LENGTH];

	for (int i = CLMN_DESCRIPTION; i < total_strings; i += ROW_LENGTH) {
		//this is where the date, description and spend will be in the array
		//in relation to i
		const short DATE = i + (CLMN_DATE - CLMN_DESCRIPTION); 			//-4
		const short DESC = i + (CLMN_DESCRIPTION - CLMN_DESCRIPTION);	//0
		const short SPEND = i + (CLMN_DEBIT - CLMN_DESCRIPTION);		//1

		for (int x = 0; x < filt_words->count; ++x) {
			//atoi bit is to deal with blanks
			if (strstr(string_array[DESC], filt_words->vals[x]) != NULL && (atoi(string_array[SPEND]) + 1 != 1)) {
				temp[disp_words->count++] = string_array[DATE];
				temp[disp_words->count++] = string_array[DESC];
				temp[disp_words->count++] = string_array[SPEND];

				total += convert_to_pennies(string_array[SPEND]);
			}
			else
				;
		}
	}
	//+1 for the total spend
	disp_words->vals = malloc(sizeof(char*) * ++(disp_words->count));
	for (int x = 0; x < disp_words->count-1; ++x)
		*(disp_words->vals++) = temp[x];

	*(disp_words->vals) = penny_formatter(total);
	disp_words->vals = disp_words->vals - (disp_words->count-1);
}

void save_results_to_text_file(char* filename) {
	FILE* f;
	f = fopen(filename, "w");

	if (f==NULL){
		printf("File not saved!\n- %s -\n",filename);
		return;
	}

	for (int x=0; x< disp_words->count-1;x=x+3)
		fprintf(f, "%s\t%25s\t\t�%6s\n", disp_words->vals[x], disp_words->vals[x+1], disp_words->vals[x+2]);

	fprintf(f, "==========================================================\nTOTAL: �%s\n==========================================================", disp_words->vals[disp_words->count-1]);
}

int main(int argc, char* argv[]) {
	const short CSV_INPUT = 1;
	const short FOOD_OUTPUT = 2;
	const short FILTER_WORDS = 3;
	void load_strings(char*);
	void free(void*);
	void save_food_costs(char*);
	void load_costs_array();
	void load_filter_struct_with_list_of_filter_words(char*);
	void save_results_to_text_file(char*);

	load_strings(argv[CSV_INPUT]);
	load_filter_struct_with_list_of_filter_words(argv[FILTER_WORDS]);
	load_costs_array();
	save_results_to_text_file(argv[FOOD_OUTPUT]);

	free(disp_words->vals);
	free(string_array);
	free(filt_words->vals);

	return 0;
}
