/** \file TSupport.h
 * Contains the declaration of the TSupport class.
 */

#ifndef TSUPPORT_H
#define TSUPPORT_H

#include "TNode.h"
#include <iostream>

// Forward declaration to TStructure class.
class TStructure;

//!  A class that implements a structure support.
/*!
A class that implements a structure support.
An object of this class has a bool array that tracks if each one of the
horizontal, vertical and rotational degrees of freedom are fixed (true) or
free (false). It also contains a TStructure parent object and the ID of the
node of the support.
*/
class TSupport {
public:
    //! Default constructor.
    /*!
    \param Structure a pointer to the parent TStructure object.
    \param Fx the bool related to the horizontal support condition.
    \param Fy the bool related to the vertical support condition.
    \param M the bool related to the rotational support condition.
    \param NodeID ID of the support node.
    \return the new TSupport object.
    */
    TSupport(TStructure *Structure = nullptr, const bool Fx = false,
             const bool Fy = false, const bool M = false, const int NodeID = 0);

    //! Copy constructor.
    /*!
    \param Other the TSupport object to be copied.
    \return the new TSupport object.
    */
    TSupport(const TSupport &Other);

    //! Destructor.
    ~TSupport();

    //! Assignment operator.
    /*!
    \param Other the TSupport object to be copied.
    \return the modified TSupport object.
    */
    TSupport &operator=(const TSupport &Other);

    //! Gets the Fx support condition.
    /*!
    \return the Fx support condition: fixed (true) or free (false).
    */
    bool RestrictsFx() const;

    //! Gets the Fy support condition.
    /*!
    \return the Fy support condition: fixed (true) or free (false).
    */
    bool RestrictsFy() const;

    //! Gets the M support condition.
    /*!
    \return the M support condition: fixed (true) or free (false).
    */
    bool RestrictsM() const;

    //! Gets the array of support conditions.
    /*!
    \return the array of support conditions.
    */
    bool *getConditions();

    //! Gets the node of the support.
    /*!
    \return the ID of the support node.
    */
    int getNodeID() const;

    //! Modifies the Fx support condition.
    /*!
    \param Fx the new Fx support condition: fixed (true) or free (false).
    */
    void setFx(const bool Fx);

    //! Modifies the Fy support condition.
    /*!
    \param Fy the new Fy support condition: fixed (true) or free (false).
    */
    void setFy(const bool Fy);

    //! Modifies the M support condition.
    /*!
    \param M the new M support condition: fixed (true) or free (false).
    */
    void setM(const bool M);

    //! Modifies the node of the support.
    /*!
    \param NodeID the ID of the new support node.
    */
    void setNodeID(const int NodeID);

    //! Modifies the parent TStructure object.
    /*!
    \param Structure the pointer to the new parent TStructure object.
    */
    void setStructure(TStructure *Structure);

    //! Prints the support information to std::cout.
    void print();

private:
    //! A pointer to the parent TStructure object.
    TStructure *fStructure;
    //! The array of support conditions.
    bool fConditions[3];
    //! The ID of the node of the support.
    int fNodeID;
};

#endif // TSUPPORT_H
