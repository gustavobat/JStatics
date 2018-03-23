/* This file was created by Gustavo BATISTELA.
It is a header file containing the definition of the TSupport class
and the declaration of its members. */

#ifndef TSUPPORT_H
#define TSUPPORT_H

#include <iostream>
#include "TNode.h"

// Forward declaration to TStructure class.
class TStructure;

// TSupport class and declarations of its functions.
class TSupport {
public:
	// Default constructor.
	TSupport(TStructure* Structure = nullptr, const bool Fx = false, 
			 const bool Fy = false, const bool M = false, const int NodeID = 0);
	// Copy constructor.
	TSupport(const TSupport& Other);
	// Destructor.
	~TSupport();
	// Assignment operator.
	TSupport& operator=(const TSupport& Other);

	// getFx - accesses the Fx DOF of the support.
	bool getFx() const;
	// getFy - accesses the Fy DOF of the support.
	bool getFy() const;
	// getM - accesses the M DOF of the support.
	bool getM() const;
	// getConditions - accesses the vector of suport conditions.
	bool* const getConditions();
	// getNode - accesses the node of the support.
	int getNodeID() const;

	// setFx - modifies the Fx DOF of the support.
	void setFx(const bool Fx);
	// setFy - modifies the Fy DOF of the support.
	void setFy(const bool Fy);
	// setM - modifies the M DOF of the support.
	void setM(const bool M);
	// setNode - modifies the node of the support.
	void setNodeID(const int NodeID);
	// setStructure - modifies the parent TStructure element.
	void setStructure(TStructure* Structure);

	// Function that prints the support information.
	void print();

private:
	// Member variables:
	TStructure* fStructure;
	bool fConditions[3];
	int fNodeID;
};

#endif // TSUPPORT_H