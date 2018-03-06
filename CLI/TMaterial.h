/** \file TMaterial.h
* Contains the declaration of the TMaterial class.
*/

#ifndef TMATERIAL_H
#define TMATERIAL_H

#include <iostream>

//!  A class that implements the material of a frame element.
/*!
     A class that implements the material of a frame element.
	 An object of this class has three double variables: the material Young's
	 Modulus E, the cross-sectional area A and the second moment of area I.
*/
class TMaterial
{
public:
	//! Default constructor.
	/*!
	\param E the Young's Modulus of the material.
	\param A the cross-sectional area of the material.
	\param I the second moment of area of the material.
	\return the new TMaterial object.
	*/
	TMaterial(double E = 0, double A = 0, double I = 0);

	//! Copy constructor.
	/*!
	\param Other the TMaterial object to be copied.
	\return the new TMaterial object.
	*/
	TMaterial(const TMaterial& Other);

	//! Destructor.
	~TMaterial();

	//! Assignment operator.
	/*!
	\param Other the TMaterial object to be copied.
	\return the modified TMaterial object.
	*/
	TMaterial& operator=(const TMaterial& Other);

	//! Gets the Young's Modulus of the material.
	/*!
	\return the Young's Modulus of the material.
	*/
	double getE() const;

	//! Gets the cross-sectional area of the material.
	/*!
	\return the cross-sectional area of the material.
	*/
	double getA() const;

	//! Gets the second moment of area of the material.
	/*!
	\return the second moment of area of the material.
	*/
	double getI() const;

	//! Modifies the Young's Modulus of the material.
	/*!
	\param E the Young's Modulus of the material.
	*/
	void setE(double E);

	//! Modifies the cross-sectional area of the material.
	/*!
	\param A the cross-sectional area of the material.
	*/
	void setA(double A);

	//! Modifies the second moment of area.
	/*!
	\param I the second moment of area of the material.
	*/
	void setI(double I);

//! Prints the material information to std::cout.
	void print();

private:
	//! The Young's Modulus of the material.
	double fE;
	//! The cross-sectional area of the material.
	double fA;
	//! The second moment of area of the material.
	double fI;
};

#endif // TMATERIAL_H
