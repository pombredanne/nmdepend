// Copyright (c) 2004 E.J. Pronk and R.E.W. van Beusekom
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
#include <cppunit/extensions/HelperMacros.h>

#define private public

#include "ObjectFile.h"
#include "ObjectPackage.h"
#include "SymbolStore.h"
#include "ObjectPackage.h"
#include "ObjectFile.h"
#include "SymbolStore.h"
#include "Analyser.h"
#include "Graph.h"
#include "Archive.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

namespace fs = boost::filesystem;

using namespace std;


class NewTest;

CPPUNIT_TEST_SUITE_REGISTRATION( NewTest );

using namespace std;




class NewTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( NewTest );
	CPPUNIT_TEST( newTest );
	CPPUNIT_TEST( newTest2 );
	CPPUNIT_TEST( newTest3 );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
	}

protected:

	void newTest()  
	{
		SymbolAdded on_symbol_added;
		Analyser a(1, on_symbol_added);
		fs::path exampleDir = fs::initial_path() / "project1";
		exampleDir.normalize();
		a.find_file(exampleDir);
		a.ReadObjects();
	}

	void newTest2()  
	{
		fs::path a = "aaa/bbb/ccc/x.o";
		//Bfd reader;
		CPPUNIT_ASSERT_EQUAL(string("x.o"), Bfd::packageName(a, 0));
		CPPUNIT_ASSERT_EQUAL(string("ccc"), Bfd::packageName(a, 1));
		CPPUNIT_ASSERT_EQUAL(string("bbb"), Bfd::packageName(a, 2));
		CPPUNIT_ASSERT_EQUAL(string("aaa"), Bfd::packageName(a, 3));
	}

	void newTest3()  
	{
		Graph g;
		SymbolAdded on_symbol_added;
		SymbolStore store;
		ObjectFile b(g, on_symbol_added, "b.obj", store);
		ObjectFile f(g, on_symbol_added, "f.obj", store);
		ObjectFile h(g, on_symbol_added, "h.obj", store);
		vector<Entity*> m_ObjectFiles;
		m_ObjectFiles.push_back(&b);
		m_ObjectFiles.push_back(&f);
		m_ObjectFiles.push_back(&h);
    
		g.init(m_ObjectFiles);
		g(b,f);
		g(f,h);
		g(f,h); // add the same dependency a 2nd time
		g(b,h);
    
		my_label_writer w(m_ObjectFiles);
		boost::default_writer epw;
		boost::default_writer gpw;
		boost::write_graphviz(cout, g.get(), w, epw, gpw);
		//boost::write_graphviz(cout, g.get());
	}
};
