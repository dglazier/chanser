#include "AnaDB.h"
#include <TSystem.h>

namespace chanser{

  using namespace std;

  AnaDBTable::AnaDBTable(const tablevals_t& vals,const  std::vector<size_t> shape){
    //make sure total number of entries defined
    //by shape is same as size of value vector
    uint shape_entries=1;
    for(const auto& s:shape)
      shape_entries*=s;
    
    if(shape_entries!=vals.size()){
      std::cerr<<"ERROR AnaDBTable shape does not fit value vector "<<shape_entries<<" asked for but we  have "<<_vals.size()<<std::endl;
    }
    _vals = vals;
    _shape=shape;
  }
 
  float AnaDBTable::GetEntry(const std::vector<size_t>& entry) const{

    int prodshape=1;
    uint position=0;
    
    //position = entry[0]*(shape[1]*shape[2]) + entry[1]*shape[2] + entry[2]
    for(int i = _shape.size()-1 ; i>=0; --i  ){
      position+=entry[i]*prodshape;
      prodshape*=_shape[i];
    }
    if( position>_vals.size() ) {
      std::cerr<<"ERROR AnaDBTable::GetEntry not enough values for this entry, we have "<<_vals.size()<<" and requested element "<<position<<std::endl;
      return 0;
    }
    return _vals[position];
  }

  size_t AnaDBTable::IsValid(){
    size_t prod=1;
    for(const auto& s:_shape )
      prod*=s;
    return _vals.size()==prod;
    
  }

  void AnaDBTable::Fill(tablevals_t& target){
    
    if(target.size()!=_shape[0]){
      std::cerr<<"ERROR AnaDBTable::Fill(tablevals_t target) target wrong size"<<target.size()<<" should be "<<_shape[0]<<std::endl;
      return;
    }
    
   for(uint i0=0;i0<_shape[0];i0++)
     target[i0]=GetEntry({i0});
     
  }
 void AnaDBTable::Fill(tablevals2D_t& target){
    
    if(target.size()!=_shape[0]){
      std::cerr<<"ERROR AnaDBTable::Fill(tablevals2D_t target) target wrong size"<<target.size()<<" should be "<<_shape[0]<<std::endl;
      return;
    }
   if(target[0].size()!=_shape[1]){
      std::cerr<<"ERROR AnaDBTable::Fill(tablevals2D_t target) target[0] wrong size"<<target[0].size()<<" should be "<<_shape[1]<<std::endl;
      return;
    }
   
   for(uint i0=0;i0<_shape[0];i0++)
     for(uint i1=0;i1<_shape[1];i1++)
       target[i0][i1]=GetEntry({i0,i1});
     
  }
  void AnaDBTable::Fill(tablevals3D_t& target){
    
    if(target.size()!=_shape[0]){
      std::cerr<<"ERROR AnaDBTable::Fill(tablevals3D_t target) target wrong size"<<target.size()<<" should be "<<_shape[0]<<std::endl;
      return;
    }
   if(target[0].size()!=_shape[1]){
      std::cerr<<"ERROR AnaDBTable::Fill(tablevals3D_t target) target[0] wrong size"<<target[0].size()<<" should be "<<_shape[1]<<std::endl;
      return;
    }
   if(target[0][0].size()!=_shape[2]){
      std::cerr<<"ERROR AnaDBTable::Fill(tablevals3D_t target) target[0][0] wrong size"<<target[0][0].size()<<" should be "<<_shape[2]<<std::endl;
      return;
    }
   
   for(uint i0=0;i0<_shape[0];i0++)
     for(uint i1=0;i1<_shape[1];i1++)
       for(uint i2=0;i2<_shape[2];i2++)
	 target[i0][i1][i2]=GetEntry({i0,i1,i2});
     
  }
 void AnaDBTable::Fill(tablevals4D_t& target){
    
    if(target.size()!=_shape[0]){
      std::cerr<<"ERROR AnaDBTable::Fill(tablevals4D_t target) target wrong size"<<target.size()<<" should be "<<_shape[0]<<std::endl;
      return;
    }
   if(target[0].size()!=_shape[1]){
      std::cerr<<"ERROR AnaDBTable::Fill(tablevals4D_t target) target[0] wrong size "<<target[0].size()<<" should be "<<_shape[1]<<std::endl;
      return;
    }
   if(target[0][0].size()!=_shape[2]){
      std::cerr<<"ERROR AnaDBTable::Fill(tablevals4D_t target) target[0][0] wrong size "<<target[0][0].size()<<" should be "<<_shape[2]<<std::endl;
      return;
    }
   if(target[0][0][0].size()!=_shape[3]){
      std::cerr<<"ERROR AnaDBTable::Fill(tablevals4D_t target) target[0][0][0] wrong size "<<target[0][0][0].size()<<" should be "<<_shape[3]<<std::endl;
      return;
    }
   
   for(uint i0=0;i0<_shape[0];i0++)
     for(uint i1=0;i1<_shape[1];i1++)
       for(uint i2=0;i2<_shape[2];i2++)
	 for(uint i3=0;i3<_shape[3];i3++)
	   target[i0][i1][i2][i3]=GetEntry({i0,i1,i2,i3});
     
  }

