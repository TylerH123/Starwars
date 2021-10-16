// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include "Eecs281PQ.h"

// A specialized version of the 'heap' ADT implemented as a binary heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty heap with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function.
    } // BinaryPQ


    // Description: Construct a heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } , data{ start, end } {
            updatePriorities(); 
        // Implement this function.
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automaticslly.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)   
    virtual void updatePriorities() {
        for (std::size_t i = data.size(); i > 0; --i) {
            fixDown(i, data.size());
        }
    }
    
    // Description: Add a new element to the heap.
    // Runtime: O(log(n))
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE &val) {
        // TODO: Implement this function.
        data.push_back(val);
        fixUp(data.size()); 
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop() {
        std::swap(data.front(), data.back()); 
        data.pop_back(); 
        fixDown(1, data.size()); 
        // Implement this function.
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // Implement this function.

        // These lines are present only so that this provided file compiles.
        return data.front();
    } // top()


    // Description: Get the number of elements in the heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // implement this function.  Might be very simple,
        // depending on your implementation.
        return data.size(); // TODO: Delete or change this line
    } // size()


    // Description: Return true if the heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function.  Might be very simple,
        // depending on your implementation.
        return data.empty(); // TODO: Delete or change this line
    } // empty()


private:
    // Note: This vector *must* be used your heap implementation.
    std::vector<TYPE> data;
    
    // TODO: Add any additional member functions or data you require here.
    // For instance, you might add fixUp() and fixDown().
    
    TYPE &getElement(std::size_t i) {
        return data[i-1]; 
    }
    
    const TYPE &getElement(std::size_t i) const {
        return data[i-1];
    }
    
    void fixUp(std::size_t idx) { 
        while (idx - 1 > 0 && this->compare(getElement(idx / 2), getElement(idx))) { 
            std::swap(getElement(idx), getElement(idx / 2)); 
            idx /= 2; 
        }
    }
    
    void fixDown(std::size_t idx, std::size_t size) {
        while (2 * idx <= size) {
            std::size_t child = 2 * idx; 
            if (child + 1 <= size && this->compare(getElement(child),getElement(child+1))) 
                ++child;
            if (this->compare(getElement(idx),getElement(child))) {
                std::swap(getElement(idx), getElement(child)); 
                idx = child; 
            }
            else {
                idx = size; 
            }
        }
    }

}; // BinaryPQ


#endif // BINARYPQ_H
