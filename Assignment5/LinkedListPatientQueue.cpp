/**
 * @brief The following code involves the methods neccessary to make a linked list behave as a
 * priority queue. It completes the method definitions of the LinkedListPatientQueue class.
 * SectionLeader: Ryan Kurohara
 * @author EFE ACER
 * @version 1.0
 */

#include "LinkedListPatientQueue.h"
#include "strlib.h" //for integerToString method

/**
 * @brief LinkedListPatientQueue::LinkedListPatientQueue The default constructor of the
 * LinkedListPatientQueue class. Initializes the front pointer of the linked list to null pointer.
 */
LinkedListPatientQueue::LinkedListPatientQueue() {
    front = nullptr;
}

/**
 * @brief LinkedListPatientQueue::~LinkedListPatientQueue The destructor of the
 * LinkedListPatientQueue class. Walks through the linked list and deletes all elements.
 * Prevents any memory leak.
 */
LinkedListPatientQueue::~LinkedListPatientQueue() {
    while (front != nullptr) {
        PatientNode* trash = front;
        front = front->next;
        delete trash;
    }
}

/**
 * @brief LinkedListPatientQueue::clear Clears the queue of patients and turns it to an empty
 * list. Prevents any memory leak by deleting the data stored in the heap.
 */
void LinkedListPatientQueue::clear() {
    while (front != nullptr) {
        PatientNode* trash = front;
        front = front->next;
        delete trash;
    }
}

/**
 * @brief LinkedListPatientQueue::frontName Returns the name of the most ugrent patient without
 * removing him/her from the queue. Throws a string exception if the queue is empty.
 * @return The name of the most urgent patient.
 */
string LinkedListPatientQueue::frontName() {
    if (front == nullptr) {
        throw("Invalid Operation: The queue of patients is empty.");
    } else {
        return front->name;
    }
}

/**
 * @brief LinkedListPatientQueue::frontName Returns the pripority of the most ugrent patient
 * without removing him/her from the queue. Throws a string exception if the queue is empty.
 * @return The priority level of the most urgent patient.
 */
int LinkedListPatientQueue::frontPriority() {
    if (front == nullptr) {
        throw("Invalid Operation: The queue of patients is empty.");
    } else {
        return front->priority;
    }
}

/**
 * @brief VectorPatientQueue::isEmpty Returns false if there are at least one person in the queue
 * of patients and returns true if the queue is empty.
 * @return True if the queue is empty, false otherwise.
 */
bool LinkedListPatientQueue::isEmpty() {
    return front == nullptr;
}

/**
 * @brief LinkedListPatientQueue::newPatient Adds an incoming patient to the queue of patients.
 * The incoming patient is placed in the queue according to his/her priority level.
 * @param name The name of the incoming patient.
 * @param priority The priority value of the incoming patient.
 */
void LinkedListPatientQueue::newPatient(string name, int priority) {
    PatientNode* incoming = new PatientNode(name, priority);
    if (front == nullptr) { //case 1: link list is empty
        front = incoming;
    } else if (front->priority > priority) { //case 2: the incoming element is the front element
        incoming->next = front;
        front = incoming;
    } else { //case 3: link list has N elements
        PatientNode* current = front;
        while (current->next != nullptr && current->next->priority <= priority) {
            current = current->next;
        }
        incoming->next = current->next;
        current->next = incoming;
    }
}

/**
 * @brief LinkedListPatientQueue::processPatient Dequeues the most urgent patient from the queue
 * of patients. In other words, removes the front element of the linked list (Linked list
 * implementation allows us to store a sorted priority queue.) Throws a string exception if the
 * queue of patients is empty.
 * @return The most urgent patient in the queue (The front of the linked list).
 */
string LinkedListPatientQueue::processPatient() {
    if (front == nullptr) { //error case
        throw("Invalid Operation: The queue of patients is empty.");
    } else {
        PatientNode* trash = front;
        string outgoing = trash->name;
        front = front->next;
        delete trash;
        return outgoing;
    }
}

/**
 * @brief LinkedListPatientQueue::upgradePatient Modifies the priority of a given existing patient
 * to be more urgent. Throws a string exception if the given priority is not more urgent than
 * the patient's priority or there is no such patient with the given name.
 * @param name Name of the patient whose priority will be modified.
 * @param newPriority The modified priority of the patient.
 */
void LinkedListPatientQueue::upgradePatient(string name, int newPriority) {
    if (front == nullptr) { //error case
        throw("Invalid Operation: The queue of patients is empty.");
    } else {
        PatientNode* current = front;
        while (current->next != nullptr && current->next->name != name) {
            current = current->next;
        }
        if (current->next == nullptr) { //error case
            throw("Invalid Operation: There is no patient with the given name.");
        } else {
            PatientNode* toModify = current->next;
            if (toModify->priority < newPriority) { //error case
                throw("Invalid Operation: The priority of the patient is already greater than the"
                " new priority.");
            } else {
                current->next = toModify->next;
                newPatient(toModify->name, newPriority);
                delete toModify; //preventing memory leak.
            }
        }
    }
}

/**
 * @brief LinkedListPatientQueue::toString Returns a string representation of the queue of
 * patients. The returned string contains information such as patients priority levels and
 * names.
 * @return The string representation of the list.
 */
string LinkedListPatientQueue::toString() {
    if (front == nullptr) {
        return "{}";
    } else {
        string patientQueue;
        PatientNode* current = front;
        while (current->next != nullptr) {
            patientQueue += integerToString(current->priority) + ":" + current->name + ", ";
            current = current->next;
        }
        patientQueue += integerToString(current->priority) + ":" + current->name;
        return "{" + patientQueue + "}";
    }
}
