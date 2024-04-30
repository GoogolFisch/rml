
#ifndef TOKENS_H_
#define TOKENS_H_

#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>

#define TOKTYPE int32_t

struct TokString{
	char length;
	char* string;
};
struct TokStrString{
	uint32_t length;
	char string[];
};

struct TokTokenDB{
	uint32_t maxLength;
	uint32_t length;
	struct TokString merged[]; // ending with \0
};

struct TokTokenDB *tokReadList(char *file){
	FILE *fptr = fopen(file,"r");
	if(fptr == NULL)return NULL;
	uint32_t currChar;
	uint32_t size = (currChar = getc(fptr));
	size += (currChar = getc(fptr)) << 8;
	size += (currChar = getc(fptr)) << 16;
	struct TokTokenDB *lists = malloc(sizeof(struct TokTokenDB) + sizeof(struct TokString) * size);
	lists->length = size;
	lists->maxLength = size;
	int strSize;
	for(int overing = 0;overing < size;overing++){
		strSize = getc(fptr);
		lists->merged[overing].string = malloc(sizeof(char) * strSize);
		lists->merged[overing].length = strSize;
		for(int strOver = 0;strOver < strSize;strOver++){
			lists->merged[overing].string[strOver] = getc(fptr);
		}
	}
	fclose(fptr);
	return lists;
}

void tokWriteList(char *file,struct TokTokenDB *lists){
	FILE *fptr = fopen(file,"w");
	if(fptr == NULL) return;
	putc(0xff & lists->length,fptr);
	putc(0xff & (lists->length >> 8),fptr);
	putc(0xff & (lists->length >> 16),fptr);
	char strSize;
	for(int overing = 0;overing < lists->length;overing++){
		strSize = lists->merged[overing].length;
		putc(strSize,fptr);
		for(int strOver = 0;strOver < strSize;strOver++){
			putc(lists->merged[overing].string[strOver],fptr);
		}
	}
}

void tokListSizeing(struct TokTokenDB **lists,int total){
	struct TokTokenDB *past = *lists;
	if(total < 0)
		total = past->length - total;
	if(total < past->length)
		total = past->length;
	struct TokTokenDB *future = malloc(sizeof(struct TokTokenDB) + sizeof(struct TokString) * total);
	future->length = past->length;
	future->maxLength = total;
	*lists = future;
	int mergeSize = past->length;
	for(int over = 0;over < mergeSize;over++){
		// copy all of the old data!
		future->merged[over].string = past->merged[over].string;
		future->merged[over].length = past->merged[over].length;
	}
	// swap so nothing breaks!
	free(past);
}

uint32_t *tok2Tokens(struct TokTokenDB *list,struct TokStrString *string){
	int32_t stringPos;
	int32_t mergingPos;
	int32_t checkPos;
	int32_t stringLength;
	int32_t count = 0;
	char *stringStart;
	for(stringPos = 0;stringPos < string->length;stringPos++){
		count++;
		for(mergingPos = list->length - 1;mergingPos >= 0;mergingPos--){
			stringStart = (list->merged[mergingPos].string);
			stringLength = list->merged[mergingPos].length;
			for(checkPos = 0;checkPos < stringLength && checkPos + stringPos < string->length;checkPos++){
				if(string->string[stringPos + checkPos] != stringStart[checkPos])
					goto tok2Tok_strinSkip;
			}
			stringPos += checkPos - 1;
			goto tok2Tok_stringEnding;
tok2Tok_strinSkip:
		}
tok2Tok_stringEnding:
	}
	int32_t *tokenListing = malloc(sizeof(int32_t) * (count + 2));
	tokenListing[0] = count;
	tokenListing[count + 1] = 0;
	count = 0;
	for(stringPos = 0;stringPos < string->length;stringPos++){
		count++;
		tokenListing[count] = -string->string[stringPos] - 1;
		for(mergingPos = list->length - 1;mergingPos >= 0;mergingPos--){
			stringStart = (list->merged[mergingPos].string);
			stringLength = list->merged[mergingPos].length;
			for(checkPos = 0;checkPos < stringLength && checkPos + stringPos < string->length;checkPos++){
				if(string->string[stringPos + checkPos] != stringStart[checkPos])
					goto tok2Tok_strinSkip2;
			}
			stringPos += checkPos - 1;
			tokenListing[count] = mergingPos + 1;
			goto tok2Tok_stringEnding2;
tok2Tok_strinSkip2:
		}
tok2Tok_stringEnding2:
	}
	return tokenListing;
}
struct TokStrString *tok2Char(struct TokTokenDB *list,int32_t *tokens){
	int32_t size = tokens[0];
	int32_t *looking = tokens + 1;
	int32_t count = 0;
	int32_t tokenPos;
	int32_t token;

