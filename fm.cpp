#include "fm.h"


void FM_PrintDNF(const DNF &dnf)
{
    cout << "{" << endl;
    for(unsigned i=0; i<dnf.size(); i++)
        FM_PrintClause(dnf[i]);

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


//RADI ALI MORA IZBOR PROMENLJIVE JOS MALO DA SE SREDI
void FM_ApplyIsolation(Clause &c, unsigned &index, vector<int> &vlcm)
{
    vector< pair<unsigned, unsigned> > freq;
    FM_SideFrequency(c, freq);

    //PRVO PROVERA DA LI POSTOJI JEDNAKOST KAKO BI NJU PRVO ELIMINISALI,
    //AKO POSTOJI ONDA SE BIRA PROMENLJIVA IZ PRVE PROADJENE JEDNAKOSTI
    int k;
    if(FM_ExistsEquality(c, k))
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
    else
    {
        //INACE,
        //PROVERA DA LI POSTOJI PROMENLJIVA KOJA SA JAVLJA SA OBE STRANE
        //U RAZLICITIM RELACIJAMA I BIRA SE ONA KOJA PROIZVODI NAJMANJI
        //BROJ DODATNIH RELACIJA NAKON REZOLVIRANJA RELACIJA
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
    }

    //IZOLACIJA IZABRANE PROMENLJIVE
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

            if(c[j].GetSymbol() == "=" || c[k].GetSymbol() == "=")
            {
                if(left1[index] != 0 && left2[index] != 0)
                {
                    vector<int> right1p = right1;
                    right1p[index] = right1p.back();
                    right1p.pop_back();

                    right2[index] = right2.back();
                    right2.pop_back();

                    c1.push_back(Relation(symbol, vars.size(), right1p, right2, vars));
                }
                else if(left1[index] != 0 && right2[index] != 0)
                {
                    vector<int> right1p = right1;
                    right1p[index] = right1p.back();
                    right1p.pop_back();

                    left2[index] = left2.back();
                    left2.pop_back();

                    c1.push_back(Relation(symbol, vars.size(), left2, right1p, vars));
                }
                else if(right1[index] != 0 && left2[index] != 0)
                {
                    vector<int> left1p = left1;
                    left1p[index] = left1p.back();
                    left1p.pop_back();

                    right2[index] = right2.back();
                    right2.pop_back();

                    c1.push_back(Relation(symbol, vars.size(), left1p, right2, vars));
                }
                else if(right1[index] != 0 && right2[index] != 0)
                {
                    vector<int> left1p = left1;
                    left1p[index] = left1p.back();
                    left1p.pop_back();

                    left2[index] = left2.back();
                    left2.pop_back();

                    c1.push_back(Relation(symbol, vars.size(), left2, left1p, vars));
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


void FM_Iterate(Clause &c)
{
//------------------------CHOSE VARIABLE----------------------------------------------
    unsigned index;
    vector<int> vlcm;

    FM_ApplyIsolation(c, index, vlcm);

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
    cout << endl << "Normalize constraints:" << endl;
    FM_ApplyNormalization(c);
    FM_PrintClause(c);

    FM_RemoveDuplicates(c);
    FM_PrintClause(c);
    cout << endl;
}

bool FM_CheckSAT(const DNF &dnf, bool &sat)
{
    for(unsigned i=0; i<dnf.size(); i++)
        if(!FM_CheckSAT(dnf[i], sat))
            return false;
        else if(!sat)
            return true;

    return true;
}

bool FM_CheckSAT(const Clause &c, bool &sat)
{
    if(c.size() == 1 && c[0].GetSymbol() == "<")
    {
        vector<int> left = c[0].GetLeftOperand();
        vector<int> right = c[0].GetRightOperand();

        sat = false;
        for(unsigned i=0; i<left.size(); i++)
            if(left[i] != 0 || right[i] != 0)
            {
                sat = true;
                break;
            }

        return true;
    }

    vector< pair<unsigned, unsigned> > freq;
    FM_SideFrequency(c, freq);

    for(unsigned j=0; j<freq.size(); j++)
        if(freq[j].first*freq[j].second != 0)
            return false;

    int k;
    if(!FM_ExistsEquality(c, k))
    {
        sat = true;
        return true;
    }
}

/*
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
*/
