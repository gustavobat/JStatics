/** \file TStructure.cpp
* Contains the definitions of the TStructure methods.
*/

#include "TStructure.h"

//! Default constructor.
TStructure::TStructure(const std::vector<TNode>& Nodes,
                       const std::vector<TMaterial>& Materials,
                       const std::vector<TSupport>& Supports,
                       const std::vector<TElement>& Elements)
{
    fNodes = Nodes;
    fMaterials = Materials;
    fSupports = Supports;
    fElements = Elements;

    fK = TPZFMatrix<double>(0, 0, 0);
    fQ = TPZFMatrix<double>(0, 0, 0);
    fQ0 = TPZFMatrix<double>(0, 0, 0);
    fD = TPZFMatrix<double>(0, 0, 0);
}

//! Copy constructor.
TStructure::TStructure(const TStructure& Other)
{
    fNodes = Other.fNodes;
    fMaterials = Other.fMaterials;
    fSupports = Other.fSupports;
    fElements = Other.fElements;
    fNodeEquations = Other.fNodeEquations;
    fK = Other.fK;
    fQ = Other.fQ;
    fQ0 = Other.fQ0;
    fD = Other.fD;
}

//! Destructor.
TStructure::~TStructure() {}

//! Modifies the vector of TNode objects.
void TStructure::setNodes(const std::vector<TNode>& Nodes)
{
    fNodes = Nodes;
}

//! Modifies the vector of TMaterial objects.
void TStructure::setMaterials(const std::vector<TMaterial>& Materials)
{
    fMaterials = Materials;
}

//! Modifies the vector of TSupport objects.
void TStructure::setSupports(const std::vector<TSupport>& Supports)
{
    fSupports = Supports;
}

//! Modifies the vector of TElement objects.
void TStructure::setElements(const std::vector<TElement>& Elements)
{
    fElements = Elements;
}

//! Gets the vector of TNode objects.
std::vector<TNode> TStructure::getNodes()
{
    return fNodes;
}

//! Gets the vector of TMaterial objects.
std::vector<TMaterial> TStructure::getMaterials()
{
    return fMaterials;
}

//! Gets the vector of TSupport objects.
std::vector<TSupport> TStructure::getSupports()
{
    return fSupports;
}

//! Gets the vector of TElement objects.
std::vector<TElement> TStructure::getElements()
{
    return fElements;
}

//! Gets one of the TElement objects by copy.
TElement TStructure::getElement(int elementID)
{
    return fElements[elementID];
}

//! Gets the addres of one of the TElement objects.
TElement& TStructure::getElementAddress(int elementID)
{
    return fElements[elementID];
}

//! Gets a support ID by giving its node ID.
int TStructure::getSupportID(int NodeID)
{
    int SupportID = -1;

    for (int i = 0; i < (int)fSupports.size(); i++) {
        if (fSupports[i].getNodeID() == NodeID) {
            SupportID = i;
            break;
        }
    }
    return SupportID;
}

//! Gets the matrix of degrees of freedom of the nodes.
TPZFMatrix<int> TStructure::getNodeEquations()
{
    return fNodeEquations;
}

//! Gets the total number of degrees of freedom of the structure.
int TStructure::getNDOF() const
{
    // DOF counter variable, intialized considering vertical and horizontal
    // displacements.
    int NDOF = 2 * fNodes.size();

    // Auxiliar that keeps track if there are already non-hinged elements
    // connected to a certain node.
    std::vector<bool> aux(fNodes.size());
    std::fill(aux.begin(), aux.end(), false);

    // Counts the number of constrained rotation DOF.
    for (int i = 0; i < (int)fSupports.size(); i++) {
        if (fSupports[i].RestrictsM() == true) {
            aux[fSupports[i].getNodeID()] = true;
            NDOF++;
        }
    }

    // Counts the number of unconstrained rotation DOF.
    for (int i = 0; i < (int)fElements.size(); i++) {
        int Node0ID = fElements[i].getNode0ID();
        int Node1ID = fElements[i].getNode1ID();

        // Checks if the element is non-hinged at its node 0.
        if (fElements[i].getHinge0() == false) {
            // Ensure element is the first non-hinged element to be
            // connected at its node 0.
            if (aux[Node0ID] == false) {
                aux[Node0ID] = true;
                NDOF++;
            }
        }
            // Case the element is hinged at its node 0.
        else {
            NDOF++;
        }

        // Checks if the element is non-hinged at its node 1.
        if (fElements[i].getHinge1() == false) {
            // Ensure element is the first non-hinged element to be
            // connected at its node 1.
            if (aux[Node1ID] == false) {
                aux[Node1ID] = true;
                NDOF++;
            }
        }
            // Case the element is hinged at its node 1.
        else {
            NDOF++;
        }
    }
    return NDOF;
}

