#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#define STRING_COUNTER_SIZE 10000
#include <stdio.h>

int main(int argc, char** argv) {
	FILE* file;
	file = fopen("C:/Users/Wiiiill/Documents/CSVs/Oscar.csv", "r");
	if (file == NULL)
		printf("%s\n", "Null pointer");
	char c;
	char temp[50];
	char* string_array[STRING_COUNTER_SIZE];
	short string_counter = 0;
	short character_counter = 0;
	while ((c = getc(file))!=EOF) {
		if (c != ',') {
			temp[character_counter++] = c;
		}
		if (c == ','||c=='\n') {
			temp[character_counter] = '\0';
			char* new_string = temp;
			string_array[string_counter] = new_string;
			printf("%d: %s\n", string_counter,string_array[string_counter++]);
			character_counter = 0;
		}
	}
	//commit the last string reached before EOF was reached
		temp[character_counter] = '\0';
		string_array[string_counter] = temp;
		printf("%d: %s\n", string_counter, string_array[string_counter]);
		++string_counter;
		character_counter = 0;

	//Problem is string_array has garbage values in it
	//need to make it only as big as necessary
	for (int x = 0; x < STRING_COUNTER_SIZE; ++x) {
		printf("%s,\n", string_array[x]);
		}
	return 0;
}