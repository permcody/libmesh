// $Id: cell_inf_hex.h,v 1.2 2004-01-03 15:37:42 benkirk Exp $

// The libMesh Finite Element Library.
// Copyright (C) 2002-2004  Benjamin S. Kirk, John W. Peterson
  
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifndef __cell_inf_hex_h__
#define __cell_inf_hex_h__

// C++ includes

// Local includes
#include "libmesh_config.h"
#ifdef ENABLE_INFINITE_ELEMENTS
#include "cell_inf.h"




/**
 * The \p InfHex is an element in 3D with 5 sides.
 * The \f$ 6^{th} \f$ side is theoretically located at infinity, 
 * and therefore not accounted for.
 * However, one could say that the \f$ 6^{th} \f$ side actually 
 * @e does exist in the mesh, since the outer nodes are located 
 * at a specific distance from the mesh origin (and therefore
 * define a side).  Still, this face is not to be used!
 */

// ------------------------------------------------------------
// InfHex class definition
class InfHex : public InfCell
{
public:

  /**
   * Default infinite brick element, takes number of nodes and 
   * parent. Derived classes implement 'true' elements.
   */
  InfHex(const unsigned int nn, const Elem* p);

  /**
   * @returns 4 for the base \p s=0 and 2 for side faces. 
   */
  unsigned int n_children_per_side(const unsigned int s) const;

  /**
   * @returns 5.  Infinite elements have one side less
   * than their conventional counterparts, since one
   * side is supposed to be located at infinity.
   */
  unsigned int n_sides() const { return 5; }

  /**
   * @returns 8.  All infinite hexahedrals (in our 
   * setting) have 8 vertices.
   */
  unsigned int n_vertices() const { return 8; }

  /**
   * @returns 8.  All infinite hexahedrals have 8 edges,
   * 4 lying in the base, and 4 perpendicular to the base.
   */
  unsigned int n_edges() const { return 8; }

  /**
   * @returns 5.  All hexahedrals have 5 faces.
   */
  unsigned int n_faces() const { return 5; }
  
  /**
   * @returns 4
   */
  unsigned int n_children() const { return 4; }
  
  /**
   * @returns an id associated with the \p s side of this element.
   * The id is not necessariy unique, but should be close.  This is
   * particularly useful in the \p MeshBase::find_neighbors() routine.
   */
  unsigned int key (const unsigned int s) const;

  /**
   * @returns a primitive (4-noded) quad or infquad for 
   * face i.
   */
  AutoPtr<Elem> side (const unsigned int i) const;

  /**
   * Based on the quality metric q specified by the user,
   * returns a quantitative assessment of element quality.
   */
  Real quality (const ElemQuality q) const;

  /**
   * Returns the suggested quality bounds for
   * the hex based on quality measure q.  These are
   * the values suggested by the CUBIT User's Manual.
   */
  std::pair<Real, Real> qual_bounds (const ElemQuality q) const;


  
protected:


  
#ifdef ENABLE_AMR
  
  /**
   * Matrix that allows children to inherit boundary conditions.
   */
  unsigned int side_children_matrix (const unsigned int i,
				     const unsigned int j) const
  { return _side_children_matrix[i][j]; }

#endif  


  /**
   * For higher-order elements, namely \p InfHex16 and
   * \p InfHex18, the matrices for adjacent vertices
   * of second order nodes are quite similar (apart from
   * the face nodes, which are directly handled by \p InfHex18).
   * Therefore hold this matrix here, so that both can re-use
   * this.  Matrix that tells which vertices define the location
   * of mid-side (or second-order) nodes.  
   */
  static const unsigned short int _second_order_adjacent_vertices[8][2];
  

private:
  
#ifdef ENABLE_AMR
  
  /**
   * Matrix that tells which children share which of
   * my sides.
   */
  static const unsigned int _side_children_matrix[5][4];
  
#endif  
};



// ------------------------------------------------------------
// InfHex class member functions
inline
InfHex::InfHex(const unsigned int nn, const Elem* p) :
  InfCell(nn, InfHex::n_sides(), p) 
{
}


inline
unsigned int InfHex::n_children_per_side(const unsigned int s) const
{
  assert (s < this->n_sides());

  switch (s)
  {
    case 0:
      // every infinite element has 4 children in the base side
      return 4;

    default:
      // on infinite faces (sides), only 2 children exist
      //
      // note that the face at infinity is already caught by the assertion
      return 2;
  }
}


#endif // ifdef ENABLE_INFINITE_ELEMENTS

#endif
