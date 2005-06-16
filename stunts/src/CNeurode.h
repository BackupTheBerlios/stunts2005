#ifndef CNeurode_h
#define CNeurode_h

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN64)
    #include <windows.h>
#endif

using namespace std;
using std::vector;

const int neurode_CONST = 100;
const int neurode_LINEAR = 200;
const int neurode_EXP = 300;

class CNeurode {
  
 private:
 public:
    vector<float*> weights;
	vector<CNeurode*> Inputs;
	int num;
	int function;
    
// public:
    CNeurode(int number);
    CNeurode(int number, int Function);
    ~CNeurode();
    
	float output;
    void addInput(int n, CNeurode* Input, float* weight);
    float getOutput();
    void runNeurode();
    
};

#endif

