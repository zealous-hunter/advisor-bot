#pragma once
#include <fstream>
#include <vector>
#include <string>

#include "step.h"

class StepIterator;

/* Represents the data of the simulation. Using lazy loading to speed up the bootstrap. */
class OrderBook
{
public:
    /* empty orderbook */
    OrderBook();
    /* load orderbook from csv file */
    OrderBook(std::string csvFileName);
    /* Construct from a csv file */
    ~OrderBook();

    /* Load the orderbook from a csv file */
    void open(std::string csvFileName);

    /** copy constructor */
    OrderBook(const OrderBook &other);
    /** copy assigment operator */
    OrderBook &operator=(const OrderBook &other);
    /** move constructor */
    OrderBook(OrderBook &&other);
    /** move assignment operator */
    OrderBook &operator==(OrderBook &&other);

    /**
     * @brief Access element at specified index
     * @param index The index of the element
     * @throws out_of_range
     */
    Step &at(size_t index);

    /** return a new iterator set at the beginning of the data */
    StepIterator begin();
    /** iterator points to the pos after the last record */
    const StepIterator &end();
    /** get previous steps from the iterator */
    std::vector<Step> getPreviousSteps(StepIterator it, size_t maxSteps);

private:
    std::string csvFileName;
    mutable std::fstream file;
    std::vector<Step> buf;
    void copy(const OrderBook &other);
    void swap(OrderBook &&other);
    void update();
    virtual bool isProcessed() const;
    void readNextStep();
    bool isBeyondEnd(size_t index) const;

    friend class StepIterator;
};

/**
 * @brief Iterator for OrderBook
 */
class StepIterator
{
public:
    /* default constructor */
    StepIterator();
    /* construct from orderbook */
    StepIterator(OrderBook *orderBook);
    /* copy constructor */
    StepIterator(const StepIterator &other);
    /* move constructor */
    StepIterator(StepIterator &&other);

    /* copy assignment operator */
    StepIterator &operator=(const StepIterator &other);
    /* move assignment operator */
    StepIterator &operator=(StepIterator &&other);

    Step &operator*();
    Step *operator->();

    StepIterator &operator++();
    StepIterator operator++(int);
    StepIterator &operator--();
    StepIterator operator--(int);

    bool equals(const StepIterator &other) const;

protected:
    StepIterator(size_t pos);

private:
    OrderBook *orderBook;
    size_t pos;

    void swap(StepIterator &&other);
    void copy(const StepIterator &other);

    friend class OrderBook;
};

bool operator==(const StepIterator &lhs, const StepIterator &rhs);
bool operator!=(const StepIterator &lhs, const StepIterator &rhs);