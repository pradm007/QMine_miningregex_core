#ifndef COMMON_HEADER
#define COMMON_HEADER


#ifndef CSVOUTPUT
#define CSVOUTPUT 0
#endif

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef THREADS
#define THREADS 16
#endif

#ifndef DISPLAY_MAP
#define DISPLAY_MAP 0
#endif

#ifndef SERVER_MODE
#define SERVER_MODE 0
#endif

#include <bits/stdc++.h>
#include <fstream>
#include <omp.h>
#include <sw/redis++/redis++.h>
#include <thread>
#include <chrono>

#define maxOf(a, b) ((a) > (b) ? (a) : (b))
#define minOf(a, b) ((a) < (b) ? (a) : (b))
#define abss(a) ((a) < 0 ? -1 * (a) : (a))
typedef long long unsigned lln;
typedef long double lld;

using namespace std;
using namespace sw::redis;


class RequestObject {
    public:
    string requestId, pattern, event_length, status, input_file_path, output_file_path;
    
    RequestObject() {
    }
    
    RequestObject(string requestId, string pattern, string event_length, string status, string input_file_path, string output_file_path) {
        this->requestId = requestId;
        this->pattern = pattern;
        this->event_length = event_length;
        this->status = status;
        this->input_file_path = input_file_path;
        this->output_file_path = output_file_path;
    }
};
#endif