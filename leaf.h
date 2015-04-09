
#include <bits/stdc++.h>
#define _ ios_base::sync_with_stdio(0);cin.tie(0);

using namespace std;

class Leaf{
public:
	string file;
	int numKeys;
	std::vector<double> keys;
	std::vector<string> values;
	string next;
	string parent;

	Leaf(string filename);
	string getFile();
	int getNumKeys();
	std::vector<double> getKeys();
	std::vector<string> getValues();
	string getNext();
	string getParent();
	void readFile(string filename);
	void writeFile(string filename);
	std::vector<std::string> tokenizer( const std::string& p_pcstStr, char delim );
	string readValueFile(string filename);
	void initialize();
};  

void Leaf::initialize(){
	numKeys=0;
	next="NULL";
	parent="NULL";
	writeFile(file);
}


std::vector<std::string> Leaf::tokenizer( const std::string& p_pcstStr, char delim )  {
        std::vector<std::string> tokens;
        std::stringstream   mySstream( p_pcstStr );
        std::string         temp;

        while( getline( mySstream, temp, delim ) ) {
            tokens.push_back( temp );
        }

        return tokens;
    }


string Leaf::getFile(){return file;}
int Leaf::getNumKeys(){return numKeys;}
std::vector<double> Leaf::getKeys(){return keys;}
std::vector<string> Leaf::getValues(){return values;}
string Leaf::getNext(){return next;}
string Leaf::getParent(){return parent;}
void Leaf::readFile(string filename){
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
				keys.push_back(stod(s)); 
			}
		}
		else if(l==2){
			values=this->tokenizer(line,' ');	
		}
		else if(l==3){
			next=line;
		}
		else if(l==4){
			parent=line;
		}
		l++;
	}
}

void Leaf::writeFile(string filename){
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
	outfile<<next<<endl;
	outfile<<parent;
}

Leaf::Leaf(string filename){
	file=filename;
	readFile(filename);
}

string Leaf::readValueFile(string filename){
	std::string line;
	const char* f = filename.c_str();
	std::ifstream infile(f);
	string res="";
	while (std::getline(infile, line))  // this does the checking!
	{
		res+=line;
	}	
	return res;
}