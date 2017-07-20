#ifndef RELATION_H_
#define RELATION_H_

#include <iostream>
#include <vector>
#include <string>

using namespace std;


typedef string PredicateSymbol;
typedef string Variable;

class Relation{
private:
    PredicateSymbol _symbol;
    vector<int> _left;
    vector<int> _right;
    vector<Variable> _vars;

public:
    Relation(PredicateSymbol symbol, vector<int> left, vector<int> right, vector<Variable> vars)
        : _symbol(symbol), _left(left), _right(right), _vars(vars)
    {}

    PredicateSymbol GetSymbol() const;
    unsigned GetNum() const;
    vector<int> GetLeftOperand() const;
    vector<int> GetRightOperand() const;
    vector<Variable> GetVars() const;

    void PrintRelation() const;

    void SetSymbol(const PredicateSymbol & s);
    void SetLeftOperand(const vector<int> ops);
    void SetRightOperand(const vector<int> ops);
//    void SetVars(const vector<string> vars);

};

#endif
