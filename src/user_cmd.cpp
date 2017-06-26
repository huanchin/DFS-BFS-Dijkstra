// **************************************************************************
// File       [ test_cmd.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2012/04/10 created ]
// **************************************************************************

#include "user_cmd.h"
#include "graph.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <queue>
#include "../lib/tm_usage.h"
using namespace std;
using namespace CommonNs;

Graph *mygraph = 0;

void BFS(int s , ofstream &output, int &count);
void DFS_VISIT(Node *s, int &time, ofstream &output,int &count);
void DFS(int s, ofstream &output,int &count);
void Dijkstra(int s, int t, ofstream & output,int &count, int &tw);
void   Relax(Node *u, Node *v, Edge * edge);
Node *Extract_Min(vector<Node *>& Q);
void Print_Path( Node *v, ofstream & output , int &count, int &tw);
size_t myStrGetTok(const string& str, string& tok, size_t pos = 0, const char del = ' ');
bool myStrtoInt(const string& str, int& num);

void BFS(int s , ofstream &output ,int &count){

	

	for (int i= 0 ; i < mygraph->nodes.size() ; i++){
		
	mygraph-> getNodeById(i)-> color = 0;
	mygraph-> getNodeById(i)-> d = DIS_INF;
	mygraph-> getNodeById(i)-> prev = 0;

	}
	

	mygraph->getNodeById(s)-> color = 1;
	mygraph->getNodeById(s)-> d =0;
	mygraph->getNodeById(s)-> prev = 0;




	queue <Node *> Q ;
	Q.push(mygraph->getNodeById(s));

	while(Q.size()!=0)
	{
		Node *u = Q.front();
		Q.pop();

		for(int i= 0 ; i <  u->edge.size() ; i++){
			Node *v = u -> edge[i] -> getNeighbor(u);
			if(v -> color == 0)
			{
			    v -> color = 1;
			    v -> d = (u -> d) + 1;
			    v -> prev = u;
			    Q.push(v);
				
			    output <<"  v"<< u->id <<" -- v" << v->id << "  [label = " << "\"" << u->edge[i]->weight << "\"" << "];" << endl ;
				count++;
			}
		}
		
		
		u -> color = 2;
	}		
	output << "}" <<endl;
}

void DFS(int s, ofstream &output,int &count){

for (int i= 0 ; i < mygraph->nodes.size() ; i++){
		
	mygraph-> getNodeById(i)-> color = 0;
	mygraph-> getNodeById(i)-> d = DIS_INF;
	mygraph-> getNodeById(i)-> prev = 0;

	}
	cout<< s;
	int time = 0;
		
	Node *u = mygraph-> getNodeById(s);
	DFS_VISIT(u, time, output, count);
	
	output << "}" <<endl;

}

void DFS_VISIT(Node *u, int &time, ofstream &output, int &count){
	
	
	time = time + 1;
	u-> d = time ;
	u-> color = 1;
	
	for(int i= 0 ; i <  u->edge.size() ; i++){
		Node *v = u -> edge[i] -> getNeighbor(u);
		if(v -> color == 0){
			v -> prev = u;
			output <<"  v"<< u->id <<" -- v" << v->id << "  [label = " << "\"" << u->edge[i]->weight << "\"" << "];" << endl ;
			count++;
			DFS_VISIT(v, time, output,count);
		}
	u -> color = 2;	
	u -> traveled = true;
	
	}
	
	
}

size_t myStrGetTok(const string& str, string& tok, size_t pos, const char del)
{
   size_t begin = str.find_first_not_of(del, pos);
   if (begin == string::npos) { tok = ""; return begin;}
   size_t end = str.find_first_of(del, begin);
   tok = str.substr(begin, end - begin);
   return end;
}

bool myStrtoInt(const string& str, int& num)
{
   num = 0;
   size_t i = 0;
   int sign = 1;
   if (str[0] == '-') { sign = -1; i = 1; }
   bool valid = false;
   for (; i < str.size(); ++i) {
      if (isdigit(str[i])) {
         num *= 10;
         num += int(str[i] - '0');
         valid = true;
      }
      else return false;
   }
   num *= sign;
   return valid;
}

///////////////////////////////TestCmd///////////////////////////////////////////////////////////////////

