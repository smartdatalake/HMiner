/* Copyright (C) 2019 Thanasis Vergoulis
 *
 * This file is part of GeSuTr. 
 *
 * GeSuTr is a free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GeSuTr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Contact email: vergoulis@athenarc.gr
 */

#include "ST.h"

ST::ST()
{
	this->_st_root=NULL;
}

ST::ST(string* strs, long strs_num)
{
	this->_st_root = new STnode(NULL,0,0,strs[0].length()-1); //create root node
	this->strInsertNaive(strs, strs_num); //insert all strings in the tree one-by-one
}

ST::~ST()
{
	//Destruct an object
    //cout << "tree deleted" << endl;
}

vector<NodeInfo*> ST::strInsertNaive(string str)
{
    vector<NodeInfo*> acc_nodes; // nodes that are accessed during the insertion

	//cout<<"- Inserting string '"<<str<<"' to the string registry..."<<endl;
	int str_id = this->strRegister(str); //register str in the registry of strings
	//cout<<"- String inserted..."<<endl;

	string suffix;

	if( this->_st_root == NULL) //empty tree - build from scratch
	{
		//Create and initialise the root node (no parent, no label)
		this->_st_root = new STnode(NULL,str_id,0,str.length()-1); 
	}

	for( long i=0; i<this->_strs[str_id].length(); i++ )
	{
		//cout<<"\t - Inserting suffix '"<<this->_strs[str_id].substr(i,this->_strs[str_id].length()-i)<<"' to the tree..."<<endl; //DEBUG
		this->insertSuffix(str_id,i, this->_strs[str_id].length()-1, acc_nodes); //insert the i-th suffix to the tree
		//cout<<"\t - Suffix inserted..."<<endl;
		//this->print(); //DEBUG
	}

	//this->print(); //DEBUG
	return acc_nodes;
}

int ST::strInsertNaive(string* strs, long str_num)
{
	for(long i=0; i<str_num; i++)
		this->strInsertNaive(strs[i]);
	return 0; 
}

