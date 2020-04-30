/** \file TElement.cpp
* Contains the definitions of the TElement methods.
*/

#include "TStructure.h"
#include "TElement.h"

//! Default constructor.
TElement::TElement(TStructure *Structure, const int Node0ID, const int Node1ID,
	const bool Hinge0, const bool Hinge1, const int MaterialID)
{
    fStructure = Structure;
    fLocalNodesIDs[0] = Node0ID;
    fLocalNodesIDs[1] = Node1ID;
    fHinges[0] = Hinge0;
    fHinges[1] = Hinge1;
    fMaterialID = MaterialID;
    fEquations[0] = -1;
    fEquations[1] = -1;
    fEquations[2] = -1;
    fEquations[3] = -1;
    fEquations[4] = -1;
    fEquations[5] = -1;
    fQ0 = TPZFMatrix<double>(6, 1, 0);
}

//! Copy constructor.
TElement::TElement(const TElement& Other)
{
    fStructure = Other.fStructure;
    fLocalNodesIDs[0] = Other.fLocalNodesIDs[0];
    fLocalNodesIDs[1] = Other.fLocalNodesIDs[1];
    fHinges[0] = Other.fHinges[0];
    fHinges[1] = Other.fHinges[1];
    fMaterialID = Other.fMaterialID;
    fEquations[0] = Other.fEquations[0];
    fEquations[1] = Other.fEquations[1];
    fEquations[2] = Other.fEquations[2];
    fEquations[3] = Other.fEquations[3];
    fEquations[4] = Other.fEquations[4];
    fEquations[5] = Other.fEquations[5];
    fQ0 = Other.fQ0;
}

//! Destructor.
TElement::~TElement() {}

//! Assignment operator.
TElement& TElement::operator=(const TElement& Other)
{
    if (this != &Other) {
        fStructure = Other.fStructure;
        fLocalNodesIDs[0] = Other.fLocalNodesIDs[0];
        fLocalNodesIDs[1] = Other.fLocalNodesIDs[1];
        fHinges[0] = Other.fHinges[0];
        fHinges[1] = Other.fHinges[1];
        fMaterialID = Other.fMaterialID;
        fEquations[0] = Other.fEquations[0];
        fEquations[1] = Other.fEquations[1];
        fEquations[2] = Other.fEquations[2];
        fEquations[3] = Other.fEquations[3];
        fEquations[4] = Other.fEquations[4];
        fEquations[5] = Other.fEquations[5];
        fQ0 = Other.fQ0;
    }
    return *this;
}

//! Gets the nodes of the element.
int* const TElement::getLocalNodesIDs()
{
    return fLocalNodesIDs;
}

//! Gets the ID of the element Node 0.
int TElement::getNode0ID() const
{
    return fLocalNodesIDs[0];
}

//! Gets the ID of the element Node 1.
int TElement::getNode1ID() const
{
    return fLocalNodesIDs[1];
}

//! Gets the hinge of the element at Node 0.
bool TElement::getHinge0() const
{
    return fHinges[0];
}

//! Gets the hinge of the element at Node 1.
bool TElement::getHinge1() const
{
    return fHinges[1];
}

//! Gets the material of the element.
int TElement::getMaterialID() const
{
    return fMaterialID;
}

//! Gets the equations of the element.
int* const TElement::getEquations()
{
    return fEquations;
}

//! Modifies the element parent TStructure object.
void TElement::setStructure(TStructure* Structure)
{
    fStructure = Structure;
}

//! Modifies the IDs of the element nodes.
void TElement::setLocalNodesIDs(int* NodesIDs)
{
    fLocalNodesIDs[0] = NodesIDs[0];
    fLocalNodesIDs[1] = NodesIDs[1];
}

//! Modifies the ID of the element node 0.
void TElement::setNode0ID(int Node0ID)
{
    fLocalNodesIDs[0] = Node0ID;
}

//! Modifies the ID of the element node 1.
void TElement::setNode1ID(int Node1ID)
{
    fLocalNodesIDs[1] = Node1ID;
}

//! Modifies the element hinge at node 0.
void TElement::setHinge0(bool Hinge0)
{
    fHinges[0] = Hinge0;
}

//! Modifies the element hinge at node 1.
void TElement::setHinge1(bool Hinge1)
{
    fHinges[1] = Hinge1;
}

//! Modifies the element material.
void TElement::setMaterialID(int MaterialID)
{
    fMaterialID = MaterialID;
}

