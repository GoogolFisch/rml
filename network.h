
#ifndef NETWORK_H_
#define NETWORK_H_

#include<stdlib.h>
#include<stdint.h>
#include"tokens.h"

#define CALC_ACC float

struct NetNetwork{
	uint8_t depth;
	uint16_t *size;
	// x is input | y is output
	CALC_ACC **layer;
};

struct NetVector{
	int size;
	CALC_ACC data[];
};

union netRandomSame{
	uint64_t lng;
       	double dlb;
};
union netRandomSame seed;


// create from file
struct NetNetwork *netFromFile(char *chptr){
	// the usual
	FILE *fptr = fopen(chptr,"rb");
	if(fptr == NULL){
		fprintf(stderr,"The File expected is not real! Use a better one!\n");
		return NULL;
	}
	// it is easyer
	struct NetNetwork *neu;
       	neu = malloc(sizeof(neu));
	neu->depth = getc(fptr);
	// a clear and a check
	const int maxima = 1 + (int) neu->depth;
	neu->size = calloc(sizeof(*(neu->size)) * maxima,0);
	if(neu->size == NULL){
		fprintf(stderr,"I used a catch just for FUN! It caught!!!\n");
		return 0;
	}
	// load the dimension size!
	unsigned char safty;
	for(int position = 0;position < maxima * 2;position++){
		if((safty = getc(fptr)) == EOF){
			fprintf(stderr,"The File was better, but not big for dimensions?\n");
			return NULL;
		}
		// what? why zeros, and then do this crap? IDK
		neu->size[position >> 1] = safty + neu->size[position >> 1] * 256;
	}
	// OH
	uint64_t matrixSize;
	neu->layer = malloc(sizeof(neu->layer) * (maxima - 1));
	for(int position = 0;position < maxima;position++){
		// injecting a extra value at the end of the input! Dump, but easy
		matrixSize = (neu->size[position] + 1) * neu->size[position + 1];
		neu->layer[position] = malloc(sizeof(*(neu->layer)) * matrixSize);
		fread(neu->layer[position],sizeof(CALC_ACC),matrixSize,fptr);
	}
	fclose(fptr);
	return neu;
}

void netToFile(struct NetNetwork *net,char *chptr){
	FILE *fptr = fopen(chptr,"wb");
	if(fptr == NULL)return;
	fputc(net->depth,fptr);
	const int maxima = (int) net->depth;
	for(int position = 0;position <= maxima;position++){
		fputc(net->size[position] >> 8,fptr);
		fputc(net->size[position] & 255,fptr);
	}
	uint64_t matrixSize, overMatrix;
	for(int position = 0;position < maxima;position++){
		matrixSize = (net->size[position] + 1) * net->size[position + 1];
		fwrite(&(net->layer[position][overMatrix]),sizeof(CALC_ACC),matrixSize,fptr);
	}
	fclose(fptr);
}

struct NetNetwork *netCopyNet(struct NetNetwork *net){
	struct NetNetwork *neu;
	uint64_t matrixSize;
	uint64_t overMatrix;
       	neu = malloc(sizeof(neu));
	neu->depth = net->depth;
	// a clear and a check
	const int maxima = 1 + (int) neu->depth;
	neu->size = malloc(sizeof(*(neu->size)) * maxima);
	neu->layer = malloc(sizeof(CALC_ACC) * neu->depth);
	if(neu->size == NULL){
		fprintf(stderr,"I used a catch just for FUN! It caught!!!\n");
		return 0;
	}
	for(int position = 0;position < maxima;position++){
		neu->size[position] = net->size[position];
		// layer copying!
		matrixSize = (net->size[position] + 1) * net->size[position + 1];
		neu->layer[position] = malloc(sizeof(CALC_ACC) * matrixSize);
		for(overMatrix = 0;overMatrix < matrixSize;overMatrix++){
			neu->layer[position][overMatrix] = net->layer[position][overMatrix];
		}
	}
	return neu;
}

void netClearNet(struct NetNetwork *net){
	const int maxima = 1 + (int) net->depth;
	uint64_t matrixSize,overMatrix;
	for(int position = 0;position < maxima;position++){
		// clearing
		matrixSize = (net->size[position] + 1) * net->size[position + 1];
		for(overMatrix = 0;overMatrix < matrixSize;overMatrix++){
			net->layer[position][overMatrix] = 0;
		}
	}
}