int ST::insertSuffix(long str_id, long suf_start, long suf_end, vector<NodeInfo*> & acc_nodes)
{
	STnode* cur_node = this->_st_root;
	STnode* pre_node; //auxiliary variable for previous sibling node
	//STnode* init_node = this->_st_root;
	long chars_matched; 
	string suffix = this->_strs[str_id].substr(suf_start,suf_end-suf_start+1);

	//Traverse the tree based on the suffix & add nodes where needed
	long str_len = suffix.length(); //string length
	long chars_read = 0; //read characters
	string remaining_str = suffix;
	long label_len;

	STnode* new_node;

	while(chars_read!=str_len)
	{
		remaining_str = suffix.substr(chars_read,str_len-chars_read);
		//cout<<"To traverse based on '"<<remaining_str<<"'"<<endl; //DEBUG
		chars_matched = this->traverseNodeNaive(cur_node,remaining_str,pre_node);
		label_len = cur_node->getInLabelEnd()-cur_node->getInLabelStart()+1;

		//cout<<"Reached node with in label [str="<<cur_node->getRefStrId()<<", st="<<cur_node->getInLabelStart()<<", en="<<cur_node->getInLabelEnd()<<"] label size = "<<label_len<<endl; //DEBUG	
		//cout<<"--- matched: "<<chars_matched<<", str_len: "<<str_len<<", chars_read: "<<chars_read<<endl; //DEBUG

		if( chars_matched == 0 ) //no character matched - no children with proper label of incoming edge
		{	
			//cout<<"no match in the edge"<<endl; //DEBUG
			new_node = new STnode(cur_node,str_id,suf_start+chars_read+chars_matched,suf_end); //create new child 
			new_node->addOccPos(str_id,suf_start); //This is a leaf, so add its info.
			cur_node->addChild(new_node); //update parent

            //cout << "\t\t1) visiting node " << this->_strs[str_id].substr(suf_start, this->_strs[str_id].size() - suf_start) << endl;
            if (new_node->getInLabelEnd() - new_node->getInLabelStart() != 0) {
                NodeInfo *node_info = new NodeInfo(str_id, suf_start, this->_strs[str_id].size() - suf_start, new_node);
                acc_nodes.push_back(node_info);
            }

            break;
		}
		else if( chars_matched==label_len ) //whole edge was read
		{

			//cout<<"whole edge was read"<<endl; //DEBUG
			//cout<<"[current: "<<cur_node->getInLabelStart()<<","<<cur_node->getInLabelEnd()<<"]"<<endl; //DEBUG
			if( cur_node->isLeaf() )
			{
				cur_node->addOccPos(str_id,suf_start);
				//cout << suffix << " " << suf_start << " " << suf_end << " " << suffix.substr(suf_start, suf_end) << endl;

                //cout << "\t\t2.1) visiting node " << this->_strs[str_id].substr(suf_start, this->_strs[str_id].size() - suf_start) << endl;
                if (cur_node->getInLabelStart() - cur_node->getInLabelEnd() != 0) {
                    NodeInfo *node_info = new NodeInfo(str_id, suf_start, this->_strs[str_id].size() - suf_start, cur_node);
                    acc_nodes.push_back(node_info);
                }

            } else {
			    //cout << chars_read << " + " << chars_matched << endl;
                //cout << "\t\t2.2) visiting node " << this->_strs[str_id].substr(suf_start, chars_read + chars_matched) << endl;
                NodeInfo *node_info = new NodeInfo(str_id, suf_start, chars_read + chars_matched, cur_node);
                acc_nodes.push_back(node_info);
            }
		}
		else
		{	
			//cout<<"part of edge matches, chars_read="<<chars_read<<", chars_matched="<<chars_matched<<", suf_start="<<suf_start<<endl; //DEBUG
			//create new (intermediate) node (inital's node, new child)
			new_node = new STnode(cur_node->getParent(),cur_node->getRefStrId(),cur_node->getInLabelStart(),cur_node->getInLabelStart()+chars_matched-1);
			new_node->setRightSibling(cur_node->getRightSibling()); //the right sibling of old node, becomes the right of this
			if( pre_node != NULL ) 
				pre_node->setRightSibling(new_node); //if there is a sibling at the left, fix its right sibling pointer
			else
				cur_node->getParent()->setChildren(new_node); //if not, fix the pointer to the first child of the parent
			//cout<<"=> new intermediate node created: "<<new_node<<" [str: "<<new_node->getRefStrId()<<", st: "<<new_node->getInLabelStart()<<", en: "<<new_node->getInLabelEnd()<<", par: "<<new_node->getParent()<<"]"<<endl; //DEBUG

			//cout<<"\t\t3.1) visiting node "<< this->_strs[str_id].substr(suf_start, chars_read + chars_matched) << " (new node - after split - upper)" << endl;
            NodeInfo *node_info = new NodeInfo(str_id, suf_start, chars_read + chars_matched, new_node);
			acc_nodes.push_back(node_info);

            //update old child of the initial node, to be child of the new (intermediate) node
			cur_node->setInLabelStart(new_node->getInLabelEnd()+1);
			cur_node->setParent(new_node);
			new_node->setChildren(cur_node);
			//cout<<"=> old intermediate updated: "<<cur_node<<" [str: "<<cur_node->getRefStrId()<<", st: "<<cur_node->getInLabelStart()<<", en: "<<cur_node->getInLabelEnd()<<", par: "<<cur_node->getParent()<<"]"<<endl; //DEBUG

			//create new child of the new intermediate node (based on the remaining str)
			new_node = new STnode(cur_node->getParent(),str_id,suf_start+chars_read+chars_matched,suf_end);
			new_node->addOccPos(str_id,suf_start); //add new suffix occurrence
			cur_node->setRightSibling(new_node); //update the right sibling of old node to show the new child
			//cout<<"=> new child of new intermediate node created: "<<new_node<<" [str: "<<new_node->getRefStrId()<<", st: "<<new_node->getInLabelStart()<<", en: "<<new_node->getInLabelEnd()<<", par: "<<new_node->getParent()<<"]"<<endl; //DEBUG

			//cout << "\t\t3.2) visiting node " << this->_strs[str_id].substr(suf_start, suf_end - suf_start + 1) << " (new node - after split - lower)" << endl;
			if (new_node->getInLabelEnd() - new_node->getInLabelStart() != 0) {
			    NodeInfo *node_info = new NodeInfo(str_id, suf_start, suf_end - suf_start + 1, new_node);
                acc_nodes.push_back(node_info);
            }

            break;
		}
		chars_read += chars_matched; 
	}


	return 0;
}

