// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "boost/assign/list_of.hpp"
#include <boost/algorithm/string/predicate.hpp>

#include "Common/Signal.hpp"
#include "Common/CBuilder.hpp"
#include "Common/OptionT.hpp"


#include "Common/XML/SignalOptions.hpp"

#include "Mesh/CMeshReader.hpp"
#include "Mesh/CDomain.hpp"
#include "Mesh/WriteMesh.hpp"

#include "Solver/CModelSteady.hpp"
#include "Solver/CSolver.hpp"

#include "RDM/Core/SteadyExplicit.hpp"

// supported physical models

#include "Physics/Scalar/Scalar2D.hpp"
#include "Physics/Scalar/ScalarSys2D.hpp"
#include "Physics/Scalar/Scalar3D.hpp"
#include "Physics/NavierStokes/NavierStokes2D.hpp"

namespace CF {
namespace RDM {

using namespace CF::Common;
using namespace CF::Common::XML;
using namespace CF::Mesh;
using namespace CF::Physics;
using namespace CF::Solver;

Common::ComponentBuilder < SteadyExplicit, Solver::CWizard, LibCore > SteadyExplicit_Builder;

////////////////////////////////////////////////////////////////////////////////

SteadyExplicit::SteadyExplicit ( const std::string& name  ) :
  Solver::CWizard ( name )
{
  // signals

  regist_signal( "create_model" )
    ->connect( boost::bind( &SteadyExplicit::signal_create_model, this, _1 ) )
    ->description("Creates a scalar advection model")
    ->pretty_name("Create Model");

  signal("create_component")->hidden(true);
  signal("rename_component")->hidden(true);
  signal("delete_component")->hidden(true);
  signal("move_component")->hidden(true);

  signal("create_model")->signature( boost::bind( &SteadyExplicit::signature_create_model, this, _1));
}

////////////////////////////////////////////////////////////////////////////////

SteadyExplicit::~SteadyExplicit()
{
}

////////////////////////////////////////////////////////////////////////////////

void SteadyExplicit::signal_create_model ( Common::SignalArgs& node )
{
  SignalOptions options( node );

  std::string name  = options.value<std::string>("ModelName");

  CModel& model = Core::instance().root().create_component<CModelSteady>( name );

  // create the domain

  CDomain& domain = model.create_domain( "Domain" );

  // create the Physical Model

  std::string phys  = options.value<std::string>("PhysicalModel");

  PhysModel::Ptr pm  = boost::algorithm::contains( phys, "." ) ?
        build_component_abstract_type< Physics::PhysModel >( phys, "Physics" ) :
        build_component_abstract_type_reduced< Physics::PhysModel >( phys, "Physics" );

  pm->mark_basic();

  model.add_component(pm);

  // setup iterative solver

  CSolver::Ptr solver = build_component_abstract_type<CSolver>( "CF.RDM.Core.RKRD", "Solver");
  solver->mark_basic();

  model.add_component( solver );

  solver->configure_option("physics", pm->uri() );
  solver->configure_option("domain", domain.uri() );

  model.create_component_ptr<WriteMesh>("writer");
}

////////////////////////////////////////////////////////////////////////////////

void SteadyExplicit::signature_create_model( SignalArgs& node )
{
  SignalOptions options( node );

  options.add_option< OptionT<std::string> >("ModelName", std::string() )
      ->set_description("Name for created model" );

  std::vector<boost::any> models = boost::assign::list_of
      ( Scalar::Scalar2D::type_name() )
      ( Scalar::Scalar3D::type_name() )
      ( Scalar::ScalarSys2D::type_name() )
      ( NavierStokes::NavierStokes2D::type_name() ) ;

  options.add_option< OptionT<std::string> >("PhysicalModel", std::string() )
      ->set_description("Name of the Physical Model")
      ->restricted_list() = models;
}

////////////////////////////////////////////////////////////////////////////////

} // RDM
} // CF