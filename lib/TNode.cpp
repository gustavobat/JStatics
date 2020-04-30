/** \file TNode.cpp
* Contains the definitions of the TNode methods.
*/

#include "TStructure.h"
#include "TNode.h"

//! Default constructor.
TNode::TNode(double X, double Y) {
    fCoordinates[0] = X;
    fCoordinates[1] = Y;
}

//! Copy constructor.
TNode::TNode(const TNode& Other) {
    fCoordinates[0] = Other.fCoordinates[0];
    fCoordinates[1] = Other.fCoordinates[1];
}

//! Destructor.
TNode::~TNode() {}

//! Assignment operator.
TNode& TNode::operator=(const TNode& Other) {
    if (this != &Other) {
        fCoordinates[0] = Other.fCoordinates[0];
        fCoordinates[1] = Other.fCoordinates[1];
    }
    return *this;
}

//! Gets the node x coordinate.
double TNode::getX() const {
    return fCoordinates[0];
}

//! Gets the node y coordinate.
double TNode::getY() const {
    return fCoordinates[1];
}

//! Gets the array of coordinates of the node.
double* const TNode::getCoord() {
    return fCoordinates;
}

//! Modifies the node x coordinate.
void TNode::setX(double X) {
    fCoordinates[0] = X;
}

//! Modifies the node y coordinate.
void TNode::setY(double Y) {
    fCoordinates[1] = Y;
}

//! Modifies the array of coordinates of the node.
void TNode::setCoord(double* Coordinates) {
    fCoordinates[0] = Coordinates[0];
    fCoordinates[1] = Coordinates[1];
}

//! Prints the node information to std::cout.
void TNode::print() {
    std::cout << "Node Info:" << std::endl
              << " X: " << fCoordinates[0] << std::endl
              << " Y: " << fCoordinates[1] << std::endl;
    std::cout << std::flush;
}
