/**
 *  CSE 100 PA2 C++ Autocomplete
 *  Authors: Jor-el Briones, Christine Alvarado
 */

#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <vector>
#include <string>
#include <queue>
#include <utility>
#include "TrieNode.hpp"
using namespace std;
/**
 *  The class for a dictionary ADT, implemented as a trie
 *  You may implement this class as either a mulit-way trie
 *  or a ternary search trie, but you must use one or the other.
 */
class DictionaryTrie
{
public:

  /* Create a new Dictionary that uses a Trie back end */
  DictionaryTrie();

  /**
   * Insert a word with its frequency into the dictionary.
   * Return true if the word was inserted, and false if it
   * was not (i.e. it was already in the dictionary or it was
   * invalid (empty string). This might be useful for testing
   * when you want to test a certain case, but don't want to
   * write out a specific word 300 times.
   */
  bool insert(std::string word, unsigned int freq);

  /* Return true if word is in the dictionary, and false otherwise. */
  bool find(std::string word) const;

  /*
   * Return up to num_completions of the most frequent completions
   * of the prefix, such that the completions are words in the dictionary.
   * These completions should be listed from most frequent to least.
   * If there are fewer than num_completions legal completions, this
   * function returns a vector with as many completions as possible.
   * If no completions exist, then the function returns a vector of size 0.
   * The prefix itself might be included in the returned words if the prefix
   * is a word (and is among the num_completions most frequent completions
   * of the prefix)
   */
  std::vector<std::string>
  predictCompletions(std::string prefix, unsigned int num_completions);

  /* Return up to num_completions of the most frequent completions
   * of the pattern, such that the completions are words in the dictionary.
   * These completions should be listed from most frequent to least.
   * If there are fewer than num_completions legal completions, this
   * function returns a vector with as many completions as possible.
   * If no completions exist, then the function returns a vector of size 0.
   * The pattern itself might be included in the returned words if the pattern
   * is a word (and is among the num_completions most frequent completions
   * of the pattern)
   */
  std::vector<std::string>
  predictUnderscore(std::string pattern, unsigned int num_completions);

  /* Destructor */
  ~DictionaryTrie();

private:
  // Add your own data members and methods here
  TrieNode * root;
  unsigned int numCmp;
  struct Priority {
    bool operator() (std::pair<std::string,TrieNode *> p1
                                    , std::pair<std::string,TrieNode *> p2) {
      if (std::get<1>(p1)->freq != std::get<1>(p2)->freq) {
        return (std::get<1>(p1))->freq < (std::get<1>(p2))->freq;
      }
      else {
        return (std::get<0>(p1)).compare((std::get<0>(p2))) > 0;
      }
    }
  };
  struct PriorityOpt {
    bool operator() (std::pair<std::string,TrieNode *> p1
                                    , std::pair<std::string,TrieNode *> p2) {
      if (std::get<1>(p1)->freq != std::get<1>(p2)->freq) {
        return (std::get<1>(p1))->freq > (std::get<1>(p2))->freq;
      }
      else {
        return (std::get<0>(p1)).compare((std::get<0>(p2))) < 0;
      }
    }
  };
  priority_queue<std::pair<std::string,TrieNode *>
                    , vector<std::pair<string,TrieNode *>>,PriorityOpt> v;
  priority_queue<std::pair<std::string,TrieNode *>
                    , vector<std::pair<string,TrieNode *>>,Priority> v2;
  vector<std::pair<string,TrieNode *>> v3;
  struct Cpriority {
    bool operator() (unsigned int p1, unsigned int p2) {
      return p1 < p2;
    }
  };
  priority_queue<unsigned int, std::vector<unsigned int>, Cpriority> criteria;
  bool findFrom(std::string word, TrieNode * node) const;
  void insertTo(std::string word, unsigned int freq, TrieNode * node);
  void wordToNodes(std::string word, unsigned int freq, TrieNode * n);
  void deleteAll(TrieNode * n);
  unsigned int completeFrom(TrieNode * n, string s, unsigned int target);
  pair<bool,TrieNode *> containFrom(std::string word,TrieNode * node) const;
  //TrieNode * findPatternFrom(std::string pattern, TrieNode * node) const;
  TrieNode * matchFrom(std::string word,TrieNode * node) const;
  void clean();
  vector<string> copyPQ(unsigned int num_completions);
  void assembleUnitLengthList(TrieNode * node);
  void traverseBelowForEpi(string post, TrieNode * traverse, int flag);
  void updateMaxBelow(TrieNode * node, unsigned int freq);
};

#endif // DICTIONARY_TRIE_H
