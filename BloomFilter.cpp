#include "BloomFilter.hpp"
#define byteSize 8
#define num1Hash 5
#define	num2Hash 16
#define numaHash 1469641
#define numbHash 63689
#define num3Hash 1314123911
#define num4Hash 27
/* Create a new bloom filter with the size in bytes */
BloomFilter::BloomFilter(size_t numBytes)
{
	//calculate numSlots and allocate space
	numSlots=byteSize*numBytes;
	table=(unsigned char *)calloc(numBytes, sizeof(char));
}
/** The first hash function */
//source:An algorithm produced by Professor Daniel J. Bernstein and 
//shown first to the world on the usenet newsgroup comp.lang.c.
//http://www.partow.net/programming/hashfunctions/
unsigned int BloomFilter:: hashFunc1( std::string  item) {
   unsigned int hashVal = 0;
   for (unsigned int x = 0; x < item.length(); ++x){
        
     hashVal = (hashVal << num1Hash) + (hashVal<< num2Hash)-hashVal + (item[x]);
    
   }
   return hashVal%numSlots; 
}

/** The second hash function */
//source: A simple hash function from Robert Sedgwicks Algorithms in C book. 
//I've added some simple optimizations to the
//algorithm in order to speed up its hashing process. 
//http://www.partow.net/programming/hashfunctions/
unsigned int BloomFilter::hashFunc2( std::string  item) {
 
   unsigned int b    = numaHash;
   unsigned int a    = numbHash;
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < item.length();  ++i)
   {
      hash = hash * a + (item[i]);
      a    = a * b;
   }

   return hash%numSlots;
}

/** The third hash function */
//source:An algorithm proposed by Donald E. Knuth in The Art Of 
//Computer Programming Volume 3, under the topic of sorting 
//and search chapter 6.4. 
//http://www.partow.net/programming/hashfunctions/
unsigned int BloomFilter::hashFunc3( std::string  item) {

   unsigned int hash = num3Hash;
   unsigned int i    = 0;

   for (i = 0; i < item.length();++i)
   {
      hash = ((hash << num1Hash) ^ (item[i]) ^ (hash >> num4Hash));  
   }
	
   return hash%numSlots;}

/* Insert an item into the bloom filter */
void BloomFilter::insert(std::string item) 
{
	//find the indexes from three hash functions 
	unsigned int idx1=hashFunc1(item);
	unsigned int idx2=hashFunc2(item);
	unsigned int idx3=hashFunc3(item);
        
        //call helper method to insert the three hashed index
	setter(idx1);
	setter(idx2);
	setter(idx3);



}

//this is a helper method which set the calculated index
//to 1 to represent inserted
void BloomFilter:: setter(unsigned int idx){
	//find the byte and bit to change
	unsigned int bitIdx=idx%byteSize;
	unsigned int byteIdx=idx/byteSize;
	unsigned int temp=1;
	//bit operation to have 
	table[byteIdx]=table[byteIdx]|(temp<<(byteSize-1-bitIdx));

}



/* Determine whether an item is in the bloom filter */
//return true if the item is found, false otherwise
bool BloomFilter::find(std::string item) 
{
	//calculate the three hash values
	unsigned int idx1=hashFunc1(item);
	unsigned int idx2=hashFunc2(item);
	unsigned int idx3=hashFunc3(item);

	//call helper function to see if the index is inserted
	bool test1 = getter(idx1);
	bool test2 = getter(idx2);
	bool test3 = getter(idx3);

	//if all three index is inserted, the element is found
	if(test1&&test2&&test3){
	  return true;
	}
	else{
	  return false;
	}
}
//this is a helper method to check if the calculated index is 1
//if not, return false
bool BloomFilter::getter(unsigned int idx){
	//calculate the byte and bit index 
	unsigned int bitIdx=idx%byteSize;
	unsigned int byteIdx=idx/byteSize;
	char temp=table[byteIdx];

	//bitwise operation to check if the number at desired
	//idx is one
	//return true if 1, return false if 0
	if(((temp>>(byteSize-1-bitIdx)) & 1) == 1){
	  return true;
	}
	else{
	  return false;
	}

}
/* Destrctor for the bloom filter */
BloomFilter::~BloomFilter()
{
	delete table;
}
