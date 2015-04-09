#include <bits/stdc++.h>
#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#include "bplustree.cpp"

using namespace std;

int maxKeys;

int readMaxKeys(){
	std::ifstream infile("bplustree.config");
	string line;
	getline(infile,line);
	maxKeys=stoi(line);
	return maxKeys;
}

void createRoot(){
	ofstream outfile;
	outfile.open("L00000001");
	outfile<<"0\n";
	outfile<<endl;
	outfile<<endl;
	outfile<<"NULL\n";
	outfile<<"NULL\n";
}

void createMetadata(){
	ofstream outfile;
	outfile.open("metadata");
	outfile<<maxKeys<<endl;
	outfile<<"L00000001"<<endl;
	outfile<<"0"<<endl;
	outfile<<"1"<<endl;
	outfile<<"0"<<endl;
}

int main() {
	int ch=1;
	maxKeys=readMaxKeys();
	createRoot();
	createMetadata();
	bplustree  *B=new bplustree();
	int a;
	
	while(cin>>a){
		// cout<<"0 to insert , 2 for Range Query , 4 for Window Query\n";
		
		double b;
		cin>>b;
		if(a==0){
			string c;
			cin>>c;
			B->insert(b,c);
		}
		else if(a==2){
			double c;
			cin>>c;
			std::vector<string> res=B->range(b,c);
			for (auto x:res)
			{
				string y=B->readValueFile(x);
				cout<<y<<endl;
			}
		}
		else if(a==1){
			// double c;
			// cin>>c;
			std::vector<string> res=B->range(b,0);
			for (auto x:res)
			{
				string y=B->readValueFile(x);
				cout<<y<<endl;
			}
		}
		// cout<<"0 to terminate\n";
		// fflush(stdout);
		// fflush(stdin);

		// cin>>ch;
	}
	B->~bplustree();
	return 0;
}