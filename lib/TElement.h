/** \file TElement.h
* Contains the declaration of the TElement class.
*/

#ifndef TELEMENT_H
#define TELEMENT_H

#include <iostream>
#include <math.h>
#include "pzfmatrix.h"
#include "TNode.h"
#include "TMaterial.h"

// Forward declaration to TStructure class.
class TStructure;

//!  A class that implements a 2D frame element.
/*!
     A class that implements a 2D frame element.
	 An object of this class has a pointer to a parent TStructure object,
	 two node IDs, two booleans that marks if the element is hinged at a node
	 (true = hinged) and the ID of its material).
*/
class TElement
{
public:
	//! Default constructor.
	/*!
	\param Structure a pointer to the parent TStructure object.
	\param Node0ID ID of the element node 0.
	\param Node1ID ID of the element node 1.
	\param Hinge0 a bool that marks if the element is hinged at its node 0.
	\param Hinge1 a bool that marks if the element is hinged at its node 1.
	\param MaterialID the ID of the element material.
	\return the new TElement object.
	*/
	TElement(TStructure* Structure = nullptr, const int Node0ID = -1,
		const int Node1ID = -1, const bool Hinge0 = false,
		const bool Hinge1 = false, const int MaterialID = -1);

	//! Copy constructor.
	/*!
	\param Other the TElement object to be copied.
	\return the new TElement object.
	*/
	TElement(const TElement& Other);

	//! Destructor.
	~TElement();

	//! Assignment operator.
	/*!
	\param Other the TElement object to be copied.
	\return the modified TElement object.
	*/
	TElement& operator=(const TElement& Other);

	//! Gets the nodes of the element.
	/*!
	\return an int array with the IDs of nodes 0 and 1 respectively.
	*/
	int* const getLocalNodesIDs();

	//! Gets the ID of the element Node 0.
	/*!
	\return the ID of the element node 0.
	*/
	int getNode0ID() const;

	//! Gets the ID of the element Node 1.
	/*!
	\return the ID of the element node 1.
	*/
	int getNode1ID() const;

	//! Gets the hinge of the element at Node 0.
	/*!
	\return a bool associated with the hinge at node 0.
	*/
	bool getHinge0() const;

	//! Gets the hinge of the element at Node 1.
	/*!
	\return a bool associated with the hinge at node 1.
	*/
	bool getHinge1() const;

	//! Gets the material of the element.
	/*!
	\return the ID of the element material.
	*/
	int getMaterialID() const;

	//! Gets the equations of the element.
	/*!
	\return an int array with the equations (degrees of freedom) of the element.
	*/
	int* const getEquations();

	//! Modifies the element parent TStructure object.
	/*!
	\param Structure a pointer to the new TStructure parent object.
	*/
	void setStructure(TStructure* Structure);

	//! Modifies the IDs of the element nodes.
	/*!
	\param NodesIDs an int array with the new node 0 and 1 IDs respectively.
	*/
	void setLocalNodesIDs(int* NodesIDs);

	//! Modifies the ID of the element node 0.
	/*!
	\param Node0ID the ID of the new node 0.
	*/
	void setNode0ID(int Node0ID);

	//! Modifies the ID of the element node 1.
	/*!
	\param Node0ID the ID of the new node 1.
	*/
	void setNode1ID(int Node1ID);

	//! Modifies the element hinge at node 0.
	/*!
	\param Hinge0 a bool that marks if the element is hinged at node 0.
	*/
	void setHinge0(bool Hinge0);

	//! Modifies the element hinge at node 1.
	/*!
	\param Hinge1 a bool that marks if the element is hinged at node 1.
	*/
	void setHinge1(bool Hinge1);

	//! Modifies the element material.
	/*!
	\param MaterialID the ID of the new material.
	*/
	void setMaterialID(int MaterialID);

	//! Modifies the element equations (degrees of freedom) indexes.
	/*!
	\param Eq0 the index of the horizontal degree of freedom at node 0.
	\param Eq1 the index of the vertical degree of freedom at node 0.
	\param Eq2 the index of the rotation degree of freedom at node 0.
	\param Eq3 the index of the horizontal degree of freedom at node 1.
	\param Eq4 the index of the vertical degree of freedom at node 1.
	\param Eq5 the index of the rotation degree of freedom at node 1.
	*/
	void setEquations(int Eq0, int Eq1, int Eq2, int Eq3, int Eq4, int Eq5);

	//! Gets the x coordinate of the element Node 0.
	/*!
	\return the x coordinate of the element Node 0.
	*/
	double getX0() const;

	//! Gets the y coordinate of the element Node 0.
	/*!
	\return the y coordinate of the element Node 0.
	*/
	double getY0() const;

	//! Gets the x coordinate of the element Node 1.
	/*!
	\return the x coordinate of the element Node 1.
	*/
	double getX1() const;

	//! Gets the y coordinate of the element Node 1.
	/*!
	\return the y coordinate of the element Node 1.
	*/
	double getY1() const;

	//! Gets the length of the element.
	/*!
	\return the length of the element.
	*/
	double getL() const;

	//! Gets the cosine of the element angle.
	/*!
	\return the cosine of the element angle.
	*/
	double getCos() const;

	//! Gets the sine of the element angle.
	/*!
	\return the sine of the element angle.
	*/
	double getSin() const;

	//! Gets the transformation matrix of the element.
	/*!
	\return the transformation matrix of the element.
	*/
	TPZFMatrix<double> getT() const;

	//! Gets the transposed transformation matrix of the element.
	/*!
	\return the transposed form of the transformation matrix of the element.
	*/
	TPZFMatrix<double> getTT() const;

	//! Gets the global stiffness matrix of the element.
	/*!
	\return the stiffness matrix of the element in global coordinates.
	*/
	TPZFMatrix<double> getK() const;

	//! Gets the local stiffness matrix of the element.
	/*!
	\return the stiffness matrix of the element in local coordinates.
	*/
	TPZFMatrix<double> getLocalK() const;

	//! Gets the vector of initial (intermediate) loads Q0 of the element.
	/*!
	\return the vector of initial (intermediate) loads Q0 of the element.
	*/
	TPZFMatrix<double> getQ0() const;

	//! Modifies the vector of initial (intermediate) loads Q0 of the element.
	/*!
	\param Q0 the new vector of initial (intermediate) loads of the element.
	*/
	void setQ0(TPZFMatrix<double> Q0);

	//! Prints the element information to std::cout.
	void print();

private:
	//! A pointer to the parent TStructure object.
	TStructure* fStructure;
	//! An int array containing the IDs of node 0 and 1.
	int fLocalNodesIDs[2];
	//! A bool array that marks if the element is hinged at its nodes.
	bool fHinges[2];
	//! The ID of the element material.
	int fMaterialID;
	//! An int array containing the element equations (degrees of freedom).
	int fEquations[6];
	//! A TPZFMatrix<double> with the initial load vector of the element.
	TPZFMatrix<double> fQ0;
};

#endif // TELEMENT_H