TestCmd::TestCmd(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("test");
    optMgr_.setDes("test");

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ, "print the string of -s", "[string]");
    opt->addFlag("s");
    optMgr_.regOpt(opt);
}

TestCmd::~TestCmd() {}

bool TestCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if (optMgr_.getParsedOpt("s")) {
        printf("%s\n", optMgr_.getParsedValue("s"));
    }
    else
        printf("hello world !!\n");


    return true;
}
///////////////////////////////ReadGraphCmd////////////////////////////////////////////////////////

ReadGraphCmd::ReadGraphCmd(const char * const name) : Cmd(name) {

    optMgr_.setShortDes("run commands from startup file");
    optMgr_.setDes("runs commands from FILE");
    optMgr_.regArg(new Arg(Arg::REQ, "target file with commands", "FILE"));

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
}

ReadGraphCmd::~ReadGraphCmd() {}

bool ReadGraphCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);
    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if (argc < 2) {
        fprintf(stderr, "**ERROR ReadGraphCmd::exec(): ");
        fprintf(stderr, "please specify source file\n");
        return false;
    }

    FILE *finle = fopen(argv[1], "r");
    if (!finle) {
        fprintf(stderr, "**ERROR ReadGraphCmd::exec(): ");
        fprintf(stderr, "file cannot be opened\n");
        return false;
    }
	
	
	string line;
	int v1 = 0;
	int v2 = 0;
	int edge = 0;
	size_t num = 0;
	ifstream fin(argv[1]);
	getline(fin, line);
	getline(fin, line);
	string graphname;
	num=myStrGetTok(line,graphname,6);
	//cout<<"graphname is "<<graphname<<endl;
	mygraph = new Graph(graphname);
	
	while(getline(fin, line)){
		if(line[0]=='}')
			break;
		//cout<<line<<endl;
		string gar;
		num=line.find_first_of("0123456789");
		
		string tok1,tok2,tok3,tokr;
		num=myStrGetTok(line,tok1,num);
		num=myStrGetTok(line,gar,num);
		num=myStrGetTok(line,tok2,num+2);
		num=myStrGetTok(line,gar,num);
		num=myStrGetTok(line,gar,num);
		num=myStrGetTok(line,tok3,num);
		num=myStrGetTok(tok3,tokr,1,'\"');
		myStrtoInt(tok1,v1);
		myStrtoInt(tok2,v2);
		myStrtoInt(tokr,edge);

		mygraph->addEdge(v1,v2,edge);//add edge
		//cout<<"v1 "<<v1<<" v2 "<<v2<<" edge  "<<edge<<endl;
	 }
    return true;
}

////////////////////////////////WriteTreeBFSCmd////////////////////////////////////////////////////////////

WriteTreeBFSCmd::WriteTreeBFSCmd (const char * const name) : Cmd(name) {
    optMgr_.setShortDes("create bfs tree");
    optMgr_.setDes("implement bfs to the graph and output a dot file");

    optMgr_.regArg(new Arg(Arg::REQ, "input dot file", "INPUT"));

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "output format. Default is dot",
                  "dot");
    opt->addFlag("s");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "output file. Default is <input>_bfs.dot",
                  "OUTPUT");
    opt->addFlag("o");
    optMgr_.regOpt(opt);
}

WriteTreeBFSCmd::~WriteTreeBFSCmd() {}

bool WriteTreeBFSCmd::exec(int argc, char **argv) {
	CommonNs::TmUsage tmusg;
	CommonNs::TmStat stat;
	tmusg.periodStart();
	optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }


	if ((argc!=3)&&(argc!=5))
	  cout << "**ERROR WriteTreeBFSCmd::exec(): wrong format\n" ;

	
	if (optMgr_.getParsedOpt("s"))
	{
        string fname;
		
		if ((argc == 5)&&(optMgr_.getParsedOpt("o")))
		{	fname = optMgr_.getParsedValue("o");	}
		else
		{	fname = optMgr_.getParsedValue("s");
			fname += "_bfs.dot";	}
		
		string source;
		int s;
		source = optMgr_.getParsedValue("s");
		myStrGetTok(source, source, 1);
		myStrtoInt(source,s);
		ofstream output(fname.c_str());	
		output << "graph gn" << s << "_bfs {" <<endl;
		int count = 0;
		if (mygraph != 0)
		{
			BFS(s, output,count);
		}
		tmusg.getPeriodUsage(stat);
		cout <<"# run time = " << (stat.uTime + stat.sTime) / 1000000.0 << " sec" << endl;
    		cout <<"# memory =" << stat.vmPeak / 1000.0 << " MB" << endl;
		output<<"// vertices = "<<mygraph -> nodes.size()<<endl
				<<"// edges = "<<count<<endl
				<<"// source = v"<< s <<endl
				<<"// runtime = "<<(stat.uTime + stat.sTime) / 1000000.0 <<" sec"<<endl
				<<"// memory = "<<stat.vmPeak / 1000.0<<" MB";

	}
	else 
	
	cout<< "**ERROR WriteTreeBFSCmd::exec(): need input file\n" ;	
    
}

