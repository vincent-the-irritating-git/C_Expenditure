#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#define STRING_COUNTER_SIZE 10000
#include <stdio.h>

char* copy_string(char temp[], int temp_size) {
	char* new_word= malloc(sizeof(char) * temp_size);
	strcpy(new_word, temp);
	return new_word;
}

void blank_array(char temp[]) {
	temp[0] = '\0';
}

int get_element_count(char* file_name) {
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

int main(int argc, char** argv) {
	FILE* file;
	char* file_name="C:/Users/Wiiiill/Documents/CSVs/Jan23.csv";

	file = fopen(file_name, "r");
	if (file == NULL)
		printf("%s\n", "Null pointer");
	fclose(file);
	int total_strings = get_row_count(file_name) * get_element_count(file_name);
	char c;
	char temp[100];
	//Don't work
	char** string_array=calloc(total_strings,sizeof(char*));
	short string_counter = 0;
	short character_counter = 0;	//The amount of characters to copy into the heap memory
	file = fopen(file_name, "r");
	while ((c = getc(file))!=EOF) {
		if (c != ','&& c!='\n') {	//Copy character into temp if it isn't a comma or new line
			temp[character_counter++] = c;
		}
		if (c == ','){
			temp[character_counter] = '\0';	
			char* new_string = copy_string(temp,character_counter);
			string_array[string_counter] = malloc(character_counter);
			strcpy(string_array[string_counter++],new_string);
			printf("%d: %s\n", string_counter,string_array[string_counter-1]);
			//clean up and reset
			blank_array(temp);
			character_counter = 0;
		}
	}
	//commit the last string reached before EOF was reached
		temp[character_counter++] = '\0';
		char* new_string = copy_string(temp, character_counter);
		string_array[string_counter++] = new_string;
		printf("%d: %s\n", string_counter, string_array[string_counter-1]);
		++string_counter;
		character_counter = 0;
		fclose(file);
	////Problem is string_array has garbage values in it
	//need to make it only as big as necessary
	for (int x = 0; x < 4; ++x) {
		printf("%s\n", string_array[x]);
		}
	fclose(file);
	return 0;
}