//! Modifies the element equations (degrees of freedom) indexes.
void TElement::setEquations(int Eq0, int Eq1, int Eq2,
                            int Eq3, int Eq4, int Eq5)
{
    fEquations[0] = Eq0;
    fEquations[1] = Eq1;
    fEquations[2] = Eq2;
    fEquations[3] = Eq3;
    fEquations[4] = Eq4;
    fEquations[5] = Eq5;
}

//! Gets the x coordinate of the element Node 0.
double TElement::getX0() const
{
    TNode node = fStructure->getNodes()[fLocalNodesIDs[0]];
    return node.getX();
}

//! Gets the y coordinate of the element Node 0.
double TElement::getY0() const
{
    TNode node = fStructure->getNodes()[fLocalNodesIDs[0]];
    return node.getY();
}

//! Gets the x coordinate of the element Node 1.
double TElement::getX1() const
{
    TNode node = fStructure->getNodes()[fLocalNodesIDs[1]];
    return node.getX();
}

//! Gets the y coordinate of the element Node 1.
double TElement::getY1() const
{
    TNode node = fStructure->getNodes()[fLocalNodesIDs[1]];
    return node.getY();
}

//! Gets the length of the element.
double TElement::getL() const
{
    double x0 = this->getX0();
    double y0 = this->getY0();
    double x1 = this->getX1();
    double y1 = this->getY1();
    double length = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    return (length);
}

