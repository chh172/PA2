#include <iostream>
#include <fstream>
#include "BloomFilter.hpp"


#define numArgs 4
#define secondIdx 2
#define thirdIdx 3
using namespace std;

/**
 * IMPORTANT! You should use the following lines of code
 * to match the correct output:
 * cout << "This program requires 3 arguments!" << endl;
 * 
 * arg1 - list of malicious urls/bad words filter out
 * arg2 - list of mixed (good/bad) to only write good urls to
 * arg3 - file to write only the good urls to (one on each line)
 */

int main(int argc, char** argv) {
    //check the number of arguments
    if(argc != numArgs){
	cout << "This program requires 3 arguments!" << endl;
	return -1;
    }

    //read file of malicious urls each line at a time 
    ifstream countInput(argv[1]);
    string line;
    unsigned int count = 0;
    while(getline(countInput, line)){
	//calculate number of lines in this file
	count++;
    }
    
    //create a bloomfilter to store
    BloomFilter filter(count*thirdIdx/secondIdx);
    string bad;

    ifstream badUrl(argv[1]);
    
    //insert each line
    while(getline(badUrl, bad)){
	filter.insert(bad);
    }

    //read the mixed file and open output file
    ifstream mixed(argv[secondIdx]);
    ofstream output;
    output.open(argv[thirdIdx], ios::app);
    string url;
    //record the count of the total urls and the number outputed
    unsigned int countUrl=0;
    unsigned int countOutput=0;
    
    //check each line of the mixed file
    while(getline(mixed, url)){
	countUrl++;
	//if the url is not found, write to output file
	if(!filter.find(url)){
		output<<url<<endl;
		countOutput++;
	}
    }

  
    //calculate the false positive rate 
    double ratio = (double)(countUrl-countOutput-count)
				/(double)(countUrl-count);
    if(countUrl==count){
    	ratio = 1;
    }
    
    //calculate the memory of the file 
    ifstream sizeFile(argv[1],ios::binary);
    sizeFile.seekg(0, sizeFile.end);
    unsigned int size = sizeFile.tellg();
    //calculate the saved memory ratio
    double memory=(double)(size)/(double)(count*thirdIdx/secondIdx);
        
    //print out the message needed
    cout<<"False positive rate: "<<ratio<<endl;
    cout<<"Saved memory ratio: "<<memory<<endl;
    output.close();
    return 1;

    
}
