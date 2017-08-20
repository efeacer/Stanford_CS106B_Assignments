/**
 * @brief The header file defining private and public methods, properties used by
 * VectorPatientQueue class.
 * Section Leader: Ryan Kurohara
 * @author EFE ACER
 * @version 1.0
 */

#pragma once

#include <iostream>
#include <string>
#include "patientqueue.h"
#include "vector.h"
using namespace std;

struct Patient { //struct modeling a Patient
    string name;
    int priority;
    int timestamp;
};

class VectorPatientQueue : public PatientQueue {
public:
    VectorPatientQueue(); //constructor
    ~VectorPatientQueue(); //destructor
    string frontName(); //accessor method for the front element's name
    void clear(); //clears the queue
    int frontPriority(); //accessor method for the front element's priority
    bool isEmpty(); //checks empty state of the queue
    void newPatient(string name, int priority); //adds a new element
    string processPatient(); //removes the front element
    void upgradePatient(string name, int newPriority); //upgrades an element's priority
    string toString(); //returns a string representing the queue

private:
    Vector<Patient> patients; //inner data structure for the priority queue
};
