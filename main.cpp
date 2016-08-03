#include <iostream>
#include <vector>
#include <limits>
#include <numeric>

using namespace std;

class Relation;

typedef vector<Relation> Clause;
typedef vector<Clause> DNF;

class Relation{
private:
    string _symbol;
    unsigned _num;
    vector<int> _left;
    vector<int> _right;
    vector<string> _vars;

public:
    Relation(string symbol, unsigned num,
             vector<int> left, vector<int> right, vector<string> vars)
        : _symbol(symbol), _num(num), _left(left), _right(right), _vars(vars)
    {}

    string GetSymbol() const;
    unsigned GetNum() const;
    vector<int> GetLeftOperand() const;
    vector<int> GetRightOperand() const;
    vector<string> GetVars() const;

    void PrintRelation() const;

    void SetSymbol(const string s);
//    void SetNum(unsigned n);
    void SetLeftOperand(const vector<int> ops);
    void SetRightOperand(const vector<int> ops);
//    void SetVars(const vector<string> vars);

};


//-----------------------------------------------------------------------------

void FM_PrintDNF(const DNF &dnf);

void FM_PrintClause(const Clause &c);

void FM_ApplyOrientation(DNF &dnf);

void FM_ApplyDecomposition(DNF &dnf);

int FM_GreatestCommonDivisor(int a, int b);

int FM_GreatestCommonDivisor(const vector<int> &v);

void FM_ApplyNormalization(Clause &c);

void FM_RemoveDuplicates(DNF &dnf);

bool FM_ExistsEquality(const Clause &c, int &k);

void FM_SideFrequency(const Clause &c, vector< pair<unsigned, unsigned> > &freq);

//bool FM_ContainsVariable(const Relation &r, unsigned i);

int FM_LeastCommonMultiple(int a, int b);

int FM_LeastCommonMultiple(const vector<int> &v);

bool FM_ApplyIsolation(Clause &c, unsigned &index, vector<int> &vlcm);

void FM_ComputeLCM(Clause &c, unsigned index, vector<int> vlcm);

void FM_ResolveConstraints(Clause &c, unsigned index);


void FM_ResolveConstraints2();


bool FM_CheckSAT(const DNF &dnf);

bool FM_CheckSAT(const Clause &c);

bool FM_Iterate(Clause &c);


//-----------------------------------------------------------------------------

string Relation::GetSymbol() const
{
    return _symbol;
}

unsigned Relation::GetNum() const
{
    return _num;
}

vector<int> Relation::GetLeftOperand() const
{
    return _left;
}

vector<int> Relation::GetRightOperand() const
{
    return _right;
}

vector<string> Relation::GetVars() const
{
    return _vars;
}


void Relation::PrintRelation() const
{
    bool first = false;

    cout << "( ";
    for(unsigned i=0; i<_left.size(); i++)
    {
        if(_left[i] < 0)
            cout << "(" << _left[i] << _vars[i] << ")";
        else if(_left[i] == 1)
            cout << _vars[i];
        else if(_left[i] > 0)
            cout << _left[i] << _vars[i];

        if(!first && _left[i] != 0)
            first = true;

        if(i < _left.size()-1 && _left[i+1] != 0 && first)
            cout << "+";

    }

    if(!first)
        cout << 0;

    cout << " " << GetSymbol() << " ";

    first = false;
    for(unsigned i=0; i<_right.size(); i++)
    {
        if(_right[i] < 0)
            cout << "(" << _right[i] << _vars[i] << ")";
        else if(_right[i] == 1)
            cout << _vars[i];
        else if(_right[i] > 0)
            cout << _right[i] << _vars[i];

        if(!first && _right[i] != 0)
            first = true;

        if(i < _right.size()-1 && _right[i+1] != 0 && first)
            cout << "+";
    }

    if(!first)
        cout << 0;
    cout << " )";
}


void Relation::SetSymbol(const string s)
{
    _symbol = s;
}

void Relation::SetLeftOperand(const vector<int> ops)
{
    _left = ops;
}

void Relation::SetRightOperand(const vector<int> ops)
{
    _right = ops;
}


//------------------------------------------------------------------------------

