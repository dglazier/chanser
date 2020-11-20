//////////////////////////////////////////////////////////////
///
///Class:		AnaDB
///Description:
///
#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

namespace chanser{

  using std::string;
  using tablevals_t = std::vector<double>;
  using table_t = std::map<string,tablevals_t>;
  using runperiod_t = std::map<string,table_t>;

  using tablevals2D_t = std::vector<tablevals_t>;
  using tablevals3D_t = std::vector<tablevals2D_t>;
  using tablevals4D_t = std::vector<tablevals3D_t>;


    
  class AnaDBTable{
    
  public:

    AnaDBTable(const tablevals_t& vals,const  std::vector<size_t> shape){
      //make sure total number of entries defined
      //by shape is same as size of value vector
      auto shape_entries=1;
      for(const auto& s:shape)
	shape_entries*=s;

      if(shape_entries!=vals.size()){
	std::cerr<<"ERROR AnaDBTable shape does not fit value vector "<<shape_entries<<" asked for but we  have "<<_vals.size()<<std::endl;
      }
      _vals = vals;
      _shape=shape;
    }
    
    float GetEntry(const std::vector<size_t>& entry) const;

    void Fill(tablevals_t& target);
    void Fill(tablevals2D_t& target);
    void Fill(tablevals3D_t& target);
    void Fill(tablevals4D_t& target);

    size_t IsValid();
    
  private:

    tablevals_t  _vals;
    std::vector<size_t> _shape;

  };

    
  class AnaDB{


  public:

    void LoadTopFile(string fileName);
    void LoadDBFile(string fileName);
    
    void LoadPeriodTable(const string& periodName,const table_t& table);
    
    const tablevals_t& GetValues(const string& periodName,const string& tableName) const {
      if ( _db.find(periodName) == _db.end() )
  	return _nullTable;
      if ( _db.at(periodName).find(tableName) == _db.at(periodName).end() )
  	return _nullTable;
      return _db.at(periodName).at(tableName);
    }

    void RegisterPeriod(const string& periodName){
      if(_db.find(periodName)!=_db.end()) return;
      _db[periodName]=table_t();
    }

    void AddPeriodEntry(const string& period,const string& item,const tablevals_t& vals);


    AnaDBTable GetTable(const string& periodName,const string& tableName,const  std::vector<size_t> shape) const{
      return AnaDBTable(GetValues(periodName,tableName),shape);
    }
    
  private:

    runperiod_t _db;
    tablevals_t _nullTable;
    
  };

  bool StartsWith(const string& text,const string& pre);
  bool IsFloat( const string& myString );
  void ReadVals(const string& line,tablevals_t& vals);
  void ReplaceEnvVars(string& path);
}
