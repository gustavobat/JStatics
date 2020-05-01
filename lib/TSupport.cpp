/** \file TSupport.cpp
* Contains the definitions of the TSupport methods.
*/

#include "TStructure.h"
#include "TSupport.h"

//! Default constructor.
TSupport::TSupport(TStructure* Structure, const bool Fx, const bool Fy,
                   const bool M, const int NodeID) {
    fStructure = Structure;
    fConditions[0] = Fx;
    fConditions[1] = Fy;
    fConditions[2] = M;
    fNodeID = NodeID;
}

//! Copy constructor.
TSupport::TSupport(const TSupport& Other) {
    fStructure = Other.fStructure;
    fConditions[0] = Other.fConditions[0];
    fConditions[1] = Other.fConditions[1];
    fConditions[2] = Other.fConditions[2];
    fNodeID = Other.fNodeID;
}

//! Destructor.
TSupport::~TSupport() = default;

//! Assignment operator.
TSupport& TSupport::operator=(const TSupport& Other) {
    if (this != &Other) {
        fStructure = Other.fStructure;
        fConditions[0] = Other.fConditions[0];
        fConditions[1] = Other.fConditions[1];
        fConditions[2] = Other.fConditions[2];
        fNodeID = Other.fNodeID;
    }
    return *this;
}

//! Gets the Fx support condition.
bool TSupport::RestrictsFx() const {
    return fConditions[0];
}

//! Gets the Fy support condition.
bool TSupport::RestrictsFy() const {
    return fConditions[1];
}

//! Gets the M support condition.
bool TSupport::RestrictsM() const {
    return fConditions[2];
}

//! Gets the array of support conditions.
bool* TSupport::getConditions() {
    return fConditions;
}

//! Gets the node of the support.
int TSupport::getNodeID() const {
    return fNodeID;
}

//! Modifies the Fx support condition.
void TSupport::setFx(const bool Fx) {
    fConditions[0] = Fx;
}

//! Modifies the Fy support condition.
void TSupport::setFy(const bool Fy) {
    fConditions[1] = Fy;
}

//! Modifies the M support condition.
void TSupport::setM(const bool M) {
    fConditions[2] = M;
}

//! Modifies the node of the support.
void TSupport::setNodeID(const int NodeID) {
    fNodeID = NodeID;
}

//! Modifies the parent TStructure object.
void TSupport::setStructure(TStructure * Structure) {
    fStructure = Structure;
}

//! Prints the support information to std::cout.
void TSupport::print() {
    std::cout << "Support Info: " << std::endl
              << " Fx: " << fConditions[0] << std::endl
              << " Fy: " << fConditions[1] << std::endl
              << " M: " << fConditions[2] << std::endl
              << " Node ID: " << fNodeID << std::endl;
    std::cout << std::flush;
}
