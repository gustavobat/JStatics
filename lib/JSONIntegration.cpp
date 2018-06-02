/** \file JSONIntegration.cpp
* Contains the definition of methods that transforms the JSON file into the
* class objects and vice-versa.
*/

#include "JSONIntegration.h"

//! Prints a JSON object to std::cout.
void printJSON(const nlohmann::json& J)
{
	std::cout << std::setw(4) << J << std::endl;
	std::cout << std::flush;
}

//! Converts JSON into structure and loads objects.
void importData(const nlohmann::json &J, TStructure *Parent,
	std::vector<TNodalLoad> &NodalLoads,
	std::vector<TDistributedLoad> &DistributedLoads,
	std::vector<TElementEndMoment> &EndMoments)
{
	// Reads the vector of TNode.
	if (J.find("Nodes") != J.end()) {
		std::vector<TNode> nodes;

		for (int i = 0; i < (int)J["Nodes"].size(); i++) {
			TNode node = J["Nodes"][i];
			nodes.push_back(node);
		}
		Parent->setNodes(nodes);
	}
	else {
		// Stops debug if no node has been assigned to the structure.
		DebugStop();
	}

	// Reads the vector of TMaterial.
	if (J.find("Materials") != J.end()) {
		std::vector<TMaterial> materials;

		for (int i = 0; i < (int)J["Materials"].size(); i++) {
			TMaterial material = J["Materials"][i];
			materials.push_back(material);
		}
		Parent->setMaterials(materials);
	}
	else {
		// Stops debug if no material has been assigned to the structure.
		DebugStop();
	}

	// Reads the vector of TSupport.
	if (J.find("Supports") != J.end()) {
		std::vector<TSupport> supports;
		std::vector<bool> checker(Parent->getNodes().size(), false);

		for (int i = 0; i < (int)J["Supports"].size(); i++) {
			// Ensures that a node has up to one TSupport object.
			if (checker[J["Supports"][i]["Node"].get<int>()] == false) {
				checker[J["Supports"][i]["Node"].get<int>()] = true;
				TSupport support = J["Supports"][i];
				support.setStructure(Parent);
				supports.push_back(support);
			}
		}
		Parent->setSupports(supports);
	}
	else {
		// Stops debug if no support has been assigned to the structure.
		DebugStop();
	}

	// Reads the vector of TElements.
	if (J.find("Elements") != J.end()) {
		std::vector<TElement> elements;

		for (int i = 0; i < (int)J["Elements"].size(); i++) {
			TElement element = J["Elements"][i];
			element.setStructure(Parent);
			elements.push_back(element);
		}
		Parent->setElements(elements);
	}
	else {
		// Stops debug if no element has been assigned to the structure.
		DebugStop();
	}

	bool structureHasLoad = false;
	// Reads the vector of TNodalLoad.
	if (J.find("Nodal Loads") != J.end()) {
		std::vector<bool> checker(Parent->getNodes().size(), false);

		for (int i = 0; i < (int)J["Nodal Loads"].size(); i++) {
			// Ensures that only one nodal load object is applied to a node.
			if (checker[J["Nodal Loads"][i]["Node"].get<int>()] == false) {
				checker[J["Nodal Loads"][i]["Node"].get<int>()] = true;

				TNodalLoad newLoad = J["Nodal Loads"][i];
				newLoad.setStructure(Parent);
				NodalLoads.push_back(newLoad);
			}
		}
		structureHasLoad = true;
	}

	// Reads the vector of TDistributedLoad.
	if (J.find("Distributed Loads") != J.end()) {
		std::vector<bool> checker(Parent->getElements().size(), false);

		for (int i = 0; i < (int)J["Distributed Loads"].size(); i++) {
			// Ensures that only one distributed load is applied to an element.
			if (checker[J["Distributed Loads"][i]["Element"].get<int>()]
			 	== false) {
				checker[J["Distributed Loads"][i]["Element"].get<int>()] = true;
				TDistributedLoad newLoad = J["Distributed Loads"][i];
				newLoad.setStructure(Parent);
				DistributedLoads.push_back(newLoad);
			}
		}
		structureHasLoad = true;
	}

	// Reads the vector of TElementEndMoment.
	if (J.find("Element End Moments") != J.end()) {
		std::vector<bool> node0Checker(Parent->getElements().size(), false);
		std::vector<bool> node1Checker(Parent->getElements().size(), false);

		for (int i = 0; i < (int)J["Element End Moments"].size(); i++) {
			// Ensures that only one end moment is applied to an element node 0.
			if (J["Element End Moments"][i]["Node"].get<int>() == 0) {
				if (node0Checker[J["Element End Moments"][i]["Element"]]
					== false) {
					node0Checker[J["Element End Moments"][i]["Element"]] = true;
					TElementEndMoment newLoad = J["Element End Moments"][i];
					newLoad.setStructure(Parent);
					EndMoments.push_back(newLoad);
				}
			}
			// Ensures that only one end moment is applied to an element node 1.
			if (J["Element End Moments"][i]["Node"].get<int>() == 1) {
				if (node1Checker[J["Element End Moments"][i]["Element"]]
					== false) {
					node1Checker[J["Element End Moments"][i]["Element"]] = true;
					TElementEndMoment newLoad = J["Element End Moments"][i];
					newLoad.setStructure(Parent);
					EndMoments.push_back(newLoad);
				}
			}
		}
		structureHasLoad = true;
	}

	if (!structureHasLoad) {
		// Stops debug if no load has been assigned to the structure.
		DebugStop();
	}
}

