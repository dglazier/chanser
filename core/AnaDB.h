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

  using tablestrings_t = std::vector<string>;
  using tabletags_t = std::map<string,tablestrings_t>;
  using runperiodtags_t = std::map<string,tabletags_t>;

  using tablevals2D_t = std::vector<tablevals_t>;
  using tablevals3D_t = std::vector<tablevals2D_t>;
  using tablevals4D_t = std::vector<tablevals3D_t>;


    
  class AnaDBTable{
    
  public:

    AnaDBTable(const tablevals_t& vals,const  std::vector<size_t> shape,const tablestrings_t& tags);
    
    float GetEntry(const std::vector<size_t>& entry) const;

    void Fill(tablevals_t& target);
    void Fill(tablevals2D_t& target);
    void Fill(tablevals3D_t& target);
    void Fill(tablevals4D_t& target);

    size_t IsValid();
    
    const tablestrings_t& GetTags() const {return _tags;}
  private:

    tablevals_t  _vals;
    std::vector<size_t> _shape;
    tablestrings_t  _tags;

  };

    
  class AnaDB{


  public:

    void LoadTopFile(string fileName);
    void LoadDBFile(string fileName);
    
    void LoadPeriodTable(const string& periodName,const table_t& table);
    
    const tablevals_t& GetValues(const string& periodName,const string& tableName) const {
      if ( _db.find(periodName) == _db.end() ){
	std::cerr<<"WARNING AnaDB::GetValues no period "<<periodName<<" for "<<tableName<<std::endl;
  	return _nullTable;
      }
      if ( _db.at(periodName).find(tableName) == _db.at(periodName).end() ){
	std::cerr<<"WARNING AnaDB::GetValues no table in Period "<<periodName<<" for "<<tableName<<std::endl;

  	return _nullTable;
      }
      return _db.at(periodName).at(tableName);
    }

   const tablestrings_t& GetTags(const string& periodName,const string& tableName) const {
      if ( _dbtags.find(periodName) == _dbtags.end() )
  	return _nullSTable;
      if ( _dbtags.at(periodName).find(tableName) == _dbtags.at(periodName).end() )
  	return _nullSTable;
      return _dbtags.at(periodName).at(tableName);
    }

    void RegisterPeriod(const string& periodName){
      if(_db.find(periodName)!=_db.end()) return;
      _db[periodName]=table_t();
      _dbtags[periodName]=tabletags_t();
    }

    void AddPeriodEntry(const string& period,const string& item,const tablevals_t& vals,const tablestrings_t& tags);


    AnaDBTable GetTable(const string& periodName,const string& tableName,const  std::vector<size_t> shape) const{
      return AnaDBTable(GetValues(periodName,tableName),shape,(GetTags(periodName,tableName)));
    }
    
  private:

    runperiod_t _db;
    runperiodtags_t _dbtags;
    
    tablevals_t _nullTable;
    tablestrings_t _nullSTable;
    
  };

  bool StartsWith(const string& text,const string& pre);
  bool IsFloat( const string& myString );
  bool IsString( const string& myString );
  void ReadVals(const string& line,tablevals_t& vals,string& tag);
  void ReplaceEnvVars(string& path);
}