//! Gets the number of constrained degrees of freedom of the structure.
int TStructure::getCDOF() const
{
    int CDOF = 0;
    // Counts the number of constrained DOF.
    for (int i = 0; i < (int)fSupports.size(); i++) {
        if (fSupports[i].RestrictsFx() == true) {
            CDOF++;
        }
        if (fSupports[i].RestrictsFy() == true) {
            CDOF++;
        }
        if (fSupports[i].RestrictsM() == true) {
            CDOF++;
        }
    }
    return CDOF;
}

//! Gets the number of unconstrained degrees of freedom of the structure.
int TStructure::getUDOF() const
{
    return (this->getNDOF() - this->getCDOF());
}

//! Enumerates the degrees of freedom of each TElement object.
void TStructure::enumerateEquations()
{
    // Matrix that stores the equations associated with each node.
    TPZFMatrix<int> equations(fNodes.size(), 3, -1);

    int CDOF = this->getCDOF();
    int count = this->getNDOF() - CDOF;

    // Enumerates the constrained DOF.
    for (int i = 0; i < (int)fSupports.size(); i++) {
        if (fSupports[i].RestrictsFx() == true) {
            equations(fSupports[i].getNodeID(), 0) = count;
            count++;
        }
        if (fSupports[i].RestrictsFy() == true) {
            equations(fSupports[i].getNodeID(), 1) = count;
            count++;
        }
        if (fSupports[i].RestrictsM() == true) {
            equations(fSupports[i].getNodeID(), 2) = count;
            count++;
        }
    }

    // Enumerates the unconstrained DOF.
    count = 0;
    for (int i = 0; i < (int)fElements.size(); i++) {
        int Node0ID = fElements[i].getNode0ID();
        int Node1ID = fElements[i].getNode1ID();
        int Node0RotationDOF;
        int Node1RotationDOF;

        if (equations(Node0ID, 0) == -1) {
            equations(Node0ID, 0) = count;
            count++;
        }
        if (equations(Node0ID, 1) == -1) {
            equations(Node0ID, 1) = count;
            count++;
        }
        if (fElements[i].getHinge0() == false) {
            if (equations(Node0ID, 2) == -1) {
                equations(Node0ID, 2) = count;
                count++;
            }
            Node0RotationDOF = equations(Node0ID, 2);
        }
        else {
            Node0RotationDOF = count;
            count++;
        }

        if (equations(Node1ID, 0) == -1) {
            equations(Node1ID, 0) = count;
            count++;
        }
        if (equations(Node1ID, 1) == -1) {
            equations(Node1ID, 1) = count;
            count++;
        }
        if (fElements[i].getHinge1() == false) {
            if (equations(Node1ID, 2) == -1) {
                equations(Node1ID, 2) = count;
                count++;
            }
            Node1RotationDOF = equations(Node1ID, 2);
        }
        else {
            Node1RotationDOF = count;
            count++;
        }
        fElements[i].setEquations(equations(Node0ID, 0), equations(Node0ID, 1),
                                  Node0RotationDOF, equations(Node1ID, 0),
                                  equations(Node1ID, 1), Node1RotationDOF);
    }

    fNodeEquations = equations;
}

//! Gets the structure stiffness matrix.
TPZFMatrix<double> TStructure::getK() const
{
    return fK;
}

//! Gets the left upper block of the structure stiffness matrix K.
TPZFMatrix<double> TStructure::getK11() const
{
    int UDOF = this->getUDOF();
    TPZFMatrix<double> K11(UDOF, UDOF, 0);

    for (int i = 0; i < UDOF; i++) {
        for (int j = 0; j < UDOF; j++) {
            K11(i, j) = fK.Get(i, j);
        }
    }
    return K11;
}

//! Gets the right upper block of the structure stiffness matrix K.
TPZFMatrix<double> TStructure::getK12() const
{
    int UDOF = this->getUDOF();
    int CDOF = this->getCDOF();

    TPZFMatrix<double> K12(UDOF, CDOF, 0);

    for (int i = 0; i < UDOF; i++) {
        for (int j = 0; j < CDOF; j++) {
            K12(i, j) = fK.Get(i, UDOF + j);
        }
    }
    return K12;
}

