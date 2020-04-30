#include "JSONIntegration.h"
#include "TDistributedLoad.h"
#include "TElement.h"
#include "TElementEndMoment.h"
#include "TNodalLoad.h"
#include <iostream>

using json = nlohmann::json;

int main() {
    // Reads input JSON file and converts it into a TStructure object and vectors
    // of each type of load.
    ifstream input("InputJSON.json");
    json J;
    input >> J;

    TStructure structure;
    std::vector<TNodalLoad> nodalLoads;
    std::vector<TDistributedLoad> distrLoads;
    std::vector<TElementEndMoment> endMoments;
    importData(J, &structure, nodalLoads, distrLoads, endMoments);

    // Solves structure.
    std::vector<TPZFMatrix<double>> internalLoads;
    TPZFMatrix<double> loadVector, displacementVector;
    structure.solve(nodalLoads, distrLoads, endMoments, internalLoads);

    // Displays the node equation numbering matrix.
    cout << "Node equations:" << endl;
    structure.enumerateEquations();
    structure.getNodeEquations().Print(cout);

    // Displays the Global Stiffness Matrix K.
    cout << "Stiffness matrix K:" << endl;
    structure.getK().Print(cout);

    // Displays results.
    cout << "Displacement vector:" << endl;
    displacementVector.Print(cout);
    cout << "Load vector:" << endl;
    loadVector.Print(cout);

    for (size_t i = 0; i < internalLoads.size(); i++) {
        cout << "Internal loads at element " << i << ":" << endl;
        internalLoads[i].Print(cout);
    }

#ifdef _WIN32
    system("pause");
#endif

    return 0;
}
