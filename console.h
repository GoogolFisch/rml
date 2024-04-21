
#ifndef CONSOLE_H_
#define CONSOLE_H_

#include<stdbool.h>

bool containsArg(int argc, char **argv,char *pattern) {
        // retrns if there is an element as the pattern in the args
        for (int argPos = 0; argPos < argc; argPos++) {
                char *argString = argv[argPos];
                int matchPos = 0;
                // go over every thing, until there is a nullByte
                while (argv[argPos][matchPos] != '\0' && pattern[matchPos] != '\0') {
                        if (argv[argPos][matchPos] != pattern[matchPos])
                                break;
                        matchPos++;
                        if (argv[argPos][matchPos] == pattern[matchPos] && pattern[matchPos] == '\0')
                                return true;
                }
        }
        return false;
}
char *getAfterArg(int argc, char **argv, char *pattern){
	// get following the current Arg
        argc--;
        for (int argPos = 0;argPos < argc; argPos++){
                char *argString = argv[argPos];
                int matchPos = 0;
                while (argv[argPos][matchPos] != '\0' && pattern[matchPos] != '\0') {
                        if (argv[argPos][matchPos] != pattern[matchPos])
                                break;
                        matchPos++;
                        if (argv[argPos][matchPos] == pattern[matchPos] && pattern[matchPos] == '\0')
                                return argv[argPos + 1];
                }
        }
        return NULL;
}

char *getArgValue(int argc, char **argv, char *pattern){
	// get inside current arg
        for (int argPos = 0;argPos < argc; argPos++){
                char *argString = argv[argPos];
                int matchPos = 0;
                while (argv[argPos][matchPos] != '\0') {
                        if (argv[argPos][matchPos] != pattern[matchPos])
                                break;
                        matchPos++;
                        if (pattern[matchPos] == '\0')
                                return argv[argPos] + matchPos;
                }
        }
        return NULL;
}

#endif
