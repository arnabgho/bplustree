#include <bits/stdc++.h>
#define _ ios_base::sync_with_stdio(0);cin.tie(0);

using namespace std;
#include "node.h"
#include "leaf.h"

class bplustree{
public:
	int maxKeys;
	string ROOT;
	int nNode;
	int nLeaf;
	int nValue;

	void readMaxKeys();
	string getRoot();
	void insert(double key,string val);
	std::vector<string> range(double center,double width);
	std::vector<string> window(double left,double right);
	string leafSearch(string root,double key);
	string createValueFile(string value);
	string createLeafFile();
	string createNodeFile();
	void insert_into_node(string target,double newkey,string val);
	void insert_into_tree(string root,double newkey,string value);
	string readValueFile(string filename);
	bplustree();
	~bplustree();
};

bplustree::bplustree(){
	std::string line;
	std::ifstream infile("metadata");
	int i=0;
	while (std::getline(infile, line))  // this does the checking!
	{
		if(i==0){maxKeys=stoi(line);}
		else if(i==1){ROOT=line;}
		else if(i==2){nNode=stoi(line);}
		else if(i==3){nLeaf=stoi(line);}
		else if(i==4){nValue=stoi(line);}
		i++;	
	}	
}

bplustree::~bplustree(){
	// cout<<"Hi updating metadata\n";
	ofstream outfile;
	outfile.open("metadata");
	outfile<<maxKeys<<endl;
	outfile<<ROOT<<endl;
	outfile<<nNode<<endl;
	outfile<<nLeaf<<endl;
	outfile<<nValue<<endl;
}

void bplustree::readMaxKeys(){
	std::ifstream infile("bplustree.config");
	string line;
	getline(infile,line);
	maxKeys=stoi(line);
}


