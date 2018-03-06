/* This file was created by Gustavo BATISTELA.
It contains the definitions of functions of the TSupportGraphics class.*/

#include "TSupportGraphics.h"

// TSupportGraphics default constructor.
TSupportGraphics::TSupportGraphics(TStructureGraphics* StructureGraphics) {

	fSupportSize = 40;
	fHingeRadius = 5;

	// Assigns TStructureGraphics pointer and retrieves support size data from its settings.
	fStructureGraphics = StructureGraphics;
	std::vector<TSupport> supports = StructureGraphics->getStructure()->getSupports();

	// Creates instance of each support type accordingly.
	TPZFMatrix<double>* nodeCoordinates = fStructureGraphics->getNodeCoordinates();
	for (int i = 0; i < (int) supports.size(); i++) {
		
		// Creates a QPoint object with the node coordinates.
		int nodeID = supports[i].getNodeID();
		QPoint referencePoint((*nodeCoordinates)(supports[i].getNodeID(), 0),
			(*nodeCoordinates)(supports[i].getNodeID(), 1));
		
		// Checks the type of support and redirects to the proper drawing function.
		if (supports[i].getFx() == true) {
			if (supports[i].getFy() == true) {
				if (supports[i].getM() == true) {
					fSupportGraphics.push_back(new T111Support(referencePoint, fSupportSize));
				}
				else {
					fSupportGraphics.push_back(new T110Support(referencePoint, fSupportSize, fHingeRadius));
				}
			}
			else {
				if (supports[i].getM() == true) {
					fSupportGraphics.push_back(new T101Support(referencePoint, fSupportSize, fHingeRadius));
				}
				else {
					fSupportGraphics.push_back(new T100Support(referencePoint, fSupportSize, fHingeRadius));
				}
			}
		}
		else {
			if (supports[i].getFy() == true) {
				if (supports[i].getM() == true) {
					fSupportGraphics.push_back(new T011Support(referencePoint, fSupportSize, fHingeRadius));
				}
				else {
					fSupportGraphics.push_back(new T010Support(referencePoint, fSupportSize, fHingeRadius));
				}
			}
			else {
				if (supports[i].getM() == true) {
					fSupportGraphics.push_back(new T001Support(referencePoint, fSupportSize));
				}
			}
		}
	}
}

// Destructor.
TSupportGraphics::~TSupportGraphics() {}

// Bounding rectangle function.
QRectF TSupportGraphics::boundingRect() const {
	int xmin, xmax, ymin, ymax;
	for (int i = 0; i < (int)fSupportGraphics.size(); i++) {
		QRectF rec = fSupportGraphics[i]->boundingRect();
		if (i == 0) {
			// First assignment of xmin and ymin.
			xmin = rec.bottomLeft().x();
			ymin = rec.bottomLeft().y();

			// First assignment of ymax and ymax.
			xmax = rec.topRight().x();
			ymax = rec.topRight().y();
		}
		else {
			if (rec.bottomLeft().x() < xmin) { xmin = rec.bottomLeft().x(); }
			if (rec.bottomLeft().y() < ymin) { ymin = rec.bottomLeft().y(); }
			if (rec.topRight().x() > xmax) { xmax = rec.topRight().x(); }
			if (rec.topRight().y() > ymax) { ymax = rec.topRight().y(); }
		}
	}
	return QRectF(QPointF(xmin, ymax), QPointF(xmax, ymin)).normalized().adjusted(-5, -5, 5, 5);
}

// Painter function.
void TSupportGraphics::paint(QPainter * Painter,
							 const QStyleOptionGraphicsItem * Option,
							 QWidget * Widget) {
	(void)Option;
	(void)Widget;

	// Changes pen color.
	Painter->setPen(QPen(QColor(80, 80, 80, 255), 1.5));

	for (int i = 0; i < (int) fSupportGraphics.size(); i++) {
		fSupportGraphics[i]->paint(Painter, nullptr, nullptr);
	}
}

// T001Support default constructor.
T001Support::T001Support(QPoint& ReferencePoint, int ReferenceSize) {
	fReferencePoint = ReferencePoint;
	fReferenceSize = ReferenceSize;
}

