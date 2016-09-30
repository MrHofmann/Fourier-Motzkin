#ifndef FM_H_
#define FM_H_

#include <vector>
#include <limits>
#include "relation.h"

using namespace std;

typedef vector<Relation> Clause;
typedef vector<Clause> DNF;



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

void FM_ApplyIsolation(Clause &c, unsigned &index, vector<int> &vlcm);

void FM_ComputeLCM(Clause &c, unsigned index, vector<int> vlcm);

void FM_ResolveConstraints(Clause &c, unsigned index);


void FM_ResolveConstraints2();


bool FM_CheckSAT(const DNF &dnf, bool &sat);

bool FM_CheckSAT(const Clause &c, bool &sat);

void FM_Iterate(Clause &c);



#endif
