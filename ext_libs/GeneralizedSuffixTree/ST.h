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

#ifndef ST_H
#define ST_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "STnode.h"
#include "NodeInfo.h"

using namespace std;

/**
 * The class represented the (generalised) suffix tree.  
 *
 * @var _strs A registry (vector) containing all strings to be inserted in the tree. 
 * @var _st_root Pointer to the root node of the tree. 
 *
 * @author Thanasis Vergoulis
 */
class ST
{
public:

	/**
	 * An empty class constructor. It just initialises the root to NULL.
	 *
	 * @author Thanasis Vergoulis
	 */
    ST();

	/** 
	 * The class constrcutor. It constracts the tree for a given set of strings.
	 *
	 * @param strs Array containing the strings to be included in the tree.
	 * @param strs_num The number of strings in strs. 
	 *
	 * @author Thanasis Vergoulis
	 */
	ST(string* strs, long strs_num);

	/**
	 * The class destructor. 
	 */
	~ST(); 			

	/**
	 * Inserts a given string in the suffix tree using the naive construction method (i.e., not Ukkonen's).
	 *
	 * @param str The string to be inserted. 
	 *
	 * @return vector of STnode pointers with the nodes that have been accessed during insertion.
	 *
	 * @author Thanasis Vergoulis
	 */
    vector<NodeInfo*> strInsertNaive(string str);

	/**
	 * Inserts one-by-one the strings contained in a given array to the tree. It calls strInsertNaive() multiple times. 
	 *
	 * @param strs The array of strings to be inserted.
	 * @param strs_num The number of strings in strs.
	 *
	 * @return It returns 0 on success. 
	 *
	 * @author Thanasis Vergoulis
	 */
	int strInsertNaive(string* strs, long strs_num);

	/**
	 * Inserts a suffix of the str_id-th string of the registry in the tree. 
	 *
	 * @param str_id The id of the string in the registry.
	 * @param suf_start The start position of the suffix in the string. 
	 * @param suf_end The end position of the suffix in the string. 
	 *
	 * @return It returns 0 on success. 
	 */
	int insertSuffix(long str_id, long suf_start, long suf_end, vector<NodeInfo*> & acc_nodes);

	/**
	 * Inserts a given string in the registry (vector) of strings. 
	 *
	 * @param str The string to be registered.
	 *
	 * @return The index of the str in the registry (vector).
	 *
	 * @author Thanasis Vergoulis
	 */
	int strRegister(string str);

	/**
	 * Batch insertion of given strings in the registry (vector) of strings.
	 *
	 * @param strs Array of strings to be registered. 
	 * @param strs_num The number of strings to be registered.
	 *
	 * @return Zero in case of success.
	 *
	 * @author Thanasis Vergoulis
	 */
	int strRegister(string* strs, int strs_num);

	/**
	 * Prints the contents of the string registry. 
	 *
	 * @author Thanasis Vergoulis
	 */
	void printRegistry(void);

	/**
	 * Use str to go down from a given (initial) node. After the execution, param 'node' (is accessed by 
	 * reference) will  point:
	 *    (a) to the initial node (if no outgoing edge matching a prefix of str was found), or
	 *    (b) to the end-node of the matching outgoing edge (the end-node is a child of the init node)
	 *
	 * @param node Pointer to the initial node (by reference).  
	 * @param str The string used for the traversal.
	 *
	 * @return The number of matching characters between the prefix of str and the matching edge (if any).
	 *
	 * @author Thanasis Vergoulis
	 */
	long traverseNodeNaive( STnode*& node, string str);

	/**
	 * Use str to go down from a given (initial) node. After the execution, param 'node' (is accessed by 
	 * reference) will  point:
	 *    (a) to the initial node (if no outgoing edge matching a prefix of str was found), or
	 *    (b) to the end-node of the matching outgoing edge (the end-node is a child of the init node)
	 * This is a variant of the basic function that also return the left sibling of the node.
	 *
	 * @param node Pointer to the initial node (by reference).  
	 * @param str The string used for the traversal.
	 * @param pre_node Pointer to the left sibling of the node (by reference). 
	 *
	 * @return The number of matching characters between the prefix of str and the matching edge (if any).
	 *
	 * @author Thanasis Vergoulis
	 */
	long traverseNodeNaive( STnode*& node, string str, STnode*& pre_node);

