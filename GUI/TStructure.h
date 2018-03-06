/* This file was created by Gustavo BATISTELA.
It is a header file containing the definition of the TStructure class
and the declaration of its members. */

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
	// Default constructor.
	TStructure(const std::vector<TNode>& Nodes = {},
			   const std::vector<TMaterial>& Materials = {},
			   const std::vector<TSupport>& Supports = {},
		       const std::vector<TElement>& Elements = {});
	// Copy constructor.
	TStructure(const TStructure& Other);
	// Destructor.
	~TStructure();

	// setNodes - modifies the vector of nodes.
	void setNodes(const std::vector<TNode>& Nodes);
	// setMaterials - modifies the vector of materials.
	void setMaterials(const std::vector<TMaterial>& Materials);
	// setSupports - modifies the vector of supports.
	void setSupports(const std::vector<TSupport>& Supports);
	// setElements - modifies the vector of elements.
	void setElements(const std::vector<TElement>& Elements);

	// getNodes - returns the vector of nodes.
	std::vector<TNode> getNodes();
	// getMaterials - returns the vector of materials.
	std::vector<TMaterial> getMaterials();
	// getSupports - returns the vector of supports.
	std::vector<TSupport> getSupports();
	// getElements - returns the vector of elements.
	std::vector<TElement> getElements();

	// getElement - returns a copy of an element.
	TElement getElement(int elementID);
	// getElementAddress - returns the reference to a TElement object.
	TElement& getElementAddress(int elementID);
	// getSupport - returns a support ID by giving the reference node ID.
	int getSupportID(int NodeID);
	// getNodeEquations - returns the matrix of DOFs of the nodes.
	TPZFMatrix<int> getNodeEquations();

	// getNDOF - returns the number of DOF of the structure.
	int getNDOF() const;
	// getCDOF - returns the number of constrained DOF of the structure.
	int getCDOF() const;
	// getUDOF - returns the number of unconstrained DOF of the structure.
	int getUDOF() const;
	// enumerateEquations - enumerates the DOF associated with each element.
	void enumerateEquations();

	// getK - returns the global stiffness matrix.
	TPZFMatrix<double> getK() const;
	// getK11 - returns the top left block of the global stiffness matrix K.
	TPZFMatrix<double> getK11() const;
	// getK12 - returns the top right block of the global stiffness matrix K.
	TPZFMatrix<double> getK12() const;
	// getK21 - returns the bottom left block of the global stiffness matrix K.
	TPZFMatrix<double> getK21() const;
	// getK22 - returns the bottom right block of the global stiffness matrix K.
	TPZFMatrix<double> getK22() const;

	// getQ - returns the external force vector.
	TPZFMatrix<double> getQ() const;
	// getQK - returns the known external forces (applied forces).
	TPZFMatrix<double> getQK() const;
	// getQU - returns the unknown external forces (reactions).
	TPZFMatrix<double> getQU() const;

	// getQ0 - returns the initial external force vector, caused by intermediate loads.
	TPZFMatrix<double> getQ0() const;
	// getQK0 - returns the known part of the initial force vector.
	TPZFMatrix<double> getQK0() const;
	// getQU0 - returns the unknown part of the initial force vector.
	TPZFMatrix<double> getQU0() const;

	// getD - returns the displacement vector.
	TPZFMatrix<double> getD() const;
	// getDK - returns the known displacements (constrained degrees of freedom and initial displacements).
	TPZFMatrix<double> getDK() const;
	// getDU - returns the unknown displacements (unconstrained degrees of freedom).
	TPZFMatrix<double> getDU() const;

	// getInternalLoads - calculates the internal loads of a given element.
	void getInternalLoads(int ElementID, TPZFMatrix<double>& q);

	// solve - solves structure and stores results in objects accordingly.
	void solve(std::vector<TNodalLoad>& NodalLoads, std::vector<TDistributedLoad>& DistrLoads,
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

	// populateK - stores the global stiffness matrix.
	void populateK();
	// populateQ - calculates the known external loads and stores them in the vector of global forces Q.
	void populateQ(std::vector<TNodalLoad>& NodalLoads, std::vector<TElementEndMoment>& EndMoments);
	// populateQ0 - calculates the initial external load vector due to intermediate loads.
	void populateQ0(std::vector<TDistributedLoad>& DistrLoads);
	// populateDK - adds the initial displacements to the displacement vector D.
	//void populateDK(std::vector<TSupportDisplacement>& supportDisplacements);

	// solveDU - solves the unknown displacements DU and stores them in the vector of global displacements fD.
	void solveDU();
	// solveQU - solves the support reactions and stores them in the vector of global forces fQ.
	void solveQU();
};

#endif // TSTRUCTURE_H
