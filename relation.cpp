#include "relation.h"

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

