/* This file was created by Gustavo BATISTELA.
It contains the definitions of functions of the TStructureGraphics class. */

#include "TStructureGraphics.h"

// TStructureGraphics default constructor.
TStructureGraphics::TStructureGraphics(TStructure *Structure) {

    fHingeRadius = 5;
    fImageWidth = 400;
    fImageHeight = 300;
    fStructure = Structure;
    this->convertNodes();
}

// Copy constructor.
TStructureGraphics::TStructureGraphics(const TStructureGraphics & Other) {
    fStructure = Other.fStructure;
    fNodeCoordinates = Other.fNodeCoordinates;
    fImageWidth = Other.fImageWidth;
    fImageHeight = Other.fImageHeight;
    fHingeRadius = Other.fHingeRadius;
}

// TStructureGraphics destructor.
TStructureGraphics::~TStructureGraphics() {
}

// Bounding rectangle function.
QRectF TStructureGraphics::boundingRect() const {
    double xmin, xmax, ymin, ymax;
    for (int i = 0; i < fNodeCoordinates->Rows(); i++) {
        if (i == 0) {
            // First assignment of xmin and xmax.
            xmin = fNodeCoordinates->GetVal(i, 0);
            xmax = fNodeCoordinates->GetVal(i, 0);

            // First assignment of ymin and ymax.
            ymin = fNodeCoordinates->GetVal(i, 1);
            ymax = fNodeCoordinates->GetVal(i, 1);
        }
        else {
            if (fNodeCoordinates->GetVal(i, 0) < xmin) { xmin = fNodeCoordinates->GetVal(i, 0); }
            if (fNodeCoordinates->GetVal(i, 0) > xmax) { xmax = fNodeCoordinates->GetVal(i, 0); }
            if (fNodeCoordinates->GetVal(i, 1) < ymin) { ymin = fNodeCoordinates->GetVal(i, 1); }
            if (fNodeCoordinates->GetVal(i, 1) > ymax) { ymax = fNodeCoordinates->GetVal(i, 1); }
        }
    }

    xmin -= fHingeRadius;
    xmax += fHingeRadius;
    ymin -= fHingeRadius;
    ymax += fHingeRadius;

    qreal extra = 5.0;
    return QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax)).normalized().adjusted(-extra, -extra, extra, extra);
}

// Painter function.
void TStructureGraphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // Changes pen color.
    QBrush structureBrush(Qt::black, Qt::SolidPattern);
    painter->setPen(QPen(structureBrush, 2));

    // Draws the structure elements and checks if all elements are hinged at a node.
    std::vector<TNode> nodes(fStructure->getNodes());
    TPZFMatrix<int> hingeAux(nodes.size(), 1, -1);

    std::vector<TElement> elements = fStructure->getElements();
    for (int i = 0; i < (int)elements.size(); i++) {
        int node0ID = elements[i].getNode0ID();
        int node1ID = elements[i].getNode1ID();

        double x0 = fNodeCoordinates->Get(node0ID, 0);
        double y0 = fNodeCoordinates->Get(node0ID, 1);
        double x1 = fNodeCoordinates->Get(node1ID, 0);
        double y1 = fNodeCoordinates->Get(node1ID, 1);
        painter->drawLine(x0, y0, x1, y1);

        if (elements[i].getHinge0() == true) {
            if (hingeAux(node0ID, 0) == -1) {
                hingeAux(node0ID, 0) = 1;
            }
            else if (hingeAux(node0ID, 0) == 1) {
                hingeAux(node0ID, 0) = 0;
            }
        }
        if (elements[i].getHinge1() == true) {
            if (hingeAux(node1ID, 0) == -1) {
                hingeAux(node1ID, 0) = 1;
            }
            else if (hingeAux(node1ID, 0) == 1) {
                hingeAux(node1ID, 0) = 0;
            }
        }
    }

    // Draws hinges at nodes.
    painter->setBrush(Qt::white);
    QPoint center;
    for (int i = 0; i < (int)fNodeCoordinates->Rows(); i++) {
        if (hingeAux(i, 0) == 0) {
            center.rx() = fNodeCoordinates->Get(i, 0);
            center.ry() = fNodeCoordinates->Get(i, 1);
            painter->drawEllipse(center, fHingeRadius, fHingeRadius);
        }
    }

    // Draws hinges at elements end.
    for (int i = 0; i < (int)elements.size(); i++) {
        int x0 = fNodeCoordinates->Get(elements[i].getNode0ID(), 0);
        int y0 = fNodeCoordinates->Get(elements[i].getNode0ID(), 1);
        int x1 = fNodeCoordinates->Get(elements[i].getNode1ID(), 0);
        int y1 = fNodeCoordinates->Get(elements[i].getNode1ID(), 1);

        // Draws hinge at node 0 if applicable.
        if (elements[i].getHinge0() == true && hingeAux(elements[i].getNode0ID(), 0) == 1) {
            center.rx() = x0 + (fHingeRadius + 2)*elements[i].getCos();
            center.ry() = y0 - (fHingeRadius + 2)*elements[i].getSin();
            painter->drawEllipse(center, fHingeRadius, fHingeRadius);
        }
        // Draws hinge at node 1 if applicable.
        if (elements[i].getHinge1() == true && hingeAux(elements[i].getNode1ID(), 0) == 1) {
            center.rx() = x1 - (fHingeRadius + 2)*elements[i].getCos();
            center.ry() = y1 + (fHingeRadius + 2)*elements[i].getSin();
            painter->drawEllipse(center, fHingeRadius, fHingeRadius);
        }
    }
}

