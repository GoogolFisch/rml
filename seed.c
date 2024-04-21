

#include<stdio.h>
#include"console.h"
#include"network.h"

int main(int argc,char **argv){
	if(containsArg(argc,argv,"-h")){
		printf("[FILE] [OPTIONS]\n");
		return 0;
	}
	//char chrNet = argv[1];
	return 0;
}
