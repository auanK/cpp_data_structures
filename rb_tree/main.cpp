#include <iostream>
#include "red_black_tree.h"

using namespace std;

int main() {
    red_black_tree<int> tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(15);
    tree.insert(25);
    tree.insert(5);


    tree.print();
}