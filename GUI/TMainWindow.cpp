#include "TMainWindow.h"
#include "ui_TMainWindow.h"
#include <QtWidgets>

TMainWindow::TMainWindow(QWidget* parent)
	: QMainWindow(parent), fUi(new Ui::TMainWindow) {
	fUi->setupUi(this);
	fUi->graphicsView->setRenderHint(QPainter::Antialiasing);

	createActions();
	createStatusBar();

	setCurrentFile(QString());

	fAppliedLoads = new QGraphicsScene();
	fAxialForceDiagram = new QGraphicsScene();
	fShearForceDiagram = new QGraphicsScene();
	fBendingMomentDiagram = new QGraphicsScene();
	fDisplacement = new QGraphicsScene();

	connect(fUi->jsonTextEdit->document(), &QTextDocument::contentsChanged, this,
		&TMainWindow::documentWasModified);
	connect(fUi->scaleSlider, SIGNAL(valueChanged(int)), this,
		SLOT(updateLoadScale()));
	connect(fUi->zoomSlider, SIGNAL(valueChanged(int)), this,
		SLOT(setupMatrix()));

	setUnifiedTitleAndToolBarOnMac(true);
}

TMainWindow::~TMainWindow() {
	delete fUi;
	delete fStructure, fNodalLoads, fDistributedLoads, fEndMoments;
	delete fInternalLoads, fDisplacementVector;
	delete fStructureGraphics;
}

void TMainWindow::closeEvent(QCloseEvent* event) {
	if (maybeSave()) {
		event->accept();
	}
	else {
		event->ignore();
	}
}

void TMainWindow::newFile() {
	if (maybeSave()) {
		fUi->jsonTextEdit->clear();
		setCurrentFile(QString());
	}
}

void TMainWindow::open() {
	if (maybeSave()) {
		QString fileName = QFileDialog::getOpenFileName(
			this, tr("Open JSON File"), "",
			tr("JSON File (*.json);;Text File (*.txt);;All Files (*)"));
		if (!fileName.isEmpty()) loadFile(fileName);
	}
}

bool TMainWindow::save() {
	if (fCurFile.isEmpty()) {
		return saveAs();
	}
	else {
		return saveFile(fCurFile);
	}
}

bool TMainWindow::saveAs() {
	QFileDialog dialog(this, tr("Save file"), "",
		tr("JSON File (*.json);;Text File (*.txt)"));
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() != QDialog::Accepted) {
		return false;
	}
	return saveFile(dialog.selectedFiles().first());
}

void TMainWindow::showJSONEditor() { fUi->jsonEditorWidget->show(); }

void TMainWindow::aboutJStatics() {
	QMessageBox::about(
		this, tr("About JStatics"),
		tr("JStatics is an open-source application that allows users to "
			"calculate 2D frame structures"
			" using a JSON file input.\n"
			"For further information please visit: "
			"https://github.com/gustavobat/JStatics"));
}

void TMainWindow::aboutLabMeC() {
	QMessageBox::about(
		this, tr("About LabMeC"),
		tr("LabMeC is a laboratory belonging to the Department of Structures of "
			"the Faculty of "
			"Civil Engineering, Architecture and Urbanism (FEC) of Unicamp and "
			"aims to support "
			"research in the field of Computational Mechanics, developing "
			"computer programs to "
			"automate the process of solving engineering problems.\n"
			"For further information please visit: www.labmec.org.br"));
}

void TMainWindow::documentWasModified() {
	setWindowModified(fUi->jsonTextEdit->document()->isModified());
}

void TMainWindow::setupMatrix() {
	qreal scale = qPow(qreal(2), (fUi->zoomSlider->value()) / qreal(50));

	QMatrix matrix;
	matrix.scale(scale, scale);

	fUi->graphicsView->setMatrix(matrix);
}

void TMainWindow::updateLoadScale() {
	qreal scale = 0.01 * qPow(qreal(2.0), (fUi->scaleSlider->value() / qreal(5)));

	if (fUi->appliedLoadsButton->isChecked()) {
		populateAppliedLoadsScene(scale);
	}
	if (fUi->axialDiagramButton->isChecked()) {
		populateAxialForceDiagramScene(scale);
	}
	if (fUi->shearDiagramButton->isChecked()) {
		populateShearForceDiagramScene(scale);
	}
	if (fUi->bendingMomentDiagramButton->isChecked()) {
		populateBendingMomentDiagramScene(scale);
	}
	if (fUi->displacementButton->isChecked()) {
		populateDisplacement(scale);
	}
}

