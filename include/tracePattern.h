#ifndef TRACE_PATTERN
#define TRACE_PATTERN

#include "../include/commonHeader.h"
#include "../include/writeToFile.h"
#include "../include/redisUtil.h"
#include <dlfcn.h>

class TracePattern {
    string shareObject_FileName = "./bin/fsm.so";

    public:
    void loadAndTrace(RequestObject reqObj);
    void loopAndPresentData(string &patternKey, vector<vector<string> > &numberList, string &regexPattern);

};
#endif