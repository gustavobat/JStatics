/** \file TNodalLoad.h
* Contains the declaration of the TNodalLoad  class.
*/

#ifndef TNODALLOAD_H
#define TNODALLOAD_H

#include <iostream>

// Forward declaration to TStructure class.
class TStructure;

//!  A class that implements the nodal loads.
/*!
     A class that implements the nodal loads.
	 An object of this class has a different value for each load direction
	 (Fx, Fy and M) applied to the same node.
*/
class TNodalLoad {
public:
	//! Default constructor.
	TNodalLoad(TStructure* Structure = nullptr, double Fx = 0, double Fy = 0,
		double M = 0, int NodeID = -1);
	//! Copy constructor.
	TNodalLoad(const TNodalLoad& Other);
	//! Destructor.
	~TNodalLoad();
	//! Assignment operator.
	TNodalLoad& operator=(const TNodalLoad& Other);

	//! Gets the horizontal load applied to the node.
	double getFx() const;
	// Gets the vertical load applied to the node.
	double getFy() const;
	//! Gets the moment applied to the node.
	double getM() const;
	//! Gets the ID of the node to which the load is applied.
	int getNodeID() const;

	//! Modifies the horizontal load applied to the node.
	void setFx(double Fx);
	//! Modifies the vertical load applied to the node.
	void setFy(double Fy);
	//! Modifies the moment applied to the node.
	void setM(double M);
	//! Modifies the ID of the node to which the load is applied.
	void setNodeID(int NodeID);
	//! Modifies the load parent TStructure object.
	void setStructure(TStructure* Structure);

	//! Adds the effects of the nodal load to the vector of loads.
	void store(TPZFMatrix<double>& Q);

	//! Prints the load information.
	void print();

private:
	// Member variables:
	TStructure *fStructure;
	double fFx;
	double fFy;
	double fM;
	int fNodeID;
};

#endif // TNODALLOAD_H
