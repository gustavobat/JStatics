/** \file TDistributedLoad.h
* Contains the declaration of the TDistributedLoad class.
*/

#ifndef TDISTRIBUTEDLOAD_H
#define TDISTRIBUTEDLOAD_H

#include <iostream>

// Forward declaration to TStructure class.
class TStructure;

//!  A class that implements a distributed load on an element.
/*!
     A class that implements a distributed load on an element.
	 The class accepts two different values at the element extremes, varying
	 linearly between them. The load plane can be set to global
	 (the structure plane) or local (the element plane).
*/
class TDistributedLoad
{
public:
	//! Default constructor.
	/*!
	\param Structure a pointer to the parent TStructure object.
	\param ElementID ID of the element to which the load is applied.
	\param Node0Load value of the load at the element local node 0.
	\param Node1Load value of the load at the element local node 1.
	\param LoadPlane direction of the load: global (true) or local (false).
	\return the new TDistributedLoad object.
	*/
	TDistributedLoad(TStructure* Structure = nullptr, int ElementID = -1,
		double Node0Load = 0, double Node1Load = 0, bool LoadPlane = false);

	//! Copy constructor.
	/*!
	\param Other the TDistributedLoad object to be copied.
	\return the new TDistributedLoad object.
	*/
	TDistributedLoad(const TDistributedLoad& Other);

	//! Destructor.
	~TDistributedLoad();

	//! Assignment operator.
	/*!
	\param Other the TDistributedLoad object to be copied.
	\return the modified TDistributedLoad object.
	*/
	TDistributedLoad& operator=(const TDistributedLoad& Other);

	//! Gets the ID of the element to which the load is applied.
	/*!
	\return the ID of the element to which the load is applied.
	*/
	int getElementID() const;

	//! Gets the load value at the element local node 0.
	/*!
	\return the value of the load at the element local node 0.
	*/
	double getNode0Load() const;

	//! Gets the load value at the element local node 1.
	/*!
	\return the value of the load at the element local node 1.
	*/
	double getNode1Load() const;

	//! Gets the boolean associated with the load direction.
	/*!
	\return the load direction: global (true) or local (false).
	*/
	bool getLoadPlane() const;

	//! Modifies the ID of the element to which the load is applied.
	/*!
	\param ElementID the ID of the new element.
	*/
	void setElementID(int ElementID);

	//! Modifies the load value at the element local node 0.
	/*!
	\param Node0Load the new value of the load at node 0.
	*/
	void setNode0Load(double Node0Load);

	//! Modifies the load value at the element local node 1.
	/*!
	\param Node1Load the new value of the load at node 1.
	*/
	void setNode1Load(double Node1Load);

	//! Modifies the boolean associated with the plane of the load.
	/*!
	\param LoadPlane the new load direction: global (true) or local (false).
	*/
	void setLoadPlane(bool LoadPlane);

	//! Modifies the parent TStructure object.
	/*!
	\param Structure the pointer to the parent TStructure object.
	*/
	void setStructure(TStructure* Structure);

	//! Adds the effects of the distributed load to the initial load vector Q0.
	void store();

	//! Prints the load information into std::cout.
	void print();

private:
	//! A pointer to the parent TStructure object.
	TStructure* fStructure;
	//! The ID of the element to which the load is applied.
	int fElementID;
	//! The load value at the element local node 0.
	double fNode0Load;
	//! The load value at the element local node 1.
	double fNode1Load;
	//! A boolean associated with the direction of the load.
	bool fLoadPlane;
};

#endif // TDISTRIBUTEDLOAD_H