//! Converts a TMaterial object to JSON.
void to_json(nlohmann::json& J, const TMaterial& M)
{
	J = nlohmann::json{ { "E", M.getE() },{ "A", M.getA() },{ "I", M.getI() } };
}

//! Converts a JSON object to a TMaterial one.
void from_json(const nlohmann::json& J, TMaterial& M)
{
	M.setE(J.at("E").get<double>());
	M.setA(J.at("A").get<double>());
	M.setI(J.at("I").get<double>());
}

//! Converts a TNode object to JSON.
void to_json(nlohmann::json& J, const TNode& N)
{
	J = nlohmann::json{ N.getX(), N.getY() };
}

//! Converts a JSON object to a TNode one.
void from_json(const nlohmann::json& J, TNode& N)
{
	N.setX(J[0].get<double>());
	N.setY(J[1].get<double>());
}

//! Converts a TSupport object to JSON.
void to_json(nlohmann::json& J, const TSupport& S)
{
	J = nlohmann::json{ { "Conditions",{ S.getFx(), S.getFy(), S.getM() } },
	{ "Node", S.getNodeID() } };
}

//! Converts a JSON object to a TSupport one.
void from_json(const nlohmann::json& J, TSupport& S)
{
	bool fx = J["Conditions"][0].get<int>() != 0;
	bool fy = J["Conditions"][1].get<int>() != 0;
	bool m = J["Conditions"][2].get<int>() != 0;
	S.setFx(fx);
	S.setFy(fy);
	S.setM(m);
	S.setNodeID(J["Node"].get<int>());
}

//! Converts a TElement object to JSON.
void to_json(nlohmann::json& J, const TElement& E)
{
	J = nlohmann::json{ { "Nodes",{ E.getNode0ID(), E.getNode1ID() } },
	{ "Hinges",{ E.getHinge0(), E.getHinge1() } },
	{ "Material", E.getMaterialID() } };
}

//! Converts a JSON object to a TElement one.
void from_json(const nlohmann::json& J, TElement& E)
{
	E.setNode0ID(J["Nodes"][0].get<int>());
	E.setNode1ID(J["Nodes"][1].get<int>());

	bool hinge0, hinge1;
	if (J.find("Hinges") != J.end()) {
		hinge0 = J["Hinges"][0].get<int>() != 0;
		hinge1 = J["Hinges"][1].get<int>() != 0;
	}
	else {
		hinge0 = false;
		hinge1 = false;
	}

	E.setHinge0(hinge0);
	E.setHinge1(hinge1);
	E.setMaterialID(J["Material"].get<int>());
}

//! Converts a TNodalLoad object to JSON.
void to_json(nlohmann::json& J, const TNodalLoad& NL)
{
	J = nlohmann::json{ { "Node", NL.getNodeID() },
	{ "Fx", NL.getFx() },
	{ "Fy", NL.getFy() },
	{ "M", NL.getM() } };
}

//! Converts a JSON object to a TNodalLoad one.
void from_json(const nlohmann::json& J, TNodalLoad& NL)
{

	NL.setNodeID(J["Node"].get<int>());

	if (J.find("Fx") != J.end()) {
		NL.setFx(J["Fx"].get<double>());
	}
	else {
		NL.setFx(0.0);
	}

	if (J.find("Fy") != J.end()) {
		NL.setFy(J["Fy"].get<double>());
	}
	else {
		NL.setFy(0.0);
	}

	if (J.find("M") != J.end()) {
		NL.setM(J["M"].get<double>());
	}
	else {
		NL.setM(0.0);
	}
}

//! Converts a TDistributedLoad object to JSON.
void to_json(nlohmann::json& J, const TDistributedLoad& DL)
{
	J = nlohmann::json{ { "Element", DL.getElementID() },
	{ "Node 0 Load", DL.getNode0Load() },
	{ "Node 1 Load", DL.getNode1Load() },
	{ "Load Plane", DL.getLoadPlane() } };
}

//! Converts a JSON object to a TDistributedLoad one.
void from_json(const nlohmann::json& J, TDistributedLoad& DL)
{
	DL.setElementID(J["Element"].get<int>());
	DL.setNode0Load(J["Node 0 Load"].get<double>());
	DL.setNode1Load(J["Node 1 Load"].get<double>());
	bool loadPlane = J["Load Plane"].get<int>() != 0;
	DL.setLoadPlane(loadPlane);
}

//! Converts a TElementEndMoment object to JSON.
void to_json(nlohmann::json& J, const TElementEndMoment& EEM)
{
	J = nlohmann::json{ { "Element", EEM.getElementID() },
	{ "Node", EEM.getNode() },
	{ "M", EEM.getM() } };
}

//! Converts a JSON object to a TElementEndMoment one.
void from_json(const nlohmann::json& J, TElementEndMoment& EEM)
{
	EEM.setElementID(J["Element"].get<int>());
	EEM.setNode(J["Node"].get<int>());
	EEM.setM(J["M"].get<double>());
}