// Copy constructor.
T001Support::T001Support(const T001Support & S) {
	fReferencePoint = S.fReferencePoint;
	fReferenceSize = S.fReferenceSize;
}

// Destructor.
T001Support::~T001Support() {}

// T001Support painter function.
void T001Support::paint(QPainter* Painter,
						const QStyleOptionGraphicsItem* Option,
						QWidget* Widget) {
	(void)Option;
	(void)Widget;

	// Draws square.
	int x0 = fReferencePoint.x() - 0.30 * fReferenceSize;
	int y0 = fReferencePoint.y() - 0.30 * fReferenceSize;
	Painter->drawRect(x0, y0, 0.60 * fReferenceSize, 0.60 * fReferenceSize);
}

// Bounding rectangle function.
QRectF T001Support::boundingRect() const {
	int x0 = fReferencePoint.x() - 0.30 * fReferenceSize;
	int y0 = fReferencePoint.y() - 0.30 * fReferenceSize;
	return QRectF(x0, y0, 0.60 * fReferenceSize, 0.60 * fReferenceSize).normalized().adjusted(-5, -5, 5, 5);
}

// Assignment operator.
T001Support & T001Support::operator=(const T001Support & S) {
	if (this != &S) {
		fReferencePoint = S.fReferencePoint;
		fReferenceSize = S.fReferenceSize;
	}
	return *this;
}

// T010Support default constructor.
T010Support::T010Support(QPoint& ReferencePoint, int ReferenceSize,
	int HingeRadius) {
	fReferencePoint = ReferencePoint;
	fReferenceSize = ReferenceSize;
	fHingeRadius = HingeRadius;
}

// Copy constructor.
T010Support::T010Support(const T010Support & S) {
	fReferencePoint = S.fReferencePoint;
	fReferenceSize = S.fReferenceSize;
	fHingeRadius = S.fHingeRadius;
}

// Destructor.
T010Support::~T010Support() {}

// T010Support painter function.
void T010Support::paint(QPainter* Painter,
						const QStyleOptionGraphicsItem* Option,
						QWidget* Widget) {
	(void)Option;
	(void)Widget;

	// Draws triangle and base lines.
	QPoint aux1(fReferencePoint);
	QPoint aux2(fReferencePoint);

	aux1.rx() -= 0.50 * fReferenceSize;
	aux1.ry() += 0.80 * fReferenceSize;
	aux2.rx() += 0.50 * fReferenceSize;
	aux2.ry() += 0.80 * fReferenceSize;
	Painter->drawLine(fReferencePoint, aux1);
	Painter->drawLine(fReferencePoint, aux2);

	aux1.rx() -= 0.25 * fReferenceSize;
	aux2.rx() += 0.25 * fReferenceSize;
	Painter->drawLine(aux1, aux2);

	aux1.ry() += 0.15 * fReferenceSize;
	aux2.ry() += 0.15 * fReferenceSize;
	Painter->drawLine(aux1, aux2);

	// Draws circle at node.
	Painter->drawEllipse(fReferencePoint, fHingeRadius, fHingeRadius);
}

// Bounding rectangle function.
QRectF T010Support::boundingRect() const {
	int x0 = fReferencePoint.x() - 0.75 * fReferenceSize;
	int y0 = fReferencePoint.y();
	return QRectF(x0, y0, 1.50 * fReferenceSize, 0.95 * fReferenceSize).normalized().adjusted(-5, -5, 5, 5);
}

// Assignment operator.
T010Support & T010Support::operator=(const T010Support & S) {
	if (this != &S) {
		fReferencePoint = S.fReferencePoint;
		fReferenceSize = S.fReferenceSize;
		fHingeRadius = S.fHingeRadius;
	}
	return *this;
}

// T011Support default constructor.
T011Support::T011Support(QPoint& ReferencePoint, int ReferenceSize,
	int RollerRadius) {
	fReferencePoint = ReferencePoint;
	fReferenceSize = ReferenceSize;
	fRollerRadius = RollerRadius;
}