/////////////////////////////////WriteTreeDFSCmd///////////////////////////////////////////////////////////////////////

WriteTreeDFSCmd::WriteTreeDFSCmd (const char * const name) : Cmd(name) {


    optMgr_.setShortDes("create dfs tree");
    optMgr_.setDes("implement dfs to the graph and output a dot file");

    optMgr_.regArg(new Arg(Arg::REQ, "input dot file", "INPUT"));

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "output format. Default is dot",
                  "dot");
    opt->addFlag("s");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "output file. Default is <input>_dfs.dot",
                  "OUTPUT");
    opt->addFlag("o");
    optMgr_.regOpt(opt);
}

WriteTreeDFSCmd::~WriteTreeDFSCmd() {}

bool WriteTreeDFSCmd::exec(int argc, char **argv) {
	CommonNs::TmUsage tmusg;
	CommonNs::TmStat stat;
	tmusg.periodStart();
	optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }


	if ((argc!=3)&&(argc!=5))
	  cout << "**ERROR WriteTreeDFSCmd::exec(): wrong format\n" ;

	
	if (optMgr_.getParsedOpt("s"))
	{
        string fname;
		
		if ((argc == 5)&&(optMgr_.getParsedOpt("o")))
		{	fname = optMgr_.getParsedValue("o");	}
		else
		{	fname = optMgr_.getParsedValue("s");
			fname += "_dfs.dot";	}
		
		string source;
		int s;
		source = optMgr_.getParsedValue("s");
		myStrGetTok(source, source, 1);
		myStrtoInt(source,s);
		
		ofstream output(fname.c_str());	
		output << "graph gn" << s << "_dfs {" <<endl;
		int count = 0;
		if (mygraph != 0)
		{

			DFS(s, output,count);
		}
		tmusg.getPeriodUsage(stat);
	    	cout <<"# run time = " << (stat.uTime + stat.sTime) / 1000000.0 << " sec" << endl;
    		cout <<"# memory =" << stat.vmPeak / 1000.0 << " MB" << endl;
		output<<"// vertices = "<<mygraph -> nodes.size()<<endl
				<<"// edges = "<<count<<endl
				<<"// source = v"<< s <<endl
				<<"// runtime = "<<(stat.uTime + stat.sTime) / 1000000.0 <<" sec"<<endl
				<<"// memory = "<<stat.vmPeak / 1000.0<<" MB";
	}
	else 
	
	cout<< "**ERROR WriteTreeDFSCmd::exec(): need input file\n" ;	
    
}


//////////////////////////DijkstraCmd////////////////////////////////////////////////////////////////////


DijkstraCmd::DijkstraCmd (const char * const name) : Cmd(name) {
    optMgr_.setShortDes("create dfs tree");
    optMgr_.setDes("implement dfs to the graph and output a dot file");

    optMgr_.regArg(new Arg(Arg::REQ, "input dot file", "INPUT"));

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "output format. Default is dot",
                  "dot");
    opt->addFlag("o");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "output file. Default is <input>_dfs.dot",
                  "OUTPUT");
    opt->addFlag("from");
    optMgr_.regOpt(opt);
    opt = new Opt(Opt::STR_REQ,
                  "output file. Default is <input>_dfs.dot",
                  "OUTPUT");
    opt->addFlag("to");
    optMgr_.regOpt(opt);
}

DijkstraCmd::~DijkstraCmd() {}

