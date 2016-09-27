#ifndef RELATION_H_
#define RELATION_H_

#include <iostream>
#include <vector>

using namespace std;

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

#endif
