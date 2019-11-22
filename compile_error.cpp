// Code for Project 1
// Report poll results

#include <iostream>
using namespace std;   // pp. 38-39 in Savitch 6/e explains this line

int main()
{
    int forImpeachment;
    int antiImpeachment;

    cout << "How many people were surveyed? ";
    cin >> numberSurveyed;
    cout << "How many of them support impeachment of the president? ";
    cin >> forImpeachment;
    cout << "How many of them oppose impeachment of the president? ";
    cin >> antiImpeachment;

    double pctFor = 100.0 * forImpeachment / numberSurveyed;
    double pctAnti = 100.0 * antiImpeachment / numberSurveyed;

    cout.setf(ios::fixed);       // see pp. 32-33 in Savitch 6/e
    cout.precision(1);

    cout << endl
    cout << pctFor << "% say they support impeachment." << endl;
    cout << pctAnti << "% say they oppose impeachment." << endl;

    if (forImpeachment > antiImpeachment)
        cout << "More people support impeachment than oppose it." << endl;
    else
        cout << "More people oppose impeachment than support it." << endl;
}
