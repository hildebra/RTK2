#pragma once
#include "Matrix.h"

#ifdef _MSC_VER
//remove win fopen warning
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#endif



const string path2abundance = "/assemblies/metag/ContigStats/Coverage.pergene";
const string path2counts = "/assemblies/metag/ContigStats/Coverage.count_pergene";
const string path2mediAB = "/assemblies/metag/ContigStats/Coverage.median.pergene";

const string pseudoAssMarker = "/assemblies/metag/longReads.fasta.filt.sto";

//typedef std::unordered_map<uint, uint>::iterator SmplAbunIT;
//typedef std::unordered_map<uint, uint> SmplAbun;


class ContigCrossHit
{
public:
	ContigCrossHit(int sn) :smplN(sn), CtgsPerSmpl(smplN,0),
		SmplNms(0,""){}
	void setSmplNms(vector<string> &sns) { SmplNms = sns; }
	void addHit(int Smpl, int Ctg);
	//~ContigCrossHit() {}
private:
	size_t smplN;
	vector<uint> CtgsPerSmpl;
	vector<string> SmplNms;
	
};
struct clusWrk {
	clusWrk(void) :geneNamesStr(""), Vec(0), Clnum(-1) {}
	clusWrk(int s, long x) :geneNamesStr(""), Vec(s, (smat_fl)0), Clnum(x) {}
	string geneNamesStr;
	vector<smat_fl> Vec;
	long Clnum;
};
clusWrk* workClusBlock(textBlock*, const size_t, const string& sampleSeq,
	const vector<GeneAbundance*>& GAs, const SmplOccurMult*, long);
void printVec(clusWrk * curClus, ostream*, ofstream*);// , const vector<bool>& useSmpl);




class ClStr2Mat
{
	//class for gene catalog creation with cd-hit
public:
	ClStr2Mat(options* opts);
		//const  string inF, const string outF, const string mapF, const string baseP, 	bool covCalc, bool oldMap);
	virtual ~ClStr2Mat();
private:
	struct job {
		std::future <clusWrk*> fut;
		bool inUse = false;
	};
	struct jobW {
		std::future <void> fut;
		bool inUse = false;
	};
	void read_map(const string,bool,bool,bool);
	void sealMap();

	void addSums(clusWrk * curClus) {
		const vector<smat_fl>& pr = curClus->Vec;
		for (size_t i = 0; i < pr.size(); i++) {
			SmplSum[i] += pr[i];
		}
	}

	//takes care of output
	void manage_write(clusWrk* curClus);
	void finish_write();
	long lastClIdWr;
	list<clusWrk*> tmpSave;
	//core routines (con be parralelized later)


	vector<GeneAbundance*> GAs;
	ContigCrossHit* CCH;
	SmplOccurMult smpls;
	string2string smplRid;
	vector<string> smplLoc; // only needed in readmap
	vector<string> baseP; // only needed in readmap
	vector<string> mapGr;
	//vector<bool> useSmpl; // potentially slow, but simple
	vector<smat_fl> SmplSum;
	size_t smplN;
	size_t curr ;
	string lastline;

	//output IO
	ostream* matO;
	ofstream* geneNames;

	//threads
	jobW wrThr;
};

