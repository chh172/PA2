#include "DictionaryTrie.hpp"
#include "TrieNode.hpp"
#include <algorithm>
#include <utility>
/* Create a new Dictionary that uses a Trie back end */
DictionaryTrie::DictionaryTrie() {
  root = new TrieNode();
  numCmp = 0;
}

/**
 * Insert a word with its frequency into the dictionary.
 * Return true if the word was inserted, and false if it
 * was not (i.e. it was already in the dictionary or it was
 * invalid (empty string). This might be useful for testing
 * when you want to test a certain case, but don't want to
 * write out a specific word 300 times.
 */
bool DictionaryTrie::insert(std::string word, unsigned int freq) {
  // if the DictionaryTrie is just inited
  if (word.empty()) {
    return false;
  }
  if (root->data == '\0') {
    root->data = word[0];
    wordToNodes(word.substr(1,word.length()-1),freq,root);
    TrieNode * traverse = root;
    while (traverse != nullptr) {
      updateMaxBelow(traverse,freq);
      traverse = traverse->mid;
    }
    return true;
  }
  if (find(word)) {
    return false;
  }
  else {
    insertTo(word,freq,root);
    return true;
  }
}

void DictionaryTrie::insertTo(std::string word, unsigned int freq
                                                    , TrieNode * node) {

  if (word.empty()) {
    return;
  }
  if (node == nullptr) {
    return;
  }
  updateMaxBelow(node,freq);
  if (word[0] == node->data) {
    if (node->mid != nullptr) {
      if (word.length() == 1) {
        node->isEndofWord = true;
        node->freq = freq;
        node->maxBelow = max(node->maxBelow,freq);
      }
      else {
        insertTo(word.substr(1,word.length()-1),freq,node->mid);
      }
    }
    else { // append the rest
      wordToNodes(word.substr(1,word.length()-1),freq,node);
    }
  }
  else if (word[0] < node->data) {
    if (node->left != nullptr) {
      insertTo(word,freq,node->left);
    }
    else { // append at this node
      node->left = new TrieNode(word[0],false);
      wordToNodes(word.substr(1,word.length()-1),freq,node->left);
    }
  }
  else { // word[0] > node->data
    if (node->right != nullptr) {
      insertTo(word,freq,node->right);
    }
    else { // append at this node
      node->right = new TrieNode(word[0],false);
      //wordToNodes(word,freq,node);
      wordToNodes(word.substr(1,word.length()-1),freq,node->right);
    }
  }
}
void DictionaryTrie::updateMaxBelow(TrieNode * node, unsigned int freq) {
  if (node->maxBelow < freq) {
    node->maxBelow = freq;
  }
}
void DictionaryTrie::wordToNodes(std::string word, unsigned int freq, TrieNode * n) {
  TrieNode * traverse = n;
  while (!word.empty()) {
    traverse->mid = new TrieNode(word[0],false);
    updateMaxBelow(traverse,freq);
    traverse = traverse->mid;
    //traverse->maxBelow = freq;
    word = word.substr(1,word.length()-1);
  }
  updateMaxBelow(traverse,freq);
  traverse->isEndofWord = true;
  traverse->freq = freq;
}
/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryTrie::find(std::string word) const {
  return findFrom(word,root);
}
bool DictionaryTrie::findFrom(std::string word, TrieNode * node) const {
  if (node == nullptr) {
    return false;
  }
  if (word.empty()) {
    return false;
  }
  if (word[0] < node->data) {
      return findFrom(word,node->left);
  }
  else if (word[0] > node->data) {
      return findFrom(word,node->right);
  }
  else { // equal case
    if (word.length() > 1) {
        return findFrom(word.substr(1,word.length()-1),node->mid);
    }
    else {
      return node->isEndofWord;
    }
  }
}

/** return a pair that contains information:
  * 1. if prefix contained in TST
  * 2. the pointer to node that corresponds to the last char of prefix.
   */
std::pair<bool,TrieNode *> DictionaryTrie::containFrom(std::string word, TrieNode * node) const {
  if (node == nullptr) {
    return std::pair<bool,TrieNode *>(false,nullptr);
  }
  if (word[0] < node->data) {
    return containFrom(word,node->left);
  }
  else if (word[0] > node->data) {
    return containFrom(word,node->right);
  }
  else {
    if (word.length() == 1) {
      return std::pair<bool,TrieNode *>(true,node);
    }
    else {
     return containFrom(word.substr(1,word.length()-1),node->mid);
    }
  }
}
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
std::vector<std::string> DictionaryTrie::predictCompletions(std::string prefix
                                              , unsigned int num_completions) {
  numCmp = num_completions;
  while (!v.empty()) {
    v.pop();
  }
  // case prefix not in dictionary
  TrieNode * curr = std::get<1>(containFrom(prefix,root));
  if (curr == nullptr) {
    return {};
  }
  // case prefix in dictionary
  //string record(prefix);
  if (curr->freq >= curr->maxBelow) {
    v.push(std::pair<std::string,TrieNode *>(prefix,curr));
  }
  if (curr->mid != nullptr) {
    unsigned int record = curr->mid->maxBelow;
    while (v.size() < (long unsigned int) num_completions && record != 0) {
      record = completeFrom(curr->mid,prefix,record);
    }
  }

  vector<string> ret;
  while(v.size() != 0) {
    ret.insert(ret.begin(), v.top().first);
    v.pop();
  }

  return ret;
}
/** dfs helper returns second maxBelow */
unsigned int DictionaryTrie::completeFrom(TrieNode * n, string s, unsigned int target) {
  unsigned int secondBest = 0;
  if (n == nullptr) {
    return secondBest;
  }
  if (n != nullptr) {
    if (n->maxBelow < target) {
      secondBest = n->maxBelow;
    }
    if (n->freq < target && n->freq > secondBest) {
      secondBest = n->freq;
    }
    if (n->maxBelow < target) {
      return secondBest;
    }
    string temp(s);
    //completeFrom(n->left,temp);
    s.push_back(n->data);
   // completeFrom(n->left,temp);
    if (n->freq == target) {
      v.push(std::pair<std::string,TrieNode *>(s,n));
      if (v.size() > numCmp) {
        v.pop();
      }
    }
    unsigned int tep = max(max(completeFrom(n->mid,s,target)
      ,completeFrom(n->left,temp,target)),completeFrom(n->right,temp,target));
    secondBest = max(secondBest,tep);
    return secondBest;
  }
}
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