//! Gets the left lower block of the structure stiffness matrix K.
TPZFMatrix<double> TStructure::getK21() const
{
    int CDOF = this->getCDOF();
    int UDOF = this->getUDOF();

    TPZFMatrix<double> K21(CDOF, UDOF);

    for (int i = 0; i < CDOF; i++) {
        for (int j = 0; j < UDOF; j++) {
            K21(i, j) = fK.Get(UDOF + i, j);
        }
    }
    return K21;
}

//! Gets the right lower block of the structure stiffness matrix K.
TPZFMatrix<double> TStructure::getK22() const
{
    int CDOF = this->getCDOF();
    int UDOF = this->getUDOF();

    TPZFMatrix<double> K22(CDOF, CDOF);

    for (int i = 0; i < CDOF; i++) {
        for (int j = 0; j < CDOF; j++) {
            K22(i, j) = fK.Get(UDOF + i, UDOF + j);
        }
    }
    return K22;
}

//! Gets the vector of external loads Q.
TPZFMatrix<double> TStructure::getQ() const
{
    return fQ;
}

//! Gets the known part Qk (applied nodal loads) of the external loads vector Q.
TPZFMatrix<double> TStructure::getQK() const
{
    int UDOF = this->getUDOF();
    TPZFMatrix<double> QK(UDOF, 1, 0);

    for (int i = 0; i < UDOF; i++) {
        QK(i, 0) = fQ.Get(i, 0);
    }
    return QK;
}

//! Gets the unknown part Qu (support reactions) of the external loads vector Q.
TPZFMatrix<double> TStructure::getQU() const
{
    int UDOF = this->getUDOF();
    int CDOF = this->getCDOF();
    TPZFMatrix<double> QU(CDOF, 1, 0);

    for (int i = 0; i < CDOF; i++) {
        QU(i, 0) = fQ.Get(UDOF + i, 0);
    }
    return QU;
}

//! Gets the vector of equivalent nodal loads caused by intermediate loads.
TPZFMatrix<double> TStructure::getQ0() const
{
    return fQ0;
}

//! Gets the known part Qk0 of the equivalent nodal loads vector Q0.
TPZFMatrix<double> TStructure::getQK0() const
{
    int UDOF = this->getUDOF();
    TPZFMatrix<double> QK0(UDOF, 1, 0);

    for (int i = 0; i < UDOF; i++) {
        QK0(i, 0) = fQ0.Get(i, 0);
    }
    return QK0;
}

//! Gets the unknown part Qu0 of the equivalent nodal loads vector Q0.
TPZFMatrix<double> TStructure::getQU0() const
{
    int UDOF = this->getUDOF();
    int CDOF = this->getCDOF();
    TPZFMatrix<double> Q0U(CDOF, 1, 0);

    for (int i = 0; i < CDOF; i++) {
        Q0U(i, 0) = fQ0.Get(UDOF + i, 0);
    }
    return Q0U;
}

//! Gets the vector of nodal displacements.
TPZFMatrix<double> TStructure::getD() const
{
    return fD;
}

//! Gets the known displacements Dk at constrained degrees of freedom.
TPZFMatrix<double> TStructure::getDK() const
{
    int UDOF = this->getUDOF();
    int CDOF = this->getCDOF();
    TPZFMatrix<double> DK(CDOF, 1, 0);

    for (int i = 0; i < CDOF; i++) {
        DK(i, 0) = fD.Get(UDOF + i, 0);
    }
    return DK;
}

//! Gets the unknown displacements Du at unconstrained degrees of freedom.
TPZFMatrix<double> TStructure::getDU() const
{
    int UDOF = this->getUDOF();
    TPZFMatrix<double> DU(UDOF, 1, 0);

    for (int i = 0; i < UDOF; i++) {
        DU(i, 0) = fD.Get(i, 0);
    }
    return DU;
}

//! Calculates the internal loads of a given element.
void TStructure::getInternalLoads(int ElementID, TPZFMatrix<double>& q)
{
    TPZFMatrix<double> localK = this->getElement(ElementID).getLocalK();
    TPZFMatrix<double> T = this->getElement(ElementID).getT();
    TPZFMatrix<double> q0 = this->getElement(ElementID).getQ0();

    TPZFMatrix<double> D(6, 1, 0);
    for (int i = 0; i < 6; i++)
    {
        D(i, 0) = fD(this->getElement(ElementID).getEquations()[i], 0);
    }

    q = localK * T * D + q0;
}

