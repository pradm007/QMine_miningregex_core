#include "../include/tracePattern.h"

/**
 * Function to display the mean and variance of the dataset
 */
void TracePattern::loopAndPresentData(string &patternKey, vector<vector<string> > &numberList, string &regexPattern) {
	vector<string> patternKeyMap;
	int currentIndex = 0, bracketStart = 0;

	if (DEBUG) {
		cout << " patternKey " << patternKey << endl;
	}

	//This breaks the pattern key into sub elements
	while(patternKey[currentIndex] != '\0') {
		if (patternKey[currentIndex] != '[' && bracketStart == 0) {
			if (patternKeyMap.empty()) {
				string element (1, patternKey.at(currentIndex));
				patternKeyMap.push_back(element);

			} else {
				int size = patternKeyMap.size();
				patternKeyMap[size-1] += (char) patternKey.at(currentIndex);
			}
		} else {
			//everything after the bracket for number mining starts
			if (patternKey[currentIndex] == '[') {
				bracketStart = 1;
				patternKeyMap.push_back("");
			} else if (patternKey[currentIndex] >= 97 && patternKey[currentIndex] <= 122) {
				bracketStart = 0;
				currentIndex--;
			}
		}
		currentIndex++;

	}

  if (DEBUG) {
    cout << "Partition done " << endl;
    for (int i=0;i<patternKeyMap.size();i++) {
      cout << patternKeyMap[i] << " ";
    }
    cout << endl;
  }
	
	try {
		vector<vector<double>> numberListInDouble;
		for (int i =0; i < numberList.size(); i++) {
				// printf("\tList %d : \n\t\t\t", i+1);
				int start = 0;
				string fullMatchedPattern;

				for (int j=0;j<numberList[i].size(); j++) {
					string numberIs = Util::removeComma(numberList[i][j]);

					if ( numberIs.size() > 0) {

						if (start < patternKeyMap.size()) {
							fullMatchedPattern += patternKeyMap[start++];
						}
						fullMatchedPattern += numberIs;
					}
				}

				if (start < patternKeyMap.size()) { //flush the remainning, there should be only 1 pattern left at max
					fullMatchedPattern += patternKeyMap[start++];
				}

				if (regex_match(fullMatchedPattern, regex(regexPattern)) ) {
					if (DEBUG) {
						cout << "Matched with " << regexPattern << endl;
					}
					vector<double> *innerList = new vector<double>;
					for (int j=0;j<numberList[i].size(); j++) {
						string innerNumber = Util::removeComma(numberList[i][j]);
						if (innerNumber.size() > 0) {
							innerList->push_back(stod(innerNumber));
						}
					}
					numberListInDouble.push_back(*innerList);
				} else {
					if (DEBUG) {
						cout << "Not matched with " << regexPattern << endl;
					}
				}

		}

		cout << "Inner pattern match completed. Total matches found : " << numberListInDouble.size() << endl;
		if (numberListInDouble.size() > 0) {
			int innerSize = numberListInDouble[0].size();
			for (int j=0;j<innerSize;j++) {
				long double MU = 0;
				long double VAR = 0;
				for (int i=0;i<numberListInDouble.size();i++) {
					MU += (numberListInDouble[i][j] / numberListInDouble.size());
				}

				for (int i=0;i<numberListInDouble.size();i++) {
					VAR += ( pow( (MU - numberListInDouble[i][j]) , 2) / numberListInDouble.size());
				}

				cout << "For placeholder at " << j+1 << " : " << endl;
				cout << "\t Mean " << MU << endl;
				cout << "\t Variance " << VAR << endl;
				cout << endl;
			}
		}

	} catch(exception& err) {
		cout << " a standard exception was caught, with message '" << err.what() << "'\n";
	}
}

/**
 * Function to load the trace and perform mining
 */
