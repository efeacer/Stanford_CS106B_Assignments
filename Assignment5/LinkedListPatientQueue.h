/**
 * @brief The header file defining private/public methods and properties used by
 * LinkedListPatientQueue class.
 * Section Leader: Ryan Kurohara
 * @author EFE ACER
 * @version 1.0
 */

#pragma once

#include <iostream>
#include <string>
#include "patientnode.h"
#include "patientqueue.h"
using namespace std;

class LinkedListPatientQueue : public PatientQueue {
public:
    LinkedListPatientQueue(); //constructor
    ~LinkedListPatientQueue(); //destructor
    string frontName(); //accessor method for the front element's name
    void clear(); //clears the queue
    int frontPriority(); //accessor method for the front element's priority
    bool isEmpty(); //checks empty state of the queue
    void newPatient(string name, int priority); //adds a new element
    string processPatient(); //removes the front element
    void upgradePatient(string name, int newPriority); //upgrades an element's priority
    string toString(); //returns a string representing the queue

private:
    PatientNode* front; //the reference to the front of the queue
};