	for(tokenPos = 0;tokenPos < size;tokenPos++){
		token = looking[tokenPos];
		if(token < 0)count++;
		else if(token > 0)
			count += list->merged[token - 1].length;
	}
	struct TokStrString *string = malloc(sizeof(struct TokStrString) + sizeof(char) * (count) + 1);
	int32_t stringPos = 0;
	for(tokenPos = 0;tokenPos < size;tokenPos++){
		token = looking[tokenPos];
		if(token < 0){
			string->string[count++] = -token - 1;
		}
		else if(token > 0){
			for(int cop = 0;cop < list->merged[token - 1].length;cop++){
				string->string[count++] = list->merged[token - 1].string[cop];
			}
		}
	}
	return string;
}

void tokTrain(struct TokTokenDB *list,struct TokStrString *string){
	if(list->length >= list->maxLength)return;
	int32_t listLength = list->length + 256;
	// this is a pain!!!
	uint32_t *basicCounter = calloc(sizeof(uint32_t) * listLength * listLength,sizeof(uint32_t));
	int32_t *asTokens = tok2Tokens(list,string);
	int32_t size = asTokens[0];
	int32_t *onlyToken = asTokens + 1;
	int32_t past, newer;
	if(asTokens[0] > 0){
		past = onlyToken[0];
		if(past < 0) past = -past - 1;
		else past+=255; // there is no null
	}
	for(uint32_t over = 1;over < asTokens[0];over++){
		newer = onlyToken[over];
		if(newer < 0) newer = -newer - 1;
		else newer+=255;
		basicCounter[past + newer * listLength]++;
		past = newer;
	}
	// get highest value
	uint32_t bestIndex = 0;
	uint32_t bestValue = 0;
	for(int32_t over = listLength * listLength - 1;over >= 0;over--){
		if(bestValue < basicCounter[over]){
			bestValue = basicCounter[over];
			bestIndex = over;
		}
	}
	// do somestuff?!? TODO
	int32_t lowerPart = bestIndex % listLength;
	int32_t upperPart = bestIndex / listLength;
	char totalLength = 0;
	char lowerLength = 1;
	if(lowerPart >= 256){
		lowerPart -= 256;
		lowerLength = list->merged[lowerPart].length;
		totalLength += lowerLength;
	}
	else{lowerPart = -lowerPart - 1;totalLength++;}
	if(upperPart >= 256){
		upperPart -= 256;
		totalLength += list->merged[upperPart].length;
	}
	else{upperPart = -upperPart - 1;totalLength++;}
	listLength -= 256;
	list->merged[listLength].string = malloc(sizeof(char) * totalLength);
	list->merged[listLength].length = totalLength;
	int16_t over = 0;
	if(lowerPart >= 0)
		for(over = 0;over < lowerLength;over++)
			list->merged[listLength].string[over] = list->merged[lowerPart].string[over];
	else list->merged[listLength].string[over++] = -lowerPart - 1;
	if(upperPart >= 0)
		for(int16_t later = 0;over < totalLength;later++)
			list->merged[listLength].string[over++] = list->merged[upperPart].string[later];
	else list->merged[listLength].string[over++] = -upperPart - 1;
	list->length++;
	free(asTokens);
	free(basicCounter);
}
struct TokStrString *tokStringFile(struct TokTokenDB *list,char *fileName){
	FILE *fptr = fopen(fileName,"r");
	if(fptr == NULL)return NULL;
	// get length of file and save it
	fseek(fptr,0L,SEEK_END);
	int32_t size = ftell(fptr);
	fseek(fptr,0L,SEEK_SET);
	// the fun part
	struct TokStrString *fileString = malloc(sizeof(struct TokStrString) + sizeof(char) * size);
	fileString->length = size;
	fread(fileString->string,sizeof(char),size,fptr);

	fclose(fptr);
	return fileString;
}

#endif
