/* This file was created by Gustavo BATISTELA.
It is a header file containing the definition of the TLoadGraphics classes
and the declaration of their members. */

#ifndef TLOADGRAPHICS_H
#define TLOADGRAPHICS_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

#include <QGraphicsItem>
#include <QPainter>
#include <math.h>
#include "TStructure.h"
#include "TStructureGraphics.h"

// Declaration of TLabel class.
class TLabel : public QGraphicsItem
{
public:

    // Default constructor.
    TLabel(QString Message, QPointF Position, int Size, QPen Pen = QPen(Qt::black));
    // Destructor.
    ~TLabel();

    // Painter function.
    void paint(QPainter *Painter, const QStyleOptionGraphicsItem *Option,
               QWidget *Widget) override;

    // Bounding rectangle function.
    QRectF boundingRect() const override;

private:
    QString fMessage;
    QPointF fPosition;
    QFont fFont;
    QPen fPen;
};

// Declaration of TLoadArrow class.
class TLoadArrow : public QGraphicsItem {
public:
    enum ArrowStyle {
        Nodal = 0,
        Reaction = 1,
        Distributed = 2
    };

    // Default constructor.
    TLoadArrow(const QPointF ReferencePoint, qreal ArrowLength,
               qreal TabLength, qreal Angle, ArrowStyle Style);
    // Copy constructor.
    TLoadArrow(const TLoadArrow& Other);
    // Destructor.
    ~TLoadArrow();

    // Painter function.
    void paint(QPainter *Painter, const QStyleOptionGraphicsItem *Option,
               QWidget *Widget) override;

    // Bounding rectangle function.
    QRectF boundingRect() const override;

    // Assignment operator.
    TLoadArrow& operator=(const TLoadArrow& Other);

    // getArrowLength - returns the length of the arrow.
    int getArrowLength();

private:
    QPointF fReferencePoint;
    qreal fArrowLength;
    qreal fTabLength;
    qreal fAngle;
    ArrowStyle fArrowStyle;
};

// Declaration of TMomentArrow class.
class TMomentArrow : public QGraphicsItem {
public:
    enum ArrowStyle {
        Nodal = 0,
        Reaction = 1
    };

    // Default constructor.
    TMomentArrow(const QPointF ReferencePoint, int ArcRadius, double ArcStartAngle,
                 double ArcSpanAngle, int TabLength, ArrowStyle Style);
    // Destructor.
    ~TMomentArrow();

    // Painter function.
    void paint(QPainter *Painter, const QStyleOptionGraphicsItem *Option,
               QWidget *Widget) override;

    // Bounding rectangle function.
    QRectF boundingRect() const override;

private:
    QPointF fReferencePoint;
    int fArcRadius;
    double fArcStartAngle;
    double fArcSpanAngle;
    int fTabLength;
    ArrowStyle fArrowStyle;
};

// Declaration of TNodalLoadGraphics class.
class TNodalLoadGraphics : public QGraphicsItem {
public:
    enum LoadType {
        Fx = 0,
        Fy = 1,
        M = 2
    };
    // Default constructor.
    TNodalLoadGraphics(QPointF LoadPosition, LoadType Type, bool Orientation);
    // Destructor.
    ~TNodalLoadGraphics();

    // Bounding rectangle function.
    QRectF boundingRect() const override;

    // Painter function.
    void paint(QPainter *Painter, const QStyleOptionGraphicsItem *Option,
               QWidget *Widget) override;

private:
    QPointF fLoadPosition;
    LoadType fLoadType;
    QGraphicsItem *fNodalLoad;
};

// Declaration of TReactionLoadGraphics class.
class TReactionLoadGraphics : public QGraphicsItem {
public:
    enum LoadType {
        Fx = 0,
        Fy = 1,
        M = 2
    };
    // Default constructor.
    TReactionLoadGraphics(QPointF LoadPosition, LoadType Type, bool Orientation);
    // Destructor.
    ~TReactionLoadGraphics();

    // Bounding rectangle function.
    QRectF boundingRect() const override;

    // Painter function.
    void paint(QPainter *Painter, const QStyleOptionGraphicsItem *Option,
               QWidget *Widget) override;

private:
    QPointF fLoadPosition;
    LoadType fLoadType;
    QGraphicsItem *fNodalLoad;
};

// Declaration of TDistributedLoadGraphics class.
class TDistributedLoadGraphics : public QGraphicsItem {
public:
    // Default constructor.
    TDistributedLoadGraphics(TDistributedLoad * DistributedLoad, QPointF StartPoint, QPointF EndPoint, qreal ScalingFactor);
    // Destructor.
    ~TDistributedLoadGraphics();

    // Bounding rectangle function.
    QRectF boundingRect() const override;

    // Painter function.
    void paint(QPainter *Painter, const QStyleOptionGraphicsItem *Option,
               QWidget *Widget) override;

private:
    bool fLoadPlane;
    QPointF fStartPoint;
    QPointF fEndPoint;
    std::vector<TLoadArrow *> fDistributedLoad;
};

// Declaration of TDiagramLoadGraphics class.
class TDiagramLoadGraphics : public QGraphicsItem {
public:
    // Default constructor.
    TDiagramLoadGraphics(QPointF StartPoint, QPointF EndPoint, const double *Coefficients, qreal LoadScale);
    // Destructor.
    ~TDiagramLoadGraphics();

    // Bounding rectangle function.
    QRectF boundingRect() const override;

    // Painter function.
    void paint(QPainter *Painter, const QStyleOptionGraphicsItem *Option,
               QWidget *Widget) override;

    // Evaluates the cubic function.
    qreal eval(qreal u);

private:
    QPointF fStartPoint;
    QPointF fEndPoint;
    double fCoefficients[4];
    QPainterPath fCurve;
    qreal fUScale;
};

#endif // TLOADGRAPHICS_H
