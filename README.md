### Metapath-Based Pattern Mining in HINs

Many mining algorithms in HINs have a common computationally intensive step, that consists of finding the pairs of nodes connected given a particular metapath.
This repository provides open-source implementations of the `Sequential` and `Dynamic Programming` matrix multiplication strategies proposed in [1] for this problem.

#### Input File Formats

Input files can be one of the following types:

* **File containing node attributes.** These are tab-separated files containing all node attributes. The first line is the header that contains all attribute names with `_n` and `_s` suffixes for numeric and alphanumeric attributes respectively.
The first column should be an incremental integer identifier starting from 1, denoted as "id_n" in the header. These files should be named with the first letter of the entity they are representing. For example, the file that contains the attributes for node type `Author` should be named `A.csv`. An example of a file containing node attributes is the following: 

```
id_n	name_s    surname_s
0	Makoto  Satoh
1	Ryo Muramatsu
...
```

* **File containing relations.** These are tab-separated files needed to construct the relations among nodes. These files contain two columns, the source and target identidiers respectively and should be sorted based on the first column and named with the extension `.crs`. They do not contain a header and they should be named according to the source and target node types. For example, the file with the relations between node types `Author` and `Paper` should be named `AP.crs`. An example of file containing node relations is the following: 

```
0	1
0	2
0	3
0	5
1	0
...
```

##### Example Input Files

All methods use the same format of input files. Examples of input files can be found in the `./data/` folder. Two sample datasets are included: (a) a subset of the GDELT dataset  `./data/GDELT/`, (b) a subset of the DBLP dataset from AMiner (folder `./data/DBLP_subset/`).

##### Example Query File

An example query can be found in `data/DBLP_subset/query.txt`

#### Clone & Build

```
# clone repository
git clone --recursive https://github.com/smartdatalake/HMiner.git

# update submodules
git submodule update --remote

# build project
make
```

#### Configuration 
The program recieves parameters from a json config file. An example config file is the following: 
```
{
    "indir": "./data/DBLP_subset/nodes/",
    "irdir": "./data/DBLP_subset/relations/",
    "algorithm": "OTree",
    "hin_out": "./data/out/",
    "qf": "./data/DBLP_subset/queries.txt",
    "cache_size": 4096,
    "cache_policy": "PGDS",
    "dynamic_optimizer": "Sparse",
    "ranking": {
        "ranking_out": "./data/out/",
        "pr_alpha": 0.5,
        "pr_tol": 0.00000000001,
        "threshold": 2
    }
}
```

#### Query Workload 
Thq query workload has the following format: 
```
{ "metapath": "APPA", "constraints": { "P": "year >= 2000" } }
{ "metapath": "APPAP", "constraints": { "P": "year >= 2000" }, "src_field": "name" }}
```
Fields "metapath" and "constraints" are required. 
The field "src_field" is optional and is used to append the specified column in the output in the case of ranking.

#### Execution

Execute ranking with the following command: 
```
./run -c config.json
```

#### References
[1] Y. P. S. W. B. Shi Chuan, Li Yitong. Constrained-meta-path-based ranking in heterogeneous information network. Knowledge and Information Systems, 2016