void netMutateNet(struct NetNetwork *net,int every, CALC_ACC strength){
	const int maxima = 1 + (int) net->depth;
	uint64_t matrixSize, overMatrix;
	for(int position = 0;position < maxima;position++){
		// clearing
		matrixSize = (net->size[position] + 1) * net->size[position + 1];
		for(overMatrix = 0;overMatrix < matrixSize;overMatrix++){
			seed.lng = seed.lng * 7650763 + 10211647;
			if(seed.lng % every == 0){
				seed.lng = seed.lng * 7650763 + 10211647;
				net->layer[position][overMatrix] += seed.dlb * strength;
			}
		}
	}
}

CALC_ACC netValueMod(CALC_ACC value){ return value > 0 ? value : 0.1 * value; }
CALC_ACC netRevValue(CALC_ACC value){ return value > 0 ? 1.0 : 0.1; }

CALC_ACC *netLayerCalcing(CALC_ACC *layer,int sizeX,int sizeY,CALC_ACC *input){
	CALC_ACC *out = calloc(sizeof(CALC_ACC) * sizeY + 1,sizeof(char));
	uint64_t overIndex = 0;
	for(int overY = 0;overY < sizeY;overY++)
		for(int overX = 0;overX < sizeX;overX++)
			out[overY] += input[overX] * layer[overIndex++];
	return out;
}

struct NetVector *netMakeNullVec(int size){
	struct NetVector *out;
	out = calloc(sizeof(out->size) + sizeof(CALC_ACC) * size,sizeof(char));
	out->size = size;
	return out;
}
struct NetVector *netMakeVector(int size,CALC_ACC *data){
	struct NetVector *out;
	out = malloc(sizeof(out->size) + sizeof(CALC_ACC) * size);
	for(int position = 0;position < size;out->data[position] = data[position++]);
	out->size = size;
	return out;
}

struct NetVector *netCollapseNetwork(struct NetNetwork *net,struct NetVector *vec){
	// FLAG FIXME
	// let it burn...
	CALC_ACC *copy = malloc(sizeof(CALC_ACC) * (vec->size + 1));
	for(int position = 0;position < vec->size;copy[position] = vec->data[position++]);
	copy[vec->size] = 1.0;
	CALC_ACC *saveInto;
	for(int position = 0;position < net->depth;position++){
		saveInto = netLayerCalcing(net->layer[position],net->size[position] + 1,net->size[position + 1],copy);
		// am I a good person?
		free(copy);
		copy = saveInto;
		for(int position = 0;position < net->size[position + 1];copy[position] = netValueMod(copy[position++]));
	}
	// if not already buring, a buring should not happen...
	struct NetVector *output = netMakeVector(net->size[net->depth - 1],copy);
	free(copy);
	return output;
}

CALC_ACC *netReverseVector(CALC_ACC *layer,CALC_ACC *erLay,int sizeX,int sizeY,CALC_ACC *input,CALC_ACC *upper){
	CALC_ACC *erros = malloc(sizeof(CALC_ACC) * sizeX);
	int overIndex = 0;
	for(int oY = 0;oY < sizeY;oY++){
		for(int oX = 0;oX < sizeX;oX++,overIndex++){
			erros[oX] += input[oY] * layer[overIndex];
			erLay[overIndex] += input[oY] * upper[oX];
		}
	}
	return erros;
}
struct NetVector *netReverseTraining(struct NetNetwork *net,struct NetNetwork *dif,struct NetVector *vec,struct NetVector *tar,float strength){
	if(net->depth != dif->depth)return NULL;
	// let it burn...
	CALC_ACC *copy = malloc(sizeof(CALC_ACC) * (vec->size + 1));
	for(int position = 0;position < vec->size;copy[position] = vec->data[position++]);
	copy[vec->size] = 1.0;
	CALC_ACC *saveInto;
	CALC_ACC *layerSave[(int)net->depth];
	int matrixOver;
	int matrixMaxima;
	int vectorWidth;