// Copy constructor.
T011Support::T011Support(const T011Support & S) {
	fReferencePoint = S.fReferencePoint;
	fReferenceSize = S.fReferenceSize;
	fRollerRadius = S.fRollerRadius;
}

// Destructor.
T011Support::~T011Support() {}

// T011Support painter function.
void T011Support::paint(QPainter* Painter,
						const QStyleOptionGraphicsItem* Option,
						QWidget* Widget) {
	(void)Option;
	(void)Widget;

	// Draws base lines.
	QPoint aux1(fReferencePoint);
	QPoint aux2(fReferencePoint);
	aux1.rx() -= 0.75 * fReferenceSize;
	aux2.rx() += 0.75 * fReferenceSize;
	Painter->drawLine(aux1, aux2);

	aux1.ry() += 2 * fRollerRadius;
	aux2.ry() += 2 * fRollerRadius;
	Painter->drawLine(aux1, aux2);

	// Draws set of diagonal lines.
	aux2.ry() += 0.15 * fReferenceSize;
	for (int i = 0; i < 7; i++) {
		aux1.rx() = fReferencePoint.x() + (0.25 * i - 0.75) * fReferenceSize;
		aux2.rx() = aux1.x() - 0.10 * fReferenceSize;
		Painter->drawLine(aux1, aux2);
	}

	// Draws circles of rollers.
	aux1.rx() = fReferencePoint.x() - 0.375 * fReferenceSize;
	aux1.ry() = fReferencePoint.y() + fRollerRadius;
	Painter->drawEllipse(aux1, fRollerRadius, fRollerRadius);

	aux1.rx() =fReferencePoint.x() + 0.375 * fReferenceSize;
	Painter->drawEllipse(aux1, fRollerRadius, fRollerRadius);
}

// Bounding rectangle function.
QRectF T011Support::boundingRect() const {
	int x0 = fReferencePoint.x() - 0.85 * fReferenceSize;
	int y0 = fReferencePoint.y();
	return QRectF(x0, y0, 1.60 * fReferenceSize,
		2 * fRollerRadius + 0.15 * fReferenceSize).normalized().adjusted(-5, -5, 5, 5);
}

// Assignment operator.
T011Support & T011Support::operator=(const T011Support & S) {
	if (this != &S) {
		fReferencePoint = S.fReferencePoint;
		fReferenceSize = S.fReferenceSize;
		fRollerRadius = S.fRollerRadius;
	}
	return *this;
}

// T100Support default constructor.
T100Support::T100Support(QPoint& ReferencePoint, int ReferenceSize,
	int HingeRadius) {
	fReferencePoint = ReferencePoint;
	fReferenceSize = ReferenceSize;
	fHingeRadius = HingeRadius;
}

// Copy constructor.
T100Support::T100Support(const T100Support & S) {
	fReferencePoint = S.fReferencePoint;
	fReferenceSize = S.fReferenceSize;
	fHingeRadius = S.fHingeRadius;
}

// Destructor.
T100Support::~T100Support() {}

// T100Support painter function.
void T100Support::paint(QPainter* Painter,
						const QStyleOptionGraphicsItem* Option,
						QWidget* Widget) {
	(void)Option;
	(void)Widget;

	// Draws triangle and base lines.
	QPoint aux1(fReferencePoint);
	QPoint aux2(fReferencePoint);

	aux1.rx() -= 0.80 * fReferenceSize;
	aux1.ry() -= 0.50 * fReferenceSize;
	aux2.rx() -= 0.80 * fReferenceSize;
	aux2.ry() += 0.50 * fReferenceSize;
	Painter->drawLine(fReferencePoint, aux1);
	Painter->drawLine(fReferencePoint, aux2);

	aux1.ry() -= 0.25 * fReferenceSize;
	aux2.ry() += 0.25 * fReferenceSize;
	Painter->drawLine(aux1, aux2);

	aux1.rx() -= 0.15 * fReferenceSize;
	aux2.rx() -= 0.15 * fReferenceSize;
	Painter->drawLine(aux1, aux2);

	// Draws circle at node.
	Painter->drawEllipse(fReferencePoint, fHingeRadius, fHingeRadius);
}

