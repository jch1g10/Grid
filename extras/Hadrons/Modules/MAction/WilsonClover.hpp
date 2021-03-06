/*************************************************************************************

Grid physics library, www.github.com/paboyle/Grid 

Source file: extras/Hadrons/Modules/MAction/Wilson.hpp

Copyright (C) 2015
Copyright (C) 2016

Author: Antonin Portelli <antonin.portelli@me.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

See the full license in the file "LICENSE" in the top level distribution directory
*************************************************************************************/
/*  END LEGAL */

#ifndef Hadrons_MAction_WilsonClover_hpp_
#define Hadrons_MAction_WilsonClover_hpp_

#include <Grid/Hadrons/Global.hpp>
#include <Grid/Hadrons/Module.hpp>
#include <Grid/Hadrons/ModuleFactory.hpp>

BEGIN_HADRONS_NAMESPACE

/******************************************************************************
 *                            TWilson quark action                            *
 ******************************************************************************/
BEGIN_MODULE_NAMESPACE(MAction)

class WilsonCloverPar: Serializable
{
public:
    GRID_SERIALIZABLE_CLASS_MEMBERS(WilsonCloverPar,
                                    std::string, gauge,
                                    double     , mass,
				                    double     , csw_r,
				                    double     , csw_t,
				                    WilsonAnisotropyCoefficients ,clover_anisotropy,
                                    std::string, boundary
				    );
};

template <typename FImpl>
class TWilsonClover: public Module<WilsonCloverPar>
{
public:
    FGS_TYPE_ALIASES(FImpl,);
public:
    // constructor
    TWilsonClover(const std::string name);
    // destructor
    virtual ~TWilsonClover(void) = default;
    // dependencies/products
    virtual std::vector<std::string> getInput(void);
    virtual std::vector<std::string> getOutput(void);
    // setup
    virtual void setup(void);
    // execution
    virtual void execute(void);
};

MODULE_REGISTER_NS(WilsonClover, TWilsonClover<FIMPL>, MAction);

/******************************************************************************
 *                     TWilsonClover template implementation                        *
 ******************************************************************************/
// constructor /////////////////////////////////////////////////////////////////
template <typename FImpl>
TWilsonClover<FImpl>::TWilsonClover(const std::string name)
: Module<WilsonCloverPar>(name)
{}

// dependencies/products ///////////////////////////////////////////////////////
template <typename FImpl>
std::vector<std::string> TWilsonClover<FImpl>::getInput(void)
{
    std::vector<std::string> in = {par().gauge};

    return in;
}

template <typename FImpl>
std::vector<std::string> TWilsonClover<FImpl>::getOutput(void)
{
    std::vector<std::string> out = {getName()};

    return out;
}

// setup ///////////////////////////////////////////////////////////////////////
template <typename FImpl>
void TWilsonClover<FImpl>::setup(void)
{
    //unsigned int size;

    // size = 2*env().template lattice4dSize<typename FImpl::DoubledGaugeField>();
    // env().registerObject(getName(), size);


    LOG(Message) << "Setting up TWilsonClover fermion matrix with m= " << par().mass
                 << " using gauge field '" << par().gauge << "'" << std::endl;
    LOG(Message) << "Fermion boundary conditions: " << par().boundary 
                 << std::endl;
    LOG(Message) << "Clover term csw_r: " << par().csw_r
                 << " csw_t: " << par().csw_t
                 << std::endl;
    auto &U      = envGet(LatticeGaugeField, par().gauge);
    auto &grid   = *env().getGrid();
    auto &gridRb = *env().getRbGrid();
    std::vector<Complex> boundary = strToVec<Complex>(par().boundary);
    typename WilsonCloverFermion<FImpl>::ImplParams implParams(boundary);
    envCreateDerived(FMat, WilsonCloverFermion<FImpl>, getName(), 1, U, grid, gridRb, par().mass,
						  par().csw_r,
						  par().csw_t,
					      par().clover_anisotropy,
						  implParams); 


    //FMat *fMatPt = new WilsonCloverFermion<FImpl>(U, grid, gridRb, par().mass,
	//					  par().csw_r,
	//					  par().csw_t,
	//				      par().clover_anisotropy,
	//					  implParams);
    //env().setObject(getName(), fMatPt);

}

// execution ///////////////////////////////////////////////////////////////////
template <typename FImpl>
void TWilsonClover<FImpl>::execute()
{

}

END_MODULE_NAMESPACE

END_HADRONS_NAMESPACE

#endif // Hadrons_WilsonClover_hpp_
