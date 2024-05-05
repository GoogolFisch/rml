
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
		if(arg != '\n'){
			nl = false;
			continue;
		}
		if(nl){
			newLineing++;
			nl = false;
		}else
			nl = true;
	}
	struct DatTrain *splited = malloc(sizeof(struct DatTrain) + sizeof(struct TokString) * newLineing);
	splited->length = newLineing;
	int partPos = 0;
	int subStringLength = 0;
	int lastIndex = 0;
	int cp;
	int splitPos = 0;
	nl = false;
	for(int pos = 1;pos < str->length;pos++){
		arg = str->string[pos];
		subStringLength++;
		if(arg != '\n'){
			nl = false;
			continue;
		}
		if(nl){
			subStringLength--;
			splited->part[partPos].string = malloc(sizeof(char) * (subStringLength + 7));
			splited->part[partPos].length = subStringLength + 7;
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
			subStringLength = 0;
			partPos++;
			nl = false;
			continue;
		}
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
		free((str->part[pos].string));
		free(carryingMe);
	}
	free(str);
	return tokens;
}

struct NetVector *datVecFromToken(int32_t token,uint32_t size){
	struct NetVector *vec = malloc(sizeof(struct NetVector) + sizeof(CALC_ACC) * (size));
	vec->size = size;
	for(int over = 0;over < vec->size;vec->data[over++] = 0.0F);
	if(token < 0)
		vec->data[-token - 1] = 1.0F;
	else
		vec->data[token + 255] = 1.0F; // gotten via testing
	return vec;
}


#endif
