/* This file was created by Gustavo BATISTELA.
It contains the definitions of functions of the TLoadGraphics class. */

#include "TLoadGraphics.h"

#include <utility>

TLabel::TLabel(QString Message, QPointF Position, int Size, QPen Pen) {
    fMessage = std::move(Message);
    fPosition = Position;
    fPen = std::move(Pen);

    fFont = QFont("Helvetica", Size);

    setFlag(ItemIsMovable, true);
}

TLabel::~TLabel() = default;

void TLabel::paint(QPainter *Painter, const QStyleOptionGraphicsItem *Option,
                   QWidget *Widget) {
    Painter->save();
    Painter->setFont(fFont);
    Painter->setPen(fPen);
    Painter->drawText(fPosition, fMessage);
    Painter->restore();
}

QRectF TLabel::boundingRect() const {
    QFontMetrics fontMetrics(fFont);
    QRectF rect = fontMetrics.boundingRect(fMessage);

    QTransform transform;
    transform.translate(fPosition.x(), fPosition.y());
    rect = transform.mapRect(rect);

    qreal extra = 5.;
    return rect.adjusted(-extra, -extra, extra, extra);
}

TLoadArrow::TLoadArrow(const QPointF ReferencePoint, qreal ArrowLength,
                       qreal TabLength, qreal Angle, ArrowStyle Style) {
    fReferencePoint = ReferencePoint;
    fArrowLength = ArrowLength;
    fTabLength = TabLength;
    fAngle = Angle;
    fArrowStyle = Style;
}

// Copy constructor.
TLoadArrow::TLoadArrow(const TLoadArrow &Other) {
    fReferencePoint = Other.fReferencePoint;
    fArrowLength = Other.fArrowLength;
    fTabLength = Other.fTabLength;
    fAngle = Other.fAngle;
    fArrowStyle = Other.fArrowStyle;
}

TLoadArrow::~TLoadArrow() = default;

void TLoadArrow::paint(QPainter *Painter,
                       const QStyleOptionGraphicsItem *Option,
                       QWidget *Widget) {

    // Sets painter transformation.
    Painter->save();
    Painter->translate(fReferencePoint.x(), fReferencePoint.y());
    Painter->rotate(-fAngle * 180 / PI);

    // Draws the arrow axis.
    QPointF origin(0.0, 0.0);
    QPointF aux1(origin);
    aux1.rx() += fArrowLength;
    Painter->drawLine(origin, aux1);

    if (fArrowStyle == Distributed) {
        // Only draws tabs if the arrow axis is longer than the tabs.
        if (abs(fArrowLength) >= fTabLength) {
            if (fArrowLength < 0.) {
                // Draws arrow tabs.
                aux1.rx() = -fTabLength * cos(PI / 6);
                aux1.ry() = -fTabLength * sin(PI / 6);
                Painter->drawLine(origin, aux1);

                aux1.rx() = -fTabLength * cos(PI / 6);
                aux1.ry() = fTabLength * sin(PI / 6);
                Painter->drawLine(origin, aux1);

            } else {
                // Draws arrow tabs.
                aux1.rx() = fTabLength * cos(PI / 6);
                aux1.ry() = -fTabLength * sin(PI / 6);
                Painter->drawLine(origin, aux1);

                aux1.rx() = fTabLength * cos(PI / 6);
                aux1.ry() = fTabLength * sin(PI / 6);
                Painter->drawLine(origin, aux1);
            }
        }
    } else {
        // Draws an equilateral triangle at the tip of the arrow.
        QPointF aux2;
        aux1.rx() = fTabLength * cos(PI / 6);
        aux1.ry() = -fTabLength * sin(PI / 6);
        aux2.rx() = fTabLength * cos(PI / 6);
        aux2.ry() = fTabLength * sin(PI / 6);

        QPolygonF triangle;
        triangle << origin << aux1 << aux2;
        Painter->drawPolygon(triangle);

        // Fills triangle with a solid color.
        QPainterPath filling;
        filling.addPolygon(triangle);
        Painter->fillPath(
            filling, QBrush(QColor(Painter->pen().color()), Qt::SolidPattern));

        // Draws a crossing line if the arrow represents a reaction load.
        if (fArrowStyle == Reaction) {
            aux1.rx() = fArrowLength / 2;
            aux1.ry() = -fTabLength / 2;

            aux2.rx() = fArrowLength / 2;
            aux2.ry() = fTabLength / 2;
            Painter->drawLine(aux1, aux2);
        }
    }
    // Restores painter transformation.
    Painter->restore();
}

