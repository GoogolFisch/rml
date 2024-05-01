
#include<stdio.h>
#include"console.h"
#include"network.h"
#include"tokens.h"

int tokenTraining(int argc,char **argv){
	char *tokenFile = getArgAfter(argc,argv,"-t");
	char *dataFile = getArgAfter(argc,argv,"-d");
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
	struct TokStrString *string = tokStringFile(tokenList,dataFile);

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
	char *tokenFile = getArgAfter(argc,argv,"-t");
	char *netFile = getArgAfter(argc,argv,"-n");
	char *trainFile = getArgAfter(argc,argv,"-d");
	if(tokenFile == NULL || netFile == NULL || trainFile == NULL)
	{
		printf("OPTIONS:\n");
		printf("-o [FILE] > output of the training\n");
		printf("-t [FILE] > tokenFile\n");
		printf("-n [FILE] > networkFile\n");
		printf("-d [FILE] > dataFile\n");
		return 2;
	}
	int32_t tokeAmmount = 0;
	struct TokTokenDB *tokenListing = tokReadList(tokenFile);
	struct NetNetwork *net = netFromFile(netFile);
	struct NetNetwork *cp = netCopyNet(net);
	struct DatTokens *trainTok = datBigTokens(tokenListing,datStringing(trainFile));
	netClearNet(cp);
	// doing stuff
	int roundCount = 100;
	int32_t rand;
	uint32_t *takeTokens;
	struct NetVector *ttokenVec;
	struct NetVector **copyVector = netMakeNullVec(tokenListing.length);
	struct NetVector *error,*cerror,*upperError,*lowerError;
	int over;
	while(roundCount-- >= 0){
		error = calloc(sizeof(CALC_ACC) * 0,sizeof(char));
		rand = (rand >> 1) * 19 + 7;
		takeTokens = trainTok->token[rand];
		ttokenVec = malloc(sizeof(struct NetVector*) * takeTokens->length);
		for(over = 0;over < taketokens->length;over++){
			ttokenVec[over] = copyVector;
			// get upper part of error.
			upperError = netSplitVectors(copyVector,tokAmmount,-1);
			// get lower part of error
			lowerError = datVecFromToken(takeTokens[1 + over],tokenListing.length);
			// combine Vectors
			free(copyVector);
			copyVector = netJoinVectors(lowerError,upperError);
			free(uppererror);
			free(lowerError);
			copyVector = netCollapseNetwork(net,copyVector);
		}
		cperror = copyVector;
		for(;over >= 0;over--){
			// get upper part of error.
			upperError = netSplitVectors(cperror,tokAmmount,-1);
			// get lower part of error
			lowerError = datVecFromToken(takeTokens[1 + over],tokenListing.length);
			difVector(lowerError,ttokenVec[over]);

			// combine Vectors
			free(cperror);
			error = netJoinVectors(lowerError,upperError);
			free(uppererror);
			free(lowerError);
			// the error calc
			cperror = netReverseErroring(net,cp,ttokenVec[over],error,0.1f);
			free(error);
			error = cperror;
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
