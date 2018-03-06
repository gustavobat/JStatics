/* This file was created by Gustavo BATISTELA.
It is a header file containing the definition of the TStructureGraphics class
and the declaration of its members. */

#ifndef TSTRUCTUREGRAPHICS_H 
#define TSTRUCTUREGRAPHICS_H

#include <QGraphicsItem>
#include <QPainter>
#include "TStructure.h"
#include "pzfmatrix.h"

// Declaration of TStructureGraphics class.
class TStructureGraphics : public QGraphicsItem {
public:
	// Default constructor.
	TStructureGraphics(TStructure * Structure);
	// Copy constructor.
	TStructureGraphics(const TStructureGraphics & Other);
	// Destructor.
	~TStructureGraphics();

	// Bounding rectangle function.
	QRectF boundingRect() const override;

	// Paint function.
	void paint(QPainter * painter,
		const QStyleOptionGraphicsItem * option,
		QWidget * widget) override;

	// Assignment operator.
	TStructureGraphics & operator=(const TStructureGraphics & Other);

	// getStructure - returns a pointer to the TStructure object.
	TStructure *getStructure() const;

	// getNodeCoordinates - returns fNodeCoordinates member.
	TPZFMatrix<double> *getNodeCoordinates();

private:
	// Calculates the node coordinates on the image object and stores in fNodeCoordinates member object.
	void convertNodes();

	TStructure *fStructure;
	TPZFMatrix<double> *fNodeCoordinates;

	int fImageWidth;
	int fImageHeight;
	int fHingeRadius;
};


// Declaration of TDeflectedStructureGraphics class.
class TDeflectedStructureGraphics : public QGraphicsItem {
public:
	// Default constructor.
	TDeflectedStructureGraphics();
	// Destructor.
	~TDeflectedStructureGraphics();

	// Calculates the node coordinates on the image object and stores in fNodeCoordinates member object.
	void convertNodes();

	// Painter function.
	void paint(QPainter *Painter, const QStyleOptionGraphicsItem *Option,
		QWidget *Widget) override;

	// Bounding rectangle function.
	QRectF boundingRect() const override;

	// getNodeCoordinates - returns fNodeCoordinates member.
	TPZFMatrix<double>* getNodeCoordinates();

private:
	TPZFMatrix<double>* fNodeCoordinates;
	TPZFMatrix<double>* fDisplacements;
	double fDisplacementScale;

	int fHingeRadius;
};

#endif // TSTRUCTUREGRAPHICS_H