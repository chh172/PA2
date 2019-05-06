/**
 *  FileHeader:
 *  FileName: TrieNode.hpp
 *  Description: this file contains a class that construct the node in our Trie
 *  Authors: Chuhuan Huang, Junyi Wu 
 *  source of help: discussion slides. Tutors. Writeup
 */
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
  unsigned int maxBelow;
  /** Default constructor */
  TrieNode() {
    left = mid = right = nullptr;
    isEndofWord = false;
    freq = 0;
    maxBelow = 0;
    data = (char) 0;
  }
  /** constructor with information */
  TrieNode(const char data, bool isEndofWord)
  : isEndofWord(isEndofWord), data(data) {
    left = mid = right = nullptr;
    freq = 0;
    maxBelow = 0;
    if (isEndofWord) {
      freq = 1;
    }
  }

};
#endif
