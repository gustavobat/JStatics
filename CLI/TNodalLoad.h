/** \file TNodalLoad.h
* Contains the declaration of the TNodalLoad class.
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
	/*!
	\param Structure a pointer to the parent TStructure object.
	\param Fx value of the horizontal force.
	\param Fy value of the vertical force.
	\param M value of the moment load.
	\param NodeID ID of the node to which the load is applied.
	\return the new TNodalLoad object.
	*/
	TNodalLoad(TStructure* Structure = nullptr, double Fx = 0, double Fy = 0,
		double M = 0, int NodeID = -1);
	//! Copy constructor.
	/*!
	\param Other the TNodalLoad object to be copied.
	\return the new TNodalLoad object.
	*/
	TNodalLoad(const TNodalLoad& Other);
	//! Destructor.
	~TNodalLoad();

	//! Assignment operator.
	/*!
	\param Other the TNodalLoad object to be copied.
	\return the modified TNodalLoad object.
	*/
	TNodalLoad& operator=(const TNodalLoad& Other);

	//! Gets the horizontal load applied to the node.
	/*!
	\return the value of the horizontal force.
	*/
	double getFx() const;

	//! Gets the vertical load applied to the node.
	/*!
	\return the value of the vertical force.
	*/
	double getFy() const;

	//! Gets the moment applied to the node.
	/*!
	\return the value of the moment load.
	*/
	double getM() const;

	//! Gets the ID of the node to which the load is applied.
	/*!
	\return the ID of the node to which the load is applied.
	*/
	int getNodeID() const;

	//! Modifies the horizontal load applied to the node.
	/*!
	\param Fx the new value of the horizontal force.
	*/
	void setFx(double Fx);

	//! Modifies the vertical load applied to the node.
	/*!
	\param Fy the new value of the vertical force.
	*/
	void setFy(double Fy);

	//! Modifies the moment applied to the node.
	/*!
	\param M the new value of the moment load.
	*/
	void setM(double M);

	//! Modifies the ID of the node to which the load is applied.
	/*!
	\param NodeID the ID of the new node to which the load is applied.
	*/
	void setNodeID(int NodeID);

	//! Modifies the load parent TStructure object.
	/*!
	\param Structure the pointer to the new parent TStructure object.
	*/
	void setStructure(TStructure* Structure);

	//! Adds the effects of the nodal load to the vector of loads.
	/*!
	\param Q the addres to the vector of loads Q to be modified.
	*/
	void store(TPZFMatrix<double>& Q);

	//! Prints the load information to std::cout.
	void print();

private:
	//! A pointer to the parent TStructure object.
	TStructure *fStructure;
	//! The value of the horizontal force.
	double fFx;
	//! The value of the vertical force.
	double fFy;
	//! The value of the moment load.
	double fM;
	//! The ID of the node to which the load is applied.
	int fNodeID;
};

#endif // TNODALLOAD_H
