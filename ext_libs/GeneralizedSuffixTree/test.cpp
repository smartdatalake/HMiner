/* 
 * This is just a toy example to showcase the use of GeSuTr library. 
 *
 * Copyright (C) 2019 Thanasis Vergoulis
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
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include "ST.h"
using namespace std;


int main(int argc, char* argv[])
{
	string* strs = new string[2];
	strs[0] = "banana$";
	strs[1] = "moana$";

	//create the generalised suffix tree
	ST* my_tree;
	my_tree = new ST(strs,2);

	//use the generalised suffix tree to find exact occurrences (matches) of a string
	vector<OccPos*> str_occs;
	my_tree->findStr("ana",str_occs);

	//print the results
	cout<<endl;
	cout<<"Found "<<str_occs.size()<<" matches."<<endl;
	vector<OccPos*>::iterator cur_occ;
	cur_occ = str_occs.begin();
	while(cur_occ!=str_occs.end())
	{
		cout<<"(str_id: "<<(*cur_occ)->_str_id<<", pos: "<<(*cur_occ)->_occ_pos<<")"<<endl;
		cur_occ++;
	}

	//test occ counters for each node (if needed)
	my_tree->updNodeCnts();

	return 0;
}