//! Gets the cosine of the element angle.
double TElement::getCos() const
{
    double x0 = this->getX0();
    double y0 = this->getY0();
    double x1 = this->getX1();
    double y1 = this->getY1();
    double cos = (x1 - x0) / sqrt((x1- x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    return (cos);
}

//! Gets the sine of the element angle.
double TElement::getSin() const
{
    double x0 = this->getX0();
    double y0 = this->getY0();
    double x1 = this->getX1();
    double y1 = this->getY1();
    double sin = (y1 - y0) / sqrt((x1- x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    return (sin);
}

//! Gets the transformation matrix of the element.
TPZFMatrix<double> TElement::getT() const
{
    TPZFMatrix<double> T(6, 6, 0);
    double lx = this->getCos();
    double ly = this->getSin();

    T(0, 0) = lx;
    T(0, 1) = ly;
    T(1, 0) = -1 * ly;
    T(1, 1) = lx;
    T(2, 2) = 1;
    T(3, 3) = lx;
    T(3, 4) = ly;
    T(4, 3) = -1 * ly;
    T(4, 4) = lx;
    T(5, 5) = 1;

    return T;
}

//! Gets the transposed transformation matrix of the element.
TPZFMatrix<double> TElement::getTT() const
{
    TPZFMatrix<double> TT(6, 6, 0);
    double lx = this->getCos();
    double ly = this->getSin();

    TT(0, 0) = lx;
    TT(0, 1) = -1 * ly;
    TT(1, 0) = ly;
    TT(1, 1) = lx;
    TT(2, 2) = 1;
    TT(3, 3) = lx;
    TT(3, 4) = -1 * ly;
    TT(4, 3) = ly;
    TT(4, 4) = lx;
    TT(5, 5) = 1;

    return TT;
}

//! Gets the global stiffness matrix of the element.
TPZFMatrix<double> TElement::getK() const
{
    TMaterial material = fStructure->getMaterials()[fMaterialID];
    double E = material.getE();
    double A = material.getA();
    double I = material.getI();
    double L = this->getL();
    double lx = this->getCos();
    double ly = this->getSin();

    TPZFMatrix<double> K(6, 6, 0);

    // Fills K.
    K(0, 0) = (A * E / L * (lx * lx) + 12 * E * I * (ly * ly) / (L * L * L));
    K(0, 1) = (A * E / L - 12 * E * I / (L * L * L)) * (lx * ly);
    K(0, 2) = -(6 * E * I / (L * L)) * ly;
    K(0, 3) = -(A * E / L * (lx * lx) + 12 * E * I * (ly * ly) / (L * L * L));
    K(0, 4) = -(A * E / L - 12 * E * I / (L * L * L)) * (lx * ly);
    K(0, 5) = -(6 * E * I / (L * L)) * ly;

    K(1, 0) = (A * E / L - 12 * E * I / (L * L * L)) * (lx * ly);
    K(1, 1) = (A * E / L * (ly * ly) + 12 * E * I * (lx * lx) / (L * L * L));
    K(1, 2) = (6 * E * I / (L * L)) * lx;
    K(1, 3) = -(A * E / L - 12 * E * I / (L * L * L)) * (lx * ly);
    K(1, 4) = -(A * E / L * (ly * ly) + 12 * E * I * (lx * lx) / (L * L * L));
    K(1, 5) = (6 * E * I / (L * L)) * lx;

    K(2, 0) = -(6 * E * I / (L * L)) * ly;
    K(2, 1) = (6 * E * I / (L * L)) * lx;
    K(2, 2) = (4 * E * I / (L));
    K(2, 3) = (6 * E * I / (L * L)) * ly;
    K(2, 4) = -(6 * E * I / (L * L)) * lx;
    K(2, 5) = (2 * E * I / (L));

    K(3, 0) = -(A * E / L * (lx * lx) + 12 * E * I * (ly * ly) / (L * L * L));
    K(3, 1) = -(A * E / L - 12 * E * I / (L * L * L)) * (lx * ly);
    K(3, 2) = (6 * E * I / (L * L)) * ly;
    K(3, 3) = (A * E / L * (lx * lx) + 12 * E * I * (ly * ly) / (L * L * L));
    K(3, 4) = (A * E / L - 12 * E * I / (L * L * L)) * (lx * ly);
    K(3, 5) = (6 * E * I / (L * L)) * ly;

    K(4, 0) = -(A * E / L - 12 * E * I / (L * L * L)) * (lx * ly);
    K(4, 1) = -(A * E / L * (ly * ly) + 12 * E * I * (lx * lx) / (L * L * L));
    K(4, 2) = -(6 * E * I / (L * L)) * lx;
    K(4, 3) = (A * E / L - 12 * E * I / (L * L * L)) * (lx * ly);
    K(4, 4) = (A * E / L * (ly * ly) + 12 * E * I * (lx * lx) / (L * L * L));
    K(4, 5) = -(6 * E * I / (L * L)) * lx;

    K(5, 0) = -(6 * E * I / (L * L)) * ly;
    K(5, 1) = (6 * E * I / (L * L)) * lx;
    K(5, 2) = (2 * E * I / (L));
    K(5, 3) = (6 * E * I / (L * L)) * ly;
    K(5, 4) = -(6 * E * I / (L * L)) * lx;
    K(5, 5) = (4 * E * I / (L));

    return K;
}

//! Gets the local stiffness matrix of the element.
TPZFMatrix<double> TElement::getLocalK() const
{
    TMaterial material = fStructure->getMaterials()[fMaterialID];
    double E = material.getE();
    double A = material.getA();
    double I = material.getI();
    double L = this->getL();

    TPZFMatrix<double> localK(6, 6, 0);

    // Fills localK.
    localK(0, 0) = A * E / L;
    localK(0, 3) = -A * E / L;

    localK(1, 1) = 12 * E * I / (L * L * L);
    localK(1, 2) = 6 * E * I / (L * L);
    localK(1, 4) = -12 * E * I / (L * L * L);
    localK(1, 5) = 6 * E * I / (L * L);

    localK(2, 1) = 6 * E * I / (L * L);
    localK(2, 2) = 4 * E * I / L;
    localK(2, 4) = -6 * E * I / (L * L);
    localK(2, 5) = 2 * E * I / L;

    localK(3, 0) = -A * E / L;
    localK(3, 3) = A * E / L;

    localK(4, 1) = -12 * E * I / (L * L * L);
    localK(4, 2) = -6 * E * I / (L * L);
    localK(4, 4) = 12 * E * I / (L * L * L);
    localK(4, 5) = -6 * E * I / (L * L);

    localK(5, 1) = 6 * E * I / (L * L);
    localK(5, 2) = 2 * E * I / L;
    localK(5, 4) = -6 * E * I / (L * L);
    localK(5, 5) = 4 * E * I / L;

    return localK;
}

//! Gets the vector of initial (intermediate) loads Q0 of the element.
TPZFMatrix<double> TElement::getQ0() const
{
    return fQ0;
}

//! Modifies the vector of initial (intermediate) loads Q0 of the element.
void TElement::setQ0(TPZFMatrix<double> Q0)
{
    fQ0 = Q0;
}

//! Prints the element information to std::cout.
void TElement::print()
{
    std::cout << "Element Info: " << std::endl
              << " Node 0: " << fLocalNodesIDs[0] << std::endl
              << " Node 1: " << fLocalNodesIDs[1] << std::endl
              << " Hinge 0: " << fHinges[0] << std::endl
              << " Hinge 1: " << fHinges[1] << std::endl
              << " Material: " << fMaterialID << std::endl
              << " L: " << this->getL() << std::endl
              << " Cos: " << this->getCos() << std::endl
              << " Sin: " << this->getSin() << std::endl
              << " Equations: (" << fEquations[0] << ", " << fEquations[1] << ", "
              << fEquations[2] << "; " << fEquations[3] << ", " << fEquations[4]
              << ", " << fEquations[5] << ")" << std::endl;
    std::cout << std::flush;
}