// Bounding rectangle function.
QRectF T100Support::boundingRect() const {
	int x0 = fReferencePoint.x() - 0.95 * fReferenceSize;
	int y0 = fReferencePoint.y() - 0.75 * fReferenceSize;
	return QRectF(x0, y0, 0.95 * fReferenceSize, 1.50 * fReferenceSize).normalized().adjusted(-5, -5, 5, 5);
}

// Assignment operator.
T100Support & T100Support::operator=(const T100Support & S) {
	if (this != &S) {
		fReferencePoint = S.fReferencePoint;
		fReferenceSize = S.fReferenceSize;
		fHingeRadius = S.fHingeRadius;
	}
	return *this;
}

// T101Support default constructor.
T101Support::T101Support(QPoint& ReferencePoint, int ReferenceSize,
	int RollerRadius) {
	fReferencePoint = ReferencePoint;
	fReferenceSize = ReferenceSize;
	fRollerRadius = RollerRadius;
}

// Copy constructor.
T101Support::T101Support(const T101Support & S) {
	fReferencePoint = S.fReferencePoint;
	fReferenceSize = S.fReferenceSize;
	fRollerRadius = S.fRollerRadius;
}

// Destructor.
T101Support::~T101Support() {}

// T101Support painter function.
void T101Support::paint(QPainter* Painter,
						const QStyleOptionGraphicsItem* Option,
						QWidget* Widget) {
	(void)Option;
	(void)Widget;

	// Draws base lines.
	QPoint aux1(fReferencePoint);
	QPoint aux2(fReferencePoint);
	aux1.ry() -= 0.75 * fReferenceSize;
	aux2.ry() += 0.75 * fReferenceSize;
	Painter->drawLine(aux1, aux2);

	aux1.rx() -= 2 * fRollerRadius;
	aux2.rx() -= 2 * fRollerRadius;
	Painter->drawLine(aux1, aux2);

	// Draws set of diagonal lines.
	aux2.rx() -= 0.15 * fReferenceSize;
	for (int i = 0; i < 7; i++) {
		aux1.ry() = fReferencePoint.y() + (0.25 * i - 0.75) * fReferenceSize;
		aux2.ry() = aux1.y() - 0.1 * fReferenceSize;
		Painter->drawLine(aux1, aux2);
	}

	// Draws circles of rollers.
	aux1.rx() = fReferencePoint.x() - fRollerRadius;
	aux1.ry() = fReferencePoint.y() - 0.375 * fReferenceSize;
	Painter->drawEllipse(aux1, fRollerRadius, fRollerRadius);

	aux1.ry() = fReferencePoint.y() + 0.375 * fReferenceSize;
	Painter->drawEllipse(aux1, fRollerRadius, fRollerRadius);
}

// Bounding rectangle function.
QRectF T101Support::boundingRect() const {
	int x0 = fReferencePoint.x() - (2 * fRollerRadius + 0.15 * fReferenceSize);
	int y0 = fReferencePoint.y() - 0.85 * fReferenceSize;
	return QRectF(x0, y0, 2 * fRollerRadius + 0.15 * fReferenceSize,
		1.60 * fReferenceSize).normalized().adjusted(-5, -5, 5, 5);
}

// Assignment operator.
T101Support & T101Support::operator=(const T101Support & S) {
	if (this != &S) {
		fReferencePoint = S.fReferencePoint;
		fReferenceSize = S.fReferenceSize;
		fRollerRadius = S.fRollerRadius;
	}
	return *this;
}

// T110Support default constructor.
T110Support::T110Support(QPoint& ReferencePoint, int ReferenceSize,
	int HingeRadius) {
	fReferencePoint = ReferencePoint;
	fReferenceSize = ReferenceSize;
	fHingeRadius = HingeRadius;
}

// Copy constructor.
T110Support::T110Support(const T110Support & S) {
	fReferencePoint = S.fReferencePoint;
	fReferenceSize = S.fReferenceSize;
	fHingeRadius = S.fHingeRadius;
}

