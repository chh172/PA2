#include "DictionaryTrie.hpp"
#include <iostream>
#include "util.hpp"
#include <fstream>
using namespace std;

/**
* IMPORTANT! You should use the following lines of code to match the correct output:
*
* cout << "This program needs exactly one argument!" << endl;
* cout << "Reading file: " << file << endl;
* cout << "Enter a prefix/pattern to search for:" << endl;
* cout << "Enter a number of completions:" << endl;
* cout << completion << endl;
* cout << "Continue? (y/n)" << endl;
*
* arg 1 - Input file name (in format like freq_dict.txt)
*/
int main(int argc, char** argv)
{
  bool continuing = true;
  //check the number of argumen
  if(argc != 2){
    cout << "This program needs exactly one argument!" <<endl;
    return -1;
  }

  //read data from file
  ifstream in;
  string file = argv[1];
  cout << "Reading file: " << file <<endl;
  in.open(file);
  DictionaryTrie dic = DictionaryTrie();

  //load the words into the trie
  Utils::load_dict(dic, in);


  while(continuing){
    cout << "Enter a prefix/pattern to search for:" << endl;
    string inputPattern;
    getline(cin, inputPattern);

    //get number of completions and convert to int
    unsigned int completion;
    cout << "Enter a number of completions:" << endl;
    cin >> completion;
    cin.ignore();
    //unsigned int com = (unsigned int) (stoi(completion, nullptr));
    vector<string> result;
    if (inputPattern.find('_') == std::string::npos) {
      //get the result from predictCompletions
      result = dic.predictCompletions(inputPattern, completion);
    }
    else {
      result = dic.predictUnderscore(inputPattern, completion);
    }

    for (unsigned int i = 0;  i< result.size();i++){
      cout << result[i] << endl;
    }

    cout << "Continue? (y/n)" << endl;
    string input;
    getline(cin, input);
    if(input == "n"){
      continuing = false;
    }
  }
}
