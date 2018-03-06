/** \file TMaterial.cpp
* Contains the definitions of the TMaterial methods.
*/

#include "TMaterial.h"

//! Default constructor.
TMaterial::TMaterial(double E, double A, double I) : fE(E), fA(A), fI(I) {}

//! Copy constructor.
TMaterial::TMaterial(const TMaterial& Other) : fE(Other.fE), fA(Other.fA),
	fI(Other.fI) {}

//! Destructor.
TMaterial::~TMaterial() {}

//! Assignment operator.
TMaterial& TMaterial::operator=(const TMaterial& Other)
{
	if (this != &Other) {
		fE = Other.fE;
		fA = Other.fA;
		fI = Other.fI;
	}
	return *this;
}

//! Gets the Young's Modulus of the material.
double TMaterial::getE() const
{
	return fE;
}

//! Gets the cross-sectional area of the material.
double TMaterial::getA() const
{
	return fA;
}

//! Gets the second moment of area of the material.
double TMaterial::getI() const
{
	return fI;
}

//! Modifies the Young's Modulus of the material.
void TMaterial::setE(double E)
{
	fE = E;
}

//! Modifies the cross-sectional area of the material.
void TMaterial::setA(double A)
{
	fA = A;
}

//! Modifies the second moment of area of the material.
void TMaterial::setI(double I)
{
	fI = I;
}

//! Prints the information of a node.
void TMaterial::print()
{
	std::cout << "Material Properties:" << std::endl
		<< " E: " << fE << std::endl
		<< " A: " << fA << std::endl
		<< " I: " << fI << std::endl;
	std::cout << std::flush;
}
