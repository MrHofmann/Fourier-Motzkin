#include <iostream>
#include <vector>
#include "fm.h"


using namespace std;


int main()
{
    vector<string> vars;

    vars.push_back("a");
    vars.push_back("b");
    vars.push_back("c");
    vars.push_back("d");

    Relation r1("<", vars.size(), {1, 0, 0, 0}, {0, 5, 2, 0}, vars);
    Relation r2("=", vars.size(), {2, 3, 2, 0}, {0, 0, 0, 1}, vars);
    Relation r3(">", vars.size(), {1, 2, -2, 0}, {0, 0, 0, 3}, vars);
    Relation r4("<=", vars.size(), {2, 10, 2, 0}, {0, 0, 0, 4}, vars);

    Clause c = {r1, r2, r3, r4};
    DNF dnf = {c};

    cout << endl << "Relation vector (DNF):" << endl;
    FM_PrintDNF(dnf);
    cout << endl;

    cout << endl << "Oriented DNF:" << endl;
    FM_ApplyOrientation(dnf);
    FM_PrintDNF(dnf);
    cout << endl;


    cout << endl << "Normalized DNF:" << endl;
    FM_ApplyNormalization(dnf[0]);
    FM_PrintDNF(dnf);
    cout << endl;


    cout << endl << "Decomposed DNF:" << endl;
    FM_ApplyDecomposition(dnf);
    FM_PrintDNF(dnf);
    cout << endl;


    for(unsigned i=0; i<dnf.size(); i++)
    {
        int iteration = 0;
        bool sat = true;

        while(1)
        {
            cout << endl << "CLAUSE " << i+1 << " ITERATION " << ++iteration << ":" << endl
                 << "--------------------------------------------------------------" << endl;
            if(!FM_Iterate(dnf[i]))
            {
                if(!FM_CheckSAT(dnf))
                    sat = false;

                break;
            }

            cout << "Press enter to continue..." << endl;
            system("read");
        }

        if(sat)
            cout << "SAT" << endl;
        else
            cout << "UNSAT" << endl;
    }


    return 0;
}

/*
            for(j=0; j<freq.size(); j++)
            {
                cout << freq[j].first << " " << freq[j].second << endl;
            }

            for(unsigned j=0; j<low.size(); j++)
            {
                Relation r = low[j];

                vector<int> left = r.GetLeftOperand();
                vector<int> right = r.GetRightOperand();

                for(unsigned k=0; k<left.size(); k++)
                {
                    cout << left[k] << ", ";
                }

                cout << r.GetSymbol() << " ";

                for(unsigned k=0; k<right.size(); k++)
                {
                    cout << right[k] << ", ";
                }

                cout << endl;
            }

            cout << "---------------------------------" << endl;

            for(unsigned j=0; j<great.size(); j++)
            {
                Relation r = great[j];

                vector<int> left = r.GetLeftOperand();
                vector<int> right = r.GetRightOperand();

                for(unsigned k=0; k<left.size(); k++)
                {
                    cout << left[k] << ", ";
                }

                cout << r.GetSymbol() << " ";

                for(unsigned k=0; k<right.size(); k++)
                {
                    cout << right[k] << ", ";
                }

                cout << endl;
            }

            cout << "---------------------------------" << endl;

            for(unsigned j=0; j<none.size(); j++)
            {
                Relation r = none[j];

                vector<int> left = r.GetLeftOperand();
                vector<int> right = r.GetRightOperand();

                for(unsigned k=0; k<left.size(); k++)
                {
                    cout << left[k] << ", ";
                }

                cout << r.GetSymbol() << " ";

                for(unsigned k=0; k<right.size(); k++)
                {
                    cout << right[k] << ", ";
                }

                cout << endl;
            }

void FM_Iterate(DNF &dnf)
{
    for(unsigned i=0; i<dnf.size(); i++)
    {
        int j;

//        if(FM_ExistsEquality(dnf[i], j))
        if(false)
        {
            vector<int> left = dnf[i][j].GetLeftOperand();
            vector<int> right = dnf[i][j].GetRightOperand();


            unsigned lindex, rindex;
            int lmin = numeric_limits<int>::max();
            int rmin = numeric_limits<int>::max();

            for(unsigned k=0; k<left.size(); k++)
                if(left[k] != 0 && left[k] < lmin)
                {
                    lmin = left[k];
                    lindex = k;
                }

            for(unsigned k=0; k<right.size(); k++)
                if(right[k] != 0 && right[k]<rmin)
                {
                    rmin = right[k];
                    rindex = k;
                }

            if(lmin < rmin)
                for(unsigned k=0; k<left.size(); k++)
                {
                    if(k != lindex)
                    {
                        right[k] -= left[k];
                        left[k] = 0;
                    }
                }
            else
                for(unsigned k=0; k<right.size(); k++)
                {
                    if(k != rindex)
                    {
                        left[k] -= right[k];
                        right[k] = 0;
                    }
                }
        }
        else
        {
//------------------------CHOSE VARIABLE----------------------------------------------
            unsigned index;
            vector<int> vlcm;

//-----------------------COMPUTE LCM AND ISOLATE VARIABLE-----------------------------
            FM_ApplyIsolation(dnf[i], index, vlcm);

            cout << endl << "Isolate variable: " << endl;
            FM_PrintDNF(dnf);
            cout << endl;


//-------------------------------EQUALIZE COEFS--------------------------------------
            FM_ComputeLCM(dnf[i], index, vlcm);

            cout << endl << "Least common multiple:" << endl;
            FM_PrintDNF(dnf);
            cout << endl;


//--------------RESOLVE CONSTRAINTS-----------------------------------------------
            FM_ResolveConstraints(dnf[i], index);

            cout << endl << "Resolve constraints:" << endl;
            FM_PrintDNF(dnf);
            cout << endl;


//-----------------NORMALIZE------------------------------------------------------
            FM_SwitchSide(dnf[i]);
            FM_RemoveDuplicates(dnf[i]);

            cout << endl << "Normalize constraints:" << endl;
            FM_PrintDNF(dnf);
            cout << endl;
        }
    }
}
*/
