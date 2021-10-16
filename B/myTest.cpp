// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

/*
 * Compile this test against your .h files to make sure they compile. Note how
 * the eecs281 priority queues can be constructed with the different types. We
 * suggest adding to this file or creating your own test cases to test your
 * priority queue implementations more thoroughly.  If you do not call a
 * function from here, it is NOT compiled due to templates!  So for instance,
 * if you don't add code to call updatePriorities(), you could have compiler
 * errors that you don't even know about.
 *
 * Our makefile will build an executable named testPQ if you type 'make testPQ'
 * or 'make alltests' (without the quotes).
 *
 * Notice that testPairing() tests the range-based constructor but main() and
 * testPriorityQueue() do not.  Make sure to test the range-based constructor
 * for other PQ types, and also test the PairingPQ-specific member functions.
 *
 * This is NOT a complete test of your priority queues, you have to add code
 * to do more testing!
 *
 * You do not have to submit this file, but it won't cause problems if you do.
 */

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "Eecs281PQ.h"
#include "PairingPQ.h"

using namespace std;


// Used to test a priority queue containing pointers to integers.
struct IntPtrComp {
    bool operator() (const int *a, const int *b) const {
        return *a < *b;
    }
};

// TODO: Add more code to this function to test if updatePriorities()
// is working properly.
void testUpdatePrioritiesHelper(Eecs281PQ<int *, IntPtrComp> *pq) {
    vector<int> data;
    data.reserve(100);
    data.push_back(1);
    data.push_back(5);
    data.push_back(7);
    data.push_back(2);
    data.push_back(4);

    // NOTE: If you add more data to the vector, don't push the pointers
    // until AFTER the vector stops changing size!  Think about why.
    // You can add up to 100 values, or change the reserve if you want more.

    for (size_t i = 0; i < data.size(); ++i) {
        pq->push(&data[i]);
    } // for

    // Change the first value (which is pointed to by the pq), and check it.
    data[0] = 10;
    pq->updatePriorities();
    assert(*pq->top() == 10);
} // testUpdatePrioritiesHelper()


// TODO: Add more code to this function to test if updatePriorities()
// is working properly.
void testUpdatePriorities() {
    Eecs281PQ<int *, IntPtrComp> *pq = nullptr;
    cout << "Testing updatePriorities() on pairing" << endl;
    
    pq = new PairingPQ<int *, IntPtrComp>;
    
    if (!pq) {
        cout << "Invalid pq pointer; did you forget to create it?" << endl;
        return;
    } // if

    testUpdatePrioritiesHelper(pq);
    delete pq;
} // testUpdatePriorities()`


// Very basic testing.
void testPriorityQueue(Eecs281PQ<int> *pq) {
    cout << "Testing priority queue: pairing" << endl;

    pq->push(3);
    pq->push(4);
    assert(pq->size() == 2);
    assert(pq->top() == 4);

    pq->pop();
    assert(pq->size() == 1);
    assert(pq->top() == 3);
    assert(!pq->empty());

    pq->pop();
    assert(pq->size() == 0);
    assert(pq->empty());

    // TODO: Add more testing here!

    cout << "testPriorityQueue() succeeded!" << endl;
} // testPriorityQueue()


// Test the pairing heap's range-based constructor, copy constructor,
// and operator=().
//
void testPairing(vector<int> & vec) {
    cout << "Testing Pairing Heap separately" << endl;
    Eecs281PQ<int> * pq1 = new PairingPQ<int>(vec.begin(), vec.end());
    Eecs281PQ<int> * pq2 = new PairingPQ<int>(*((PairingPQ<int> *)pq1));
    // This line is different just to show two different ways to declare a
    // pairing heap: as an Eecs281PQ and as a PairingPQ. Yay for inheritance!
    PairingPQ<int> * pq3 = new PairingPQ<int>();
    PairingPQ<int> * pq4 = new PairingPQ<int>();

    pq1->push(3);
    pq2->pop();
    assert(pq1->size() == 3);
    assert(!pq1->empty());
    assert(pq1->top() == 3);

    cout << "Basic tests done." << endl;
    // TODO: Add more code to test addNode(), updateElt(), etc.
    
    cout << "Testing Add Node" << endl;
    for (int i = 0; i < 10000; ++i) {
        PairingPQ<int>::Node * n = pq4->addNode(i);
        if (n->getElt() != pq4->top()) {
            exit(1);
        }
        //cout << (n->getElt()==pq4->top()) << '\n';
        assert(n->getElt() == pq4->top());
    }
    
    cout << "Testing = operator" << endl;
    *pq3 = *pq4; 
    for (int i = 0; i < 10000; ++i) {
        int x = pq3->top(); 
        if (x != 10000 - i - 1) {
            cerr << "x: " << x << " != " << 10000 - i - 1 << '\n';
            exit(1);
        }
        // assert(pq3->top() == 1000000 - i - 1);
        pq3->pop();
    }
    assert(pq3->empty()); 
    
    cout << "Testing Update Elt" << endl;
    int i = 2; 
    PairingPQ<int>::Node * n = pq4->addNode(i);
    pq4->updateElt(n, 10000);
    cout << pq4->top() << '\n'; 
    pq4->updateElt(n, 20000);
    cout << pq4->top() << '\n'; 
    n = pq4->addNode(i);
    pq4->updateElt(n, 50);
    cout << pq4->top() << '\n'; 
    
    cout << "Calling destructors" << endl;
    delete pq1;
    delete pq2;
    delete pq3;
    delete pq4;

    cout << "testPairing() succeeded" << endl;
} // testPairing()


int main() {
    // Basic pointer, allocate a new PQ later based on user choice.
    Eecs281PQ<int> *pq;

    cout << "PQ tester" << endl << endl;
    pq = new PairingPQ<int>;
    testPriorityQueue(pq);
    testUpdatePriorities();

    vector<int> vec;
    vec.push_back(0);
    vec.push_back(1);
    testPairing(vec);

    // Clean up!
    delete pq;

    return 0;
} // main()