void TracePattern::loadAndTrace(RequestObject reqObj) {

  void *lib = dlopen(shareObject_FileName.c_str(), RTLD_NOW);
  if (!lib) {
    printf("dlopen failed: %s\n", dlerror());
    return;
  }
  cout << "Dynamic Shared Object :" << endl;
  cout << "\t\t File Name : " << shareObject_FileName << endl;
  cout << "\t\t Status : Success" << endl;


  char *input = "";
  ifstream myfile(reqObj.input_file_path);
//   ifstream myfile("./traceBin/trace");
//   ifstream myfile("./traceBin/arrhythmia_cleaned.data");
  string inp;
  if (myfile.is_open()) {
    inp.assign( (istreambuf_iterator<char>(myfile) ), (istreambuf_iterator<char>()) );
    myfile.close();
  } else {
    cout << Util::timestamp_as_string() << ": " << "Something went wrong while opening file" << endl;
  }

  input = (char *)malloc((inp.size() + 1) * sizeof(char *));
  strcpy(input, inp.c_str());

  if (input != NULL && !inp.empty()) {
    double t = omp_get_wtime();
    unordered_map<string, vector<vector<string> > >* (*f)(char *);
    f = (unordered_map<string, vector<vector<string> > >* (*)(char *))dlsym(lib, "mine_pattern_parallelExecution");
    if (f) {
      printf("Dynamic Linker loaded successfully\n");
      unordered_map<string, vector<vector<string> > >* patternMap = f(input);

      if (SERVER_MODE || (CSVOUTPUT && inp.size() < 10000000)) { //10 MB
        string outputFile = ((reqObj.output_file_path.size() > 0) ? reqObj.output_file_path :  "./output/") + "mine-map"+Util::timestamp_as_string()+".csv";
		
        if (Util::writeToCSV(outputFile, *patternMap) != 0) {
		  reqObj.output_file_path = outputFile;
		  reqObj.status = to_string(2);
		  
          cout << Util::timestamp_as_string() << ": " << "Output file generated successfully" << endl;
		  
        } else {
          cout << Util::timestamp_as_string() << ": " << "Something went wrong while generating output file" << endl;
        }
      }

      int quit = 0;
      string inputString = "";
      while(!quit && !SERVER_MODE) {
        cout << "Enter pattern that you are interested with specific event symbol (Enter N to quit) : ";
        inputString = "a[0-9]+b";
        cin >> inputString;
        if (Util::willingToQuit(inputString)) {
          cout << "Exiting..." << endl;
          break;
        }

        auto itr = patternMap->find(inputString);
        const bool is_in = itr != patternMap->end();
        if (is_in) {
          string inputString2 = "a[0-9]+b";
          cout << "Enter interested regex (Enter N to quit) : " ;
          cin >> inputString2;
          if (Util::willingToQuit(inputString2)) {
            cout << "Exiting..." << endl;
            break;
          }

          vector<vector<string> > numberList = itr->second;
          loopAndPresentData(inputString, numberList, inputString2);

          cout << "Enter N to quit, anything else to continue : ";
          cin >> inputString2;
          if (Util::willingToQuit(inputString2)) {
            cout << "Exiting..." << endl;
            break;
          }
        } else {
          cout << "Pattern not found. Try again " << endl;
        }
      }

        
    } else {
	  cout << Util::timestamp_as_string() << ": "  << "Dynamic Linker loaded failed" << endl;
	  cout << Util::timestamp_as_string() << ": " << "dlsym for function grab failed: " << dlerror() << endl;
    }
    
	  cout << fixed << setprecision(2) << Util::timestamp_as_string() << ": " << "Full trace completed [Elapsed time: " << (1000 * (omp_get_wtime() - t)) << " ms]" << endl;
  } else {
	reqObj.status = to_string(3);
    cout << Util::timestamp_as_string() << ": " << "File might be empty. Size detected " << inp.size() << endl;
  }
  
  if (SERVER_MODE) {
	try {
		RedisUtil::insertOrUpdate(reqObj);
	} catch (const Error &e) {
		cout << e.what() << endl;
	}
  }

  dlclose(lib);
}