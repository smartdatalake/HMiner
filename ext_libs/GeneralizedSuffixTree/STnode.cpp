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

#include "STnode.h"

OccPos::OccPos(long str_id, long occ_pos, OccPos* next_pos)
{
	this->_str_id = str_id;
	this->_occ_pos = occ_pos;
	this->_next_pos = next_pos;
}

STnode::STnode(STnode* parent, long str_id, long start, long end)
{
	this->_str_id = str_id;

	this->_children = NULL; //no children
	this->_right_sibling = NULL; //no right sibling

	this->_parent = parent; //get the node's parent in the tree
	this->_in_label_start = start; //get the start of the incoming edge's label (based on str_id string)
	this->_in_label_end = end; //get the end of the incoming edge's label (based on str_id string)

	this->_occs_num = 0;
	this->_occ_positions = NULL;
}

STnode::~STnode(void)
{
    //cout << "node deleted" << endl;
}

void STnode::addChild(STnode* ch)
{
	if( this->_children == NULL) //this will be the first child
	{
		this->_children = ch;
	}
	else
	{
		STnode* cur_child = this->_children;
		while( cur_child->_right_sibling != NULL ) //browse until you find one child with no right siblings (i.e., the last one)
		{
			cur_child = cur_child->_right_sibling;
		}
		cur_child->_right_sibling = ch; //add the child, as the rightmost
	}
}

bool STnode::isLeaf()
{
	if( this->_occ_positions==NULL )
		return false;
	else
		return true;
}

STnode* STnode::getParent(void)
{
	return this->_parent;
}

void STnode::setParent(STnode* parent)
{
	this->_parent = parent;
}

STnode* STnode::getChildren()
{
	return this->_children;
}

void STnode::setChildren(STnode* ch)
{
	_children = ch;
}

STnode* STnode::getRightSibling(void)
{
	return _right_sibling;
}

void STnode::setRightSibling(STnode* right_sib)
{
	_right_sibling = right_sib;
}

long STnode::getInLabelStart()
{
	return this->_in_label_start;
}

void STnode::setInLabelStart(long start)
{
	this->_in_label_start = start; 
}

long STnode::getInLabelEnd()
{
	return this->_in_label_end;
}

void STnode::setInLabelEnd(long end)
{
	this->_in_label_end = end;
}

long STnode::getRefStrId()
{
	return this->_str_id;
}

long STnode::getInLabelLength()
{
	return (this->_in_label_end - this->_in_label_start + 1);
}

void STnode::addOccPos(long str_id, long pos)
{
	OccPos* p = new OccPos(str_id, pos, NULL);
	OccPos* cur_p = this->_occ_positions;
	if( cur_p == NULL )
	{
		this->_occ_positions = p;
	}
	else
	{ 
		while( cur_p->_next_pos!=NULL )
		{
			cur_p = cur_p->_next_pos;
		}
		cur_p->_next_pos = p;
	}
}

void STnode::setOccPos(OccPos* p)
{
	this->_occ_positions = p;
}

OccPos* STnode::getOccPos()
{
	return this->_occ_positions;
}

long STnode::getOccPosNum()
{
	OccPos* cur_p = this->_occ_positions;
	long pos_num=0;
	while( cur_p!=NULL )
	{
		pos_num++;
		cur_p = cur_p->_next_pos;
	}
	return pos_num;
}

void STnode::printOccPos()
{
	OccPos* cur_p = this->_occ_positions;
	while( cur_p!=NULL )
	{
		cout<<"("<<cur_p->_str_id<<","<<cur_p->_occ_pos<<") ";
		cur_p = cur_p->_next_pos;
	}
}

void STnode::getSubtreeLeaves(vector<STnode*>& leaves)
{
	STnode* ch_node = this->getChildren();	//Point to the first child

	if( ch_node == NULL ) //this is a leaf
		leaves.push_back(this);

	while(ch_node != NULL) 
	{  
		if( ch_node->isLeaf() )
			leaves.push_back(ch_node); //record this leaf
		else
			ch_node->getSubtreeLeaves(leaves); //find this node's leaves

		ch_node = ch_node->getRightSibling();	//Check the next sibling
	}
}

void STnode::getSubtreeOccs(vector<OccPos*>& occs)
{
	STnode* ch_node = this->getChildren();	//Point to the first child
	OccPos* cur_p;

	if( ch_node == NULL ) //this is a leaf
    {
        // add all starting positions to occs
        OccPos* cur_p = this->_occ_positions;

        while( cur_p != NULL )
        {
            occs.push_back(cur_p);
            cur_p = cur_p->_next_pos;
        }
    }


	while(ch_node != NULL) 
	{  
		if( ch_node->isLeaf() )
		{
			cur_p = ch_node->_occ_positions;
			while( cur_p!=NULL )
			{
				occs.push_back(cur_p);
				cur_p = cur_p->_next_pos;
			}
		}
		else
		{
			ch_node->getSubtreeOccs(occs); //find this node's leaves
		}

		ch_node = ch_node->getRightSibling();	//Check the next sibling
	}
}

void STnode::updateSubtreeOccNum()
{
	vector<OccPos*> occs;
	this->getSubtreeOccs(occs);
	this->_occs_num = occs.size();
	//cout << this->_occs_num << endl;
}

long STnode::getOccsNum() const {
    return _occs_num;
}

void STnode::setCachedResult(TransitionMatrix *cachedResult) {
    _cached_result = cachedResult;
}

TransitionMatrix *STnode::getCachedResult() const {
    return _cached_result;
}
