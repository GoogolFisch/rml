
#include<stdio.h>
#include<stdlib.h>
#include"network.h"
#include"tokens.h"
#include"dataMod.h"
#include"console.h"

int main(int argc,char **argv){
	char *types = getAfterArg(argc,argv,"-t");
	char *output = getAfterArg(argc,argv,"-o");
	if(types == NULL)return 1;
	if(output == NULL)return 1;
	int32_t depthCount = 1; // counting ,
	char carry;
	int pos = 0;
	do{
		carry = types[pos++];
		if(carry == ',')depthCount++;
	}while(carry != '\0');
	// making a new nn...
	struct NetNetwork *net = malloc(sizeof(struct NetNetwork));
	net->depth = depthCount - 1;
	net->size = malloc(sizeof(net->size) * depthCount);
	pos = 0;
	net->size[0] = 0;
	int32_t numberPos = 0;
	do{
		carry = types[pos++];
		if(carry == ','){
			numberPos++;
			net->size[numberPos] = 0;
		}
		else if(carry >= '0' && carry <= '9'){
			net->size[numberPos] *= 10;
			net->size[numberPos] += carry - '0';
		}
	}while(carry != '\0');
	net->layer = malloc(sizeof(net->layer) * net->depth);
	union netRandomSame mask; mask.dlb = -1.99999999999999977795539507497D;
	union netRandomSame base; base.dlb = 1.0D;
	union netRandomSame value;
	int64_t randSeed;
	int64_t matrixSize;
	int64_t matrixOver;
	for(int layer = 0;layer < net->depth;layer++){
		printf("- %i -> (%i,%i)\n",layer,net->size[layer],net->size[layer + 1]);
		matrixSize = (net->size[layer] + 1) * net->size[layer + 1];
		net->layer[layer] = malloc(sizeof(CALC_ACC) * matrixSize);
		for(matrixOver = 0;matrixOver < matrixSize;matrixOver++){
			value.dlb = net->layer[layer][matrixOver];
			randSeed = (randSeed >> 1) * 23 + 19;
			value.lng += randSeed;
			value.lng += randSeed;
			value.lng &= mask.lng;
			value.lng |= base.lng;
			net->layer[layer][matrixOver] = value.dlb;
		}
	}
	// now saving it
	netToFile(net,output);

	return 0;
}
