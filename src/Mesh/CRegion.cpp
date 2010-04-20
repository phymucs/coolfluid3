#include "Mesh/CRegion.hpp"

namespace CF {
namespace Mesh {

using namespace Common;

////////////////////////////////////////////////////////////////////////////////

CRegion::CRegion ( const CName& name  ) :
  Component ( name ),
  m_elementType()
{
}

////////////////////////////////////////////////////////////////////////////////

CRegion::~CRegion()
{
}

////////////////////////////////////////////////////////////////////////////////

void CRegion::create_region( const CName& name )
{
  boost::shared_ptr<CRegion> new_region ( new CRegion(name) );

  m_subregions.push_back(new_region);

  add_component ( new_region );

}

////////////////////////////////////////////////////////////////////////////////

void CRegion::create_connectivityTable( const CName& name )
{
  boost::shared_ptr<CTable> new_connTable ( new CTable(name) );
  m_connTable = new_connTable;
  
  add_component ( m_connTable );
}

////////////////////////////////////////////////////////////////////////////////

} // Mesh
} // CF