QRectF TLoadArrow::boundingRect() const {
    // Draws a polygon from the arrow relevant points and transforms its
    // coordinates into an appropriate coordinate system given by the
    // transformation matrix.

    // Sets transformation matrix.
    QTransform transform;
    transform.translate(fReferencePoint.x(), fReferencePoint.y());
    transform.rotate(-fAngle * 180 / PI);

    // Creates polygon.
    QPolygonF arrowBorder;
    QPointF aux(0.0, 0.0);
    arrowBorder << aux;

    aux.rx() += fArrowLength;
    arrowBorder << aux;

    aux.rx() = fTabLength * cos(PI / 6);
    aux.ry() = -fTabLength * sin(PI / 6);
    arrowBorder << aux;

    aux.rx() = fTabLength * cos(PI / 6);
    aux.ry() = fTabLength * sin(PI / 6);
    arrowBorder << aux;

    // Transforms polygon coordinates.
    arrowBorder = transform.map(arrowBorder);

    // Returns the polygon bounding rect with the addition of an extra border.
    qreal extra = 4;
    return arrowBorder.boundingRect().adjusted(-extra, -extra, extra, extra);
}

// Assignment operator.
TLoadArrow &TLoadArrow::operator=(const TLoadArrow &Other) {
    if (this != &Other) {
        fReferencePoint = Other.fReferencePoint;
        fArrowLength = Other.fArrowLength;
        fTabLength = Other.fTabLength;
        fAngle = Other.fAngle;
        fArrowStyle = Other.fArrowStyle;
    }
    return *this;
}

int TLoadArrow::getArrowLength() { return fArrowLength; }

TMomentArrow::TMomentArrow(const QPointF ReferencePoint, int ArcRadius,
                           double ArcStartAngle, double ArcSpanAngle,
                           int TabLength, ArrowStyle Style) {

    fReferencePoint = ReferencePoint;
    fArcRadius = ArcRadius;
    fArcStartAngle = ArcStartAngle;
    fArcSpanAngle = ArcSpanAngle;
    fTabLength = TabLength;
    fArrowStyle = Style;
}

TMomentArrow::~TMomentArrow() {}

void TMomentArrow::paint(QPainter *Painter,
                         const QStyleOptionGraphicsItem *Option,
                         QWidget *Widget) {

    // Draws arc.
    QRectF boundary(fReferencePoint.x() - fArcRadius,
                    fReferencePoint.y() - fArcRadius, 2 * fArcRadius,
                    2 * fArcRadius);
    Painter->drawArc(boundary, fArcStartAngle, fArcSpanAngle);

    // Draws filled triangle.
    double angle = (fArcStartAngle + fArcSpanAngle) * (PI / 180) / 16;
    QPointF aux1(fReferencePoint);
    aux1.rx() += fArcRadius * cos(angle);
    aux1.ry() -= fArcRadius * sin(angle);

    QPointF aux2, aux3;
    angle = angle - PI / 2 - sin(PI / 5) * fTabLength / fArcRadius;
    aux2.rx() = aux1.x() + fTabLength * cos(angle + PI / 5);
    aux2.ry() = aux1.y() - fTabLength * sin(angle + PI / 5);
    aux3.rx() = aux1.x() + fTabLength * cos(angle - PI / 5);
    aux3.ry() = aux1.y() - fTabLength * sin(angle - PI / 5);

    QPolygonF triangle;
    triangle << aux1 << aux2 << aux3;
    Painter->drawPolygon(triangle);

    // Fills triangle.
    QPainterPath path;
    path.addPolygon(triangle);
    Painter->fillPath(path,
                      QBrush(QColor(Painter->pen().color()), Qt::SolidPattern));

    if (fArrowStyle == Reaction) {
        if (fTabLength > 0) {
            aux1.rx() = fReferencePoint.x() + (fArcRadius - fTabLength / 2);
            aux1.ry() = fReferencePoint.y();
            aux2.rx() = fReferencePoint.x() + (fArcRadius + fTabLength / 2);
            aux2.ry() = fReferencePoint.y();
        } else {
            aux1.rx() = fReferencePoint.x() - (fArcRadius - fTabLength / 2);
            aux1.ry() = fReferencePoint.y();
            aux2.rx() = fReferencePoint.x() - (fArcRadius + fTabLength / 2);
            aux2.ry() = fReferencePoint.y();
        }
        Painter->drawLine(aux1, aux2);
    }
}

