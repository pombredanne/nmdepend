#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <set>
#include <map>

#include "Package.h"

class ObjectFile;

class Symbol
{
public:
   typedef std::string Name_t;
   typedef std::map<Name_t, Symbol*> SymbolIndex_t;

   Symbol(ObjectFile* o, const Name_t& name);

   ObjectFile* m_Owner;
   std::string Demangled();

public:
   Name_t m_Name;

   // Package exporting the symbol
   //std::set<Package*> m_Referenced;
};

#endif
