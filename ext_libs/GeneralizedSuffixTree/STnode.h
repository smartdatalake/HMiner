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

#ifndef STNODE_H
#define STNODE_H

#include <iostream>
#include <vector>
#include "../../TransitionMatrix/TransitionMatrix.h"
using namespace std;

/**
 * Class representing a string occurrence in a leaf's list. It contains a pointer to the next item to
 * implement the leaf's list of occurrences. 
 *
 * @var _str_id The id of the string on which the occurrence is located.
 * @var _occ_pos The position on the string that the occurrence appears.
 * @var _next_pos Pointer to the next occurrence.
 *
 * @author Thanasis Vergoulis
 */
class OccPos
{
public:
	long _str_id;
	long _occ_pos;
	OccPos* _next_pos;
	/**
	 * The constructor of the class. 
	 *
	 * @param str_id The id of the string on which the occurrence exists.
	 * @param occ_pos The exact position of the occurrence on the string. 
	 * @param next_pos Pointer to the next occurrence of the same string/suffix.
	 *
	 * @author Thanasis Vergoulis
	 */
	OccPos(long str_id, long occ_pos, OccPos* next_pos);
};

/**
 * Class representing a (generalised) suffix tree node (internal or leaf). 
 *
 * @var _str_id The id of the string used for the label of the incoming edge. 
 * @var _parent A pointer to the parent of the node. 
 * @var _children A linked list of the children of the node. In fact its is a link to the first (leftmost) child. 
 * @var _right_sibling A pointer to the right sibling of the node.
 * @var _in_label_start Start index of the label of the incoming edge.
 * @var _in_label_end End index of the label of the incoming edge. 
 * @var _occ_positions If leaf this is pointer to all starting positions when the corresponding string/suffix occurs. If not leaf, it is null. 
 * @var _occs_num The number of occurrences in the subtree below the node. 
 * @var _cached_result Pointer to an intermediate cached result
 *
 * @author Thanasis Vergoulis
 */
class STnode
{
public:
	/**
	 * The constructor of the class. 
	 *
	 * @param parent Pointer to the parent node. 
	 * @param str_id The id of the string (in the string registry-vector) used for the label of the incoming edge.
	 * @param start The start index of the label of the incoming edge.
	 * @param end The end index of the label of the incoming edge. 
	 *
	 * @author Thanasis Vergoulis
	 */
	STnode(STnode* parent, long str_id, long start, long end);

	/**
	 * The destructor of the class.
	 *
	 * @author Thanasis Vergoulis
	 */
	~STnode();

	/**
	 * It adds a new child to the node. 
	 *
	 * @param ch The pointer to the new child.
	 *
	 * @author Thanasis Vergoulis
	 */
	void addChild(STnode* ch);

	/**
	 * It returns true if the node is a leaf, false otherwise.
	 *
	 * @return True if the node is leaf, false otherwise. 
	 * 
	 * @author Thanasis Vergoulis
	 */
	bool isLeaf();

	/**
	 * It returns a pointer to the node's parent. 
	 *
	 * @return A pointer to the node's parent. 
	 *
	 * @author Thanasis Vergoulis
	 */
	STnode* getParent();

	/**
	 * It sets the pointer to the node's parent.
	 *
	 * @param The pointer to the ndoe's parent (to be).
	 *
	 * @author Thanasis Vergoulis
	 */
	void setParent(STnode* parent);

	/**
	 * It returns a pointer to the first (leftmost) child of the node. Since each child has a pointer
	 * to the next one, in fact this function can be used to get the linked list of a node's children.
	 *
	 * @return A pointer to the node's leftmost child. 
	 *
	 * @author Thanasis Vergoulis
	 */
	STnode* getChildren();			

	/**
	 * It sets the pointer to the node's leftmost child. 
	 *
	 * @param ch The pointer to the node's leftmost child (to be).
	 *
	 * @author Thanasis Vergoulis
	 */
	void setChildren(STnode* ch);		

	/**
	 * It returns a pointer to the node's right sibling.
	 * 
	 * @return A pointer to the node's right sibling. 
	 *
	 * @author Thanasis Vergoulis
	 */
	STnode* getRightSibling();

	/**
	 * It sets the pointer to the node's right sibling. 
	 *
	 * @param The pointer to the node's right siblimng (to be).
	 *
	 * @author Thanasis Vergoulis 
	 */
	void setRightSibling(STnode* right_sib);

