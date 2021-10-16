// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the 'priority_queue' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // TODO: After you add add one extra pointer (see below), be sure to
            // initialize it here.
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }, parent{ nullptr }
            {}

            // Description: Allows access to the element at that Node's position.
			// There are two versions, getElt() and a dereference operator, use
			// whichever one seems more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data members of this
            // Node class from within the PairingPQ class. (ie: myNode.elt is a legal
            // statement in PairingPQ's add_node() function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            Node *parent; 
            // Add one extra pointer (parent or previous) as desired.
    }; // Node


    // Description: Construct an empty priority_queue with an optional comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root{ nullptr }, numNodes{ 0 } {
        // Implement this function.
    } // PairingPQ()


    // Description: Construct a priority_queue out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root{ nullptr }, numNodes{ 0 } {
            for (; start != end; ++start) {
                push(*start); 
            }
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare }, root{ nullptr }, numNodes{ 0 } {
            if (other.root != nullptr) {
                std::deque<Node *> d;
                d.push_back(other.root); 
                while (!d.empty()) {
                    Node * temp = d.front();
                    d.pop_front();
                    if (temp->child)    
                        d.push_back(temp->child);
                    if (temp->sibling)
                        d.push_back(temp->sibling);
                    push(temp->elt); 
                }
            }
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    // when you implement this function, uncomment the parameter names.
    PairingPQ &operator=(const PairingPQ &rhs) {
        PairingPQ temp(rhs);
        std::swap(numNodes, temp.numNodes);
        std::swap(root, temp.root); 
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        if (root != nullptr) {
            std::deque<Node *> d;
            d.push_back(root); 
            while (!d.empty()) {
                Node * temp = d.front();
                d.pop_front();
                if (temp->child)    
                    d.push_back(temp->child);
                if (temp->sibling)
                    d.push_back(temp->sibling);
                delete temp;
            }
            root = nullptr; 
        }
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the priority_queue are out of order and
    //              'rebuilds' the priority_queue by fixing the priority_queue invariant.
    //              You CANNOT delete 'old' nodes and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        std::deque<Node *> d; 
        Node * curr = root; 
        d.push_back(curr); 
        root = nullptr; 
        while (!d.empty()) {
            curr = d.front(); 
            d.pop_front();
            if (curr->child) 
                d.push_back(curr->child); 
            if (curr->sibling) 
                d.push_back(curr->sibling); 
            curr->child = nullptr;
            curr->sibling = nullptr; 
            if (root == nullptr) 
                root = curr; 
            else 
               root = meld(root, curr);
        }
    } // updatePriorities()


    // Description: Add a new element to the priority_queue. This is already done.
    //              You should implement push functionality entirely in the addNode()
    //              function, and this function calls addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the priority_queue.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the priority_queue is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        // Implement this function.
        if (numNodes == 1) {
            delete root; 
            numNodes = 0; 
            root = nullptr; 
        }
        else {
            Node * temp = root->child; 
            temp->parent = nullptr;
            delete root; 
            root = nullptr;
            --numNodes;  
            std::deque<Node *> d; 
            while (temp != nullptr) {
                d.push_back(temp); 
                Node * t = temp;              
                temp = temp->sibling; 
                t->sibling = nullptr; 
            }
            while (d.size() > 1) {
                d.push_back(meld(d[0], d[1])); 
                d.pop_front(); 
                d.pop_front();
            }
            root = d.front(); 
        }
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        return root->elt;
    } // top()


    // Description: Get the number of elements in the priority_queue.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // Implement this function
        return numNodes; 
    } // size()

    // Description: Return true if the priority_queue is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // Implement this function
        return numNodes == 0; 
    } // empty()


    // Description: Updates the priority of an element already in the priority_queue by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain priority_queue invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    // TODO: when you implement this function, uncomment the parameter names.
    void updateElt(Node* node, const TYPE &new_value) {
        node->elt = new_value;
        if (node != root) {
            if (this->compare(node->parent->elt, new_value)) {
                Node * prev = node->parent->child;
                if (prev == node) {
                    node->parent->child = node->sibling; 
                }
                else{ 
                    while (prev->sibling != node) {
                        prev = prev->sibling;
                    }
                    prev->sibling = node->sibling; 
                }
                node->parent = nullptr;
                node->sibling = nullptr; 
                root = meld(root, node);
            } 
        }    
    } // updateElt()


    // Description: Add a new element to the priority_queue. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    // NOTE: Whenever you create a node, and thus return a Node *, you must be sure to
    //       never move or copy/delete that node in the future, until it is eliminated
    //       by the user calling pop().  Remember this when you implement updateElt() and
    //       updatePriorities().
    Node* addNode(const TYPE &val) {
        // Implement this function
        Node * n = new Node(val); 
        if (numNodes == 0)
            root = n; 
        else 
            root = meld(root, n); 
        ++numNodes;
        return n;
    } // addNode()


private:
    // TODO: Add any additional member functions or data you require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap papers).
    Node * root;
    std::size_t numNodes; 
    
    Node* meld(Node *pa, Node *pb) {
        if(this->compare(pa->elt, pb->elt)) {
            pa->parent = pb; 
            if (pb->child) 
                pa->sibling = pb->child;
            pb->child = pa; 
            return pb; 
        }
        else {
            pb->parent = pa; 
            if (pa->child) 
                pb->sibling = pa->child;
            pa->child = pb; 
            return pa; 
        }
    }
};


#endif // PAIRINGPQ_H
