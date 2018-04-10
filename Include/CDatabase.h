//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								CDatabase
// SUBSYSTEM:						Database library
// LANGUAGE:						C++
// TARGET OS:						None - Standard C++
// LIBRARY DEPENDANCE:	None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
// Copyright 2013 Gavin Blakeman.
// This file is part of the General Class Library (GCL)
//
// GCL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
//
// GCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with GCL.  If not, 
// see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            This file implements a database class that enables mapping different table names and field names in the 
//                      database.
//											This enables an application to use a database that is already existing from another application, but
//                      to map the tables and fields across without having to rename the fields and tables.
//
// CLASSES INCLUDED:    CDatabase
//
// HISTORY:           0000.00.000 2013-01-26 GGB - Development of class for application AIRDAS
//
//*********************************************************************************************************************************

#ifndef __GCL_CDATABASE_H
#define __GCL_CDATABASE_H

  // Standard Header Files

#include <map>
#include <set>

namespace GCL
{
    typedef std::map<std::string, std::string> TTableMap;
    typedef std::map<std::string, TTableMap> TDatabaseMap;
    typedef std::set<std::string> TTableData;
    typedef std::map<std::string, TTableData> TDatabaseData;

    class CDatabase
    {
    private: 
      TDatabaseMap databaseMap;

      virtual void readMapFile(std::string const &);

    protected:

    public:
    };


};  // namespace GCL

#endif // __GCL_CDATABASE_H