void FM_PrintDNF(const DNF &dnf)
{
    cout << "{" << endl;
    for(unsigned i=0; i<dnf.size(); i++)
    {
        cout << "{";
        for(unsigned j=0; j<dnf[i].size(); j++)
        {
            dnf[i][j].PrintRelation();
            if(j != dnf[i].size()-1)
                cout << " /\\ ";
        }
        if(i != dnf.size()-1)
            cout << "}," << endl;
        else
            cout << "}" << endl;
    }
    cout << "}" << endl;
}

void FM_PrintClause(const Clause &c)
{
    cout << "{";
    for(unsigned j=0; j<c.size(); j++)
    {
        c[j].PrintRelation();
        if(j != c.size()-1)
            cout << " /\\ ";
    }
    cout << "}" << endl;
}

void FM_ApplyOrientation(DNF &dnf)
{
    for(unsigned i=0; i<dnf.size(); i++)
        for(unsigned j=0; j<dnf[i].size(); j++)
        {
            if(dnf[i][j].GetSymbol() == ">=")
            {
                dnf[i][j] = Relation("<=", dnf[i][j].GetNum(),
                                     dnf[i][j].GetRightOperand(),
                                     dnf[i][j].GetLeftOperand(),
                                     dnf[i][j].GetVars());
            }
            else if(dnf[i][j].GetSymbol() == ">")
            {
                dnf[i][j] = Relation("<", dnf[i][j].GetNum(),
                                     dnf[i][j].GetRightOperand(),
                                     dnf[i][j].GetLeftOperand(),
                                     dnf[i][j].GetVars());
            }
        }
}

void FM_ApplyDecomposition(DNF &dnf)
{
    for(unsigned i=0; i<dnf.size(); i++)
        for(unsigned j=0; j<dnf[i].size(); j++)
        {
            if(dnf[i][j].GetSymbol() == "<=")
            {
                dnf[i][j].SetSymbol("=");
                dnf.push_back(dnf[i]);

                dnf[i][j].SetSymbol("<");
            }
        }
}

int FM_GreatestCommonDivisor(int a, int b)
{
    if(b == 0)
        return a;

    if(a%b == 0)
        return b;

    return FM_GreatestCommonDivisor(b, a%b);
}

int FM_GreatestCommonDivisor(const vector<int> &v)
{
    int gcd = v[0];
    for(unsigned i=1; i<v.size(); i++)
        gcd = FM_GreatestCommonDivisor(gcd, v[i]);

    return gcd;
}

void FM_ApplyNormalization(Clause &c)
{
    for(unsigned j=0; j<c.size(); j++)
    {
        vector<int> left, right, vgcd;
        int gcd;

        left = c[j].GetLeftOperand();
        right = c[j].GetRightOperand();

        for(unsigned k=0; k<left.size(); k++)
        {
            int dif = left[k]-right[k];
            if((left[k] ^ right[k]) >= 0)
            {
                if(dif>0)
                {
                    left[k] = dif;
                    right[k] = 0;
                }
                else
                {
                    right[k] = -dif;
                    left[k] = 0;
                }
            }
            else if(left[k] < 0)
            {
                left[k] = 0;
                right[k] = -dif;
            }
            else
            {
                left[k] = dif;
                right[k] = 0;
            }
        }

        vgcd = left;
        vgcd.insert(vgcd.end(), right.begin(), right.end());
        gcd = FM_GreatestCommonDivisor(vgcd);

        if(gcd != 0)
            for(unsigned k=0; k<left.size(); k++)
            {
                left[k] /= gcd;
                right[k] /= gcd;
            }

        c[j].SetLeftOperand(left);
        c[j].SetRightOperand(right);
    }
}

//MOZDA I OVA MALO ZEZA
void FM_RemoveDuplicates(Clause &c)
{
    for(unsigned j=0; j<c.size(); j++)
    {
        vector<int> j_left = c[j].GetLeftOperand();
        vector<int> j_right = c[j].GetRightOperand();

        unsigned k = j+1;
        while(k<c.size())
        {
            vector<int> k_left = c[k].GetLeftOperand();
            vector<int> k_right = c[k].GetRightOperand();

            if(c[j].GetSymbol() == c[k].GetSymbol())
            {
                if(c[j].GetSymbol() == "=")
                {
                    if((j_left == k_left && j_right == k_right) ||
                        (j_left == k_right && j_right == k_left))
                    {
                        c[k] = c.back();
                        c.pop_back();

                    }
                    else
                        k++;

                }
                else if(j_left == k_left && j_right == k_right)
                {
                        c[k] = c.back();
                        c.pop_back();
                }
                else
                    k++;
            }
            else
                k++;

        }
    }
}

