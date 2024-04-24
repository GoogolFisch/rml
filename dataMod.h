
#ifndef DATA_MOD_H_
#define DATA_MOD_H_

#include"stdlib.h"
#include"stdint.h"
#include"stdbool.h"
#include"network.h"
#include"tokens.h"

struct DatTrain{
	uint64_t length;
	struct TokString part[];
};


struct DatTrain *datStringing(struct TokStrString *str){
	int newLineing = 0;
	bool nl = false;
	char arg;
	for(int pos = 0;pos < str->length;pos++){
		arg = str->string[pos];
		if(arg != '\n')continue;
		if(nl){
			newLineing++;
			nl = false;
		}else
			nl = true;
	}
	struct DatTrain *splited = malloc(sizeof(struct DatTrain) + sizeof(struct TokString) * newLineing);
	int partPos = 0;
	int subStringLength = 7;
	int ĺastIndex = 0;
	int cp;
	for(int pos = 0;pos < str->length;pos++){
		arg = str->string[pos];
		subStringLength++;
		if(arg != '\n')continue;
		if(nl){
			splited->length = subStringLength;
			splited->string = malloc(sizeof(char) * splited->length);
			for(cp = 0;cp < subStringLength;cp++){splited->string[cp] = str->string[lastIndex + cp];}
			splited->string[cp++] = 'H';
			splited->string[cp++] = 'u';
			splited->string[cp++] = 'm';
			splited->string[cp++] = 'a';
			splited->string[cp++] = 'n';
			splited->string[cp++] = ':';
			splited->string[cp++] = ' ';
			lastIndex = pos;
			subStringLength = 7;
			partPos++;
			nl = false;
		}else
			nl = true;
	}

	return splited;
}

#endif
