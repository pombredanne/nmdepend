// Copyright (c) 2005 E.J. Pronk and R.E.W. van Beusekom
//
// This file is part of Nmdepend.
// 
// Nmdepend is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// Nmdepend is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Nmdepend; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include <iostream>
#include <algorithm>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "Analyser.h"

namespace fs = boost::filesystem;
using namespace std;

Analyser::Analyser(int packageLevel)
 : m_packageLevel(packageLevel)
 , m_Bfd(m_ObjectGraph, m_PackageGraph, m_symbols)
{
}
   
void Analyser::find_file( const fs::path& dir_path)
{
  if ( !fs::exists( dir_path ) )
  {
    std::cout << "doesn't exist " << dir_path.native_directory_string() << std::endl;
  }

  fs::directory_iterator end_itr;
  for(fs::directory_iterator itr( dir_path );
      itr != end_itr;
      ++itr )
      {
         if ( fs::is_directory( *itr ) )
         {
            find_file(*itr);
         }
         else
         {
            if (itr->leaf().rfind(".o") != std::string::npos)
            {
               list.push_back(*itr);
            }
         }
      }
   }

   void Analyser::ReadObjects()
   {
      for(filelist_t::iterator pos = list.begin(); pos != list.end(); ++pos)
      {
         fs::path::iterator p = pos->end();
         --p;
         std::string name = *p;
         --p;
         if(m_packageLevel == 2)
         {
           --p;
         }
         std::string packagename = *p;
         
         Entity* o = m_Bfd.Read(*pos);

         std::pair<std::set<ObjectPackage>::iterator,bool> status =
              m_PackageSet.insert(ObjectPackage(m_PackageGraph, packagename));

         ObjectPackage* op = const_cast<ObjectPackage*>(&(*(status.first)));
         o->SetParent(*op);
         m_ObjectFiles.push_back(o);
      }
      
      for(std::set<ObjectPackage>::iterator i = m_PackageSet.begin(); i != m_PackageSet.end(); ++i)
      {
        m_Packages.push_back(const_cast<ObjectPackage*>(&(*i)));
      }
   }

   void Analyser::Link()
   {
      m_ObjectGraph.init(m_ObjectFiles);
      m_PackageGraph.init(m_Packages);
      for (std::vector<Entity*>::iterator pos = m_ObjectFiles.begin();
      pos != m_ObjectFiles.end();
      ++pos)
      {
         std::cout << "linking obj " << (*pos)->Name() << std::endl;
         (*pos)->Link();
      }
      m_symbols.Statistics();
   }
   
   void Analyser::WriteObjectGraph(std::ostream& out)
   {
      wrapper<Entity> w(m_ObjectFiles);
      boost::default_writer def;
      sample_graph_writer sample;
      boost::write_graphviz(out, m_ObjectGraph.get(), boost::make_label_writer(w), def, sample);
   }
   
   void Analyser::WritePackageGraph(std::ostream& out)
   {
      wrapper<Entity> w(m_Packages);
      boost::default_writer def;
      sample_graph_writer sample;
      boost::write_graphviz(out, m_PackageGraph.get(),
        boost::make_label_writer(w), def, sample);
   }