	/**
	 * Finds a child's node having label of incoming edge starting with a given character.
	 *
	 * @param node Pointer to the initial node. 
	 * @param character The given character. 
	 *
	 * @return Pointer to the child node that has the desired property. NULL pointer otherwise.  
	 *
	 * @author Thanasis Vergoulis
	 */
	STnode* findChildByStr(STnode* node, char character);

	/**
	 * Finds a child's node having label of incoming edge starting with a given character.
	 * This variant also returns (based on a parameter by reference) the previous child of 
	 * the node (i.e., the one to the left of the returned node). This needed for particular
	 * use in another place of the code. 
	 *
	 * @param node Pointer to the initial node. 
	 * @param character The given character. 
	 * @param pre_node The sibling at the left of the node (call by reference). 
	 *
	 * @return Pointer to the child node that has the desired property. NULL pointer otherwise.  
	 *
	 * @author Thanasis Vergoulis
	 */
	STnode* findChildByStr(STnode* node, char character, STnode*& pre_node);

	/**
	 * Returns a pointer to a set of positions in which a particular query string occurs (exact occurrence).
	 *
	 * @param str The query string.
	 * @param occs A vector containing all occurrences (call by reference).
	 *
	 * @author Thanasis Vergoulis 
	 */
	void findStr(string str, vector<OccPos*>& occs);

	/**
	 * Returns a particular character from the label of the incoming edge of a given node.
	 *
	 * @param node The node.
	 * @param char_id The id of the character (e.g., 1st, 2nd etc).
	 *
	 * @return The corresponding character.
	 *
	 * @author Thanasis Vergoulis
	 */
	char getLabelChar(STnode* node,long char_id);
	
	/**
	 * Returns the input edge's label of a given node.
	 *
	 * @param node The node.
	 *
	 * @return The label. 
	 *
	 * @author Thanasis Vergoulis
	 */
	string getLabel(STnode* node);

	/**
	 * Prints the suffix tree. 
	 *
	 * @author Thanasis Vergoulis
	 */
	void print();

	/**
	 * Supporting function to the print() function. It prints info about a node's children. 
	 *
	 * @param src_node The node for the children of which 
	 * @param depth The depth of the node. 
	 *
	 * @author Thanasis Vergoulis
	 */
	void printNode(STnode* src_node, long depth);

    /**
     * Supporting function to get substring of a node and its occurrences count
     *
     * @param src_node The pointer to the node
     */
	pair<string, int> getSubstrAndOccCount(STnode *src_node);

	/**
	 * Updates the value of _occ_num for all nodes of the tree.
	 *
	 * @author Thanasis Vergoulis
	 */
	void updNodeCnts();

	/**
	 * Updates the value of _occ_num for a given node.
	 *
	 * @param cur_node The node.
	 *
	 * @author Thanasis Vergoulis
	 */
	void updNodeCnt(STnode* cur_node);

	/**
	 * Returns the string in the registry having as identifier str_id. 
	 *
	 * @param str_id The identifier of the string in the registry. 
	 *
	 * @return The string. 
	 *
	 * @author Thanasis Vergoulis
	 */
	string getRegStr(long str_id);

	/**
	 * Returns a substring of a string in the registry. 
	 *
	 * @param str_id The identifier of the string in the registry.
	 * @param str_start The starting point of the substring.
	 * @param str_len The length of the substring.
	 *
	 * @return The substring. 
	 *
	 * @author Thanasis Vergoulis
	 */
	string getRegSubStr(long str_id, long str_start, long str_len);
private:
	//All variables are explained in the javadoc comments of the STnode class.
	vector<string> _strs; 
	STnode* _st_root;
};

#endif /* ST_H*/
