#ifndef TrieNode_hpp
#define TrieNode_hpp

#include <string>
using namespace std;


class TrieNode {
public:

  TrieNode * left;
  TrieNode * mid;
  TrieNode * right;
  bool isEndofWord;
  char data;
  unsigned int freq;

  /** Default constructor */
  TrieNode() {
    left = mid = right = nullptr;
    isEndofWord = false;
    freq = 0;
    data = (char) 0;
  }
  /** constructor with information */
  TrieNode(const char data, bool isEndofWord)
  : data(data), isEndofWord(isEndofWord) {
    left = mid = right = nullptr;
    freq = 0;
    if (isEndofWord) {
      freq = 1;
    }
  }

};
#endif