	for(int position = 0;position < net->depth;position++){
		saveInto = netLayerCalcing(net->layer[position],net->size[position] + 1,net->size[position + 1],copy);
		// am I a good person?
		layerSave[position] = copy;
		copy = saveInto;
		for(int position = 0;position < net->size[position + 1];copy[position] = netValueMod(copy[position++]));
	}
	for(int position = 0;position < tar->size && position < net->size[net->depth];copy[position] -= tar->data[position++]);
	for(int position = net->depth - 1;position >= 0;position--){
		saveInto = netReverseVector(net->layer[position],dif->layer[position],net->size[position] + 1,net->size[position + 1],copy,layerSave[position]);
		free(copy);
		copy = saveInto;
	}

	struct NetVector *output = netMakeVector(net->size[net->depth - 1],copy);
	free(copy);
	return output;
}
struct NetVector *netReverseErroring(struct NetNetwork *net,struct NetNetwork *dif,struct NetVector *vec,struct NetVector *err,float strength){
	if(net->depth != dif->depth)return NULL;
	// let it burn...
	CALC_ACC *copy = malloc(sizeof(CALC_ACC) * (vec->size + 1));
	for(int position = 0;position < vec->size;copy[position] = vec->data[position++]);
	copy[vec->size] = 1.0;
	CALC_ACC *saveInto;
	CALC_ACC *layerSave[(int)net->depth];
	int matrixOver;
	int matrixMaxima;
	int vectorWidth;

	for(int position = 0;position < net->depth;position++){
		saveInto = netLayerCalcing(net->layer[position],net->size[position] + 1,net->size[position + 1],copy);
		// am I a good person?
		layerSave[position] = copy;
		copy = saveInto;
		for(int position = 0;position < net->size[position + 1];copy[position] = netValueMod(copy[position++]));
	}
	for(int position = 0;position < err->size;position)copy[position] = err->data[position++];
	for(int position = net->depth - 1;position >= 0;position--){
		saveInto = netReverseVector(net->layer[position],dif->layer[position],net->size[position] + 1,net->size[position + 1],copy,layerSave[position]);
		// TODO why is copy not nice???
		free(copy);
		copy = saveInto;
	}

	struct NetVector *output = netMakeVector(net->size[net->depth - 1],copy);
	free(copy);
	return output;
}

struct NetVector *netJoinVectors(struct NetVector *lower,struct NetVector *upper){
	int64_t totSize = lower->size + upper->size;
	struct NetVector *nvec = malloc(sizeof(struct NetVector) + sizeof(CALC_ACC) * totSize);
	nvec->size = totSize;
	int indexing;
	for(    indexing = 0;indexing < lower->size;indexing++) nvec->data[indexing] = lower->data[indexing];
	for(int position = 0;position < upper->size;position++) nvec->data[indexing++] = lower->data[position];
	return nvec;
}

struct NetVector *netSplitVectors(struct NetVector *base,int starting,int ending){
	// some dump stuff?
	if(ending < 0)ending = base->size + ending + 1;
	if(ending > base->size)ending = base->size;
	if(starting > ending)starting = ending;
	if(starting < 0)starting = 0;
	if(starting > ending)ending = starting;
	int64_t totSize = ending - starting;
	struct NetVector *vec = malloc(sizeof(struct NetVector) + sizeof(CALC_ACC) * totSize);
	vec->size = totSize;
	int index;
	int over = 0;
	for(index = starting;index < ending;index++){
		vec->data[over++] = base->data[index];
	}
	return vec;
}

void difVector(struct NetVector *to, struct NetVector *dif){
	int minimal = to->size;
	if(minimal > dif->size)minimal = dif->size;
	for(int position = 0;position < minimal;position++) to->data[position] -= dif->data[position];
	for(int position = 0;position < to->size;position++) to->data[position] = 0;
}
CALC_ACC difTotalError(struct NetVector *of){
	CALC_ACC size = 0.0;
	for(int pos = 0;pos < of->size;pos++)size += of->data[pos] * of->data[pos];
	return size;
}

void printVector(struct NetVector *vec){
	printf("[");
	if(vec->size != 0) printf("%f",vec->data[0]);
	for(int i = 1;i < vec->size;i++) printf(",%f",vec->data[i]);
	printf("]\n");
}

#endif
