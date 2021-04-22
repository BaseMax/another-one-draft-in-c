/*
 * Copyright (c) 2010-2020 One Software Foundation
 * Made with love by Max Base, and other contributors especially Kirbits Valentin
 *
 * This file is part of one, which is MIT licensed.
 * See http://opensource.org/licenses/MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "../inc/scanner.h"
// static char* readFile(const char* path);


static char* readFile(const char* path) {
	FILE* file = fopen(path, "rb");
	if(file == NULL)
		return NULL;

	// Find out how big the file is.
	fseek(file, 0L, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	// Allocate a buffer for it.
	char* buffer = (char*)malloc(fileSize + 1);
	if(buffer == NULL) {
		fprintf(stderr, "Could not read file \"%s\".\n", path);
		exit(74);
	}

	// Read the entire file.
	size_t bytesRead = fread(buffer, 1, fileSize, file);
	if(bytesRead < fileSize) {
		fprintf(stderr, "Could not read file \"%s\".\n", path);
		exit(74);
	}

	// Terminate the string.
	buffer[bytesRead] = '\0';

	fclose(file);
	return buffer;
}

typedef enum {
	TOKEN_IF,
	TOKEN_WHILE,
	TOKEN_IDENT,
	TOKEN_OPERATOR,
	TOKEN_NUMBER,
} TOKEN_TYPE;
typedef enum {
	OPERATOR_PARENT_LEFT,
	OPERATOR_PARENT_RIGHT,

	OPERATOR_BRACKET_LEFT,
	OPERATOR_BRACKET_RIGHT,

	OPERATOR_SECTION_LEFT,
	OPERATOR_SECTION_RIGHT,

	OPERATOR_SEMICOLON,
	OPERATOR_VIRGOOL,
	OPERATOR_DOT,
	OPERATOR_TWODOT,

	OPERATOR_ADD,
	OPERATOR_MIN,
	OPERATOR_MUL,
	OPERATOR_DIV,
	OPERATOR_POW,
} OPERATOR_TYPE;
char *source;
typedef struct {
	TOKEN_TYPE type;
	OPERATOR_TYPE op;
	char *value;
} TOKEN;
TOKEN token;

typedef enum {
	false,
	true,
} bool;

bool isOp() {
	if(
		*source == '(' ||
		*source == ')' ||
		*source == '{' ||
		*source == '}' ||
		*source == '[' ||
		*source == ']' ||
		*source == ';' ||
		*source == ',' ||
		*source == '.' ||
		*source == ':' ||
		*source == '+' ||
		*source == '-' ||
		*source == '*' ||
		*source == '/') {
		return true;
	}
	return false;
}
bool isOperator(OPERATOR_TYPE op) {
	return (
		token.type == TOKEN_OPERATOR &&
		op == token.op
	);
}
void setOp() {
	if(*source == '(') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_PARENT_LEFT;
	}
	else if(*source == ')') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_PARENT_RIGHT;
	}
	else if(*source == '{') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_SECTION_LEFT;
	}
	else if(*source == '}') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_SECTION_RIGHT;
	}
	else if(*source == '[') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_BRACKET_LEFT;
	}
	else if(*source == ']') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_BRACKET_RIGHT;
	}
	else if(*source == ';') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_SEMICOLON;
	}
	else if(*source == ',') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_VIRGOOL;
	}
	else if(*source == '.') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_DOT;
	}
	else if(*source == ':') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_TWODOT;
	}
	else if(*source == '+') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_ADD;
	}
	else if(*source == '-') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_MIN;
	}
	else if(*source == '*') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_MUL;
	}
	else if(*source == '/') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_DIV;
	}
	else if(*source == '^') {
		token.type=TOKEN_OPERATOR;
		token.op=OPERATOR_POW;
	}
}

char * tokenType() {
	switch(token.type) {
		case TOKEN_OPERATOR:
			return "operator";
		break;
		case TOKEN_IDENT:
			return "ident";
		break;
		case TOKEN_NUMBER:
			return "number";
		break;
		case TOKEN_WHILE:
			return "while";
		break;
		case TOKEN_IF:
			return "if";
		break;
	}
}
char* initString(char* parent, char* append) {
	parent=malloc(sizeof(char)*100);
	strcpy(parent, append);
	return parent;
}
char* initChar(char* parent, char append) {
	parent=malloc(sizeof(char)*100);
	// char *appends=malloc(sizeof(char)*10);
	char appends[1]={append};
	strcpy(parent, appends);
	return parent;
}
char* appendString(char* parent, char* append) {
	// parent=(char*)realloc(parent, sizeof(char)*(100+100));
	strcat(parent, append);
	return parent;
}
char* appendChar(char* parent, char append) {
	// parent=(char*)realloc(parent, sizeof(char)*(100+100));
	// char *appends=malloc(sizeof(char)*10);
	char appends[1]={append};
	strcat(parent, appends);
	return parent;
}
bool isTrim() {
	return *source == ' ' || *source == '\t' || *source == '\n';
}
bool isIdent() {

}
void getToken() {
	if(isTrim()) {
		return;
	}
	else if(isOp()) {
		setOp();
	}
	else if(*source >= '0' && *source <= '9') {
		token.type=TOKEN_NUMBER;
		token.value="110";
	}
	else {
		token.type=TOKEN_IDENT;
		// token.value="ident";
		token.value=initChar(token.value, *source);
		*source++;
		// while(isIdent()) {
		while(!isOp() && !isTrim()) {
			token.value=appendChar(token.value, *source);
			*source++;
		}
		*source--;
	}
	if(token.type == TOKEN_IDENT) {
		printf(" - %s(%s)\n", token.value, tokenType());
	}
	else {
		printf(" - %c(%s)\n", *source, tokenType());
	}
}
void nextToken() {
	*source++;
}
void nextNextToken() {
	*source++;
	*source++;
}
void prevPrevToken() {
	*source--;
	*source--;
}
void parser() {
	// function define
	getToken();
	// if(token.type == TOKEN_IDENT) {
	// 	char* functionName=token.value;
	// 	printf("Define %s function!\n", functionName);
	// 	getToken();
	// 	if(isOperator(OPERATOR_BRACKET_LEFT)) {
	// 		// SKIP ARGUMENTS
	// 		getToken();
	// 		if(isOperator(OPERATOR_BRACKET_RIGHT)) {
	// 			getToken();
	// 		}
	// 		else {
	// 			printf("Error 1!\n");
	// 		}
	// 	}
	// 	else {
	// 		printf("Error 2!\n");
	// 	}
	// }
	// else {
	// 	printf("Error 3!\n");
	// }
	// getToken();
}
void tokenizer() {
	for(;*source;*source++) {
		// getToken();
		parser();
	}
}
int main(int argc, char const *argv[]) {
	if(argc == 1) {
		puts("Error: run ./one filename.one");
		return -1;
	}
	source=readFile(argv[1]);
	// puts(source);
	tokenizer();
	// parser();
	return 0;
}
