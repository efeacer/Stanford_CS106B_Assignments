/**
 * @brief The header file defining public/private methods and properties used by the
 * MyMap class.
 * Section Leader: Ryan Kurohara
 * @author EFE ACER
 * @version 1.0
 */

#pragma once

#include "vector.h"
#include <ostream>
#include <istream>
using namespace std;

class MyMap
{
public:
    MyMap(); //constructor
    ~MyMap(); //destructor

    int get(int key) const; //accessor method for the value of a given key
    void put(int key, int value); //adds a new value with the associated key to the map
    bool containsKey(int key); //checks whether or not a key is present in the map
    Vector<int> keys() const; //returns a Vector of keys
    int size(); //returns the size of the map

    void sanityCheck();
    MyMap(const MyMap &myMap); // copy constructor
    MyMap& operator= (const MyMap &myMap); // assignment overload
    friend ostream &operator<<(ostream &out, MyMap &myMap);
    friend istream &operator>>(istream &in, MyMap &myMap);
private:
    struct key_val_pair { //struct modeling a key value pair with its reference to the next pair
        int key;
        int value;
        key_val_pair* next;
    };

    typedef key_val_pair** bucketArray; //synonym for a bucketArray pointer

    bucketArray createBucketArray(int nBuckets); //creates a bucketArray of desired size
    int hashFunction(int input) const; //hash function used when the elements are placed to buckets

    bucketArray buckets; //the main inner data structure

    int nBuckets; //number of buckets in the hash map
    int nElems; //number of elements in the hash map
};