void TMainWindow::createActions() {
	// "File" menu and toolbar setup.
	QMenu* fileMenu = menuBar()->addMenu(tr("File"));
	QToolBar* fileToolBar = addToolBar(tr("File"));

	// Adds "New" action.
	QAction* NewAct = new QAction(tr("New"), this);
	NewAct->setShortcuts(QKeySequence::New);
	NewAct->setStatusTip(tr("Create a new file"));
	connect(NewAct, &QAction::triggered, this, &TMainWindow::newFile);
	connect(this, &TMainWindow::updateNewAction, NewAct, &QAction::setEnabled);
	fileMenu->addAction(NewAct);
	fileToolBar->addAction(NewAct);

	// Adds "Open..." action.
	QAction* openAct = new QAction(tr("Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, &QAction::triggered, this, &TMainWindow::open);
	fileMenu->addAction(openAct);
	fileToolBar->addAction(openAct);

	// Adds "Save" action.
	QAction* saveAct = new QAction(tr("Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, &QAction::triggered, this, &TMainWindow::save);
	fileMenu->addAction(saveAct);
	fileToolBar->addAction(saveAct);

	// Adds "Save As..." action.
	QAction* saveAsAct =
		fileMenu->addAction(tr("Save As..."), this, &TMainWindow::saveAs);
	saveAsAct->setShortcuts(QKeySequence::SaveAs);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));

	// Adds separator between "Save As..." and "Exit" actions.
	fileMenu->addSeparator();

	// Adds "Exit" action.
	QAction* exitAct = fileMenu->addAction(tr("Exit"), this, &QWidget::close);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Exit the application"));

	// "Edit" menu and toolbar setup.
	QMenu* editMenu = menuBar()->addMenu(tr("Edit"));
	QToolBar* editToolBar = addToolBar(tr("Edit"));

	// Adds "Edit JSON" action.
	QAction* editJSONAct = new QAction(tr("Edit JSON"), this);
	editJSONAct->setStatusTip("Open the current file in a text editor.");
	connect(editJSONAct, &QAction::triggered, this, &TMainWindow::showJSONEditor);
	editMenu->addAction(editJSONAct);
	editToolBar->addAction(editJSONAct);

	// Adds "Cut" action.
	QAction* cutAct = new QAction(tr("Cut"), this);
	cutAct->setShortcuts(QKeySequence::Cut);
	cutAct->setStatusTip(
		tr("Cut the current selection's contents to the clipboard"));
	connect(cutAct, &QAction::triggered, fUi->jsonTextEdit, &QPlainTextEdit::cut);
	editMenu->addAction(cutAct);
	editToolBar->addAction(cutAct);

	// Adds "Copy" action.
	QAction* copyAct = new QAction(tr("Copy"), this);
	copyAct->setShortcuts(QKeySequence::Copy);
	copyAct->setStatusTip(
		tr("Copy the current selection's contents to the clipboard"));
	connect(copyAct, &QAction::triggered, fUi->jsonTextEdit,
		&QPlainTextEdit::copy);
	editMenu->addAction(copyAct);
	editToolBar->addAction(copyAct);

	// Adds "Paste" action.
	QAction* pasteAct = new QAction(tr("Paste"), this);
	pasteAct->setShortcuts(QKeySequence::Paste);
	pasteAct->setStatusTip(
		tr("Paste the clipboard's contents into the current selection"));
	connect(pasteAct, &QAction::triggered, fUi->jsonTextEdit,
		&QPlainTextEdit::paste);
	editMenu->addAction(pasteAct);
	editToolBar->addAction(pasteAct);

	// "Help" menu setup.
	QMenu* helpMenu = menuBar()->addMenu(tr("Help"));

	// Adds "About JStatics" action.
	QAction* aboutJStaticsAct = helpMenu->addAction(tr("About JStatics"), this,
		&TMainWindow::aboutJStatics);
	aboutJStaticsAct->setStatusTip(tr("Show help about JStatics"));

	// Adds "About LabMeC" action.
	QAction* aboutLabMeCAct =
		helpMenu->addAction(tr("About LabMeC"), this, &TMainWindow::aboutLabMeC);
	aboutLabMeCAct->setStatusTip(tr("Show help about LabMeC"));

#ifndef QT_NO_CLIPBOARD
	cutAct->setEnabled(false);
	copyAct->setEnabled(false);
	connect(fUi->jsonTextEdit, &QPlainTextEdit::copyAvailable, cutAct,
		&QAction::setEnabled);
	connect(fUi->jsonTextEdit, &QPlainTextEdit::copyAvailable, copyAct,
		&QAction::setEnabled);
#endif  // !QT_NO_CLIPBOARD
}

void TMainWindow::createStatusBar() { statusBar()->showMessage(tr("Ready")); }

bool TMainWindow::maybeSave() {
	if (!fUi->jsonTextEdit->document()->isModified()) return true;
	const QMessageBox::StandardButton ret = QMessageBox::warning(
		this, tr("Application"), tr("The document has been modified.\n"
			"Do you want to save your changes?"),
		QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

	switch (ret) {
	case QMessageBox::Save:
		return save();
	case QMessageBox::Cancel:
		return false;
	default:
		break;
	}
	return true;
}

void TMainWindow::loadFile(const QString& fileName) {
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(
			this, tr("Application"),
			tr("Cannot read file %1:\n%2.")
			.arg(QDir::toNativeSeparators(fileName), file.errorString()));
		return;
	}

	QTextStream in(&file);
	fUi->jsonTextEdit->setPlainText(in.readAll());

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File loaded"), 2000);
}

bool TMainWindow::saveFile(const QString& fileName) {
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(
			this, tr("Application"),
			tr("Cannot write file %1:\n%2.")
			.arg(QDir::toNativeSeparators(fileName), file.errorString()));
		return false;
	}

	std::ofstream out;
	out.open(fileName.toStdString().c_str());
	out << fUi->jsonTextEdit->toPlainText().toStdString().c_str();
	out.close();

	statusBar()->showMessage(tr("File saved"), 2000);
	loadFile(fileName);
	return true;
}

void TMainWindow::setCurrentFile(const QString& fileName) {
	fCurFile = fileName;
	fUi->jsonTextEdit->document()->setModified(false);
	setWindowModified(false);

	QString shownName = fCurFile;

	if (fCurFile.isEmpty()) {
		fUi->selectViewBox->setDisabled(true);
		fUi->viewPropertiesBox->setDisabled(true);
		shownName = "untitled.json";
		emit updateNewAction(false);
	}
	else {
		// Reads input JSON file and converts into a TStructure object
		// and vectors of each type of load.
		std::ifstream input(fCurFile.toStdString().c_str());
		nlohmann::json J;
		input >> J;

		fStructure = new TStructure();
		std::vector<TNodalLoad> nodalLoads;
		std::vector<TDistributedLoad> distrLoads;
		std::vector<TElementEndMoment> endMoments;
	//	std::vector<TSupportDisplacement> supportDisplacaments;
		importData(J, fStructure, nodalLoads, distrLoads, endMoments);

		// Solves structure.
		std::vector<TPZFMatrix<double>> internalLoads(0, 0);
		fStructure->solve(nodalLoads, distrLoads, endMoments, /*supportDisplacaments,*/
			internalLoads);

		// Stores structure related members.
		fNodalLoads = new std::vector<TNodalLoad>(nodalLoads);
		fDistributedLoads = new std::vector<TDistributedLoad>(distrLoads);
		fEndMoments = new std::vector<TElementEndMoment>(endMoments);
		fInternalLoads = new std::vector<TPZFMatrix<double>>(internalLoads);
		fDisplacementVector = new TPZFMatrix<double>(fStructure->getD());
		fStructureGraphics = new TStructureGraphics(fStructure);

		fUi->selectViewBox->setEnabled(true);
		fUi->viewPropertiesBox->setEnabled(true);

		populateAppliedLoadsScene(10);
		populateAxialForceDiagramScene(.01);
		populateShearForceDiagramScene(.01);
		populateBendingMomentDiagramScene(.01);
		populateDisplacement(10);

		emit updateNewAction(true);
	}

	setWindowTitle("JStatics - " + strippedName(shownName) + "[*]");
}

QString TMainWindow::strippedName(const QString& fullFileName) {
	return QFileInfo(fullFileName).fileName();
}

void TMainWindow::on_appliedLoadsButton_clicked() {
	fUi->graphicsView->setScene(fAppliedLoads);
}

void TMainWindow::on_axialDiagramButton_clicked() {
	fUi->graphicsView->setScene(fAxialForceDiagram);
}

void TMainWindow::on_shearDiagramButton_clicked() {
	fUi->graphicsView->setScene(fShearForceDiagram);
}

void TMainWindow::on_bendingMomentDiagramButton_clicked() {
	fUi->graphicsView->setScene(fBendingMomentDiagram);
}

void TMainWindow::on_displacementButton_clicked() {
	fUi->graphicsView->setScene(fDisplacement);
}

void TMainWindow::on_saveChangesButton_clicked() { save(); }

void TMainWindow::drawReactions(QGraphicsScene* Scene) {
	// Draws reactions.
	TPZFMatrix<double> reactions = fStructure->getQU();

	std::vector<TSupport> supports = fStructure->getSupports();
	int CDOF = 0;
	// Counts the number of constrained DOF.
	for (int i = 0; i < (int)supports.size(); i++) {
		int nodeID = supports[i].getNodeID();
		qreal x = fStructureGraphics->getNodeCoordinates()->GetVal(nodeID, 0);
		qreal y = fStructureGraphics->getNodeCoordinates()->GetVal(nodeID, 1);
		QPointF loadPos(x, y);

		QString labelMessage;
		if (supports[i].getFx() == true) {
			if (abs(reactions[CDOF]) > 0.00001) {
				QPointF labelOffset(-40, 0);

				bool orientation = true;
				if (reactions[CDOF] < 0) {
					orientation = false;
					labelOffset = -1 * labelOffset;
				}

				labelMessage = QString::number(abs(reactions[CDOF]), 'g', 3) + " kN";
				Scene->addItem(new TReactionLoadGraphics(
					loadPos, TReactionLoadGraphics::LoadType::Fx, orientation));
				Scene->addItem(new TLabel(labelMessage, (loadPos + labelOffset), 8,
					QPen(Qt::darkRed)));
			}
			CDOF++;
		}

		if (supports[i].getFy() == true) {
			if (abs(reactions[CDOF]) > 0.00001) {
				QPointF labelOffset(0, 40);

				bool orientation = true;
				if (reactions[CDOF] < 0) {
					orientation = false;
					labelOffset = -1 * labelOffset;
				}

				labelMessage = QString::number(abs(reactions[CDOF]), 'g', 3) + " kN";
				Scene->addItem(new TReactionLoadGraphics(
					loadPos, TReactionLoadGraphics::LoadType::Fy, orientation));
				Scene->addItem(new TLabel(labelMessage, (loadPos + labelOffset), 8,
					QPen(Qt::darkRed)));
			}
			CDOF++;
		}

		if (supports[i].getM() == true) {
			if (abs(reactions[CDOF]) > 0.00001) {
				QPointF labelOffset(40, -20);

				bool orientation = true;
				if (reactions[CDOF] < 0) {
					orientation = false;
					labelOffset = labelOffset - QPointF(100, 0);
				}

				labelMessage = QString::number(abs(reactions[CDOF]), 'g', 3) + " kNm";

				Scene->addItem(new TReactionLoadGraphics(
					loadPos, TReactionLoadGraphics::LoadType::M, orientation));
				Scene->addItem(new TLabel(labelMessage, (loadPos + labelOffset), 8,
					QPen(Qt::darkRed)));
			}
			CDOF++;
		}
	}
}

void TMainWindow::populateAppliedLoadsScene(qreal LoadScale) {
	fAppliedLoads->clear();

	fAppliedLoads->addItem(new TStructureGraphics(*fStructureGraphics));
	fAppliedLoads->addItem(new TSupportGraphics(fStructureGraphics));

	// Draws distributed loads.
	for (int i = 0; i < (int)fDistributedLoads->size(); i++) {
		int elementID = fDistributedLoads->at(i).getElementID();
		int node0ID = fStructure->getElement(elementID).getNode0ID();
		int node1ID = fStructure->getElement(elementID).getNode1ID();

		qreal x0 = fStructureGraphics->getNodeCoordinates()->GetVal(node0ID, 0);
		qreal y0 = fStructureGraphics->getNodeCoordinates()->GetVal(node0ID, 1);
		qreal x1 = fStructureGraphics->getNodeCoordinates()->GetVal(node1ID, 0);
		qreal y1 = fStructureGraphics->getNodeCoordinates()->GetVal(node1ID, 1);
		QPointF startPoint(x0, y0);
		QPointF endPoint(x1, y1);

		QString labelMessage;
		qreal arrow0Length, arrow1Length;
		if (abs(fDistributedLoads->at(i).getNode0Load()) >=
			abs(fDistributedLoads->at(i).getNode1Load())) {
			arrow0Length = LoadScale * fDistributedLoads->at(i).getNode0Load();
			arrow1Length = arrow0Length * fDistributedLoads->at(i).getNode1Load() /
				fDistributedLoads->at(i).getNode0Load();
		}
		else {
			arrow1Length = LoadScale * fDistributedLoads->at(i).getNode1Load();
			arrow0Length = arrow1Length * fDistributedLoads->at(i).getNode0Load() /
				fDistributedLoads->at(i).getNode1Load();
		}

		if (fDistributedLoads->at(i).getNode0Load() !=
			fDistributedLoads->at(i).getNode1Load()) {
			if (abs(fDistributedLoads->at(i).getNode0Load()) > 0.00001) {
				QPointF labelPos(startPoint);
				qreal angle;
				if (fDistributedLoads->at(i).getLoadPlane() == true) {
					angle = -PI / 2;
				}
				else {
					angle = QLineF(startPoint, endPoint).angle() * PI / 180 - PI / 2;
				}

				labelPos.rx() += (arrow0Length * 1.1) * cos(angle);
				labelPos.ry() -= (arrow0Length * 1.1) * sin(angle);

				labelMessage =
					QString::number(abs(fDistributedLoads->at(i).getNode0Load()), 'g',
						3) +
					" kN/m";
				fAppliedLoads->addItem(
					new TLabel(labelMessage, labelPos, 10, QPen(Qt::darkBlue)));
			}

			if (abs(fDistributedLoads->at(i).getNode1Load()) > 0.00001) {
				QPointF labelPos(endPoint);
				qreal angle;
				if (fDistributedLoads->at(i).getLoadPlane() == true) {
					angle = -PI / 2;
				}
				else {
					angle = QLineF(startPoint, endPoint).angle() * PI / 180 - PI / 2;
				}

				labelPos.rx() += (arrow1Length * 1.1) * cos(angle);
				labelPos.ry() -= (arrow1Length * 1.1) * sin(angle);

				labelMessage =
					QString::number(abs(fDistributedLoads->at(i).getNode1Load()), 'g',
						3) +
					" kN/m";
				fAppliedLoads->addItem(
					new TLabel(labelMessage, labelPos, 10, QPen(Qt::darkBlue)));
			}
		}
		else {
			QPointF labelPos = 0.5 * (startPoint + endPoint);
			qreal angle;
			if (fDistributedLoads->at(i).getLoadPlane() == true) {
				angle = -PI / 2;
			}
			else {
				angle = QLineF(startPoint, endPoint).angle() * PI / 180 - PI / 2;
			}

			labelPos.rx() += (arrow0Length * 1.1) * cos(angle);
			labelPos.ry() -= (arrow0Length * 1.1) * sin(angle);

			labelMessage = QString::number(
				abs(fDistributedLoads->at(i).getNode0Load()), 'g', 3) +
				" kN/m";
			fAppliedLoads->addItem(
				new TLabel(labelMessage, labelPos, 10, QPen(Qt::darkBlue)));
		}

		fAppliedLoads->addItem(new TDistributedLoadGraphics(
			&(fDistributedLoads->at(i)), startPoint, endPoint, LoadScale));
	}

	// Draws nodal loads.
	for (int i = 0; i < (int)fNodalLoads->size(); i++) {
		int nodeID = fNodalLoads->at(i).getNodeID();
		qreal x = fStructureGraphics->getNodeCoordinates()->GetVal(nodeID, 0);
		qreal y = fStructureGraphics->getNodeCoordinates()->GetVal(nodeID, 1);
		QPointF loadPos(x, y);

		QString labelMessage;
		if (fNodalLoads->at(i).getFx() != 0) {
			QPointF labelOffset(-40, 0);

			bool orientation = true;
			if (fNodalLoads->at(i).getFx() < 0) {
				orientation = false;
				labelOffset = -1 * labelOffset;
			}

			labelMessage =
				QString::number(abs(fNodalLoads->at(i).getFx()), 'g', 3) + " kN";
			fAppliedLoads->addItem(new TNodalLoadGraphics(
				loadPos, TNodalLoadGraphics::LoadType::Fx, orientation));
			fAppliedLoads->addItem(new TLabel(labelMessage, (loadPos + labelOffset),
				8, QPen(Qt::darkGreen)));
		}

		if (fNodalLoads->at(i).getFy() != 0) {
			QPointF labelOffset(0, 40);

			bool orientation = true;
			if (fNodalLoads->at(i).getFy() < 0) {
				orientation = false;
				labelOffset = -1 * labelOffset;
			}

			labelMessage =
				QString::number(abs(fNodalLoads->at(i).getFy()), 'g', 3) + " kN";
			fAppliedLoads->addItem(new TNodalLoadGraphics(
				loadPos, TNodalLoadGraphics::LoadType::Fy, orientation));
			fAppliedLoads->addItem(new TLabel(labelMessage, (loadPos + labelOffset),
				8, QPen(Qt::darkGreen)));
		}

		if (fNodalLoads->at(i).getM() != 0) {
			QPointF labelOffset(40, 0);

			bool orientation = true;
			if (fNodalLoads->at(i).getM() < 0) {
				orientation = false;
				labelOffset = labelOffset - QPointF(100, 0);
			}

			labelMessage =
				QString::number(abs(fNodalLoads->at(i).getM()), 'g', 3) + " kNm";
			fAppliedLoads->addItem(new TNodalLoadGraphics(
				loadPos, TNodalLoadGraphics::LoadType::M, orientation));
			fAppliedLoads->addItem(new TLabel(labelMessage, (loadPos + labelOffset),
				8, QPen(Qt::darkGreen)));
		}
	}

	// Draws element end moments.
	for (int i = 0; i < (int)fEndMoments->size(); i++) {
		int elementID = fEndMoments->at(i).getElementID();
		int localNodeID = fEndMoments->at(i).getNode();

		int nodeID;
		if (localNodeID == 0) {
			nodeID = fStructure->getElement(elementID).getNode0ID();
		}
		else if (localNodeID == 1) {
			nodeID = fStructure->getElement(elementID).getNode1ID();
		}
		qreal x = fStructureGraphics->getNodeCoordinates()->GetVal(nodeID, 0);
		qreal y = fStructureGraphics->getNodeCoordinates()->GetVal(nodeID, 1);
		QPointF loadPos(x, y);

		QString labelMessage;
		QPointF labelOffset(40, 0);

		bool orientation = true;
		if (fEndMoments->at(i).getM() < 0) {
			orientation = false;
			labelOffset = labelOffset - QPointF(100, 0);
		}

		labelMessage =
			QString::number(abs(fEndMoments->at(i).getM()), 'g', 3) + " kNm";
		fAppliedLoads->addItem(new TNodalLoadGraphics(
			loadPos, TNodalLoadGraphics::LoadType::M, orientation));
		fAppliedLoads->addItem(new TLabel(labelMessage, (loadPos + labelOffset), 8,
			QPen(Qt::darkGreen)));
	}

	QPointF titlePosition =
		fStructureGraphics->boundingRect().topLeft() + QPointF(-50, -50);
	fAppliedLoads->addItem(new TLabel("Applied Loads", titlePosition, 12));
	fAppliedLoads->setSceneRect(fAppliedLoads->itemsBoundingRect());
}

void TMainWindow::populateAxialForceDiagramScene(qreal LoadScale) {
	fAxialForceDiagram->clear();
	fAxialForceDiagram->addItem(new TStructureGraphics(fStructure));
	fAxialForceDiagram->addItem(new TSupportGraphics(fStructureGraphics));

	std::vector<TElement> elements = fStructure->getElements();
	// Calculates diagram parameters.
	for (int i = 0; i < (int)elements.size(); i++) {
		int node0ID = fStructure->getElement(i).getNode0ID();
		int node1ID = fStructure->getElement(i).getNode1ID();

		qreal x0 = fStructureGraphics->getNodeCoordinates()->GetVal(node0ID, 0);
		qreal y0 = fStructureGraphics->getNodeCoordinates()->GetVal(node0ID, 1);
		qreal x1 = fStructureGraphics->getNodeCoordinates()->GetVal(node1ID, 0);
		qreal y1 = fStructureGraphics->getNodeCoordinates()->GetVal(node1ID, 1);

		QPointF startPoint(x0, y0);
		QPointF endPoint(x1, y1);

		double axialLoadAtNode0 = (*fInternalLoads)[i][0];
		double axialLoadAtNode1 = (*fInternalLoads)[i][3];

		double coefficients[4];
		coefficients[0] = 0;
		coefficients[1] = 0;
		coefficients[2] = 0;
		coefficients[3] = LoadScale * -axialLoadAtNode0;

		for (int j = 0; j < (int)fDistributedLoads->size(); j++) {
			if (fDistributedLoads->at(j).getElementID() == i) {
				// Checks if the load plane is global.
				if (fDistributedLoads->at(j).getLoadPlane() == true) {
					double distributedLoadAtNode0 =
						fDistributedLoads->at(j).getNode0Load();
					double distributedLoadAtNode1 =
						fDistributedLoads->at(j).getNode1Load();

					double elementAngle = QLineF(startPoint, endPoint).angle();

					// Calculates the axial fraction of the load.
					distributedLoadAtNode0 =
						distributedLoadAtNode0 * sin(elementAngle * PI / 180);
					distributedLoadAtNode1 =
						distributedLoadAtNode1 * sin(elementAngle * PI / 180);

					coefficients[1] += -LoadScale *
						(distributedLoadAtNode1 - distributedLoadAtNode0) /
						(elements[i].getL() * 2);
					coefficients[2] += -LoadScale * distributedLoadAtNode0;
				}
			}
		}

		if (x1 < x0 || y1 > y0) {
			if (!(x1 > x0 && y1 > y0)) {
				coefficients[0] = -1 * coefficients[0];
				coefficients[1] = -1 * coefficients[1];
				coefficients[2] = -1 * coefficients[2];
				coefficients[3] = -1 * coefficients[3];

				axialLoadAtNode0 = -1 * axialLoadAtNode0;
				axialLoadAtNode1 = -1 * axialLoadAtNode1;
			}
		}

		// Draws diagram load.
		qreal elementLength = QLineF(startPoint, endPoint).length();
		qreal uScale = elements[i].getL() / elementLength;
		fAxialForceDiagram->addItem(
			new TDiagramLoadGraphics(startPoint, endPoint, coefficients, uScale));

		// Draws labels.
		QString labelMessage;
		QPointF labelPosition;
		qreal elementAngle = QLineF(startPoint, endPoint).angle() * PI / 180;

		// If applicable, draws only one label in the middle of the element.
		if (abs(axialLoadAtNode0 + axialLoadAtNode1) < 0.0001 &&
			abs(coefficients[1]) < 0.0001) {
			if (x1 < x0 || y1 > y0) {
				if (!(x1 > x0 && y1 > y0)) {
					labelMessage = QString::number(axialLoadAtNode0, 'g', 3);
				}
				else {
					labelMessage = QString::number(-axialLoadAtNode0, 'g', 3);
				}
			}
			else {
				labelMessage = QString::number(-axialLoadAtNode0, 'g', 3);
			}

			double x = (startPoint.x() -
				axialLoadAtNode0 * LoadScale * cos(elementAngle + PI / 2) +
				endPoint.x() +
				axialLoadAtNode1 * LoadScale * cos(elementAngle + PI / 2)) /
				2;
			double y = (startPoint.y() +
				axialLoadAtNode0 * LoadScale * sin(elementAngle + PI / 2) +
				endPoint.y() -
				axialLoadAtNode1 * LoadScale * sin(elementAngle + PI / 2)) /
				2;
			labelPosition.rx() = x;
			labelPosition.ry() = y;

			fAxialForceDiagram->addItem(
				new TLabel(labelMessage, labelPosition, 10, QPen(Qt::darkBlue)));
		}

		// Otherwise, draws one label at each element node if the absolute value at
		// the diagram is greater than 0.00001.
		else {
			if (abs(axialLoadAtNode0) > 0.00001) {
				if (x1 < x0 || y1 > y0) {
					if (!(x1 > x0 && y1 > y0)) {
						labelMessage = QString::number(axialLoadAtNode0, 'g', 3);
					}
					else {
						labelMessage = QString::number(-axialLoadAtNode0, 'g', 3);
					}
				}
				else {
					labelMessage = QString::number(-axialLoadAtNode0, 'g', 3);
				}

				labelPosition.rx() =
					startPoint.x() -
					axialLoadAtNode0 * LoadScale * cos(elementAngle + PI / 2);
				labelPosition.ry() =
					startPoint.y() +
					axialLoadAtNode0 * LoadScale * sin(elementAngle + PI / 2);

				fAxialForceDiagram->addItem(
					new TLabel(labelMessage, labelPosition, 10, QPen(Qt::darkBlue)));
			}

			if (abs(axialLoadAtNode1) > 0.00001) {
				if (x1 < x0 || y1 > y0) {
					if (!(x1 > x0 && y1 > y0)) {
						labelMessage = QString::number(-axialLoadAtNode1, 'g', 3);
					}
					else {
						labelMessage = QString::number(axialLoadAtNode1, 'g', 3);
					}
				}
				else {
					labelMessage = QString::number(axialLoadAtNode1, 'g', 3);
				}

				labelPosition.rx() =
					endPoint.x() +
					axialLoadAtNode1 * LoadScale * cos(elementAngle + PI / 2);
				labelPosition.ry() =
					endPoint.y() -
					axialLoadAtNode1 * LoadScale * sin(elementAngle + PI / 2);

				fAxialForceDiagram->addItem(
					new TLabel(labelMessage, labelPosition, 10, QPen(Qt::darkBlue)));
			}
		}
	}
	drawReactions(fAxialForceDiagram);

	QPointF titlePosition =
		fStructureGraphics->boundingRect().topLeft() + QPointF(-50, -50);
	fAxialForceDiagram->addItem(
		new TLabel("Axial Force Diagram (kN)", titlePosition, 12));
	fAxialForceDiagram->setSceneRect(fAxialForceDiagram->itemsBoundingRect());
}

void TMainWindow::populateShearForceDiagramScene(qreal LoadScale) {
	fShearForceDiagram->clear();
	fShearForceDiagram->addItem(new TStructureGraphics(fStructure));
	fShearForceDiagram->addItem(new TSupportGraphics(fStructureGraphics));

	std::vector<TElement> elements = fStructure->getElements();
	// Calculates diagram parameters.
	for (int i = 0; i < (int)elements.size(); i++) {
		int node0ID = fStructure->getElement(i).getNode0ID();
		int node1ID = fStructure->getElement(i).getNode1ID();

		qreal x0 = fStructureGraphics->getNodeCoordinates()->GetVal(node0ID, 0);
		qreal y0 = fStructureGraphics->getNodeCoordinates()->GetVal(node0ID, 1);
		qreal x1 = fStructureGraphics->getNodeCoordinates()->GetVal(node1ID, 0);
		qreal y1 = fStructureGraphics->getNodeCoordinates()->GetVal(node1ID, 1);

		QPointF startPoint(x0, y0);
		QPointF endPoint(x1, y1);

		double shearLoadAtNode0 = (*fInternalLoads)[i][1];
		double shearLoadAtNode1 = (*fInternalLoads)[i][4];

		double coefficients[4];
		coefficients[0] = 0;
		coefficients[1] = 0;
		coefficients[2] = 0;
		coefficients[3] = LoadScale * shearLoadAtNode0;

		for (int j = 0; j < (int)fDistributedLoads->size(); j++) {
			if (fDistributedLoads->at(j).getElementID() == i) {
				double distributedLoadAtNode0 = fDistributedLoads->at(j).getNode0Load();
				double distributedLoadAtNode1 = fDistributedLoads->at(j).getNode1Load();

				// Checks if the load plane is global.
				if (fDistributedLoads->at(j).getLoadPlane() == true) {
					double elementAngle = QLineF(startPoint, endPoint).angle() * PI / 180;

					// Calculates the perpendicular fraction of the load.
					distributedLoadAtNode0 = distributedLoadAtNode0 * cos(elementAngle);
					distributedLoadAtNode1 = distributedLoadAtNode1 * cos(elementAngle);
				}
				coefficients[1] += LoadScale *
					(distributedLoadAtNode1 - distributedLoadAtNode0) /
					(elements[i].getL() * 2);
				coefficients[2] += LoadScale * distributedLoadAtNode0;
			}
		}

		if (x1 < x0 || y1 > y0) {
			if (!(x1 > x0 && y1 > y0)) {
				coefficients[0] = -1 * coefficients[0];
				coefficients[1] = -1 * coefficients[1];
				coefficients[2] = -1 * coefficients[2];
				coefficients[3] = -1 * coefficients[3];

				shearLoadAtNode0 = -1 * shearLoadAtNode0;
				shearLoadAtNode1 = -1 * shearLoadAtNode1;
			}
		}

		// Draws diagram load.
		qreal elementLength = QLineF(startPoint, endPoint).length();
		qreal uScale = elements[i].getL() / elementLength;
		fShearForceDiagram->addItem(
			new TDiagramLoadGraphics(startPoint, endPoint, coefficients, uScale));

		// Draws labels.
		QString labelMessage;
		QPointF labelPosition;
		qreal elementAngle = QLineF(startPoint, endPoint).angle() * PI / 180;

		// If applicable, draws only one label in the middle of the element.
		if (abs(shearLoadAtNode0 + shearLoadAtNode1) < 0.0001 &&
			abs(coefficients[1]) < 0.0001) {
			if (x1 < x0 || y1 > y0) {
				if (!(x1 > x0 && y1 > y0)) {
					labelMessage = QString::number(-shearLoadAtNode0, 'g', 3);
				}
				else {
					labelMessage = QString::number(shearLoadAtNode0, 'g', 3);
				}
			}
			else {
				labelMessage = QString::number(shearLoadAtNode0, 'g', 3);
			}

			double x = (startPoint.x() +
				shearLoadAtNode0 * LoadScale * cos(elementAngle + PI / 2) +
				endPoint.x() -
				shearLoadAtNode1 * LoadScale * cos(elementAngle + PI / 2)) /
				2;
			double y = (startPoint.y() -
				shearLoadAtNode0 * LoadScale * sin(elementAngle + PI / 2) +
				endPoint.y() +
				shearLoadAtNode1 * LoadScale * sin(elementAngle + PI / 2)) /
				2;
			labelPosition.rx() = x;
			labelPosition.ry() = y;

			fShearForceDiagram->addItem(
				new TLabel(labelMessage, labelPosition, 10, QPen(Qt::darkBlue)));
		}

		// Otherwise, draws one label at each element node if the absolute value at
		// the diagram is greater than 0.00001.
		else {
			if (abs(shearLoadAtNode0) > 0.00001) {
				if (x1 < x0 || y1 > y0) {
					if (!(x1 > x0 && y1 > y0)) {
						labelMessage = QString::number(-shearLoadAtNode0, 'g', 3);
					}
					else {
						labelMessage = QString::number(shearLoadAtNode0, 'g', 3);
					}
				}
				else {
					labelMessage = QString::number(shearLoadAtNode0, 'g', 3);
				}

				labelPosition.rx() =
					startPoint.x() +
					shearLoadAtNode0 * LoadScale * cos(elementAngle + PI / 2);
				labelPosition.ry() =
					startPoint.y() -
					shearLoadAtNode0 * LoadScale * sin(elementAngle + PI / 2);
				fShearForceDiagram->addItem(
					new TLabel(labelMessage, labelPosition, 10, QPen(Qt::darkBlue)));
			}
			if (abs(shearLoadAtNode1) > 0.00001) {
				if (x1 < x0 || y1 > y0) {
					if (!(x1 > x0 && y1 > y0)) {
						labelMessage = QString::number(shearLoadAtNode1, 'g', 3);
					}
					else {
						labelMessage = QString::number(-shearLoadAtNode1, 'g', 3);
					}
				}
				else {
					labelMessage = QString::number(-shearLoadAtNode1, 'g', 3);
				}

				labelPosition.rx() =
					endPoint.x() -
					shearLoadAtNode1 * LoadScale * cos(elementAngle + PI / 2);
				labelPosition.ry() =
					endPoint.y() +
					shearLoadAtNode1 * LoadScale * sin(elementAngle + PI / 2);
				fShearForceDiagram->addItem(
					new TLabel(labelMessage, labelPosition, 10, QPen(Qt::darkBlue)));
			}
		}
	}

	drawReactions(fShearForceDiagram);

	QPointF titlePosition =
		fStructureGraphics->boundingRect().topLeft() + QPointF(-50, -50);
	fShearForceDiagram->addItem(
		new TLabel("Shear Force Diagram (kN)", titlePosition, 12));
	fShearForceDiagram->setSceneRect(fShearForceDiagram->itemsBoundingRect());
}

void TMainWindow::populateBendingMomentDiagramScene(qreal LoadScale) {
	fBendingMomentDiagram->clear();
	fBendingMomentDiagram->addItem(new TStructureGraphics(fStructure));
	fBendingMomentDiagram->addItem(new TSupportGraphics(fStructureGraphics));

	std::vector<TElement> elements = fStructure->getElements();
	// Calculates diagram parameters.
	for (int i = 0; i < (int)elements.size(); i++) {
		int node0ID = fStructure->getElement(i).getNode0ID();
		int node1ID = fStructure->getElement(i).getNode1ID();

		qreal x0 = fStructureGraphics->getNodeCoordinates()->GetVal(node0ID, 0);
		qreal y0 = fStructureGraphics->getNodeCoordinates()->GetVal(node0ID, 1);
		qreal x1 = fStructureGraphics->getNodeCoordinates()->GetVal(node1ID, 0);
		qreal y1 = fStructureGraphics->getNodeCoordinates()->GetVal(node1ID, 1);

		QPointF startPoint(x0, y0);
		QPointF endPoint(x1, y1);

		double shearLoadAtNode0 = (*fInternalLoads)[i][1];
		double shearLoadAtNode1 = (*fInternalLoads)[i][4];

		double shearCoefficients[4];
		shearCoefficients[0] = 0;
		shearCoefficients[1] = 0;
		shearCoefficients[2] = 0;
		shearCoefficients[3] = LoadScale * shearLoadAtNode0;

		for (int j = 0; j < (int)fDistributedLoads->size(); j++) {
			if (fDistributedLoads->at(j).getElementID() == i) {
				double distributedLoadAtNode0 = fDistributedLoads->at(j).getNode0Load();
				double distributedLoadAtNode1 = fDistributedLoads->at(j).getNode1Load();

				// Checks if the load plane is global.
				if (fDistributedLoads->at(j).getLoadPlane() == true) {
					double elementAngle = QLineF(startPoint, endPoint).angle() * PI / 180;

					// Calculates the perpendicular fraction of the load.
					distributedLoadAtNode0 = distributedLoadAtNode0 * cos(elementAngle);
					distributedLoadAtNode1 = distributedLoadAtNode1 * cos(elementAngle);
				}
				shearCoefficients[1] +=
					LoadScale * (distributedLoadAtNode1 - distributedLoadAtNode0) /
					(elements[i].getL() * 2);
				shearCoefficients[2] += LoadScale * distributedLoadAtNode0;
			}
		}

		double bendingMomentAtNode0 = (*fInternalLoads)[i][2];
		double bendingMomentAtNode1 = (*fInternalLoads)[i][5];

		double bendingMomentCoefficients[4];
		bendingMomentCoefficients[0] = -shearCoefficients[1] / 3;
		bendingMomentCoefficients[1] = -shearCoefficients[2] / 2;
		bendingMomentCoefficients[2] = -shearCoefficients[3];
		bendingMomentCoefficients[3] = LoadScale * bendingMomentAtNode0;

		// Draws diagram load.
		qreal elementLength = QLineF(startPoint, endPoint).length();
		qreal uScale = elements[i].getL() / elementLength;
		fBendingMomentDiagram->addItem(new TDiagramLoadGraphics(
			startPoint, endPoint, bendingMomentCoefficients, uScale));

		// Draws labels.
		QString labelMessage;
		QPointF labelPosition;
		qreal elementAngle = QLineF(startPoint, endPoint).angle() * PI / 180;

		// If applicable, draws only one label in the middle of the element.
		if (bendingMomentAtNode0 == bendingMomentAtNode1 &&
			abs(bendingMomentCoefficients[1]) < 0.00001 &&
			abs(bendingMomentCoefficients[0]) < 0.00001) {
			labelMessage = QString::number(abs(bendingMomentAtNode0), 'g', 3);

			double x =
				(startPoint.x() +
					bendingMomentAtNode0 * LoadScale * cos(elementAngle + PI / 2) +
					endPoint.x() -
					bendingMomentAtNode1 * LoadScale * cos(elementAngle + PI / 2)) /
				2;
			double y =
				(startPoint.y() -
					bendingMomentAtNode0 * LoadScale * sin(elementAngle + PI / 2) +
					endPoint.y() +
					bendingMomentAtNode1 * LoadScale * sin(elementAngle + PI / 2)) /
				2;
			labelPosition.rx() = x;
			labelPosition.ry() = y;

			fBendingMomentDiagram->addItem(
				new TLabel(labelMessage, labelPosition, 10, QPen(Qt::darkBlue)));
		}
		// Otherwise, draws one label at each element node if the absolute value at
		// the diagram is greater than 0.00001.
		else {
			if (abs(bendingMomentAtNode0) > 0.00001) {
				labelMessage = QString::number(abs(bendingMomentAtNode0), 'g', 3);

				labelPosition.rx() = startPoint.x() +
					(bendingMomentAtNode0 * 1.1) * LoadScale *
					cos(elementAngle + PI / 2);
				labelPosition.ry() = startPoint.y() -
					(bendingMomentAtNode0 * 1.1) * LoadScale *
					sin(elementAngle + PI / 2);

				fBendingMomentDiagram->addItem(
					new TLabel(labelMessage, labelPosition, 10, QPen(Qt::darkBlue)));
			}

			if (abs(bendingMomentAtNode1) > 0.00001) {
				labelMessage = QString::number(abs(bendingMomentAtNode1), 'g', 3);

				labelPosition.rx() = endPoint.x() -
					(bendingMomentAtNode1 * 1.1) * LoadScale *
					cos(elementAngle + PI / 2);
				labelPosition.ry() = endPoint.y() +
					(bendingMomentAtNode1 * 1.1) * LoadScale *
					sin(elementAngle + PI / 2);

				fBendingMomentDiagram->addItem(
					new TLabel(labelMessage, labelPosition, 10, QPen(Qt::darkBlue)));
			}
		}
	}

	drawReactions(fBendingMomentDiagram);

	QPointF titlePosition =
		fStructureGraphics->boundingRect().topLeft() + QPointF(-50, -50);
	fBendingMomentDiagram->addItem(
		new TLabel("Bending Moment Diagram (kNm)", titlePosition, 12));
	fBendingMomentDiagram->setSceneRect(
		fBendingMomentDiagram->itemsBoundingRect());
}

void TMainWindow::populateDisplacement(qreal DisplacementScale) {
	fDisplacement->clear();
	fDisplacement->addItem(new TStructureGraphics(fStructure));
	fDisplacement->addItem(new TSupportGraphics(fStructureGraphics));

	TPZFMatrix<double> D = DisplacementScale * fStructure->getD();
	std::vector<TElement> elements = fStructure->getElements();

	for (int i = 0; i < (int)elements.size(); i++) {
		// Gets data related to degrees of freedom and displacement values of the
		// element.
		int node0ID = fStructure->getElement(i).getNode0ID();
		int node1ID = fStructure->getElement(i).getNode1ID();

		// Gets element drawing length.
		qreal x0 = fStructureGraphics->getNodeCoordinates()->GetVal(node0ID, 0);
		qreal y0 = fStructureGraphics->getNodeCoordinates()->GetVal(node0ID, 1);
		qreal x1 = fStructureGraphics->getNodeCoordinates()->GetVal(node1ID, 0);
		qreal y1 = fStructureGraphics->getNodeCoordinates()->GetVal(node1ID, 1);

		QPointF startPoint(x0, y0);
		QPointF endPoint(x1, y1);

		// Gets degrees of freedom indexes.
		int fx0DOF = fStructure->getElement(i).getEquations()[0];
		int fy0DOF = fStructure->getElement(i).getEquations()[1];
		int m0DOF = fStructure->getElement(i).getEquations()[2];
		int fx1DOF = fStructure->getElement(i).getEquations()[3];
		int fy1DOF = fStructure->getElement(i).getEquations()[4];
		int m1DOF = fStructure->getElement(i).getEquations()[5];

		// Fills the local displacements d with the global displacement matrix at
		// the proper DOF indexes.
		TPZFMatrix<double> d(6, 1, 0);
		d(0, 0) = D[fx0DOF];
		d(1, 0) = D[fy0DOF];
		d(2, 0) = D[m0DOF];
		d(3, 0) = D[fx1DOF];
		d(4, 0) = D[fy1DOF];
		d(5, 0) = D[m1DOF];

		// Calculates the displacements in local coordinates.
		d = fStructure->getElement(i).getT() * d;

		// Computes the perpendicular displacement coefficients.
		double L = elements[i].getL();
		double vCoefficients[4];

		vCoefficients[0] = d(1, 0) * 2 / (L * L * L) + d(2, 0) / (L * L) -
			d(4, 0) * 2 / (L * L * L) + d(5, 0) / (L * L);
		vCoefficients[1] = -d(1, 0) * 3 / (L * L) - d(2, 0) * 2 / L +
			d(4, 0) * 3 / (L * L) - d(5, 0) / L;
		vCoefficients[2] = d(2, 0);
		vCoefficients[3] = d(1, 0);

		// Gets coefficients to transform the displacement curve to fit the element
		// drawing.
		qreal displacementLength = QLineF(startPoint, endPoint).length();
		qreal step = displacementLength / 1000;
		qreal uScale = L / displacementLength;
		qreal angle = QLineF(startPoint, endPoint).angle() * PI / 180;

		// Fills curve with its path points.
		QPainterPath curve(startPoint);
		for (qreal u = 0; u <= displacementLength; u = u + step) {
			qreal scaledU = u * uScale;
			double c0 = vCoefficients[0];
			double c1 = vCoefficients[1];
			double c2 = vCoefficients[2];
			double c3 = vCoefficients[3];

			// Calculates axial and perpendicular displacements.
			qreal v = c0 * scaledU * scaledU * scaledU + c1 * scaledU * scaledU +
				c2 * scaledU + c3;
			qreal du = d(0, 0) + (d(3, 0) - d(0, 0)) * (u / displacementLength);

			// Transforms to the scene coordinate system.
			qreal x = (u + du) * cos(angle) - v * sin(angle);
			qreal y = (u + du) * sin(angle) + v * cos(angle);

			if (u == 0) {
				curve.moveTo(x, -y);
			}
			else {
				curve.lineTo(x, -y);
			}
		}

		// Moves the curve to the node 0 of the element.
		curve.translate(startPoint);
		fDisplacement->addPath(curve);
	}

	QPointF titlePosition =
		fStructureGraphics->boundingRect().topLeft() + QPointF(-50, -50);
	fDisplacement->addItem(
		new TLabel("Structure Displacement", titlePosition, 12));
	fDisplacement->setSceneRect(fDisplacement->itemsBoundingRect());
}