// Calculates the node coordinates on the image object and stores in fNodeCoordinates member object.
void TStructureGraphics::convertNodes() {

    // Assesses the lowest and highest values of x and y coordinates.
    double xmin, xmax, ymin, ymax;
    std::vector<TNode> nodes = fStructure->getNodes();
    for (int i = 0; i < (int)nodes.size(); i++) {
        if (i == 0) {
            // First assignment of xmin and xmax.
            xmin = nodes[i].getX();
            xmax = nodes[i].getX();

            // First assignment of ymin and ymax.
            ymin = nodes[i].getY();
            ymax = nodes[i].getY();
        }
        else {
            if (nodes[i].getX() < xmin) { xmin = nodes[i].getX(); }
            if (nodes[i].getX() > xmax) { xmax = nodes[i].getX(); }
            if (nodes[i].getY() < ymin) { ymin = nodes[i].getY(); }
            if (nodes[i].getY() > ymax) { ymax = nodes[i].getY(); }
        }
    }

    // Distinction between the total and usable areas of the image.
    double structureWidth = xmax - xmin;
    double structureHeight = ymax - ymin;
    double imageRatio = fImageWidth / fImageHeight;

    // Computation of the scale factor and the new coordinate system.
    double scaleFactor;
    if (structureWidth / structureHeight >= imageRatio) {
        scaleFactor = fImageWidth / structureWidth;
    }
    else {
        scaleFactor = fImageHeight / structureHeight;
    }

    // Converts the points and stores into the TPZFMatrix object.
    TPZFMatrix<double> nodeCoordinates(fStructure->getNodes().size(), 2, 0);

    for (int i = 0; i < (int)nodes.size(); i++) {
        nodeCoordinates(i, 0) = scaleFactor*nodes[i].getX();
        nodeCoordinates(i, 1) = -1 * scaleFactor*nodes[i].getY() + fImageHeight;
    }

    // Assigns the coordinate matrix to class member fNodeCoordinates.
    fNodeCoordinates = new TPZFMatrix<double>(nodeCoordinates);
}

// Assignment operator.
TStructureGraphics & TStructureGraphics::operator=(const TStructureGraphics & Other) {
    if (this != &Other) {
        fStructure = Other.fStructure;
        fNodeCoordinates = Other.fNodeCoordinates;
        fImageWidth = Other.fImageWidth;
        fImageHeight = Other.fImageHeight;
        fHingeRadius = Other.fHingeRadius;
    }
    return *this;
}

// getStructure - returns a pointer to the TStructure object.
TStructure * TStructureGraphics::getStructure() const {
    return fStructure;
}

// getNodeCoordinates - returns fNodeCoordinates member.
TPZFMatrix<double>* TStructureGraphics::getNodeCoordinates() {
    return fNodeCoordinates;
}