// Destructor.
T110Support::~T110Support() {}

// T110Support painter function.
void T110Support::paint(QPainter* Painter,
						const QStyleOptionGraphicsItem* Option,
						QWidget* Widget) {
	(void)Option;
	(void)Widget;

	// Draws triangle and base line.
	QPoint aux1(fReferencePoint);
	QPoint aux2(fReferencePoint);

	aux1.rx() -= 0.50 * fReferenceSize;
	aux1.ry() += 0.80 * fReferenceSize;
	aux2.rx() += 0.50 * fReferenceSize;
	aux2.ry() += 0.80 * fReferenceSize;
	Painter->drawLine(fReferencePoint, aux1);
	Painter->drawLine(fReferencePoint, aux2);

	aux1.rx() -= 0.25 * fReferenceSize;
	aux2.rx() += 0.25 * fReferenceSize;
	Painter->drawLine(aux1, aux2);

	// Draws set of diagonal lines.
	aux2.ry() += 0.15 * fReferenceSize;
	for (int i = 0; i < 7; i++) {
		aux1.rx() = fReferencePoint.x() + (0.25 * i - 0.75) * fReferenceSize;
		aux2.rx() = aux1.x() - 0.1 * fReferenceSize;
		Painter->drawLine(aux1, aux2);
	}

	// Draws circle at node.
	Painter->drawEllipse(fReferencePoint, fHingeRadius, fHingeRadius);
}

// Bounding rectangle function.
QRectF T110Support::boundingRect() const {
	int x0 = fReferencePoint.x() - 0.85 * fReferenceSize;
	int y0 = fReferencePoint.y();
	return QRectF(x0, y0, 1.60 * fReferenceSize, 0.95 * fReferenceSize).normalized().adjusted(-5, -5, 5, 5);
}

// Assignment operator.
T110Support & T110Support::operator=(const T110Support & S) {
	if (this != &S) {
		fReferencePoint = S.fReferencePoint;
		fReferenceSize = S.fReferenceSize;
		fHingeRadius = S.fHingeRadius;
	}
	return *this;
}

// T111Support default constructor.
T111Support::T111Support(QPoint& ReferencePoint, int ReferenceSize) {
	fReferencePoint = ReferencePoint;
	fReferenceSize = ReferenceSize;
}

// Copy constructor.
T111Support::T111Support(const T111Support & S) {
	fReferencePoint = S.fReferencePoint;
	fReferenceSize = S.fReferenceSize;
}

// Destructor.
T111Support::~T111Support() {}

// T111Support painter function.
void T111Support::paint(QPainter* Painter,
						const QStyleOptionGraphicsItem* Option,
						QWidget* Widget) {
	(void)Option;
	(void)Widget;

	// Draws base line.
	QPoint aux1(fReferencePoint);
	QPoint aux2(fReferencePoint);
	aux1.rx() -= 0.75 * fReferenceSize;
	aux2.rx() += 0.75 * fReferenceSize;
	Painter->drawLine(aux1, aux2);

	// Draws set of diagonal lines.
	aux2.ry() += 0.15 * fReferenceSize;
	for (int i = 0; i < 7; i++) {
		aux1.rx() = fReferencePoint.x() + (0.25 * i - 0.75) * fReferenceSize;
		aux2.rx() = aux1.x() - 0.1 * fReferenceSize;
		Painter->drawLine(aux1, aux2);
	}
}

// Bounding rectangle function.
QRectF T111Support::boundingRect() const {
	int x0 = fReferencePoint.x() - 0.85 * fReferenceSize;
	int y0 = fReferencePoint.y();
	return QRectF(x0, y0, 1.60 * fReferenceSize, 0.15 * fReferenceSize).normalized().adjusted(-5, -5, 5, 5);
}

// Assignment operator.
T111Support & T111Support::operator=(const T111Support & S) {
	if (this != &S) {
		fReferencePoint = S.fReferencePoint;
		fReferenceSize = S.fReferenceSize;
	}
	return *this;
}