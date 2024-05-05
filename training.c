
#include<stdio.h>
#include"console.h"
#include"network.h"
#include"tokens.h"
#include"dataMod.h"

int tokenTraining(int argc,char **argv){
	char *tokenFile = getAfterArg(argc,argv,"-t");
	char *dataFile = getAfterArg(argc,argv,"-d");
	if(NULL == dataFile || tokenFile == NULL){
		printf("-t tokenFile\n");
		printf("-d dataFile\n");
	}
	struct TokTokenDB *tokenList = tokReadList(tokenFile);
	int32_t sizeing = 0;
	int indexing = 1;
	while(argv[1][indexing] != 0){
		sizeing *= 10;
		sizeing += argv[1][indexing++] - '0';
	}
	tokListSizeing(&tokenList,sizeing);
	struct TokStrString *string = tokStringFile(dataFile);

	while(tokenList->length < tokenList->maxLength){
		tokTrain(tokenList,string);
	}

	tokWriteList(tokenFile,tokenList);

	return 0;
}


int main(int argc,char **argv){
	if(argc < 2){
		printf("OPTIONS (for network):\n");
		printf("-o [FILE] > output of the training\n");
		printf("-t [FILE] > tokenFile\n");
		printf("-n [FILE] > networkFile\n");
		printf("-d [FILE] > dataFile\n");
		printf("for tokens t00\n");
		printf("00 -> numbers any length...\n");
		printf("> more to come!");
		return 2;
	}
	if(argv[1][0] == 't')
		return tokenTraining(argc,argv);

	// loading
	char *tokenFile = getAfterArg(argc,argv,"-t");
	char *netFile = getAfterArg(argc,argv,"-n");
	char *trainFile = getAfterArg(argc,argv,"-d");
	if(tokenFile == NULL || netFile == NULL || trainFile == NULL)
	{
		printf("OPTIONS:\n");
		printf("-o [FILE] > output of the training\n");
		printf("-t [FILE] > tokenFile\n");
		printf("-n [FILE] > networkFile\n");
		printf("-d [FILE] > dataFile\n");
		return 2;
	}
	struct TokTokenDB *tokenListing = tokReadList(tokenFile);
	struct NetNetwork *net = netFromFile(netFile);
	struct NetNetwork *cp = netCopyNet(net);
	struct TokStrString *trainData = tokStringFile(trainFile);
	struct DatTokened *trainTok = datBigTokens(tokenListing,datStringing(trainData));
	int32_t tokAmmount = tokenListing->length;
	free(trainData);
	netClearNet(cp);
	// doing stuff
	int roundCount = 100;
	int32_t rand;
	int32_t *takeTokens;
	struct NetVector **ttokenVec;
	struct NetVector *copyVector = netMakeNullVec(tokenListing->length);
	struct NetVector *error,*cerror,*upperError,*lowerError;
	int over;
	while(roundCount-- >= 0){
		error = calloc(sizeof(CALC_ACC) * 0,sizeof(char));
		rand = (rand >> 1) * 19 + 7;
		takeTokens = trainTok->token[rand % trainTok->length];
		ttokenVec = malloc(sizeof(struct NetVector*) * takeTokens[0]);
		for(over = 0;over < takeTokens[0];over++){
			ttokenVec[over] = copyVector;
			// get upper part of error.
			upperError = netSplitVectors(copyVector,tokAmmount,-1);
			// get lower part of error
			lowerError = datVecFromToken(takeTokens[1 + over],tokenListing->length);
			// combine Vectors
			copyVector = netJoinVectors(lowerError,upperError);
			free(upperError);
			free(lowerError);
			copyVector = netCollapseNetwork(net,copyVector);
		}
		over--;
		cerror = netMakeNullVec(net->size[net->depth]);
		for(;over >= 0;over--){
			// get upper part of error.
			upperError = netSplitVectors(cerror,tokAmmount,net->size[net->depth]);
			// get lower part of error
			lowerError = datVecFromToken(takeTokens[1 + over],tokenListing->length);
			difVector(lowerError,ttokenVec[over]);

			// combine Vectors
			free(cerror);
			error = netJoinVectors(lowerError,upperError);
			free(upperError);
			free(lowerError);
			// the error calc
			cerror = netReverseErroring(net,cp,ttokenVec[over],error,0.1f);
			free(error);
			error = cerror;
		}

	//	netReverseTraining(net,cp,,);
		free(ttokenVec);
		free(error);
	}
	
	// saving
	char *safing;
	if((safing = getAfterArg(argc,argv,"-o")) != NULL){ netFile = safing; }
	netToFile(net,netFile);
	return 0;
}