bool FM_ExistsEquality(const Clause &c, int &k)
{

    for(unsigned i=0; i<c.size(); i++)
        if(c[i].GetSymbol() == "=")
        {
            k = i;
            return true;
        }

    k = -1;
    return false;
}

void FM_SideFrequency(const Clause &c, vector< pair<unsigned, unsigned> > &freq)
{
    unsigned n = c[0].GetNum();

    vector<unsigned> fleft(n, 0);
    vector<unsigned> fright(n, 0);


    for(unsigned i=0; i<c.size(); i++)
    {
        vector<int> left = c[i].GetLeftOperand();
        vector<int> right = c[i].GetRightOperand();

        for(unsigned j=0; j<n; j++)
        {
            if(left[j] != 0)
                fleft[j]++;

            if(right[j] != 0)
                fright[j]++;
        }
    }

    for(unsigned j=0; j<n; j++)
        freq.push_back(make_pair(fleft[j], fright[j]));
}

/*bool FM_ContainsVariable(const Relation &r, unsigned i)
{
    if(r.GetLeftOperand()[i] != 0)
        return true;
    else
        return false;
}
*/

int FM_LeastCommonMultiple(int a, int b)
{
    return a*b/FM_GreatestCommonDivisor(a, b);
}

int FM_LeastCommonMultiple(const vector<int> &v)
{
    int lcm=1;
    for(unsigned i=0; i<v.size(); i++)
        lcm = FM_LeastCommonMultiple(lcm, v[i]);

    return lcm;
}

//RADI ALI MORA IZBOR PROMENLJIVE JOS MALO DA SE SREDI
bool FM_ApplyIsolation(Clause &c, unsigned &index, vector<int> &vlcm)
{
    vector< pair<unsigned, unsigned> > freq;
    FM_SideFrequency(c, freq);

    unsigned min = numeric_limits<unsigned>::max();
    for(unsigned j=0; j<freq.size(); j++)
    {
        unsigned m = freq[j].first*freq[j].second;
        if(m < min && m > 0)
        {
            min = m;
            index = j;
        }
    }


    int k;
    if(min == numeric_limits<unsigned>::max() && !FM_ExistsEquality(c, k))
        return false;
    else if(min == numeric_limits<unsigned>::max() && FM_ExistsEquality(c, k))
    {
        vector<int> left = c[k].GetLeftOperand();
        vector<int> right = c[k].GetRightOperand();

        int m = numeric_limits<int>::max();
        for(unsigned j=0; j<left.size(); j++)
        {
            if(left[j] < m && left[j] > 0)
            {
                m = left[j];
                index = j;
            }
            if(right[j] < m && right[j] > 0)
            {
                m = right[j];
                index = j;
            }
        }
    }

    for(unsigned j=0; j<c.size(); j++)
    {
        vector<int> left = c[j].GetLeftOperand();
        vector<int> right = c[j].GetRightOperand();
        vector<string> vars = c[j].GetVars();

        int lcoef = left[index];
        int rcoef = right[index];



        if(left[index] != 0)
        {
            vlcm.push_back(lcoef);

            for(unsigned k=0; k<vars.size(); k++)
                if(k != index)
                {
                    right[k] -= left[k];
                    left[k] = 0;
                }
        }
        else if(right[index] != 0)
        {
            vlcm.push_back(rcoef);

            for(unsigned k=0; k<vars.size(); k++)
                if(k != index)
                {
                    left[k] -= right[k];
                    right[k] = 0;
                }
        }

        c[j] = Relation(c[j].GetSymbol(),
                        c[j].GetNum(), left, right, vars);
    }

    return true;
}