QRectF TMomentArrow::boundingRect() const {

    QPointF aux1(fReferencePoint), aux2(fReferencePoint);
    if (fTabLength < 0) {
        aux1.rx() -= 1.3 * fArcRadius;
        aux1.ry() -= fArcRadius;
        aux2.rx() -= fArcRadius / 3;
        aux2.ry() += fArcRadius;
    } else {
        aux1.rx() += 1.3 * fArcRadius;
        aux1.ry() -= fArcRadius;
        aux2.rx() += fArcRadius / 3;
        aux2.ry() += fArcRadius;
    }
    QRectF boundary(aux1, aux2);
    return boundary.normalized();
}

//------------------------------------------------------------------------------
// TNodalLoadGraphics
//------------------------------------------------------------------------------
// TNodalLoadGraphics default constructor.
TNodalLoadGraphics::TNodalLoadGraphics(QPointF LoadPosition, LoadType Type,
                                       bool Orientation) {
    qreal axisLength = 40;
    qreal tabLength = 7;
    qreal arcRadius = 25;
    qreal arcStartAngle = -45 * 16;
    qreal arcSpanAngle = 90 * 16;

    fLoadType = Type;
    fLoadPosition = LoadPosition;

    if (Type == Fx) {
        qreal angle;
        if (Orientation == true) {
            angle = PI;
        } else {
            angle = 0;
        }

        fNodalLoad = new TLoadArrow(fLoadPosition, axisLength, tabLength, angle,
                                    TLoadArrow::ArrowStyle::Nodal);
    } else if (Type == Fy) {
        qreal angle;
        if (Orientation == true) {
            angle = -PI / 2;
        } else {
            angle = PI / 2;
        }

        fNodalLoad = new TLoadArrow(fLoadPosition, axisLength, tabLength, angle,
                                    TLoadArrow::ArrowStyle::Nodal);
    } else {
        if (Orientation == false) {
            tabLength = -1 * tabLength;
            arcStartAngle = 180 * 16 - arcStartAngle;
            arcSpanAngle = -1 * arcSpanAngle;
        }
        fNodalLoad = new TMomentArrow(fLoadPosition, arcRadius, arcStartAngle,
                                      arcSpanAngle, tabLength,
                                      TMomentArrow::ArrowStyle::Nodal);
    }

    setFlag(ItemIsMovable, true);
}

TNodalLoadGraphics::~TNodalLoadGraphics() { delete fNodalLoad; }

QRectF TNodalLoadGraphics::boundingRect() const {
    return fNodalLoad->boundingRect();
}

void TNodalLoadGraphics::paint(QPainter *Painter,
                               const QStyleOptionGraphicsItem *Option,
                               QWidget *Widget) {

    Painter->setPen(QPen(Qt::darkGreen, 1.2));
    fNodalLoad->paint(Painter, Option, Widget);
}

