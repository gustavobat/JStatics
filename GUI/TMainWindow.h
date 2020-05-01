#ifndef TMAINWINDOW_H
#define TMAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

#include <fstream>
#include "pzfmatrix.h"
#include "JSONIntegration.h"

#include "TStructureGraphics.h"
#include "TSupportGraphics.h"
#include "TLoadGraphics.h"

class QAction;
class QMenu;

namespace Ui {
class TMainWindow;
}

class TMainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit TMainWindow(QWidget *parent = nullptr);
    ~TMainWindow() final;
    void loadFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void showJSONEditor();
    void aboutJStatics();
    void aboutLabMeC();

    void documentWasModified();
    void setupMatrix();
    void updateLoadScale();

    void on_appliedLoadsButton_clicked();
    void on_axialDiagramButton_clicked();
    void on_shearDiagramButton_clicked();
    void on_bendingMomentDiagramButton_clicked();
    void on_displacementButton_clicked();
    void on_saveChangesButton_clicked();

signals:
    void updateNewAction(bool);

private:
    Ui::TMainWindow *fUi;
    QString fCurFile;

    TStructure *fStructure{};
    std::vector<TNodalLoad> *fNodalLoads{};
    std::vector<TDistributedLoad> *fDistributedLoads{};
    std::vector<TElementEndMoment> *fEndMoments{};
    std::vector<TPZFMatrix<double>> * fInternalLoads{};
    TPZFMatrix<double> *fDisplacementVector{};

    TStructureGraphics *fStructureGraphics{};
    double fDefaultDrawingSize;
    double fLoadDrawingScales[5]{};
    double fSavedDrawingSizes[5]{};
    QGraphicsScene *fAppliedLoads;
    QGraphicsScene *fAxialForceDiagram;
    QGraphicsScene *fShearForceDiagram;
    QGraphicsScene *fBendingMomentDiagram;
    QGraphicsScene *fDisplacement;

    void createActions();
    void createStatusBar();

    bool maybeSave();
    bool saveFile(const QString& fileName);
    void setCurrentFile(const QString& fileName);
    QString strippedName(const QString& fullFileName);

    void drawReactions(QGraphicsScene *Scene);
    void initializeLoadScales();
    void populateAppliedLoadsScene(double drawingSize);
    void populateAxialForceDiagramScene(double drawingSize);
    void populateShearForceDiagramScene(double drawingSize);
    void populateBendingMomentDiagramScene(double drawingSize);
    void populateDisplacement(double drawingSize);
};

#endif // TMAINWINDOW_H
