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

#ifndef __BITPIT_ELEMENT_HPP__
#define __BITPIT_ELEMENT_HPP__

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

#include "bitpit_containers.hpp"

#include "element_type.hpp"
#include "element_reference.hpp"

namespace bitpit {
	class Element;
}

bitpit::IBinaryStream& operator>>(bitpit::IBinaryStream &buf, bitpit::Element& element);
bitpit::OBinaryStream& operator<<(bitpit::OBinaryStream &buf, const bitpit::Element& element);

namespace bitpit {

class Element {

friend bitpit::OBinaryStream& (::operator<<) (bitpit::OBinaryStream& buf, const Element& element);
friend bitpit::IBinaryStream& (::operator>>) (bitpit::IBinaryStream& buf, Element& element);

public:
	/*!
		Hasher for the ids.

		Since ids are unique, the hasher can be a function that
		takes an id and cast it to a size_t.

		The hasher is defined as a struct, because a struct can be
		passed as an object into metafunctions (meaning that the type
		deduction for the template paramenters can take place, and
		also meaning that inlining is easier for the compiler). A bare
		function would have to be passed as a function pointer.
		To transform a function template into a function pointer,
		the template would have to be manually instantiated (with a
		perhaps unknown type argument).

	*/
	struct IdHasher {
		/*!
			Function call operator that casts the specified
			value to a size_t.

			\tparam U type of the value
			\param value is the value to be casted
			\result Returns the value casted to a size_t.
		*/
		template<typename U>
		constexpr std::size_t operator()(U&& value) const noexcept
		{
			return static_cast<std::size_t>(std::forward<U>(value));
		}
	};

	static int getDimension(ElementType type);
	static bool isThreeDimensional(ElementType type);

	static int getFaceStreamPosition(const long *connectivity, int face);
	static ConstProxyVector<long> getVertexIds(ElementType type, const long *connectivity);

	Element();
	Element(long id, ElementType type, int connectSize = 0);
	Element(long id, ElementType type, std::unique_ptr<long[]> &&connectStorage);

	Element(const Element &other);
	Element(Element&& other) = default;
	Element& operator = (const Element &other);
	Element& operator=(Element&& other) = default;

	void swap(Element &other) noexcept;

	void initialize(long id, ElementType type, int connectSize = 0);
	void initialize(long id, ElementType type, std::unique_ptr<long[]> &&connectStorage);

	bool hasInfo() const;
	const ReferenceElementInfo & getInfo() const;

	void setId(long id);
	long getId() const;
	
	void setType(ElementType type);
	ElementType getType() const;

	void setPID(int pid);
	int getPID() const;

	int getDimension() const;
	bool isThreeDimensional() const;
	
	void setConnect(std::unique_ptr<long[]> &&connect);
	void unsetConnect();
	int getConnectSize() const;
	const long * getConnect() const;
	long * getConnect();
	bool hasSameConnect(const Element &other) const;

	int getFaceCount() const;
	ElementType getFaceType(int face) const;
	int getFaceVertexCount(int face) const;
	ConstProxyVector<int> getFaceLocalConnect(int face) const;
	ConstProxyVector<long> getFaceConnect(int face) const;
	ConstProxyVector<long> getFaceVertexIds(int face) const;
	long getFaceVertexId(int face, int vertex) const;
	ConstProxyVector<int> getFaceLocalVertexIds(int face) const;

	int getEdgeCount() const;
	ElementType getEdgeType(int edge) const;
	int getEdgeVertexCount(int edge) const;
	ConstProxyVector<int> getEdgeLocalConnect(int edge) const;
	ConstProxyVector<long> getEdgeConnect(int edge) const;
	ConstProxyVector<long> getEdgeVertexIds(int edge) const;
	long getEdgeVertexId(int edge, int vertex) const;
	ConstProxyVector<int> getEdgeLocalVertexIds(int edge) const;

	int getVertexCount() const;
	void renumberVertices(const std::unordered_map<long, long> &map);
	ConstProxyVector<long> getVertexIds() const;
	long getVertexId(int vertex) const;
	int findVertex(long vertexId) const;

	int getFaceStreamSize() const;
	std::vector<long> getFaceStream() const;
	static void renumberFaceStream(const PiercedStorage<long, long> &map, std::vector<long> *faceStream);
	int getFaceStreamPosition(int face) const;

