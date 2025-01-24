#include <map>
#include <random>
#include <sstream>
#include <string>
#include <stdexcept>

#include "catch.hpp"
#include "Treap.hpp"



// Test case for basic insertion and search operations
TEST_CASE("Treap Insertion and Search", "[treap]") {
    Treap<int, std::string> treap;
    REQUIRE(treap.empty()); // Treap should be initially empty

    treap.insert(50, "Root");
    bool found;
    std::string value = treap.search(50, found);
    REQUIRE(found);           // The key 50 should be found
    REQUIRE(value == "Root"); // The associated value should be "Root"

    treap.insert(30, "Left Child");
    treap.insert(70, "Right Child");
    value = treap.search(30, found);
    REQUIRE(found);
    REQUIRE(value == "Left Child");
    value = treap.search(70, found);
    REQUIRE(found);
    REQUIRE(value == "Right Child");

    // Test searching for a non-existent key
    value = treap.search(100, found);
    REQUIRE_FALSE(found); // The key 100 should not be found
}

// Test case for checking the height of the treap
TEST_CASE("Treap Height", "[treap]") {
    Treap<int, int> treap;
    REQUIRE(treap.height() == 0); // Height of an empty treap should be 0

    treap.insert(10, 100);
    REQUIRE(treap.height() == 1); // Height should be 1 after one insertion

    treap.insert(5, 50);
    treap.insert(15, 150);
    //REQUIRE(treap.height() == 2); // Height should be 2 after three insertions

    treap.insert(2, 20);
    treap.insert(7, 70);
    //REQUIRE(treap.height() >= 3); (These tests are inconsistent at times, so they've been commented out for consistency)
    //REQUIRE(treap.height() <= 4);
}

// Test case for removal operations
TEST_CASE("Treap Removal", "[treap]") {
    Treap<int, std::string> treap;
    treap.insert(40, "Forty");
    treap.insert(20, "Twenty");
    treap.insert(60, "Sixty");
    treap.insert(10, "Ten");
    treap.insert(30, "Thirty");

    bool found;
    std::string value = treap.search(20, found);
    REQUIRE(found);
    REQUIRE(value == "Twenty");

    treap.remove(20); // Remove the key 20
    value = treap.search(20, found);
    REQUIRE_FALSE(found); // The key 20 should no longer be found

    // Attempt to remove a non-existent key
    REQUIRE_THROWS_AS(treap.remove(100), std::invalid_argument);
}

// Test case for copy constructor and assignment operator
TEST_CASE("Treap Copy Constructor and Assignment", "[treap]") {
    Treap<int, std::string> treap1;
    treap1.insert(1, "One");
    treap1.insert(2, "Two");
    treap1.insert(3, "Three");

    // Test copy constructor
    Treap<int, std::string> treap2 = treap1;
    bool found;
    std::string value = treap2.search(2, found);
    REQUIRE(found);
    REQUIRE(value == "Two");

    // Test assignment operator
    Treap<int, std::string> treap3;
    treap3 = treap1;
    value = treap3.search(3, found);
    REQUIRE(found);
    REQUIRE(value == "Three");
}

// Test case for swapping two treaps
TEST_CASE("Treap Swap", "[treap]") {
    Treap<int, int> treap1;
    treap1.insert(1, 100);
    treap1.insert(2, 200);

    Treap<int, int> treap2;
    treap2.insert(3, 300);
    treap2.insert(4, 400);

    // Swap treap1 with treap2 by passing both as arguments
    treap1.swap(treap1, treap2);

    bool found;
    int value = treap1.search(3, found);
    REQUIRE(found);
    REQUIRE(value == 300); // treap1 should now contain elements from treap2

    value = treap2.search(1, found);
    REQUIRE(found);
    REQUIRE(value == 100); // treap2 should now contain elements from treap1
}

// Test case for attempting duplicate insertions
TEST_CASE("Treap Duplicate Insertion", "[treap]") {
    Treap<int, std::string> treap;
    treap.insert(10, "Ten");
    // Attempt to insert a duplicate key
    REQUIRE_THROWS_AS(treap.insert(10, "Duplicate Ten"), std::invalid_argument);
}

// Test case for multiple insertions and removals
TEST_CASE("Treap Multiple Insertions and Removals", "[treap]") {
    Treap<int, int> treap;
    // Insert 100 elements
    for (int i = 1; i <= 100; ++i) {
        treap.insert(i, i * 10);
    }
    REQUIRE(treap.height() <= 100); // Height should not exceed 100

    // Remove every odd-numbered key
    for (int i = 1; i <= 100; i += 2) {
        treap.remove(i);
    }

    // Verify that only even-numbered keys remain
    for (int i = 1; i <= 100; ++i) {
        bool found;
        int value = treap.search(i, found);
        if (i % 2 == 1) {
            REQUIRE_FALSE(found); // Odd keys should have been removed
        }
        else {
            REQUIRE(found);          // Even keys should still be present
            REQUIRE(value == i * 10); // Verify associated values
        }
    }
}
