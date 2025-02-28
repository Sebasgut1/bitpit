/*---------------------------------------------------------------------------*\
 *
 *  bitpit
 *
 *  Copyright (C) 2015-2021 OPTIMAD engineering Srl
 *
 *  -------------------------------------------------------------------------
 *  License
 *  This file is part of bitpit.
 *
 *  bitpit is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License v3 (LGPL)
 *  as published by the Free Software Foundation.
 *
 *  bitpit is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with bitpit. If not, see <http://www.gnu.org/licenses/>.
 *
\*---------------------------------------------------------------------------*/

# ifndef __BITPIT_LEVELSET_BOUNDED_OBJECT_HPP__
# define __BITPIT_LEVELSET_BOUNDED_OBJECT_HPP__

# include <array>

namespace bitpit {

class LevelSetBoundedObject {

public:
    virtual void                                getBoundingBox( std::array<double,3> &, std::array<double,3> & )const =0  ;
# if BITPIT_ENABLE_MPI
    virtual void                                getGlobalBoundingBox( std::array<double,3> &, std::array<double,3> & )const =0  ;
#endif

protected:
    LevelSetBoundedObject() = default;


};

}

#endif