void FM_ComputeLCM(Clause &c, unsigned index, vector<int> vlcm)
{
    int lcm = FM_LeastCommonMultiple(vlcm);


    for(unsigned j=0; j<c.size(); j++)
    {
        vector<Relation> great, low, none;

        vector<int> left = c[j].GetLeftOperand();
        vector<int> right = c[j].GetRightOperand();
        vector<string> vars = c[j].GetVars();

        if(left[index] != 0)
        {
//                    int q = lcm/left[index];

            for(unsigned k=0; k<right.size(); k++)
                right[k] *= lcm/left[index];
            left[index] *= lcm/left[index];

            low.push_back(Relation(c[j].GetSymbol(),
                                   c[j].GetNum(), left, right, vars));

        }
        else if(right[index] != 0)
        {
            for(unsigned k=0; k<left.size(); k++)
                left[k] *= lcm/right[index];
            right[index] *= lcm/right[index];

            great.push_back(Relation(c[j].GetSymbol(),
                                     c[j].GetNum(), left, right, vars));
        }
        else
            none.push_back(Relation(c[j].GetSymbol(),
                                    c[j].GetNum(), left, right, vars));

        c[j] = Relation(c[j].GetSymbol(), c[j].GetNum(), left, right, vars);
    }
}

void FM_ResolveConstraints(Clause &c, unsigned index)
{
    Clause c1;
    for(unsigned j=0; j<c.size(); j++)
    {
        vector<int> left1 = c[j].GetLeftOperand();
        vector<int> right1 = c[j].GetRightOperand();
        vector<string> vars = c[j].GetVars();
        string symbol;

        vars[index] = vars.back();
        vars.pop_back();

        if(left1[index] == 0  &&  right1[index] == 0)
        {
            left1[index] = left1.back();
            left1.pop_back();
            right1[index] = right1.back();
            right1.pop_back();

            c1.push_back(Relation(c[j].GetSymbol(), vars.size(), left1, right1, vars));
            continue;
        }

        for(unsigned k=j+1; k<c.size(); k++)
        {
            vector<int> left2 = c[k].GetLeftOperand();
            vector<int> right2 = c[k].GetRightOperand();

            if(c[j].GetSymbol() == "<" || c[k].GetSymbol() == "<")
                symbol = "<";
            else if(c[j].GetSymbol() == "<=" || c[k].GetSymbol() == "<=")
                symbol = "<=";
            else
                symbol = "=";

            if(c[j].GetSymbol() == "=")
            {
                if(left1[index] != 0 && left2[index] != 0)
                {
                    vector<int> right1p = right1;
                    right1p[index] = right1p.back();
                    right1p.pop_back();

                    right2[index] = right2.back();
                    right2.pop_back();

                    c1.push_back(Relation(c[j].GetSymbol(), vars.size(), right1p, right2, vars));
                }
                else if(left1[index] != 0 && right2[index] != 0)
                {
                    vector<int> right1p = right1;
                    right1p[index] = right1p.back();
                    right1p.pop_back();

                    left2[index] = left2.back();
                    left2.pop_back();

                    c1.push_back(Relation(c[j].GetSymbol(), vars.size(), left2, right1p, vars));
                }
                else if(right1[index] != 0 && left2[index] != 0)
                {
                    vector<int> left1p = left1;
                    left1p[index] = left1p.back();
                    left1p.pop_back();

                    right2[index] = right2.back();
                    right2.pop_back();

                    c1.push_back(Relation(c[j].GetSymbol(), vars.size(), left1p, right2, vars));
                }
                else if(right1[index] != 0 && right2[index] != 0)
                {
                    vector<int> left1p = left1;
                    left1p[index] = left1p.back();
                    left1p.pop_back();

                    left2[index] = left2.back();
                    left2.pop_back();

                    c1.push_back(Relation(c[j].GetSymbol(), vars.size(), left2, left1p, vars));
                }
            }
            else if(c[k].GetSymbol() == "=")
            {
                if(left1[index] != 0 && left2[index] != 0)
                {
                    vector<int> right1p = right1;
                    right1p[index] = right1p.back();
                    right1p.pop_back();

                    right2[index] = right2.back();
                    right2.pop_back();

                    c1.push_back(Relation(c[j].GetSymbol(), vars.size(), right2, right1p, vars));
                }
                else if(left1[index] != 0 && right2[index] != 0)
                {
                    vector<int> right1p = right1;
                    right1p[index] = right1p.back();
                    right1p.pop_back();

                    left2[index] = left2.back();
                    left2.pop_back();

                    c1.push_back(Relation(c[j].GetSymbol(), vars.size(), left2, right1p, vars));
                }
                else if(right1[index] != 0 && left2[index] != 0)
                {
                    vector<int> left1p = left1;
                    left1p[index] = left1p.back();
                    left1p.pop_back();

                    right2[index] = right2.back();
                    right2.pop_back();

                    c1.push_back(Relation(c[j].GetSymbol(), vars.size(), left1p, right2, vars));
                }
                else if(right1[index] != 0 && right2[index] != 0)
                {
                    vector<int> left1p = left1;
                    left1p[index] = left1p.back();
                    left1p.pop_back();

                    left2[index] = left2.back();
                    left2.pop_back();

                    c1.push_back(Relation(c[j].GetSymbol(), vars.size(), left1p, left2, vars));
                }
            }
            else
            {
                if(left1[index] != 0  &&  right2[index] != 0)
                {
                    vector<int> right1p = right1;
                    right1p[index] = right1p.back();
                    right1p.pop_back();

                    left2[index] = left2.back();
                    left2.pop_back();

                    c1.push_back(Relation(symbol, vars.size(), left2, right1p, vars));
                }
                else if(right1[index] != 0  &&  left2[index] != 0)
                {
                    vector<int> left1p = left1;
                    left1p[index] = left1p.back();
                    left1p.pop_back();

                    right2[index] = right2.back();
                    right2.pop_back();

                    c1.push_back(Relation(symbol, vars.size(), left1p, right2, vars));
                }
            }
        }

    }

    c = c1;
}


