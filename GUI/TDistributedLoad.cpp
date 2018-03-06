/** \file TDistributedLoad.cpp
* Contains the definition of the TDistributedLoad methods.
*/

#include "TStructure.h"
#include "TDistributedLoad.h"

//! Default constructor.
TDistributedLoad::TDistributedLoad(TStructure* Structure, int ElementID,
	double Node0Load, double Node1Load, bool LoadPlane)
	: fStructure(Structure),
	fElementID(ElementID),
	fNode0Load(Node0Load),
	fNode1Load(Node1Load),
	fLoadPlane(LoadPlane) {}

//! Copy constructor.
TDistributedLoad::TDistributedLoad(const TDistributedLoad& Other)
	: fStructure(Other.fStructure),
	fElementID(Other.fElementID),
	fNode0Load(Other.fNode0Load),
	fNode1Load(Other.fNode1Load),
	fLoadPlane(Other.fLoadPlane) {}

//! Destructor.
TDistributedLoad::~TDistributedLoad() {}

//! Assignment operator.
TDistributedLoad& TDistributedLoad::operator=(const TDistributedLoad& Other)
{
	if (this != &Other) {
		fStructure = Other.fStructure;
		fElementID = Other.fElementID;
		fNode0Load = Other.fNode0Load;
		fNode1Load = Other.fNode1Load;
		fLoadPlane = Other.fLoadPlane;
	}
	return *this;
}

//! Gets the ID of the element to which the load is applied.
int TDistributedLoad::getElementID() const
{
	return fElementID;
}

//! Gets the load value at the element local node 0.
double TDistributedLoad::getNode0Load() const
{
	return fNode0Load;
}

//! Gets the load value at the element local node 1.
double TDistributedLoad::getNode1Load() const
{
	return fNode1Load;
}

//! Gets the boolean associated with the load direction.
bool TDistributedLoad::getLoadPlane() const
{
	return fLoadPlane;
}

//! Modifies the ID of the element to which the load is applied.
void TDistributedLoad::setElementID(int ElementID)
{
	fElementID = ElementID;
}

//! Modifies the load value at the element local node 0.
void TDistributedLoad::setNode0Load(double Node0Load)
{
	fNode0Load = Node0Load;
}

//! Modifies the load value at the element local node 1.
void TDistributedLoad::setNode1Load(double Node1Load)
{
	fNode1Load = Node1Load;
}

//! Modifies the boolean associated with the plane of the load.
void TDistributedLoad::setLoadPlane(bool LoadPlane)
{
	fLoadPlane = LoadPlane;
}

//! Modifies the parent TStructure object.
void TDistributedLoad::setStructure(TStructure* Structure)
{
	fStructure = Structure;
}

//! Adds the effects of the distributed load to the initial load vector Q0.
void TDistributedLoad::store()
{
	// Gets required element data.
	double lx = fStructure->getElement(this->fElementID).getCos();
	double L = fStructure->getElement(this->fElementID).getL();
	double node0Load = fNode0Load;
	double node1Load = fNode1Load;

	TPZFMatrix<double> Q0(6, 1, 0);
	if (fLoadPlane == true)
	{
		// If the load plane is the global plane.
		double ly = fStructure->getElement(this->fElementID).getSin();
		Q0(0, 0) -= ly * (7 * node0Load + 3 * node1Load) * L / 20;
		Q0(1, 0) -= lx * (7 * node0Load + 3 * node1Load) * L / 20;
		Q0(2, 0) -= lx * ((node0Load / 20) + (node1Load / 30)) * L * L;
		Q0(3, 0) -= ly * (3 * node0Load + 7 * node1Load) * L / 20;
		Q0(4, 0) -= lx * (3 * node0Load + 7 * node1Load) * L / 20;
		Q0(5, 0) += lx * ((node0Load / 30) + (node1Load / 20)) * L * L;
	}
	else
	{
		// If the load plane is the element local plane.
		Q0(1, 0) -= (7 * node0Load + 3 * node1Load) * L / 20;
		Q0(2, 0) -= ((node0Load / 20) + (node1Load / 30)) * L * L;
		Q0(4, 0) -= (3 * node0Load + 7 * node1Load) * L / 20;
		Q0(5, 0) += ((node0Load / 30) + (node1Load / 20)) * L * L;
	}

	fStructure->getElementAddress(this->fElementID).setQ0(Q0);
}

//! Prints the load information into std::cout.
void TDistributedLoad::print()
{
	std::cout << "Distributed Load Info: " << std::endl
		<< " Element ID: " << fElementID << std::endl
		<< " Node 0 Load: " << fNode0Load << std::endl
		<< " Node 1 Load: " << fNode1Load << std::endl;
	std::cout << std::flush;
}
