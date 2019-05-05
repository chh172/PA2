#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "DictionaryTrie.hpp"
#include "util.hpp"

using namespace std; 

int main(){
	DictionaryTrie* a = new DictionaryTrie();
	cout << "DictionaryTrie built" << endl;
	string b1 = "abc";
  string b2 = "aba";

	if(a->insert(b1,1)){
    cout << "First insert OK!" << endl;
    cout << "Start Duplicate check : " << endl;
  }
	if(a->insert(b1,2)){
    cout << "complex insert check: " << endl;
  }
	if(a->insert(b2,3)){
    cout << "Complex OK!" << endl;
    cout << "Start Complex duplicate check: " << endl;
  }
	if(a->insert(b2,4)){
	  cout << "Insert method OK!" << endl;
  }


  cout << "----------------------------------------------------" << endl;
  a->insert("aooo", 6);
  a->insert("bt", 8);
  a->insert("appp", 9);
  vector<string> list;
  list = a->predictCompletions("a", 3);
  cout << "First: Should be [appp] actually:  [" << list[0] << "]" << endl;
  cout << "First: Should be [aooo] actually:  [" << list[1] << "]" << endl;
  cout << "First: Should be [aba] actually:  [" << list[2] << "]" << endl;
  cout << "----------------------------------------------------" << endl;

  cout << "----------------------------------------------------" << endl;
	if (a->find(b1))
    cout << "Found OK!" << endl;
	else 
    cout << "Failed"<< endl;
  if (a->find("aooo")) 
    cout << "Found ok!" << endl;
  else 
    cout << "Failed" << endl;
  if (a->find("bt")) 
    cout << "Found ok!" << endl;
  else 
    cout << "Failed" << endl;
  if (a->find("b")) 
    cout << "Shouldn't Find" << endl;
  if (a->find("adsas")) 
    cout << "Shouldn't Find" << endl;



  return 0;
}