std::vector<std::string> DictionaryTrie::predictUnderscore(std::string pattern
                                              , unsigned int num_completions) {

  if (pattern.length() == 1) {
    // complete with one-length word
    clean();
    assembleUnitLengthList(root);
    return copyPQ(num_completions);
  }
  else if (pattern.back() == '_') { // underscore in back
    clean();
    string prefix(pattern.substr(0,pattern.length()-1));
    std::pair<bool,TrieNode *> temp(containFrom(prefix,root));
    if (!temp.first) {
      return {};
    }
    TrieNode * curr = temp.second;
    TrieNode * traverse = curr->mid;
    traverseBelowForEpi(prefix, traverse, 0);
    return copyPQ(num_completions);
  }
  else { // underscore in middle and front
    clean();
    string prefix(pattern.substr(0,pattern.find('_')));
    string post(pattern.substr(pattern.find('_',0)+1,pattern.length() - prefix.length()));
    TrieNode * traverse = root;
    if (!prefix.empty()) {
      TrieNode * curr = (containFrom(prefix,root)).second;
      if (curr == nullptr) {
        return {};
      }
      traverse = curr->mid;
    }
    // need to go through all next generation of curr
    traverseBelowForEpi(post, traverse, 1);
    for (size_t i = 0; i < v3.size(); i++) {
      string copy(v3[i].first);
      string copyPrefix(prefix);
      v2.push(std::pair<std::string,TrieNode *>(copyPrefix.append(copy.append(post)),v3[i].second));
      // need that end Node pointer to have freq.
    }
    return copyPQ(num_completions);
  }
}
void DictionaryTrie::traverseBelowForEpi(string post, TrieNode * traverse, int flag) {
  if (flag == 1) {
    if (traverse == nullptr) {
      return;
    }
    TrieNode * p = matchFrom(post,traverse->mid);
    if (p != nullptr) {
      string data(1,traverse->data);
      v3.push_back(std::pair<string,TrieNode *>(data,p));
    }
    traverseBelowForEpi(post, traverse->left, 1);
    traverseBelowForEpi(post, traverse->right, 1);
  }
  if (flag == 0) {
    if (traverse == nullptr) {
      return;
    }
    if (traverse->isEndofWord) {
      string tep (1,traverse->data);
      string copy(post); // actually prefix
      v2.push(std::pair<std::string,TrieNode *>(copy.append(tep),traverse));
    }
    traverseBelowForEpi(post, traverse->left, 0);
    traverseBelowForEpi(post, traverse->right, 0);
  }
}
/** Helper Function to find the sub-pattern (post of underscore of the pattern)
 * if find the pattern, return the TrieNode * to node whose data
 * corresponds last char of pattern
 * inclusive from.
 */
TrieNode * DictionaryTrie::matchFrom(std::string word, TrieNode * node) const {
  if (node == nullptr) {
    return nullptr;
  }
  if (word[0] < node->data) {
    return matchFrom(word,node->left);
  }
  else if (word[0] > node->data) {
    return matchFrom(word,node->right);
  }
  else {
    if (word.length() == 1) {
      if (node->isEndofWord) {
        return node;
      }
      else {
        return nullptr;
      }
    }
    else {
      return matchFrom(word.substr(1,word.length()-1),node->mid);
    }
  }
}
void DictionaryTrie::assembleUnitLengthList(TrieNode * node) {
  if (node == nullptr) {
    return;
  }
  if (node->isEndofWord) {
    string temp (1,node->data);
    v2.push(std::pair<std::string,TrieNode *>(temp,node));
  }
  assembleUnitLengthList(node->right);
  assembleUnitLengthList(node->left);
}
void DictionaryTrie::clean() {
  while (!v2.empty()) {
    v2.pop();
  }
  v3.clear();
}
vector<string> DictionaryTrie::copyPQ(unsigned int num_completions) {
  vector<string> ret;
  long unsigned int numComp = (long unsigned int) num_completions;
  long unsigned int ogSize = v2.size();
  for (long unsigned int i = 0; i < numComp && i < ogSize; i++) {
    ret.push_back(std::get<0>(v2.top()));
    v2.pop();
  }
  return ret;
}
/* Destructor */
DictionaryTrie::~DictionaryTrie() {
  deleteAll(root);
}
void DictionaryTrie::deleteAll(TrieNode * n) {
  /* Pseudo Code:
  if current node is null: return;
  recursively delete left sub-tree
  recursively delete right sub-tree
  delete current node
  */

  //if current node is null, return
  if (n == nullptr) {return;}
  //recursively delete left subtree
  deleteAll(n->left);
  //recursively delete right subtree
  deleteAll(n->mid);
  deleteAll(n->right);
  //delete current node
  delete n;
  return;
}