	/**
	 * It returns the starting position of the label of the incoming edge.
	 *
	 * @return The starting position of the label of the incoming edge.
	 *
	 * @author Thanasis Vergoulis
	 */	
	long getInLabelStart();

	/**
	 * It sets the starting positions of the label of the incoming edge.
	 *
	 * @param The starting position of the label of the incoming edge (to be).
	 *
	 * @author Thanasis Vergoulis
	 */
	void setInLabelStart(long start);

	/**
	 * It returns the ending position of the label of the incoming edge.
	 *
	 * @return The ending position of the label of the incoming edge.
	 *
	 * @author Thanasis Vergoulis
	 */	
	long getInLabelEnd();

	/**
	 * It returns the ending position of the label of the incoming edge.
	 *
	 * @return The ending position of the label of the incoming edge.
	 *
	 * @author Thanasis Vergoulis
	 */	
	void setInLabelEnd(long end);	

	/**
	 * It returns the id of the string (in the registry-vector) used for the label of the incoming edge.
	 *
	 * @return The id of the string used for the label of the incoming edge.
	 *
	 * @author Thanasis Vergoulis
	 */
	long getRefStrId();

	/**
	 * It returns the length of the label of the incoming edge.
	 *
	 * @return The length of the label of the incoming edge. 
	 *
	 * @author Thanasis Vergoulis
	 */
	long getInLabelLength();

	/**
	 * Adds an extra string occurrence to the list of occurrences in the leaf.
	 *
	 * @param str_id The id of the string (in the registry) on which the occurrence resides. 
	 * @param p The position of the occurrence to be added. 
	 *
	 * @author Thanasis Vergoulis
	 */
	void addOccPos(long str_id, long p);

	/**
	 * Re-sets the list of string occurrences by re-setting the first occurrence (recall that the 
	 * occurrences are a linked list). 
	 * 
	 * @param p Pointer to the new position of occurrence. 
	 *
	 * @author Thanasis Vergoulis
	 */
	void setOccPos(OccPos* p);

	/**
	 * Get a pointer to the first of the occurrences (and, thus, a way to access the whole occurrences list).
	 *
	 * @return The pointer to the first of the leafs occurrences. If not leaf then NULL. 
	 *
	 * @author Thanasis Vergoulis
	 */
	OccPos* getOccPos();

	/**
	 * Returns the number of occurrences in the list of the current leaf. 
	 *
	 * @return The number of occurrences.
	 *
	 * @author Thanasis Vergoulis
	 */
	long getOccPosNum();

	/**
	 * Prints the list of occurrences of the leaf. 
	 *
	 * @author Thanasis Vergoulis
	 */
	void printOccPos();

	/**
	 * Gets all leaves in the subtree of the node. 
	 *
	 * @param leaves A vector containing pointers to the leaves (call by reference). Attention! Input 
	 * vector has to be empty. 
	 *
	 * @author Thanasis Vergoulis
	 */
	void getSubtreeLeaves(vector<STnode*>& leaves); 

	/**
	 * Gets all occurrences in the subtree of the node.
	 *
	 * @param occs A vector containing pointers to the occurrences (call by reference). Attention! Inpit 
	 * vector has to be empty.
	 *
	 * @author Thanasis Vergoulis
	 */
	void getSubtreeOccs(vector<OccPos*>& occs);

	/**
	 * Updates the value of _occs_num of the current node.
	 *
	 * @author Thanasis Vergoulis
	 */
	void updateSubtreeOccNum();

	/**
	 * Get the number of occurrences in the subtree below the node
	 * @return the _occs_num variable
	 */
    long getOccsNum() const;

    /**
     * Setter of the intermediate cached result of the node
     * @param cachedResult the result to be cached
     */
    void setCachedResult(TransitionMatrix *cachedResult);

    /**
     * Getter of the intermediate cached result of the node
     * @return the cached result (if any) or nullptr
     */
    TransitionMatrix *getCachedResult() const;

private:
	//All variables are explained in the javadoc comments of the STnode class.
	long _str_id;
	STnode* _parent;
	STnode* _children;
	STnode* _right_sibling;
	long _in_label_start;
	long _in_label_end;
	OccPos* _occ_positions; 
	long _occs_num;

	TransitionMatrix* _cached_result { nullptr };
};
#endif /* STNODE_H*/