	static const long NULL_ID;

	std::array<double, 3> evalCentroid(const std::array<double, 3> *coordinates) const;

	double evalSize(const std::array<double, 3> *coordinates) const;

	double evalVolume(const std::array<double, 3> *coordinates) const;
	double evalArea(const std::array<double, 3> *coordinates) const;
	double evalLength(const std::array<double, 3> *coordinates) const;

	std::array<double, 3> evalNormal(const std::array<double, 3> *coordinates, const std::array<double, 3> &orientation = {{0., 0., 1.}}, const std::array<double, 3> &point = {{0.5, 0.5, 0.5}}) const;

	void evalPointProjection(const std::array<double, 3> &point, const std::array<double, 3> *coordinates, std::array<double, 3> *projection, double *distance) const;
	double evalPointDistance(const std::array<double, 3> &point, const std::array<double, 3> *coordinates) const;

	unsigned int getBinarySize() const;

private:
	class Tesselation {

	friend class Element;

	public:
		Tesselation();

		int importVertexCoordinates(const std::array<double, 3> &coordinates);
		int importVertexCoordinates(std::array<double, 3> &&coordinates);
		std::vector<int> importVertexCoordinates(const std::array<double, 3> * coordinates, int nVertices);

		void importPolygon(const std::vector<int> &vertexIds);
		void importPolyhedron(const std::vector<int> &vertexIds, const std::vector<std::vector<int>> &faceVertexIds);

		int getTileCount() const;
		ElementType getTileType(int tile) const;
		std::vector<std::array<double, 3>> getTileVertexCoordinates(int tile) const;

	private:
		int m_nTiles;
		std::vector<ElementType> m_types;
		std::vector<std::vector<int>> m_connects;

		std::vector<std::array<double, 3>> m_coordinates;
	};

	static int countPolygonVertices(const long *connectivity);

	static int countPolygonFaces(const long *connectivity);
	static int countPolyhedronFaces(const long *connectivity);

	long m_id; //!< Is the id that identifies the element

	ElementType m_type;

	int m_pid; //!< Is the part id associated with the element

	std::unique_ptr<long[]> m_connect;

	void _initialize(long id, ElementType type = ElementType::UNDEFINED, int connectSize = 0);
	void _initialize(long id, ElementType type, std::unique_ptr<long[]> &&connectStorage);

	Tesselation generateTesselation(const std::array<double, 3> *coordinates) const;

	std::vector<ConstProxyVector<long>> evalEdgeConnects(int maxEdges = -1) const;

};

template<class DerivedElement>
class ElementHalfItem {

public:
	enum Winding {
		WINDING_NATURAL =  1,
		WINDING_REVERSE = -1
	};

	struct Hasher {
		std::size_t operator()(const ElementHalfItem &item) const;
	};

	const ConstProxyVector<long> & getVertexIds() const;

	Winding getWinding() const;
	void setWinding(Winding winding);

	bool operator==(const ElementHalfItem &other) const;
	bool operator!=(const ElementHalfItem &other) const;

protected:
	DerivedElement &m_element;

	ConstProxyVector<long> m_vertexIds;
	std::size_t m_firstVertexId;

	Winding m_winding;

	ElementHalfItem(DerivedElement &element, ConstProxyVector<long> &&vertexIds, ElementHalfItem<DerivedElement>::Winding winding);

	DerivedElement & getElement() const;

};

template<class DerivedElement>
class ElementHalfEdge : public ElementHalfItem<DerivedElement> {

public:
	typedef typename ElementHalfItem<DerivedElement>::Winding Winding;

	int getEdge() const;

protected:
	int m_edge;

	ElementHalfEdge(DerivedElement &element, int edge, Winding winding);

};

template<class DerivedElement>
class ElementHalfFace : public ElementHalfItem<DerivedElement> {

public:
	typedef typename ElementHalfItem<DerivedElement>::Winding Winding;

	int getFace() const;

protected:
	int m_face;

	ElementHalfFace(DerivedElement &element, int face, Winding winding);

};

extern template class PiercedVector<Element>;

}

// Include template implementations
#include "element.tpp"

#endif