//! Solve all the steps of structure accordingly to get the final results.
void TStructure::solve(std::vector<TNodalLoad>& NodalLoads,
                       std::vector<TDistributedLoad>& DistrLoads,
                       std::vector<TElementEndMoment>& EndMoments,
    //std::vector<TSupportDisplacement>& SupportDisplacements,
                       std::vector<TPZFMatrix<double>>& InternalLoads)
{
    fD = TPZFMatrix<double>(this->getNDOF(), 1, 0);
    fQ = TPZFMatrix<double>(this->getNDOF(), 1, 0);

    enumerateEquations();

    populateK();
    populateQ(NodalLoads, EndMoments);
    populateQ0(DistrLoads);
    //populateDK(/*SupportDisplacements*/);
    solveDU();
    solveQU();

    for (int i = 0; i < (int)this->getElements().size(); i++) {
        TPZFMatrix<double> qLocal;
        getInternalLoads(i, qLocal);
        InternalLoads.push_back(qLocal);
    }
}

//! Assembles the structure stiffness matrix.
void TStructure::populateK()
{
    int NDOF = this->getNDOF();
    fK = TPZFMatrix<double>(NDOF, NDOF, 0);

    for (int i = 0; i < (int)fElements.size(); i++) {
        TElement elem = fElements[i];
        TPZFMatrix<double> kLocal = elem.getK();

        for (int aux1 = 0; aux1 < 6; aux1++) {
            for (int aux2 = 0; aux2 < 6; aux2++) {
                fK(elem.getEquations()[aux1], elem.getEquations()[aux2]) +=
                    kLocal(aux1, aux2);
            }
        }
    }
}

//! Stores the effects of loads into Q.
void TStructure::populateQ(std::vector<TNodalLoad>& NodalLoads,
                           std::vector<TElementEndMoment>& EndMoments)
{
    for (int i = 0; i < (int)NodalLoads.size(); i++) {
        NodalLoads[i].store(fQ);
    }

    for (int i = 0; i < (int)EndMoments.size(); i++) {
        EndMoments[i].store(fQ);
    }
}

//! Stores the effects of distributed loads into Q0.
void TStructure::populateQ0(std::vector<TDistributedLoad>& DistrLoads)
{
    fQ0 = TPZFMatrix<double>(this->getNDOF(), 1, 0);

    for (int i = 0; i < (int)DistrLoads.size(); i++) {
        DistrLoads[i].store();
    }

    for (int i = 0; i < (int)fElements.size(); i++)
    {
        TPZFMatrix<double> q0 = fElements[i].getTT() * fElements[i].getQ0();
        for (int j = 0; j < 6; j++) {
            int DOF = fElements[i].getEquations()[j];
            fQ0(DOF, 0) += q0(j, 0);
        }
    }
}

//! Stores the known displacements into D.
/*void TStructure::populateDK(
std::vector<TSupportDisplacement>& supportDisplacements)
{
	for (int i = 0; i < 2; i++) {
		int a = i;
	}
	for (int i = 0; i < (int)supportDisplacements.size(); i++) {
		supportDisplacements[i].store(fD);
	}
}*/

//! Calculates the unknown displacements Du and stores them into D.
void TStructure::solveDU()
{
    int UDOF = this->getUDOF();
    int NDOF = this->getNDOF();

    if (UDOF != 0) {
        TPZFMatrix<double> DU(UDOF, 1, 0);
        TPZFMatrix<double> invK11;
        this->getK11().Inverse(invK11, ELU);
        TPZFMatrix<double> QK = this->getQK();
        TPZFMatrix<double> K12 = this->getK12();
        TPZFMatrix<double> DK = this->getDK();
        TPZFMatrix<double> QK0 = this->getQK0();

        DU = invK11 * (QK - K12 * DK - QK0);

        for (int i = 0; i < UDOF; i++) {
            fD(i, 0) = DU(i, 0);
        }
    }
}

//! Calculates the support reactions and stores them into Q.
void TStructure::solveQU()
{
    int UDOF = this->getUDOF();
    int CDOF = this->getCDOF();

    TPZFMatrix<double> QU(UDOF, 1, 0);
    TPZFMatrix<double> QU0 = this->getQU0();
    if (UDOF == 0) {
        TPZFMatrix<double> K21 = this->getK21();
        TPZFMatrix<double> DU = this->getDU();
        QU = K21 * DU + QU0;
    }
    else {
        TPZFMatrix<double> K21 = this->getK21();
        TPZFMatrix<double> DU = this->getDU();
        TPZFMatrix<double> K22 = this->getK22();
        TPZFMatrix<double> DK = this->getDK();
        QU = K21 * DU + K22 * DK + QU0;
    }

    for (int i = 0; i < CDOF; i++) {
        fQ(UDOF + i, 0) = QU(i, 0);
    }
}