int ST::strRegister(string str)
{
	this->_strs.push_back(str); //save str in the registry (vector) of strings
	return (this->_strs.size()-1); //return the index of str in the registry (vector) of strings
}

int ST::strRegister(string* strs, int strs_num)
{
	//register strings one-by-one
	for(int i=0; i<strs_num; i++)
		this->strRegister(strs[i]);
	return 0;
}

void ST::printRegistry()
{
	vector<string>::iterator cur_str;
	cout<<"== String registry =="<<endl;
	cur_str = this->_strs.begin();
	while( cur_str != this->_strs.end() )
	{
		cout<<(*cur_str)<<endl;
		cur_str++;
	}
	cout<<endl;
}

long ST::traverseNodeNaive( STnode*& node, string str)
{
	long chars_matched; //number of label matching characters between str's prefix and a proper edge
	long str_len, label_len; 

	STnode* next_node = findChildByStr(node,str[0]); //find child having label of incoming edge starting with str[0]

	if( next_node == NULL ) //no such child
	{
		return 0; //no characters found
	}

	chars_matched = 1; //1st character was matching
	node = next_node; //this is the node where the selected edge ends

	str_len = str.length();
	label_len = node->getInLabelLength();

	if( str_len < label_len)
		label_len = str_len;

	while( chars_matched<label_len )
	{
		if( getLabelChar(node,chars_matched)!=str[chars_matched] )
		{
			break;
		}
		chars_matched++; //prepare to read the next character (hope that it matches)
	}

	return chars_matched;
}

long ST::traverseNodeNaive( STnode*& node, string str, STnode*& pre_node)
{
	long chars_matched; //number of label matching characters between str's prefix and a proper edge
	long str_len, label_len; 

//	STnode* next_node = findChildByStr(node,str[0]); //find child having label of incoming edge starting with str[0]
	STnode* next_node = findChildByStr(node,str[0],pre_node); //find child having label of incoming edge starting with str[0]


	if( next_node == NULL ) //no such child
	{
		return 0; //no characters found
	}

	chars_matched = 1; //1st character was matching
	node = next_node; //this is the node where the selected edge ends

	str_len = str.length();
	label_len = node->getInLabelLength();

	if( str_len < label_len)
		label_len = str_len;

	while( chars_matched<label_len )
	{
		if( getLabelChar(node,chars_matched)!=str[chars_matched] )
		{
			break;
		}
		chars_matched++; //prepare to read the next character (hope that it matches)
	}

	return chars_matched;
}

STnode* ST::findChildByStr(STnode* node, char character)
{
	STnode* ch_node = node->getChildren();	//Point to the first child

	while(ch_node != NULL)
	{  
		if( this->getLabelChar(ch_node,0)== character )
			return ch_node;

		ch_node = ch_node->getRightSibling();	//Check the next sibling
	}

	return NULL; //no proper sibling found
}

