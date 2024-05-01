
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

struct DatTokened{
	uint64_t length;
	TOKTYPE *token[];
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
	int lastIndex = 0;
	int cp;
	int splitPos = 0;
	for(int pos = 0;pos < str->length;pos++){
		arg = str->string[pos];
		subStringLength++;
		if(arg != '\n')continue;
		if(nl){
			splited->length = subStringLength;
			splited->part[partPos].string = malloc(sizeof(char) * splited->length);
			splited->part[partPos].length = splited->length;
			for(cp = 0;cp < subStringLength;cp++){
				splited->part[partPos].string[cp] = str->string[lastIndex + cp];
			}
			splited->part[partPos].string[cp++] = 'H';
			splited->part[partPos].string[cp++] = 'u';
			splited->part[partPos].string[cp++] = 'm';
			splited->part[partPos].string[cp++] = 'a';
			splited->part[partPos].string[cp++] = 'n';
			splited->part[partPos].string[cp++] = ':';
			splited->part[partPos].string[cp++] = ' ';
			lastIndex = pos;
			subStringLength = 7;
			partPos++;
			nl = false;
		}else
			nl = true;
	}

	return splited;
}

struct DatTokened *datBigTokens(struct TokTokenDB *list,struct DatTrain *str){
	// this will also free!
	struct DatTokened *tokens = malloc(sizeof(struct DatTokened) + sizeof(TOKTYPE*) * str->length);
	struct TokStrString *carryingMe;
	tokens->length = str->length;
	for(int pos = 0;pos < tokens->length;pos++){
		carryingMe = malloc(sizeof(struct TokStrString) + sizeof(char) * str->part[pos].length);
		carryingMe->length = str->part[pos].length;
		for(int overStr = 0;overStr < carryingMe->length;overStr++){
			carryingMe->string[overStr] = str->part[pos].string[overStr];
		}
		tokens->token[pos] = tok2Tokens(list,carryingMe);
		//tokens->token[pos] = tok2Tokens(list,(str->part[pos]));
		free(&(str->part[pos]));
		free(carryingMe);
	}
	free(str);
	return tokens;
}

struct NetVector *datVecFromToken(uint32_t token,uint32_t size){
	struct NetVector *vec = calloc(sizeof(CALC_ACC) * (size + 256),sizeof(char));
	vec->size = size + 256;
	if(token < 0)
		vec->data[-token - 1] = 1.0F;
	else
		vec->data[token] = 1.0F;
	return vec;
}


#endif