//------------------------------------------------------------------------------
// TReactionLoadGraphics
//------------------------------------------------------------------------------
TReactionLoadGraphics::TReactionLoadGraphics(QPointF LoadPosition,
                                             LoadType Type, bool Orientation) {
    qreal axisLength = 50;
    qreal tabLength = 10;
    qreal arcRadius = 30;
    qreal arcStartAngle = -45 * 16;
    qreal arcSpanAngle = 90 * 16;

    fLoadType = Type;
    fLoadPosition = LoadPosition;

    if (Type == Fx) {
        qreal angle;
        if (Orientation == true) {
            angle = PI;
        } else {
            angle = 0;
        }

        fNodalLoad = new TLoadArrow(fLoadPosition, axisLength, tabLength, angle,
                                    TLoadArrow::ArrowStyle::Reaction);
    } else if (Type == Fy) {
        qreal angle;
        if (Orientation == true) {
            angle = -PI / 2;
        } else {
            angle = PI / 2;
        }

        fNodalLoad = new TLoadArrow(fLoadPosition, axisLength, tabLength, angle,
                                    TLoadArrow::ArrowStyle::Reaction);
    } else {
        if (Orientation == false) {
            tabLength = -1 * tabLength;
            arcStartAngle = 180 * 16 - arcStartAngle;
            arcSpanAngle = -1 * arcSpanAngle;
        }
        fNodalLoad = new TMomentArrow(fLoadPosition, arcRadius, arcStartAngle,
                                      arcSpanAngle, tabLength,
                                      TMomentArrow::ArrowStyle::Reaction);
    }

    setFlag(ItemIsMovable, true);
}

TReactionLoadGraphics::~TReactionLoadGraphics() { delete fNodalLoad; }

QRectF TReactionLoadGraphics::boundingRect() const {
    return fNodalLoad->boundingRect();
}

void TReactionLoadGraphics::paint(QPainter *Painter,
                                  const QStyleOptionGraphicsItem *Option,
                                  QWidget *Widget) {
    Painter->setPen(Qt::darkRed);
    fNodalLoad->paint(Painter, Option, Widget);
}

// TDistributedLoadGraphics default constructor.
TDistributedLoadGraphics::TDistributedLoadGraphics(
    TDistributedLoad *DistributedLoad, QPointF StartPoint, QPointF EndPoint,
    qreal LoadScale) {

    qreal arrowTabLength = 10;
    qreal spacing = 30;

    fStartPoint = StartPoint;
    fEndPoint = EndPoint;
    fLoadPlane = DistributedLoad->getLoadPlane();

    // Gets the angle of the arrows.
    qreal angle;
    if (fLoadPlane == true) {
        angle = -PI / 2;
    } else {
        angle = QLineF(StartPoint, EndPoint).angle() * PI / 180 - PI / 2;
    }

    // Calculates the distance between arrows.
    qreal elementLength = QLineF(fStartPoint, fEndPoint).length();
    int numberOfArrows = elementLength / spacing;
    spacing = elementLength / numberOfArrows;

    qreal xStep = cos(QLineF(StartPoint, EndPoint).angle() * PI / 180) *
                  elementLength / numberOfArrows;
    qreal yStep = sin(QLineF(StartPoint, EndPoint).angle() * PI / 180) *
                  elementLength / numberOfArrows;

    // Calculates the length of the extreme arrows proportionally.
    qreal arrow0Length, arrow1Length;
    if (abs(DistributedLoad->getNode0Load()) >=
        abs(DistributedLoad->getNode1Load())) {
        arrow0Length = LoadScale * DistributedLoad->getNode0Load();
        arrow1Length = arrow0Length * DistributedLoad->getNode1Load() /
                       DistributedLoad->getNode0Load();
    } else {
        arrow1Length = LoadScale * DistributedLoad->getNode1Load();
        arrow0Length = arrow1Length * DistributedLoad->getNode0Load() /
                       DistributedLoad->getNode1Load();
    }

    // Stores the arrows.
    QPoint referencePoint;
    qreal arrowLength;

    for (int i = 0; i <= numberOfArrows; i++) {
        referencePoint.rx() = StartPoint.x() + i * xStep;
        referencePoint.ry() = StartPoint.y() - i * yStep;
        arrowLength =
            arrow0Length + (arrow1Length - arrow0Length) * i / numberOfArrows;

        fDistributedLoad.push_back(
            new TLoadArrow(referencePoint, arrowLength, arrowTabLength, angle,
                           TLoadArrow::ArrowStyle::Distributed));
    }
}

// TDistributedLoadGraphics destructor.
TDistributedLoadGraphics::~TDistributedLoadGraphics() = default;

