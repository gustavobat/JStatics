/** \file TNodalLoad.cpp
* Contains the definitions of the TNodalLoad methods.
*/

#include "TStructure.h"
#include "TNodalLoad.h"

//! Default constructor.
TNodalLoad::TNodalLoad(TStructure* Structure, double Fx, double Fy, double M,
	int NodeID)
	: fStructure(Structure),
	  fFx(Fx),
	  fFy(Fy),
	  fM(M),
	  fNodeID(NodeID) {}

//! Copy constructor.
TNodalLoad::TNodalLoad(const TNodalLoad& Other)
	: fStructure(Other.fStructure),
	  fFx(Other.fFx),
	  fFy(Other.fFy),
	  fM(Other.fM),
	  fNodeID(Other.fNodeID) {}

//! Destructor.
TNodalLoad::~TNodalLoad() {}

//! Assignment operator.
TNodalLoad& TNodalLoad::operator=(const TNodalLoad& Other)
{
	if (this != &Other) {
		fStructure = Other.fStructure;
		fFx = Other.fFx;
		fFy = Other.fFy;
		fM = Other.fM;
		fNodeID = Other.fNodeID;
	}
	return *this;
}

//! Gets the horizontal load applied to the node.
double TNodalLoad::getFx() const
{
	return fFx;
}

// Gets the vertical load applied to the node.
double TNodalLoad::getFy() const
{
	return fFy;
}

//! Gets the moment applied to the node.
double TNodalLoad::getM() const
{
	return fM;
}

//! Gets the ID of the node to which the load is applied.
int TNodalLoad::getNodeID() const
{
	return fNodeID;
}

//! Modifies the horizontal load applied to the node.
void TNodalLoad::setFx(double Fx)
{
	fFx = Fx;
}

//! Modifies the vertical load applied to the node.
void TNodalLoad::setFy(double Fy)
{
	fFy = Fy;
}

//! Modifies the moment applied to the node.
void TNodalLoad::setM(double M)
{
	fM = M;
}

//! Modifies the ID of the node to which the load is applied.
void TNodalLoad::setNodeID(int NodeID)
{
	fNodeID = NodeID;
}

//! Modifies the load parent TStructure object.
void TNodalLoad::setStructure(TStructure* Structure)
{
	fStructure = Structure;
}

//! Adds the effects of the nodal load to the vector of loads.
void TNodalLoad::store(TPZFMatrix<double>& Q)
{
	int fxDOF = fStructure->getNodeEquations()(fNodeID, 0);
	int fyDOF = fStructure->getNodeEquations()(fNodeID, 1);
	int mDOF = fStructure->getNodeEquations()(fNodeID, 2);

	Q(fxDOF, 0) += fFx;
	Q(fyDOF, 0) += fFy;
	if (mDOF != -1) {
		// Adds the moment load if the node is not hinged.
		Q(mDOF, 0) += fM;
	}
}

//! Prints the load information.
void TNodalLoad::print()
{
	std::cout << "Nodal Load Info: " << std::endl
		<< " Fx: " << fFx << std::endl
		<< " Fy: " << fFy << std::endl
		<< " M: " << fM << std::endl
		<< " Node ID: " << fNodeID << std::endl;
	std::cout << std::flush;
}
