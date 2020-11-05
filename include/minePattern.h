#ifndef MINE_TRACE
#define MINE_TRACE

#include "../include/commonHeader.h"
#include "../include/ragelGenerator.h"
#include "../include/tracePattern.h"
#include <stdio.h>

string pattern = "0M1M2";
int alphabetLength = 3;

void invoke_mineTrace(RequestObject reqObj) {
    // Generate ragel file
    RagelGenerator rg;
    // cout << "reqObj.event_length " << reqObj.event_length << endl;
    rg.generateRagelFile(reqObj.pattern, stoi(reqObj.event_length));

    // Compile ragel file
    if (Util::fexists("./bin/fsm.rl")) {
        double t = omp_get_wtime();
        
        string exec = "ragel -C -T0 ./bin/fsm.rl -o ./bin/fsm.cpp && g++ -std=c++11 -fopenmp -Ofast -fpic -w -g -shared -D THREADS=" + to_string(THREADS) + " -D DISPLAY_MAP=" + to_string(DISPLAY_MAP) + " -o ./bin/fsm.so -ldl ./bin/fsm.cpp -ldl";

        cout << "exec " << exec << endl;
        system(exec.c_str());

        printf("State machine created successfully [Elapsed time: %.6f ms]\n", (1000 * (omp_get_wtime() - t)));
    } else {
        perror("Ragel file not found !!");
        return;
    }

    // Perform mining
    TracePattern tp; 
    tp.loadAndTrace(reqObj);
}

void _main_mineTrace() {

    // Take the input
    cout << "Enter Regex pattern (example 0M1M2M3 or 01M2): ";
    cin >> pattern;

    cout << "Enter event length (recommended max limit 100): ";
    cin >> alphabetLength;

    RequestObject reqObj;
    reqObj.pattern = pattern;
    reqObj.event_length = to_string(alphabetLength);
    reqObj.input_file_path = "./traceBin/trace"; // "./traceBin/arrhythmia_cleaned.data"
    reqObj.status = 1;
    
    invoke_mineTrace(reqObj);
}

void _main_serverTrace_threaded() {
    unsigned millisecond = 15000;
    while (true) {
      
      try {
        RequestObject reqObj = RedisUtil::getLastRequestObject();
        
        if (stoi(reqObj.status) == 1) {
            invoke_mineTrace(reqObj);
        }
        
      } catch (const Error &e) {
        cout << e.what() << endl;
      }
      
      cout << fixed << setprecision(2) << "Sleeping... for " << (double) (millisecond/1000) << " seconds  " << endl << endl;
      this_thread::sleep_for(chrono::milliseconds(millisecond));
        
    }
}

void _main_serverTrace() {
  thread qmine(_main_serverTrace_threaded);
  qmine.join();
}

#endif