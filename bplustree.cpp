#include <bits/stdc++.h>
#define _ ios_base::sync_with_stdio(0);cin.tie(0);

using namespace std;

#include "bplustree.h"
// #include "tokenizer.h"

// int main(int argc, char const *argv[]){return 0;}

string bplustree::readValueFile(string filename){
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

string bplustree::createValueFile(string value){
	nValue++;
	string filename=to_string(nValue);
	int n=filename.size();
	for (int i = 0; i < 8-n; ++i)
	{
		filename='0'+filename;
	}
	filename='V'+filename;
	ofstream outfile;
	const char* f = filename.c_str();
	outfile.open(f);
	outfile<<value;
	return filename;
}

string bplustree::createLeafFile(){
	nLeaf++;
	string filename=to_string(nLeaf);
	int n=filename.size();
	for (int i = 0; i < 8-n; ++i)
	{
		filename='0'+filename;
	}
	filename='L'+filename;
	Leaf l(filename);
	l.initialize();
	l.writeFile(filename);
	return filename;
}

string bplustree::createNodeFile(){
	nNode++;
	string filename=to_string(nNode);
	int n=filename.size();
	for (int i = 0; i < 8-n; ++i)
	{
		filename='0'+filename;
	}
	filename='N'+filename;
	Node node(filename);
	node.initialize();
	node.writeFile(filename);
	return filename;
}

string bplustree::getRoot(){
	return ROOT;
}



string bplustree::leafSearch(string root,double key){	// Searches the leaf which might possibly contain the key or might be inserted here
	if(root[0]=='L')
		return root;
	Node n(root);
	if(key<n.keys[0])
		return leafSearch(n.before(0),key);
	else if(key>=n.keys[n.getNumKeys()-1])
		return leafSearch(n.after(n.getNumKeys()-1),key);
	for (int i = 0; i+1 < n.getNumKeys() ; ++i)
	{
		if( n.keys[i]<=key && key<n.keys[i+1])
			return leafSearch(n.after(i),key);
	}
}


std::vector<string> bplustree::window(double left,double right){
	string leafFile=leafSearch(ROOT,left);
	std::vector<string> res;
	// // cout<<"leafFile "<<leafFile<<endl;
	while(leafFile!="NULL"){
		Leaf l(leafFile);
		int i=0;
		while(i<l.numKeys && l.keys[i]<left ) i++;
		// // cout<<"i "<<i<<endl;
		while(i<l.numKeys && left<=l.keys[i] && l.keys[i]<=right) {
			res.push_back(l.values[i]);
			i++;
		}
		if(i<l.numKeys && l.keys[i]>right ) break;
		leafFile=l.next;
	}
	return res;
}


std::vector<string> bplustree::range(double center,double width){
	double left,right;
	left=max(0.0,center-width);
	right=min(1.0,center+width);
	// // cout<<"hello range\n";
	return window(left,right);
}




/**
 * Tries to inserts the (newkey/val) pair into
 * the node.
 *
 * If `target` is an interior node, then `val` must be a page pointer.
 */




void bplustree::insert_into_node(string target,double newkey,string val){
	if(target[0]=='L'){
		// // cout<<"target "<<target<<endl;
		Leaf l(target);
		Node n(l.parent);
		// case 1
		string valueFile=createValueFile(val);
		if(l.numKeys==0){
			l.numKeys=1;
			l.keys.push_back(newkey);
			l.values.push_back(valueFile);
			l.writeFile(l.file);
			// // cout<<"Hi done\n";
			return;
		}
		// // cout<<"still here\n";
		int pos=-1;
		if(newkey<l.keys[0])
			pos=0;
		else if(newkey>=l.keys[l.numKeys-1])
			pos=l.numKeys;
		else{
			for (int i = 0; i+1 < l.getNumKeys() ; ++i)
			{
				if( l.keys[i]<=newkey && newkey<l.keys[i+1]){
					pos=i+1;break;
				}
			}
		}
		assert(pos!=-1);
		// // cout<<"pos "<<pos<<endl; 
		l.keys.insert(l.keys.begin()+pos,newkey);
		l.values.insert(l.values.begin()+pos,valueFile);
		l.numKeys++;
		if(l.getNumKeys()-1<maxKeys){
			// // cout<<"hi\n";
			l.writeFile(l.file);
		}

		
		else if(l.getNumKeys()-1==maxKeys){
			string leafFile=createLeafFile();
			Leaf newLeaf(leafFile);
			int left,right;
			left=l.numKeys/2;
			right=l.numKeys-l.numKeys/2;
			vector<double> rightKeys(l.keys.begin()+left , l.keys.end() );
			vector<double> leftKeys( l.keys.begin(),l.keys.begin()+left );
			vector<string> rightValues(l.values.begin()+left , l.values.end());
			vector<string> leftValues( l.values.begin(),l.values.begin()+left );
			newLeaf.keys=rightKeys;
			newLeaf.values=rightValues;
			l.keys=leftKeys;
			l.values=leftValues;
			l.numKeys=left;
			newLeaf.numKeys=right;
			newLeaf.next=l.next;
			l.next=leafFile;

			if(l.parent=="NULL"){
				string nodeFile=createNodeFile();
				Node new_interior(nodeFile);
				l.parent=newLeaf.parent=nodeFile;
				l.writeFile(l.file);
				newLeaf.writeFile(newLeaf.file);
				if(ROOT==l.file)
					ROOT=nodeFile;
				new_interior.values.push_back(l.file);
				new_interior.writeFile(new_interior.file);
				insert_into_node(nodeFile,newLeaf.keys[0],newLeaf.file);
			}

			else if(n.getNumKeys()<maxKeys){
				newLeaf.parent=l.parent;
				l.writeFile(l.file);
				newLeaf.writeFile(newLeaf.file);
				insert_into_node(newLeaf.parent,newLeaf.keys[0],newLeaf.file);
			}
			else if(n.getNumKeys()==maxKeys){
				// cout<<"HEllo in this case\n";
				string nodeFile=createNodeFile();
				Node new_interior(nodeFile);
				double k=newLeaf.keys[0];
				pos=-1;
				if(k<n.keys[0])
					pos=0;
				else if(k>=n.keys[n.numKeys-1])
					pos=n.numKeys;
				else{
					for (int i = 0; i+1 < n.getNumKeys() ; ++i)
					{
						if( n.keys[i]<=k && k<n.keys[i+1]){
							pos=i+1;break;
						}
					}
				}
				assert(pos!=-1);
				n.keys.insert(n.keys.begin()+pos,k);
				n.values.insert(n.values.begin()+pos+1,newLeaf.file);
				n.numKeys++;


				// cout<<"n.numKeys/2 "<<n.numKeys/2<<endl;
				// cout<<"-------\n";
				int left=n.numKeys/2;
				// cout<<"-----done----\n";
				// cout<<"left "<<left<<endl;
				// right=n.numKeys-n.numKeys/2;
				
				double middle_key=n.keys[left];

				// cout<<"middle_key "<<middle_key<<endl;
				vector<double> rightNodeKeys(n.keys.begin()+left+1 , n.keys.end() );
				vector<double> leftNodeKeys( n.keys.begin(),n.keys.begin()+left );
				vector<string> rightNodeValues(n.values.begin()+left+1 , n.values.end());
				vector<string> leftNodeValues( n.values.begin(),n.values.begin()+left+1 );

				new_interior.keys=rightNodeKeys;
				new_interior.values=rightNodeValues;
				n.keys=leftNodeKeys;
				n.values=leftNodeValues;

				
				new_interior.numKeys=new_interior.keys.size();
				n.numKeys=n.keys.size();
				if(l.keys[0]<middle_key)
					l.parent=n.file;
				else
					l.parent=new_interior.file;
				if(newLeaf.keys[0]<middle_key)
					newLeaf.parent=n.file;
				else
					newLeaf.parent=new_interior.file;
				l.writeFile(l.file);
				newLeaf.writeFile(newLeaf.file);
				new_interior.parent=n.parent;
				new_interior.writeFile(new_interior.file);
				n.writeFile(n.file);
				if(n.file==ROOT){
					string rootFile=createNodeFile();
					ROOT=rootFile;
					Node newRoot(rootFile);
					newRoot.values.push_back(n.file);
					newRoot.writeFile(newRoot.file);

					new_interior.parent=rootFile;
					n.parent=rootFile;
					new_interior.writeFile(new_interior.file);
					n.writeFile(n.file);
					insert_into_node(ROOT,middle_key,new_interior.file);
				}
				else{
					insert_into_node(n.parent,middle_key,new_interior.file);
				}
			}
		}
	}
	if(target[0]=='N'){
		Node l(target);
		Node n(l.parent);
		// case 1
		string valueFile=val;
		if(l.numKeys==0){
			l.numKeys=1;
			l.keys.push_back(newkey);
			l.values.push_back(valueFile);
			l.writeFile(l.file);
			// // cout<<"Hi done\n";
			return;
		}

		int pos=-1;
		if(newkey<l.keys[0])
			pos=0;
		else if(newkey>=l.keys[l.numKeys-1])
			pos=l.numKeys;
		else{
			for (int i = 0; i+1 < l.getNumKeys() ; ++i)
			{
				if( l.keys[i]<=newkey && newkey<l.keys[i+1]){
					pos=i+1;break;
				}
			}
		}
		assert(pos!=-1);
		l.keys.insert(l.keys.begin()+pos,newkey);
		l.values.insert(l.values.begin()+pos+1,valueFile);
		l.numKeys++;
		if(l.getNumKeys()-1<maxKeys){
			l.writeFile(l.file);
		}

		
		else if(l.getNumKeys()-1==maxKeys){
			string newNodeFile=createNodeFile();
			Node newNode(newNodeFile);
			int left,right;
			left=l.numKeys/2;
			right=l.numKeys-l.numKeys/2;
			double k=l.keys[left];
			vector<double> rightKeys(l.keys.begin()+left+1 , l.keys.end() );
			vector<double> leftKeys( l.keys.begin(),l.keys.begin()+left );
			vector<string> rightValues(l.values.begin()+left+1 , l.values.end());
			vector<string> leftValues( l.values.begin(),l.values.begin()+left+1 );
			newNode.keys=rightKeys;
			newNode.values=rightValues;
			l.keys=leftKeys;
			l.values=leftValues;
			l.numKeys=left;
			newNode.numKeys=right;
			
			// newNode.next=l.next;
			// l.next=leafFile;
			if(l.parent=="NULL"){
				string nodeFile=createNodeFile();
				Node new_interior(nodeFile);
				l.parent=newNode.parent=nodeFile;
				l.writeFile(l.file);
				newNode.values.push_back(l.file);
				newNode.writeFile(newNode.file);
				if(l.file==ROOT)
					ROOT=nodeFile;
				insert_into_node(nodeFile,newNode.keys[0],newNode.file);
			}

			else if(n.getNumKeys()<maxKeys){
				newNode.parent=l.parent;
				l.writeFile(l.file);
				newNode.writeFile(newNode.file);
				insert_into_node(newNode.parent,k,newNode.file);
			}
			else if(n.getNumKeys()==maxKeys){
				string nodeFile=createNodeFile();
				Node new_interior(nodeFile);
				pos=-1;
				if(k<n.keys[0])
					pos=0;
				else if(k>=n.keys[n.numKeys-1])
					pos=n.numKeys;
				else{
					for (int i = 0; i+1 < n.getNumKeys() ; ++i)
					{
						if( n.keys[i]<=k && k<n.keys[i+1]){
							pos=i+1;break;
						}
					}
				}
				assert(pos!=-1);
				n.keys.insert(n.keys.begin()+pos,k);
				n.values.insert(n.values.begin()+pos+1,newNode.file);
				n.numKeys++;

				int left=n.numKeys/2;
				// right=n.numKeys-n.numKeys/2;
				double middle_key=n.keys[left];
				vector<double> rightNodeKeys(n.keys.begin()+left+1 , n.keys.end() );
				vector<double> leftNodeKeys( n.keys.begin(),n.keys.begin()+left );
				vector<string> rightNodeValues(n.values.begin()+left+1 , n.values.end());
				vector<string> leftNodeValues( n.values.begin(),n.values.begin()+left+1 );

				new_interior.keys=rightNodeKeys;
				new_interior.values=rightNodeValues;
				n.keys=leftNodeKeys;
				n.values=leftNodeValues;
				
				new_interior.numKeys=new_interior.keys.size();
				n.numKeys=n.keys.size();
				if(l.keys[0]<middle_key)
					l.parent=n.file;
				else
					l.parent=new_interior.file;
				if(newNode.keys[0]<middle_key)
					newNode.parent=n.file;
				else
					newNode.parent=new_interior.file;
				l.writeFile(l.file);
				newNode.writeFile(newNode.file);
				new_interior.parent=n.parent;
				new_interior.writeFile(new_interior.file);
				n.writeFile(n.file);
				if(n.file==ROOT){
					string rootFile=createNodeFile();
					ROOT=rootFile;
					Node newRoot(rootFile);
					newRoot.values.push_back(n.file);
					newRoot.writeFile(newRoot.file);

					new_interior.parent=rootFile;
					n.parent=rootFile;
					new_interior.writeFile(new_interior.file);
					n.writeFile(n.file);
					insert_into_node(ROOT,middle_key,new_interior.file);
				}
				else{
					insert_into_node(n.parent,middle_key,new_interior.file);
				}
			}
		}		
	}
}


void bplustree::insert_into_tree(string root,double newkey,string value){
	string leaf=leafSearch(root,newkey);
	// cout<<"found leaf "<<leaf<<endl;
	insert_into_node(leaf,newkey,value);
	// cout<<"Inserted\n";
}

void bplustree::insert(double key,string value){
	insert_into_tree(ROOT,key,value);
	// cout<<"Completed Insertion\n";
}