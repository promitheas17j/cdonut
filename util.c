#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "util.h"

void log_message(const char* log_level, const char* msg, const char* type, ...){
	FILE *fptr;

	va_list args;
	va_start(args, type);

	fptr = fopen("log.txt", "a");

	// Log the message and log level first
	fprintf(fptr, "\n[%s]: %s", log_level, msg);

	// Check if there is a third parameter, and print it based on its type
	if (type != NULL){
		if (strcmp(type, "int") == 0){
			int int_val = va_arg(args, int);
			fprintf(fptr, ": %d", int_val);
		}
		else if (strcmp(type, "float") == 0){
			double float_val = va_arg(args, double);
			fprintf(fptr, ": %f", float_val);
		}
		else if (strcmp(type, "string") == 0){
			char* str_val = va_arg(args, char*);
			fprintf(fptr, ": %s", str_val);
		}
		else if (strcmp(type, "char") == 0){
			char char_val = va_arg(args, int);
			fprintf(fptr, ": %c", (char)char_val);
		}
		else{
			fprintf(fptr, "\nUnknown type. Maybe add it to the logging function?\n");
		}
	}
	fclose(fptr);
}

