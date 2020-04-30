/** \file main.cpp
* Main function of the GUI project.
*/

#include <QApplication>
#include "TMainWindow.h"

using namespace std;
using json = nlohmann::json;

int main(int argc, char *argv[])
{
    // Starts application and initializes the TMainWindow object.
    QApplication app(argc, argv);

    TMainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}
