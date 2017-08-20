/**
 * @brief The following code involves the methods neccessary to build a hash map.
 * It completes the methods defined by the MyMap class. Most of the methods run
 * on constant time so that accessing and manipulating the map is fast.
 * SectionLeader: Ryan Kurohara
 * @author EFE ACER
 * @version 1.0
 */

#include "mymap.h"
#include "vector.h"
using namespace std;

//constant decleration(s)
const int INITIAL_SIZE = 1000; //can be changed depending on time-space concerns

/**
 * @brief MyMap::MyMap The default constructor of the MyMap class. Initializes the size and
 * constructs an array (buckets) with desired size (this can be changed based on time-space
 * concerns by changing the INITIAL_SIZE constant).
 */
MyMap::MyMap() {
    nElems = 0;
    nBuckets = INITIAL_SIZE;
    buckets = createBucketArray(nBuckets);
}

/**
 * @brief MyMap::~MyMap Destructor of the MyMap class. Walks through the linked lists in the
 * buckets and deletes all elements. Prevents any memory leak.
 */
MyMap::~MyMap() {
    for (int i = 0; i < nBuckets; i++) {
        while (buckets[i] != nullptr) {
            key_val_pair* trash = buckets[i];
            buckets[i] = buckets[i]->next;
            delete trash;
        }
    }
    delete [] buckets;
}

/**
 * @brief MyMap::put Adds a new value associated with the given key; if such a key already exists,
 * updates its value.
 * @param key The given key which is unique for the map.
 * @param value The value associated with the given key.
 */
void MyMap::put(int key, int value) {
    int bucketIndex = hashFunction(key) % nBuckets;
    key_val_pair* existing = nullptr;
    key_val_pair* current = buckets[bucketIndex];
    while (current != nullptr) {
        if (current->key == key) {
            existing = current;
            break;
        }
        current = current->next;
    }
    if (existing != nullptr) {
        current->value = value;
    } else {
        key_val_pair* incoming = new key_val_pair();
        incoming->key = key;
        incoming->value = value;
        incoming->next = buckets[bucketIndex];
        buckets[bucketIndex] = incoming;
        nElems++;
    }
}

/**
 * @brief MyMap::get Returns the value associated with the given key. Throws a string
 * exception if there is no such key in the map.
 * @param key The given key, whose value will be returned.
 * @return The value associated with the given key.
 */
int MyMap::get(int key) const {
    int bucketIndex = hashFunction(key) % nBuckets;
    key_val_pair* current = buckets[bucketIndex];
    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    throw("Invalid operation: The given key does not exist in the map."); //error case
}

/**
 * @brief MyMap::containsKey Checks whether or not a key is present in the map.
 * @param key The key to be searched.
 * @return A bool expression indicating whether or not the given key is present.
 */
bool MyMap::containsKey(int key) {
    int bucketIndex = hashFunction(key) % nBuckets;
    key_val_pair* current = buckets[bucketIndex];
    while (current != nullptr) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}

/**
 * @brief MyMap::keys Returns a vector of the keys in the map.
 * @return The vector of the keys in the map.
 */
Vector<int> MyMap::keys() const {
    Vector<int> keys;
    for (int i = 0; i < nBuckets; i++) {
        key_val_pair* current = buckets[i];
        while (current != nullptr) {
            keys.add(current->key);
            current = current->next;
        }
    }
    return keys;
}

/**
 * @brief MyMap::size Returns the current size of the map.
 * @return Number of elements in the map.
 */
int MyMap::size() {
    return nElems;
}

/**
 * STARTER CODE, DO NOT MODIFY
 */

// copy constructor
MyMap::MyMap(const MyMap &myMap) {
    // make a deep copy of the map
    nBuckets = myMap.nBuckets;

    buckets = createBucketArray(nBuckets);

    // walk through the old array and add all elements to this one
    Vector<int> keys = myMap.keys();
    for (int i=0; i < keys.size(); i++) {
        int key = keys[i];
        int value = myMap.get(key);
        put(key,value);
    }

}

