#include <iostream>
#include <vector>
#include "fm.h"


using namespace std;


int main()
{
    vector<PredicateSymbol> vars;

//ZADATAK SA JUNSKOG ROKA (NEZADOVOLJIVA FORMULA)
//----------------------------------------------------------------------
    vars.push_back("a");
    vars.push_back("b");
    vars.push_back("c");
    vars.push_back("d");

    Relation r1("<", {1, 0, 0, 0}, {0, 5, 2, 0}, vars);
    Relation r2("=", {2, 3, 2, 0}, {0, 0, 0, 1}, vars);
    Relation r3(">", {1, 2, -2, 0}, {0, 0, 0, 3}, vars);
    Relation r4("<=", {2, 10, 2, 0}, {0, 0, 0, 4}, vars);

    Clause c1 = {r1, r2, r3, r4};
//----------------------------------------------------------------------

//PROST PRIMER ZADOVOLJIVE FORMULE
//----------------------------------------------------------------------
//    vars.push_back("a");
//    vars.push_back("b");

//    Relation r5("<=", {1, 0}, {0, 1}, vars);
//    Relation r6("<=", {1, 1}, {0, 0}, vars);

//    Clause c2 = {r5, r6};
//----------------------------------------------------------------------


    DNF dnf = {c1};

    cout << endl << "Relation vector (DNF):" << endl;
    FM_PrintDNF(dnf);
    cout << endl;

    cout << endl << "Oriented and Normalized DNF:" << endl;
    FM_ApplyOrientation(dnf);
    FM_ApplyNormalization(dnf[0]);
    FM_PrintDNF(dnf);
    cout << endl;


//    cout << endl << "Decomposed DNF:" << endl;
//    FM_ApplyDecomposition(dnf);
//    FM_PrintDNF(dnf);
//    cout << endl;


    bool sat;
    for(unsigned i=0; i<dnf.size(); i++)
    {
        int iteration = 0;

        while(!FM_CheckSAT(dnf[i], sat))
        {
	    cout << "Press enter to continue..." << endl;
            system("read");

            cout << "--------------------------------------------------------------" << endl
                 << "CLAUSE " << i+1 << " ITERATION " << ++iteration << ":"  << endl
                 << "--------------------------------------------------------------" << endl;

            FM_Iterate(dnf[i]);
        }

        if(sat)
            break;
    }

    if(sat)
        cout << "SAT" << endl << endl;
    else
        cout << "UNSAT" << endl << endl;


    return 0;
}