  void AnaDB::LoadTopFile(string fileName){
    ReplaceEnvVars(fileName);
    ifstream infile;
    infile.open (fileName, ifstream::in);
    if (infile.is_open())
      {
	
	while (infile.good()){

	  string line; 
	  while(getline( infile, line )){
	    stringstream ss( line );
	    string inc;
	    string incFile;
	    ss>>inc>>incFile;
	    if(incFile.empty())
	      continue;
	    if(StartsWith(inc,"#"))
	      continue;
	    cout<<"AnaDB::LoadTopFile  adding file "<<incFile<<endl;
	    LoadDBFile(incFile);
	  }

	}
      }
    else{
      cerr<<"AnaDB::LoadTopFile file not found "<< fileName<<endl;
      exit(0);
    }


  }
 
  void AnaDB::LoadDBFile(string fileName){

    ReplaceEnvVars(fileName);
    ifstream infile;
    infile.open (fileName, ifstream::in);
    if (infile.is_open())
      {
	string lastLine;
	
	while (infile.good()){

	  string line;
	  if(lastLine.empty()) 
	    getline( infile, line );
	  else{
	    line = lastLine;
	    lastLine.clear();
	  }
	  // now we'll use a stringstream to separate the fields out of the line
	  stringstream ss( line );
	  string field;
	  string period;
	  string item;
	  
	  getline( ss, field,' ');
	  if(StartsWith(field,"PERIOD=")){
	    field.erase(0,7);//remove PERIOD=;
	    period=field;
	    //get next bit EXP or SIM ? INBEND or OUTBEND
	    while(getline( ss, field,' ')){
	      period+="_";
	      period+=field;
	    }
	      
	    //check if this period exists
	    RegisterPeriod(period);
	    
	    //and name of this table
	    getline( infile, field);
	    item=field;

	    //std::cout<<"period "<<period<< " add item "<<item<<std::endl;

	    tablevals_t vals;
	    string dataline;
	    while(StartsWith(dataline,"PERIOD=")==false){
	      getline( infile, dataline );
	      ReadVals(dataline,vals);
	      if(infile.good()==false)
		break;
	    }

	    lastLine=dataline;
	    // std::cout<<" size of vals "<< vals.size() <<" "<<lastLine<<std::endl;
	   
	    
	    AddPeriodEntry(period,item,vals);
	    
	    if(infile.good()==false)
	      break;
	  }

	  /*
	  while (getline( ss, field,' '))
	    {
	     
	      //stringstream fs( field );
	      // double f = 0.0;  // (default value is 0.0)
	      //fs >> f;
	      
	    }
	  */
	}
      }
    else{
      cerr<<"AnaDB::LoaDBFile file "<<fileName<<" does not exist "<<endl;
      exit(0);
    }
    

    infile.close();
  }

  void AnaDB::AddPeriodEntry(const string& period,const string& item,const tablevals_t& vals){
    
    auto& periodTable=_db.at(period);
    if(periodTable.find(item)!=periodTable.end())
      cout<<" AnaDB::AddPeriodEntry Warning entry " <<item<<" already exists in "<<period<<" will ignore this one "<<std::endl;
    else{
      periodTable[item]=vals;
      std::cout<<period<<" add item "<<item<<" "<<periodTable.size()<<std::endl;
      
    }
	
  }
  ////////////////////////////////////////////////////////////////////
  


  //////////////////////////////////////////////////////////////////
  bool StartsWith(const string& text,const string& pre){
    return text.rfind(pre,0)==0;
  }
  
  bool IsFloat( const string& myString ) {
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
  }
  
  void ReadVals(const string& line,tablevals_t& vals){
    stringstream ss( line );
    string entry;
    while( getline( ss, entry,' ') ){
      // if(StartsWith(entry,"#")==false)
      if(IsFloat(entry)==true){
	vals.push_back(atof(entry.data()));
      }

    }
  }
  void ReplaceEnvVars(string& path){

    size_t pos=0;
    if((pos=path.find('$'))==string::npos)
      return;//no env vars

    auto pos2=path.find('/',pos);

    string temp;
    if(pos2==string::npos)//only env varaiable, no directories
      temp=path;
    else
      temp=path.substr(pos,pos2);

    //check if env variable of this name
    string  varName=temp.substr(1,temp.size()-1);
    string replacement=gSystem->Getenv(varName.data());
    if(replacement.length())
      path.replace(pos,pos2-pos,replacement);
    
      
  }
  
}
