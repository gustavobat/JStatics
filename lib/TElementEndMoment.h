/** \file TElementEndMoment.h
* Contains the declaration of the TElementEndMoment class.
*/

#ifndef TELEMENTENDMOMENT_H
#define TELEMENTENDMOMENT_H

#include <iostream>

// Forward declaration to TStructure class.
class TStructure;

//!  A class that implements a moment load at one of the element ends.
/*!
     A class that implements a moment load at one of the element ends. It needs
	 information on the element ID, the internal node (0 or 1) of the element
	 and the value of the moment, as well as the parent TStructure object.
*/
class TElementEndMoment
{
public:
    //! Default constructor.
    /*!
    \param Structure a pointer to the parent TStructure object.
    \param M value of the moment load.
    \param ElementID ID of the element to which the load is applied.
    \param Node the element node (0 or 1) to which the load is applied.
    \return the new TElementEndMoment object.
    */
    TElementEndMoment(TStructure* Structure = nullptr, double M = 0,
                      int ElementID = -1,	int Node = -1);

    //! Copy constructor.
    /*!
    \param Other the TElementEndMoment object to be copied.
    \return the new TElementEndMoment object.
    */
    TElementEndMoment(const TElementEndMoment& Other);

    //! Destructor.
    ~TElementEndMoment();

    //! Assignment operator.
    /*!
    \param Other the TElementEndMoment object to be copied.
    \return the modified TElementEndMoment object.
    */
    TElementEndMoment& operator=(const TElementEndMoment& Other);

    //! Returns the moment applied to the load.
    /*!
    \return the value of the moment.
    */
    double getM() const;

    //! Returns the ID of the element to which the load is applied.
    /*!
    \return the ID of the element to which the load is applied.
    */
    int getElementID() const;

    //! Returns the local ID of the node to which the load is applied.
    /*!
    \return the local ID of the node to which the load is applied.
    */
    int getNode() const;

    //! Modifies the moment applied to the load.
    /*!
    \param M the new moment value.
    */
    void setM(double M);

    //! Modifies the ID of the element to which the load is applied.
    /*!
    \param ElementID the ID of the new element.
    */
    void setElementID(int ElementID);

    //! Modifies the local ID (0 or 1) of the node to which the load is applied.
    /*!
    \param Node the local ID (0 or 1) of the new node.
    */
    void setNode(int Node);

    //! Modifies the element parent TStructure object.
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
    TStructure* fStructure;
    //! The value of the moment load.
    double fM;
    //! The ID of the element to which the load is applied.
    int fElementID;
    //! The local ID (0 or 1) of the node to which the laod is applied.
    int fNode;
};

#endif // TELEMENTENDMOMENT_H
