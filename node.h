
#include <bits/stdc++.h>
#define _ ios_base::sync_with_stdio(0);cin.tie(0);

using namespace std;

class Node{
public:
	string file;
	int numKeys;
	std::vector<double> keys;
	std::vector<string> values;
	string parent;

	Node(string filename);
	string getFile();
	int getNumKeys();
	std::vector<double> getKeys();
	std::vector<string> getValues();
	string getParent();
	void readFile(string filename);
	void writeFile(string filename);
	std::vector<std::string> tokenizer( const std::string& p_pcstStr, char delim );
	string before(int ki);
	string after(int ki);
	void initialize();
};  


void Node::initialize(){
	numKeys=0;
	parent="NULL";
	writeFile(file);
}

std::vector<std::string> Node::tokenizer( const std::string& p_pcstStr, char delim )  {
        std::vector<std::string> tokens;
        std::stringstream   mySstream( p_pcstStr );
        std::string         temp;

        while( getline( mySstream, temp, delim ) ) {
            tokens.push_back( temp );
        }

        return tokens;
    }



string Node::getFile(){return file;}
int Node::getNumKeys(){return numKeys;}
std::vector<double> Node::getKeys(){return keys;}
std::vector<string> Node::getValues(){return values;}
string Node::getParent(){return parent;}
void Node::readFile(string filename){
	file=filename;
	std::string line;
	const char* f = filename.c_str();
	std::ifstream infile(f);
	int l=0;
	while (std::getline(infile, line))  // this does the checking!
	{
		if(l==0)
			numKeys=stoi(line);
		else if(l==1){
			vector<string> temp=this->tokenizer(line,' ');
			for (int i = 0; i < numKeys; ++i){
				string s="";
				for(int j=0;j<temp[i].size();j++) if(temp[i][j]!=' ') s+=temp[i][j];	
				// cout<<"s "<<s<<endl;	
				keys.push_back(stod(temp[i])); 
			}
		}
		else if(l==2){
			values=this->tokenizer(line,' ');	
		}
		else if(l==3){
			parent=line;
		}
		l++;
	}
	// assert(keys.size()==numKeys);
	// assert(values.size()==numKeys+1);
}

void Node::writeFile(string filename){
	file=filename;
	ofstream outfile;
	const char* f = filename.c_str();
	outfile.open(f);
	outfile<<numKeys<<endl;
	for (int i = 0; i < keys.size(); ++i)
	{
		if(i!=keys.size()-1)
			outfile<<keys[i]<<" ";
		else
			outfile<<keys[i];
	}
	outfile<<endl;
	for (int i = 0; i < values.size(); ++i)
	{
		if(i!=values.size()-1)
			outfile<<values[i]<<" ";
		else
			outfile<<values[i];
	}
	outfile<<endl;
	outfile<<parent;
}

Node::Node(string filename){
	file=filename;
	readFile(filename);
}

string Node::before(int ki){
	assert(ki<numKeys);
	return values[ki];
}

string Node::after(int ki){
	// assert(ki+1<numKeys);
	return values[ki+1];
}