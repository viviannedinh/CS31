#include <iostream>
using namespace std;

int main()
{
    // declare and initialize fine rates
    const double YES_FRAUD_RATE = 0.22;
    const double NO_FRAUD_RATE = 0.1;
    const double LESS_40_RATE = 0.66;
    const double MORE_250_RATE = 0.14;
    
    // declare user input variables
    string defendantName;
    double thousandsPaid;
    string isFraud;
    
    // get user input
    cout << "Defendant: ";
    getline(cin, defendantName);
    cout << "Amount paid (in thousands): ";
    cin >> thousandsPaid;
    cin.ignore(10000, '\n');
    cout << "Fake athlete? (y/n): ";
    getline(cin, isFraud);
    cout << "---" << endl;
    
    // declare and initialize fine
    double fine = 20;
    
    // calculate fine
    if (thousandsPaid <= 40)
    {
        fine += (thousandsPaid * LESS_40_RATE);
    }
    else if ((thousandsPaid > 40) && (thousandsPaid <= 250))
    {
        if (isFraud == "n")
            fine += (40 * LESS_40_RATE) + ((thousandsPaid - 40) * NO_FRAUD_RATE);
        else if (isFraud == "y")
            fine += (40 * LESS_40_RATE) + ((thousandsPaid - 40) * YES_FRAUD_RATE);
    }
    else
    {
        if (isFraud == "n")
            fine += (40 * LESS_40_RATE) + (210 * NO_FRAUD_RATE) + ((thousandsPaid - 250) * MORE_250_RATE);
        else if (isFraud == "y")
            fine += (40 * LESS_40_RATE) + (210 * YES_FRAUD_RATE) + ((thousandsPaid - 250) * MORE_250_RATE);
    }
    
    // set precision
    cout.setf(ios::fixed);
    cout.precision(1);
    
    // output results
    if (defendantName == "")
        cout << "You must enter a defendant name." << endl;
    else if (thousandsPaid < 0)
        cout << "The amount paid must not be negative." << endl;
    else if ((isFraud != "y") && (isFraud != "n"))
        cout << "You must enter y or n." << endl;
    else
        cout << "The suggested fine for " << defendantName << " is $" << fine << " thousand." << endl;
}
