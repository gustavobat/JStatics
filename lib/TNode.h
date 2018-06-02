/** \file TNode.h
* Contains the declaration of the TNode class.
*/

#ifndef TNODE_H
#define TNODE_H

#include <iostream>

// Forward declaration to TStructure class.
class TStructure;

//!  A class that implements a structure node.
/*!
	 A class that implements a structure node.
	 An object of this class has an array of double that stores the x and y
	 node coordinates.
*/
class TNode {
public:
	//! Default constructor.
	/*!
	\param X the x coordinate of the node.
	\param Y the y coordinate of the node.
	\return the new TNode object.
	*/
	TNode(double X = 0, double Y = 0);

	//! Copy constructor.
	/*!
	\param Other the TNode object to be copied.
	\return the new TNode object.
	*/
	TNode(const TNode& Other);

	//! Destructor.
	~TNode();

	//! Assignment operator.
	/*!
	\param Other the TNode object to be copied.
	\return the modified TNode object.
	*/
	TNode& operator=(const TNode& Other);

	//! Gets the node x coordinate.
	/*!
	\return the x coordinate of the node.
	*/
	double getX() const;

	//! Gets the node y coordinate.
	/*!
	\return the y coordinate of the node.
	*/
	double getY() const;

	//! Gets the array of coordinates of the node.
	/*!
	\return the array of node coordinates
	*/
	double* const getCoord();

	//! Modifies the node x coordinate.
	/*!
	\param X the new x coordinate.
	*/
	void setX(double X);
	//! Modifies the node y coordinate.
	/*!
	\param Y the new y coordinate.
	*/
	void setY(double Y);

	//! Modifies the array of coordinates of the node.
	/*!
	\param Coordinates the new array of coordinates.
	*/
	void setCoord(double* Coordinates);

	//! Prints the node information to std::cout.
	void print();

private:
	//! The array that stores the node coordinates.
	double fCoordinates[2];
};

#endif // TNODE_H