// assignment overload
MyMap& MyMap::operator= (const MyMap &myMap) {
    // make a deep copy of the map

    // watch for self-assignment
    if (this == &myMap) {
        return *this;
    }

    // if data exists in the map, delete it
    for (int i=0; i < nBuckets; i++) {
        MyMap::key_val_pair* bucket = buckets[i];
        while (bucket != nullptr) {
            // walk the linked list and delete each node
            MyMap::key_val_pair* temp = bucket;
            bucket = bucket->next;
            delete temp;
        }
        buckets[i] = nullptr;
    }
    nElems = 0;
    // walk through the old array and add all elements to this one
    Vector<int> keys = myMap.keys();
    for (int i=0; i < keys.size(); i++) {
        int key = keys[i];
        int value = myMap.get(key);
        put(key,value);
    }

    // return the existing object so we can chain this operator
    return *this;
}

ostream &operator<<(ostream &out, MyMap &myMap) {
    out << "{";
    Vector<int> keys = myMap.keys();
    for (int i=0; i < keys.size(); i++) {
        int key = keys[i];
        int value = myMap.get(key);
        out << key << ":" << value;
        if (i < keys.size() - 1) { // print the comma after each pair except the last
            out << ", ";
        }
    }
    out << "}";
    return out;
}

istream &operator>>(istream &in, MyMap &myMap) {
    // assume the format {1:2, 3:4}
    bool done = false;
    in.get(); // get the first char, {
    int nextChar = in.get(); // get the first real character
    while (!done) {
        string nextInput;
        while (nextChar != ',' and nextChar != '}') {
            nextInput += nextChar;
            nextChar = in.get();
        }
        if (nextChar == ',') {
            // read the space as well
            in.get(); // should be a space
            nextChar = in.get(); // get the next character
        } else {
            done = true; // we have reached }
        }
        // at this point, nextInput should be in the form 1:2
        // (we should have two integers separated by a colon)
        // BUT, we might have an empty map (special case)
        if (nextInput != "") {
            vector<string> kvp = stringSplit(nextInput,":");
            myMap.put(stringToInteger(kvp[0]),stringToInteger(kvp[1]));
        }
    }
    return in;
}
/**
 * STARTER CODE, DO NOT MODIFY
 *
 * Given a number of buckets, creates a hashtable (array of linked list heads).
 *
 * @param nBuckets the number of buckets you want in the hashtable.
 * return an array of heads of linked lists of key_val_pairs
 */
MyMap::bucketArray MyMap::createBucketArray(int nBuckets) {
    bucketArray newBuckets = new key_val_pair*[nBuckets];
    for (int i = 0; i < nBuckets; i++) {
        newBuckets[i] = nullptr;
    }
    return newBuckets;
}

/**
 * STARTER CODE
 * The hash function for your HashMap implementation.
 * For an extension, you might want to improve this function.
 *
 * @param input - an integer to be hashed
 * return the hashed integer
 */
int MyMap::hashFunction(int input) const {
    // use unsigned integers for calculation
    // we are also using so-called "magic numbers"
    // see https://stackoverflow.com/a/12996028/561677 for details
    unsigned int temp = ((input >> 16) ^ input) * 0x45d9f3b;
    temp = ((temp >> 16) ^ temp) * 0x45d9f3b;
    temp = (temp >> 16) ^ temp;

    // convert back to positive signed int
    // (note: this ignores half the possible hashes!)
    int hash = (int) temp;
    if (hash < 0) {
        hash *= -1;
    }

    return hash;
}

/**
* @brief MyMap::sanityCheck
*
* Checks a map for internal consistency
*/
void MyMap::sanityCheck(){
    for(int i = 0; i < 1000; i++) {
        put(i,i);
    }
    for(int i = 0; i < 1000; i++) {

        if(!containsKey(i)) {
            string err = integerToString(i) + " should be a key in the map but cannot be found";
        }
        int val;

        try {
            val = get(i);
        } catch(string exception) {
            string err = "Unable to get value for " + integerToString(i);
            throw(err);
        }

        if (i != val) {
            string err = integerToString(i) + " should be mapped to " + integerToString(i) + " but is mapped to " + integerToString(val) + "instead";
            throw(err);
        }
    }
    cout << "Map seems ok" << endl;
}
