/** \file TStructure.h
* Contains the declaration of the TStructure class.
*/

#ifndef TSTRUCTURE_H
#define TSTRUCTURE_H

#include <iostream>
#include "TNode.h"
#include "TMaterial.h"
#include "TSupport.h"
#include "TElement.h"
#include "TNodalLoad.h"
#include "TDistributedLoad.h"
#include "TElementEndMoment.h"
//#include "TSupportDisplacement.h"

// TStructure class and declarations of its functions.
class TStructure {
public:
    //! Default constructor.
    TStructure(const std::vector<TNode>& Nodes = {},
               const std::vector<TMaterial>& Materials = {},
               const std::vector<TSupport>& Supports = {},
               const std::vector<TElement>& Elements = {});
    //! Copy constructor.
    TStructure(const TStructure& Other);
    //! Destructor.
    ~TStructure();

    //! Modifies the vector of TNode objects.
    void setNodes(const std::vector<TNode>& Nodes);
    //! Modifies the vector of TMaterial objects.
    void setMaterials(const std::vector<TMaterial>& Materials);
    //! Modifies the vector of TSupport objects.
    void setSupports(const std::vector<TSupport>& Supports);
    //! Modifies the vector of TElement objects.
    void setElements(const std::vector<TElement>& Elements);

    //! Gets the vector of TNode objects.
    std::vector<TNode> getNodes();
    //! Gets the vector of TMaterial objects.
    std::vector<TMaterial> getMaterials();
    //! Gets the vector of TSupport objects.
    std::vector<TSupport> getSupports();
    //! Gets the vector of TElement objects.
    std::vector<TElement> getElements();

    //! Gets one of the TElement objects by copy.
    TElement getElement(int elementID);
    //! Gets the addres of one of the TElement objects.
    TElement& getElementAddress(int elementID);
    //! Gets a support ID by giving its node ID.
    int getSupportID(int NodeID);
    //! Gets the matrix of degrees of freedom of the nodes.
    TPZFMatrix<int> getNodeEquations();

    //! Gets the total number of degrees of freedom of the structure.
    int getNDOF() const;
    //! Gets the number of constrained degrees of freedom of the structure.
    int getCDOF() const;
    //! Gets the number of unconstrained degrees of freedom of the structure.
    int getUDOF() const;
    //! Enumerates the degrees of freedom of each TElement object.
    void enumerateEquations();

    //! Gets the structure stiffness matrix.
    TPZFMatrix<double> getK() const;
    //! Gets the left upper block of the structure stiffness matrix K.
    TPZFMatrix<double> getK11() const;
    //! Gets the right upper block of the structure stiffness matrix K.
    TPZFMatrix<double> getK12() const;
    //! Gets the left lower block of the structure stiffness matrix K.
    TPZFMatrix<double> getK21() const;
    //! Gets the right lower block of the structure stiffness matrix K.
    TPZFMatrix<double> getK22() const;

    //! Gets the vector of external loads Q.
    TPZFMatrix<double> getQ() const;
    //! Gets the known part Qk (applied nodal loads) of loads vector Q.
    TPZFMatrix<double> getQK() const;
    //! Gets the unknown part Qu (support reactions) of loads vector Q.
    TPZFMatrix<double> getQU() const;

    //! Gets the vector of equivalent nodal loads caused by intermediate loads.
    TPZFMatrix<double> getQ0() const;
    //! Gets the known part Qk0 of the equivalent nodal loads vector Q0.
    TPZFMatrix<double> getQK0() const;
    //! Gets the unknown part Qu0 of the equivalent nodal loads vector Q0.
    TPZFMatrix<double> getQU0() const;

    //! Gets the vector of nodal displacements.
    TPZFMatrix<double> getD() const;
    //! Gets the known displacements Dk at constrained degrees of freedom.
    TPZFMatrix<double> getDK() const;
    //! Gets the unknown displacements Du at unconstrained degrees of freedom.
    TPZFMatrix<double> getDU() const;

    //! Calculates the internal loads of a given element.
    void getInternalLoads(int ElementID, TPZFMatrix<double>& q);

    //! Solve all the steps of structure accordingly to get the final results.
    void solve(std::vector<TNodalLoad>& NodalLoads,
               std::vector<TDistributedLoad>& DistrLoads,
               std::vector<TElementEndMoment>& EndMoments,
               std::vector<TPZFMatrix<double>>& InternalLoads);

private:
    // fNodes - vector containing the structure nodes.
    std::vector<TNode> fNodes;
    // fMaterials - vector containing the available materials.
    std::vector<TMaterial> fMaterials;
    // fSupports - vector containing the structure supports.
    std::vector<TSupport> fSupports;
    // fElements - vector containing the structure elements.
    std::vector<TElement> fElements;
    // fNodeEquations - matrix containing the DOFs of the nodes.
    TPZFMatrix<int> fNodeEquations;

    // fK - structure stiffness matrix.
    TPZFMatrix<double> fK;
    // fQ - structure external load vector.
    TPZFMatrix<double> fQ;
    // fQ0 - initial forces caused by intermediate loads.
    TPZFMatrix<double> fQ0;
    // fD - structure displacement vector.
    TPZFMatrix<double> fD;

    //! Assembles the structure stiffness matrix.
    void populateK();
    //! Stores the effects of loads into Q.
    void populateQ(std::vector<TNodalLoad>& NodalLoads,
                   std::vector<TElementEndMoment>& EndMoments);
    //! Stores the effects of distributed loads into Q0.
    void populateQ0(std::vector<TDistributedLoad>& DistrLoads);
    //! Stores the known displacements into D.
    //void populateDK(/*std::vector<TSupportDisplacement>& supportDisplacements*/);

    //! Calculates the unknown displacements Du and stores them into D.
    void solveDU();
    //! Calculates the support reactions and stores them into Q.
    void solveQU();
};

#endif // TSTRUCTURE_H
