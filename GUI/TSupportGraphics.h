/* This file was created by Gustavo BATISTELA.
It is a header file containing the definition of the TSupportGraphics class
and the declaration of its members. */

#ifndef TSUPPORTGRAPHICS_H 
#define TSUPPORTGRAPHICS_H

#include <QGraphicsItem>
#include <QPainter>
#include "TSupport.h"
#include "TStructureGraphics.h"

class TSupportGraphics : public QGraphicsItem {
public:
	// Default constructor.
	TSupportGraphics(TStructureGraphics* StructureGraphics = nullptr);
	// Destructor.
	~TSupportGraphics();

	// Bounding rectangle function.
	QRectF boundingRect() const override;

	// Paint function.
	void paint(QPainter * painter,
		const QStyleOptionGraphicsItem * option,
		QWidget * widget) override;

private:
	TStructureGraphics* fStructureGraphics;
	std::vector<QGraphicsItem*> fSupportGraphics;
	int fSupportSize;
	int fHingeRadius;
};

class T001Support : public QGraphicsItem {
public:
	// Default constructor.
	T001Support(QPoint &ReferencePoint, int ReferenceSize);
	// Copy constructor.
	T001Support(const T001Support& S);
	// Destructor.
	~T001Support();

	// Painter function.
	void paint(QPainter *Painter, 
			   const QStyleOptionGraphicsItem *Option,
			   QWidget *Widget) override;

	// Bounding rectangle function.
	QRectF boundingRect() const override;

	// Assignment operator.
	T001Support& operator=(const T001Support& S);

private:
	QPoint fReferencePoint;
	int fReferenceSize;
};

class T010Support : public QGraphicsItem {
public:
	// Default constructor.
	T010Support(QPoint &ReferencePoint, int ReferenceSize, int HingeRadius);
	// Copy constructor.
	T010Support(const T010Support& S);
	// Destructor.
	~T010Support();

	// Painter function.
	void paint(QPainter *Painter,
			   const QStyleOptionGraphicsItem *Option,
		       QWidget *Widget) override;

	// Bounding rectangle function.
	QRectF boundingRect() const override;

	// Assignment operator.
	T010Support& operator=(const T010Support& S);

private:
	QPoint fReferencePoint;
	int fReferenceSize;
	int fHingeRadius;
};

class T011Support : public QGraphicsItem {
public:
	// Default constructor.
	T011Support(QPoint &ReferencePoint, int ReferenceSize, int RollerRadius);
	// Copy constructor.
	T011Support(const T011Support& S);
	// Destructor.
	~T011Support();

	// Painter function.
	void paint(QPainter *Painter,
			   const QStyleOptionGraphicsItem *Option,
			   QWidget *Widget) override;

	// Bounding rectangle function.
	QRectF boundingRect() const override;

	// Assignment operator.
	T011Support& operator=(const T011Support& S);

private:
	QPoint fReferencePoint;
	int fReferenceSize;
	int fRollerRadius;
};

class T100Support : public QGraphicsItem {
public:
	// Default constructor.
	T100Support(QPoint &ReferencePoint, int ReferenceSize, int HingeRadius);
	// Copy constructor.
	T100Support(const T100Support& S);
	// Destructor.
	~T100Support();

	// Painter function.
	void paint(QPainter *Painter,
			   const QStyleOptionGraphicsItem *Option,
			   QWidget *Widget) override;

	// Bounding rectangle function.
	QRectF boundingRect() const override;

	// Assignment operator.
	T100Support& operator=(const T100Support& S);

private:
	QPoint fReferencePoint;
	int fReferenceSize;
	int fHingeRadius;
};

class T101Support : public QGraphicsItem {
public:
	// Default constructor.
	T101Support(QPoint &ReferencePoint, int ReferenceSize, int RollerRadius);
	// Copy constructor.
	T101Support(const T101Support& S);
	// Destructor.
	~T101Support();

	// Painter function.
	void paint(QPainter *Painter,
		       const QStyleOptionGraphicsItem *Option,
			   QWidget *Widget) override;

	// Bounding rectangle function.
	QRectF boundingRect() const override;

	// Assignment operator.
	T101Support& operator=(const T101Support& S);

private:
	QPoint fReferencePoint;
	int fReferenceSize;
	int fRollerRadius;
};

class T110Support : public QGraphicsItem {
public:
	// Default constructor.
	T110Support(QPoint &ReferencePoint, int ReferenceSize, int HingeRadius);
	// Copy constructor.
	T110Support(const T110Support& S);
	// Destructor.
	~T110Support();

	// Painter function.
	void paint(QPainter *Painter,
			   const QStyleOptionGraphicsItem *Option,
			   QWidget *Widget) override;

	// Bounding rectangle function.
	QRectF boundingRect() const override;

	// Assignment operator.
	T110Support& operator=(const T110Support& S);

private:
	QPoint fReferencePoint;
	int fReferenceSize;
	int fHingeRadius;
};

class T111Support : public QGraphicsItem {
public:
	// Default constructor.
	T111Support(QPoint &ReferencePoint, int ReferenceSize);
	// Copy constructor.
	T111Support(const T111Support& S);
	// Destructor.
	~T111Support();

	// Painter function.
	void paint(QPainter *Painter,
			   const QStyleOptionGraphicsItem *Option,
			   QWidget *Widget) override;

	// Bounding rectangle function.
	QRectF boundingRect() const override;

	// Assignment operator.
	T111Support& operator=(const T111Support& S);

private:
	QPoint fReferencePoint;
	int fReferenceSize;
};

#endif // TSUPPORTGRAPHICS_H