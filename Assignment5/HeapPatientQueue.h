/**
 * @brief The header file defining private/public methods and properties used by
 * HeapPatientQueue class.
 * Section Leader: Ryan Kurohara
 * @author EFE ACER
 * @version 1.0
 */

//THIS IS AN EXTENSION

#pragma once

#include <iostream>
#include <string>
#include "patientnode.h"
#include "patientqueue.h"
using namespace std;

//constant decletation(s)
const int INITIAL_CAPACITY = 10; //initial size of the inner array structure

struct HeapPatient { //struct modeling a Patient
    string name;
    int priority;
};

class HeapPatientQueue : public PatientQueue  {
public:
    HeapPatientQueue(); //constructor
    ~HeapPatientQueue(); //destructor
    string frontName(); //accessor method for the front element's name
    void clear(); //clears the queue
    int frontPriority(); //accessor method for the front element's priority
    bool isEmpty(); //checks empty state of the queue
    //Time complexity of enqueing: O(logN) but O(1) for average
    void newPatient(string name, int priority); //adds a new element
    //Time complexity of dequeing: O(logN)
    string processPatient(); //removes the front element
    void upgradePatient(string name, int newPriority); //upgrades an element's priority
    string toString(); //returns a string representing the queue

private:
    HeapPatient* patients; //array as internal storage
    int capacity;
    int count;
    void expand();
    void shrink();
    void bubbleUp(int index);
    void bubbleDown(int index);
    void swap(int index1, int index2);
};