// Bounding rectangle function.
QRectF TDistributedLoadGraphics::boundingRect() const {
    QPolygonF polygon;

    polygon << fDistributedLoad[0]->boundingRect().topLeft();
    polygon << fDistributedLoad[0]->boundingRect().bottomRight();
    polygon << fDistributedLoad[fDistributedLoad.size() - 1]
                   ->boundingRect()
                   .topLeft();
    polygon << fDistributedLoad[fDistributedLoad.size() - 1]
                   ->boundingRect()
                   .bottomRight();

    qreal extraBorder = 5.;
    return polygon.boundingRect().normalized().adjusted(
        -extraBorder, -extraBorder, extraBorder, extraBorder);
}

// TDistributedLoadGraphics painter function.
void TDistributedLoadGraphics::paint(QPainter *Painter,
                                     const QStyleOptionGraphicsItem *Option,
                                     QWidget *Widget) {

    // Changes pen color.
    QBrush supportsBrush(Qt::darkBlue, Qt::SolidPattern);
    Painter->setPen(QPen(supportsBrush, 1.2));

    for (auto & i : fDistributedLoad) {
        i->paint(Painter, Option, Widget);
    }

    qreal x0, y0, x1, y1;
    qreal angle;

    if (fLoadPlane == true) {
        angle = -PI / 2;
    } else {
        angle = QLineF(fStartPoint, fEndPoint).angle() * PI / 180 - PI / 2;
    }

    x0 = fStartPoint.x() + fDistributedLoad[0]->getArrowLength() * cos(angle);
    y0 = fStartPoint.y() - fDistributedLoad[0]->getArrowLength() * sin(angle);
    x1 = fEndPoint.x() +
         fDistributedLoad[fDistributedLoad.size() - 1]->getArrowLength() *
             cos(angle);
    y1 = fEndPoint.y() -
         fDistributedLoad[fDistributedLoad.size() - 1]->getArrowLength() *
             sin(angle);

    Painter->drawLine(x0, y0, x1, y1);
}

//------------------------------------------------------------------------------
// TDiagramLoadGraphics
//------------------------------------------------------------------------------
TDiagramLoadGraphics::TDiagramLoadGraphics(QPointF StartPoint, QPointF EndPoint,
                                           const double *Coefficients, qreal UScale) {

    fStartPoint = StartPoint;
    fEndPoint = EndPoint;
    fCoefficients[0] = Coefficients[0];
    fCoefficients[1] = Coefficients[1];
    fCoefficients[2] = Coefficients[2];
    fCoefficients[3] = Coefficients[3];
    fUScale = UScale;

    fCurve = QPainterPath(QPointF(0.0, 0.0));

    QLineF elementAxis(fStartPoint, fEndPoint);
    qreal step = elementAxis.length() / 1000;

    for (qreal u = 0; u <= elementAxis.length(); u = u + step) {
        fCurve.lineTo(u, -(this->eval(u * UScale)));
    }
    fCurve.lineTo(elementAxis.length(), 0);
}

TDiagramLoadGraphics::~TDiagramLoadGraphics() = default;

QRectF TDiagramLoadGraphics::boundingRect() const {
    QPolygonF polygon;

    polygon << fCurve.boundingRect().topLeft();
    polygon << fCurve.boundingRect().topRight();
    polygon << fCurve.boundingRect().bottomRight();
    polygon << fCurve.boundingRect().bottomLeft();

    QTransform transform;
    transform.translate(fStartPoint.x(), fStartPoint.y());
    transform.rotate(-QLineF(fStartPoint, fEndPoint).angle());

    polygon = transform.mapRect(polygon.boundingRect());
    qreal extraBorder = 5.0;
    return polygon.boundingRect().normalized().adjusted(
        -extraBorder, -extraBorder, extraBorder, extraBorder);
}

void TDiagramLoadGraphics::paint(QPainter *Painter,
                                 const QStyleOptionGraphicsItem *Option,
                                 QWidget *Widget) {
    Painter->save();
    Painter->translate(fStartPoint);
    Painter->rotate(-QLineF(fStartPoint, fEndPoint).angle());

    Painter->setPen(Qt::blue);
    Painter->drawPath(fCurve);

    Painter->restore();
}

qreal TDiagramLoadGraphics::eval(qreal u) {
    double a = fCoefficients[0];
    double b = fCoefficients[1];
    double c = fCoefficients[2];
    double d = fCoefficients[3];

    return qreal(a * u * u * u + b * u * u + c * u + d);
}