/** \file TElementEndMoment.cpp
* Contains the definitions of the TElementEndMoment methods.
*/

#include "TStructure.h"
#include "TElementEndMoment.h"

//! Default constructor.
TElementEndMoment::TElementEndMoment(TStructure* Structure, double M,
                                     int ElementID, int Node)
    : fM(M),
      fElementID(ElementID),
      fNode(Node),
      fStructure(Structure) {}

//! Copy constructor.
TElementEndMoment::TElementEndMoment(const TElementEndMoment& Other)
    : fM(Other.fM),
      fElementID(Other.fElementID),
      fNode(Other.fNode),
      fStructure(Other.fStructure) {}

//! Destructor.
TElementEndMoment::~TElementEndMoment() {}

//! Assignment operator.
TElementEndMoment& TElementEndMoment::operator=(const TElementEndMoment& Other)
{
    if (this != &Other) {
        fM = Other.fM;
        fElementID = Other.fElementID;
        fNode = Other.fNode;
        fStructure = Other.fStructure;
    }
    return *this;
}

//! Returns the moment applied to the load.
double TElementEndMoment::getM() const
{
    return fM;
}

//! Returns the ID of the element to which the load is applied.
int TElementEndMoment::getElementID() const
{
    return fElementID;
}

//! Returns the local ID of the node to which the load is applied.
int TElementEndMoment::getNode() const
{
    return fNode;
}

//! Modifies the moment applied to the load.
void TElementEndMoment::setM(double M)
{
    fM = M;
}

//! Modifies the ID of the element to which the load is applied.
void TElementEndMoment::setElementID(int ElementID)
{
    fElementID = ElementID;
}

//! Modifies the local ID of the node to which the load is applied.
void TElementEndMoment::setNode(int Node)
{
    fNode = Node;
}

//! Modifies the element parent TStructure object.
void TElementEndMoment::setStructure(TStructure* Structure)
{
    fStructure = Structure;
}

//! Adds the effects of the nodal load to the vector of loads.
void TElementEndMoment::store(TPZFMatrix<double>& Q)
{
    int nodeID;

    if (this->fNode == 0) {
        // If the load is at the element node 0.
        nodeID = fStructure->getElement(this->fElementID).getNode0ID();
    }
    else if (this->fNode == 1) {
        // If the load is at the element node 0.
        nodeID = fStructure->getElement(this->fElementID).getNode1ID();
    }
    else {
        // Stops debug if fNode is neither 0 nor 1.
        DebugStop();
    }

    int mDOF = fStructure->getNodeEquations()(nodeID, 2);
    if (mDOF != -1) {
        // Adds the moment load if the node is not hinged.
        Q(mDOF, 0) += this->fM;
    }
}

//! Prints the load information to std::cout.
void TElementEndMoment::print()
{
    std::cout << "Element End Moment Info: " << std::endl
              << " Element ID: " << fElementID << std::endl
              << " Node: " << fNode << std::endl
              << " M: " << fM << std::endl;
    std::cout << std::flush;
}
