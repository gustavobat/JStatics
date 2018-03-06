/* This file was created by Gustavo BATISTELA.
It contains the definitions of functions of the TSupport class. */

#include "TStructure.h"
#include "TSupport.h"

// Default constructor.
TSupport::TSupport(TStructure* Structure, const bool Fx, const bool Fy, const bool M, const int NodeID) {
	fStructure = Structure;
	fConditions[0] = Fx;
	fConditions[1] = Fy;
	fConditions[2] = M;
	fNodeID = NodeID;
}

// Copy constructor.
TSupport::TSupport(const TSupport& Other) {
	fStructure = Other.fStructure;
	fConditions[0] = Other.fConditions[0];
	fConditions[1] = Other.fConditions[1];
	fConditions[2] = Other.fConditions[2];
	fNodeID = Other.fNodeID;
}

// Destructor.
TSupport::~TSupport() {}

// Assignment operator.
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

// getFx - accesses the Fx DOF of the support.
bool TSupport::getFx() const {
	return fConditions[0]; 
}

// getFy - accesses the Fy DOF of the support.
bool TSupport::getFy() const {
	return fConditions[1]; 
}

// getM - accesses the M DOF of the support.
bool TSupport::getM() const { 
	return fConditions[2];
}

// getConditions - accesses the vector of support conditions.
bool* const TSupport::getConditions() {
	return fConditions; 
}

// getNodeID - accesses the node of the support.
int TSupport::getNodeID() const { 
	return fNodeID; 
}

// setFx - modifies the Fx DOF of the support.
void TSupport::setFx(const bool Fx) { 
	fConditions[0] = Fx; 
}

// setFy - modifies the Fy DOF of the support.
void TSupport::setFy(const bool Fy) {
	fConditions[1] = Fy;
}

// setM - modifies the M DOF of the support.
void TSupport::setM(const bool M) { 
	fConditions[2] = M; 
}

// setNode - modifies the node of the support.
void TSupport::setNodeID(const int NodeID) {
	fNodeID = NodeID;
}

// setStructure - modifies the pointer to the TStructure object.
void TSupport::setStructure(TStructure * Structure) {
	fStructure = Structure;
}

// Function that prints the support information.
void TSupport::print() {
	std::cout << "Support Info: " << std::endl
		<< " Fx: " << fConditions[0] << std::endl
		<< " Fy: " << fConditions[1] << std::endl
		<< " M: " << fConditions[2] << std::endl
		<< " Node ID: " << fNodeID << std::endl;
	std::cout << std::flush;
}