bool DijkstraCmd::exec(int argc, char **argv) {
	CommonNs::TmUsage tmusg;
	CommonNs::TmStat stat;
	tmusg.periodStart();
	optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }


	if ((argc!=7)&&(argc != 5))
	  cout << "**ERROR WriteTreeDFSCmd::exec(): wrong format\n" ;

	
	if (optMgr_.getParsedOpt("from"))
	{
          if (optMgr_.getParsedOpt("to")){
	
		string fname;
		
		if ((argc == 7)&&(optMgr_.getParsedOpt("o")))
		{	fname = optMgr_.getParsedValue("o");	}
		else
		{	fname = "default";
			fname += "_dijk.dot";	}
		
		string source;
		int s;
		source = optMgr_.getParsedValue("from");
		myStrGetTok(source, source, 1);
		myStrtoInt(source,s);

		string terminal;
		int t;
		terminal = optMgr_.getParsedValue("to");
		myStrGetTok(terminal, terminal, 1);
		myStrtoInt(terminal,t);
		
		ofstream output(fname.c_str());	
		output << "graph gn" << s << "_" << t << "_dijk {" <<endl;
		int count = 0;
		int tw = 0;
		if (mygraph != 0)
		{
			Dijkstra(s, t, output, count, tw);
			output<<"}"<<endl;
		}
		tmusg.getPeriodUsage(stat);
		cout <<"# run time = " << (stat.uTime + stat.sTime) / 1000000.0 << " sec" << endl;
 		cout <<"# memory =" << stat.vmPeak / 1000.0 << " MB" << endl;
		output<<"// vertices = "<<count+1<<endl
				<<"// edges = "<<count<<endl
				<<"// source = v"<< s <<endl
				<<"// end = v"<< t <<endl
				<<"// total weight = "<< tw <<endl
				<<"// runtime = "<<(stat.uTime + stat.sTime) / 1000000.0 <<" sec"<<endl
				<<"// memory = "<<stat.vmPeak / 1000.0<<" MB";
	   }
		

	}
	else 
	
	cout<< "**ERROR WriteTreeDFSCmd::exec(): need both sourcenode and endnode\n" ;	
    
}

///////////////////////////////////////////////////////////////////////////////////////////

void Dijkstra(int s, int t, ofstream & output, int &count, int &tw){

	for (int i= 0 ; i < mygraph->nodes.size() ; i++){
		
	mygraph-> getNodeById(i)-> d = DIS_INF;
	mygraph-> getNodeById(i)-> prev = 0;

	}
	
	mygraph->getNodeById(s)-> d =0;
	mygraph->getNodeById(s)-> prev = 0;

	vector <Node *> Q;
	
	for (int i= 0 ; i < mygraph->nodes.size() ; i++){
	Q.push_back(mygraph -> getNodeById(i)); }
	
	while (Q.size()!=0){
	   Node *u = Extract_Min(Q);
	   for(int i= 0 ; i < u->edge.size() ; i++){

		Node *v = u -> edge[i] -> getNeighbor(u);
		Relax(u, v, u -> edge[i]);

	   }
	}
	
	Print_Path(mygraph -> getNodeById(t), output,count, tw);


}

void   Relax(Node *u, Node *v, Edge * edge){

	if( v->d > (u->d + edge->weight) ){
	   (v->d) = (u->d) + (edge->weight);
	    v-> prev = u;	
	}
}

Node * Extract_Min(vector<Node *> &Q){
	
	Node *minnode = Q[0] ;
	int addr = 0;

	for(int i= 1 ; i < Q.size() ; i++){
	   if((Q[i] -> d) < (minnode -> d)){	
	   	minnode = Q[i];	
		addr = i;
	   }
	}
	
	Q.erase(Q.begin()+addr);
	return minnode;
	
}

void Print_Path( Node *v, ofstream & output ,int &count, int &tw){

	if (v -> prev ){ 

		Print_Path(v -> prev, output,count,tw);
		int w = 0;
		for(int i = 0; i < (v->prev)->edge.size();i++){
			if((v->prev)->edge[i]->getNeighbor(v->prev) == v){
				w = (v->prev)->edge[i]-> weight;
			}
		}
		output << "  v" << v->prev->id << " -- v" << v->id << "  [label = " << "\"" << w << "\"" << "];" << endl ;
		count++;
		tw+=w;
	}	
}	
	


