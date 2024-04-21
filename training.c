
#include<stdio.h>
#include"console.h"
#include"network.h"
#include"tokens.h"

int tokenTraining(int argc,char **argv){
	char tokenFile[] = "tokens.tok\0";
	char dataFile[] = "data.txt\0";
	struct TokTokenDB *tokenList = tokReadList(tokenFile);
	int32_t sizeing = 0;
	int indexing = 1;
	while(argv[1][indexing] != 0){
		sizeing *= 10;
		sizeing += argv[1][indexing++] - '0';
	}
	tokListSizeing(&tokenList,sizeing);
	struct TokStrString *string = tokStringFile(tokenList,dataFile);

	while(tokenList->length < tokenList->maxLength){
		tokTrain(tokenList,string);
	}

	tokWriteList(tokenFile,tokenList);

	return 0;
}


int main(int argc,char **argv){
	if(argc < 2){
		printf("[NET-FILE] [TRAIN-FILE] [OPTIONS]\n");
		printf("OPTIONS:\n");
		printf("-o [FILE] > output of the training");
		return 2;
	}
	if(argv[1][0] == 't')
		return tokenTraining(argc,argv);

	// loading
	char *netFile = argv[1];
	char *trainFile = argv[2];
	struct NetNetwork *net = netFromFile(netFile);
	struct NetNetwork *cp = netCopyNet(net);
	netClearNet(cp);
	// doing stuff
	int roundCount = 100;
	while(roundCount-- >= 0){
	//	netReverseTraining(net,cp,,);
	}
	
	// saving
	char *safing;
	if((safing = getAfterArg(argc,argv,"-o")) != NULL){ netFile = safing; }
	netToFile(net,netFile);
	return 0;
}