void FM_ResolveConstraints2()
{
    /*
                dnf[i].clear();
                for(unsigned j=0; j<great.size(); j++)
                {
                    for(unsigned k=0; k<low.size(); k++)
                        if(great[j].GetSymbol() == "<=" && low[k].GetSymbol() == "<=")
                            dnf[i].push_back(Relation("<=", great[j].GetNum(),
                                                      great[j].GetLeftOperand(),
                                                      low[k].GetRightOperand(),
                                                      great[j].GetVars()));
                        else
                            dnf[i].push_back(Relation("<", great[j].GetNum(),
                                                      great[j].GetLeftOperand(),
                                                      low[k].GetRightOperand(),
                                                      great[j].GetVars()));
                }
                for(unsigned j=0; j<none.size(); j++)
                    dnf[i].push_back(none[j]);
    */
}


bool FM_CheckSAT(const DNF &dnf)
{
    for(unsigned i=0; i<dnf.size(); i++)
        for(unsigned j=0; j<dnf[i].size(); j++)
            if(dnf[i][j].GetSymbol() == "<")
            {
                vector<int> left = dnf[i][j].GetLeftOperand();
                vector<int> right = dnf[i][j].GetRightOperand();

                bool ret = false;
                for(unsigned k=0; k<left.size(); k++)
                    if(left[k] !=0 || right[k] !=0)
                        ret = true;

                if(!ret)
                    return false;
            }

    return true;
}


bool FM_CheckSAT(const Clause &c)
{
    for(unsigned j=0; j<c.size(); j++)
        if(c[j].GetSymbol() == "<")
        {
            vector<int> left = c[j].GetLeftOperand();
            vector<int> right = c[j].GetRightOperand();

            bool ret = false;
            for(unsigned k=0; k<left.size(); k++)
                if(left[k] !=0 || right[k] !=0)
                    ret = true;

            if(!ret)
                return false;
        }

    return true;
}

bool FM_Iterate(Clause &c)
{
//------------------------CHOSE VARIABLE----------------------------------------------
    unsigned index;
    vector<int> vlcm;

    if(!FM_ApplyIsolation(c, index, vlcm))
        return false;

    cout << endl << "Isolate variable " << c[0].GetVars()[index] << ":" << endl;
    FM_PrintClause(c);
    cout << endl;


//-------------------------------EQUALIZE COEFS--------------------------------------
    FM_ComputeLCM(c, index, vlcm);

    cout << endl << "Least common multiple:" << endl;
    FM_PrintClause(c);
    cout << endl;


//--------------RESOLVE CONSTRAINTS-----------------------------------------------
    FM_ResolveConstraints(c, index);

    cout << endl << "Resolve constraints:" << endl;
    FM_PrintClause(c);
    cout << endl;


//-----------------NORMALIZE------------------------------------------------------
    FM_ApplyNormalization(c);
    FM_RemoveDuplicates(c);

    cout << endl << "Normalize constraints:" << endl;
    FM_PrintClause(c);
    cout << endl;

    return true;
}


//------------------------------------------------------------------------------
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
