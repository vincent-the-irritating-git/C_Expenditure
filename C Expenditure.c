#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#define STRING_COUNTER_SIZE 10000
#include <stdio.h>
//TO DO
//the while if bit needs rejigging
//the row count isn't working properly, it's -1
//the bank data only works properly because it ends with a newline not eof
//delete the hacky post add, breaking program

char** string_array;
int total_strings=0;
char* shop_names[3] = { "TESCO STORES","ALDI","CO-OPERATIVE" };

char* copy_string(char temp[], int temp_size) {
	char* new_word= malloc(sizeof(char) * temp_size);
	strcpy(new_word, temp);
	return new_word;
}

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
	for (int x = 0; x < total_strings; ++x) {
		printf("%s\n", string_array[x]);
	}
}

int main(int argc, char** argv) {
	load_strings("C:/Users/Wiiiill/Documents/CSVs/Jan23.csv");
	show_strings();
	free(string_array);
	printf("-------------------------------------------------------------------");

	return 0;
}