STnode* ST::findChildByStr(STnode* node, char character, STnode*& pre_node)
{
	STnode* ch_node = node->getChildren();	//Point to the first child
	pre_node = NULL; 

	while(ch_node != NULL)
	{  
		if( this->getLabelChar(ch_node,0)== character )
			return ch_node;

		pre_node = ch_node; 
		ch_node = ch_node->getRightSibling();	//Check the next sibling
	}

	return NULL; //no proper sibling found
}

void ST::findStr(string str, vector<OccPos*>& occs)
{
	STnode* cur_node = this->_st_root; 
	long read_chr = 0; //the number of read characters
	long str_len = str.length(); //the initial length of the string
	
	while( str_len !=0 ) //while there is still part of the string unread
	{
		read_chr = traverseNodeNaive( cur_node, str); //traverse to the next level

		if( read_chr == 0 ) //nowhere to go
			return; //no recorded occurrence

		str_len -= read_chr;
		str = str.substr(read_chr,str_len);
	}

	cur_node->getSubtreeOccs(occs);
}

char ST::getLabelChar(STnode* node,long char_id)
{
	return this->_strs[node->getRefStrId()][node->getInLabelStart()+char_id];
}

string ST::getLabel(STnode* node)
{
	return this->_strs[node->getRefStrId()].substr(node->getInLabelStart(),node->getInLabelLength());
}

void ST::print(void)
{
	this->printNode( this->_st_root, 0);
}

void ST::printNode( STnode* src_node, long depth)
{
	STnode* node2 = src_node->getChildren();	//Get the first of the node's children
	long d = depth;
	long start = src_node->getInLabelStart();
	long end = src_node->getInLabelEnd();
	long str_id = src_node->getRefStrId();

	//If depth == 0 (root) then print nothing, just call STprintNode for the first child.
	if (depth == 0)
    {
	    cout << "[~] (" << src_node->getOccsNum() << ")" << " " << src_node << endl;
    }
	else
	{
		//Print the branches coming from higher nodes
		while( d>1)
		{
			cout<<"|";
			d--;
		}
		cout<<"+";
		
		cout<<this->_strs[str_id].substr(start,end-start+1)<<"[";
		src_node->printOccPos();
		cout<<"]"<< " (" << src_node->getOccsNum() << ")"<< " " << src_node << endl;
	}
	//Recursive call for all node1's children
	while( node2!=NULL)
	{
		printNode(node2, depth+1);
		node2 = node2->getRightSibling();
	}
}

pair<string, int> ST::getSubstrAndOccCount(STnode* src_node)
{
    vector<STnode*> path;

    // find all nodes up to the root
    while (src_node != this->_st_root) {
        path.insert(path.begin(), src_node);
        src_node = src_node->getParent();
    }

    if (path.size() == 3) {
        for(int i = 0; i< path.size(); i++) {
            cout << path[i] << endl;
        }
    }
    // construct full substring, based on the path from the root
    stringstream ss;

    for (unsigned int i = 0; i < path.size(); i++) {
        src_node = path[i];

        long start = src_node->getInLabelStart();
        long end = src_node->getInLabelEnd();
        long str_id = src_node->getRefStrId();

        ss << this->_strs[str_id].substr(start, end - start + 1);
    }

    return make_pair(ss.str(), src_node->getOccsNum());
}

void ST::updNodeCnts()
{
	this->updNodeCnt(this->_st_root);
}

void ST::updNodeCnt( STnode* cur_node)
{
	cur_node->updateSubtreeOccNum();
	cur_node = cur_node->getChildren();
	if( cur_node!= NULL )
	{
		this->updNodeCnt(cur_node); //get first child
	}
	while( cur_node!= NULL )
	{
		cur_node = cur_node->getRightSibling(); //get rest children
		if( cur_node == NULL )
			break;
		this->updNodeCnt(cur_node);
	}
}

string ST::getRegStr(long str_id)
{
	return this->_strs[str_id];
}

string ST::getRegSubStr(long str_id, long str_start, long str_len)
{
	return this->_strs[str_id].substr(str_start,str_len);
}
