#ifndef PYRAMIDWORKS_GEOMETRY_CONVEXPOLYGON_H_
#define PYRAMIDWORKS_GEOMETRY_CONVEXPOLYGON_H_

#include <pyramidworks/geometry/geometricshape.h>
#include <vector>

namespace pyramidworks {
namespace geometry {

/// \class RotatedRect rotatedrect.h "pyramidworks/geometry/rotatedrect.h"
/// A rotated rect, one type of GeometricShape. This determines a rect just like the Rect class, but allows it to be rotated giving more geometric options.
class ConvexPolygon : public GeometricShape {
  public:
    /** @param vertices	The list of vertices of the polygon, in local coordinates (0,0 is the "center", where this polygon is positioned).
						vertices[i] and vertices[i+1] form a edge, and vertices[n]-vertices[0] is the closing edge.
	  */
	ConvexPolygon(const std::vector<ugdk::Vector2D>& vertices);

	void set_vertices(const std::vector<ugdk::Vector2D>& vertices) { vertices_ = vertices; this->calculateSize(); }

    bool Intersects(const ugdk::Vector2D& this_pos, const GeometricShape *obj, const ugdk::Vector2D& that_pos) const;
    bool Intersects(const ugdk::Vector2D& this_pos, const           Rect *obj, const ugdk::Vector2D& that_pos) const;
    bool Intersects(const ugdk::Vector2D& this_pos, const         Circle *obj, const ugdk::Vector2D& that_pos) const;
	bool Intersects(const ugdk::Vector2D& this_pos, const  ConvexPolygon *obj, const ugdk::Vector2D& that_pos) const;
    ugdk::ikdtree::Box<2> GetBoundingBox(const ugdk::Vector2D& position) const;

	double bbox_width() { return bbox_half_width_*2; }
	double bbox_height() { return bbox_half_height_*2; }

  private:
    std::vector<ugdk::Vector2D> vertices_;
	double bbox_half_width_, bbox_half_height_;

	void calculateSize();

	/// Checks if there's an separating axis between the two given objects (convex polygons represented as a Vector2D list and a position). 
	/// If a separating axis exist, the two polygons do not intersect.
	/** @param obj1 The list of vertices, in local coordinates, of the first convex polygon. Each pair of adjacent vectors 
				(plus the last and initial vectors) in the list correspond to a pair of vertices that form a edge of the polygon. Vertices should be ordered clockwise.
        @param obj1pos The position of the first convex polygon.
        @param obj2 The list of vertices, in local coordinates, of the second convex polygon. Each pair of adjacent vectors 
				(plus the last and initial vectors) in the list correspond to a pair of vertices that form a edge of the polygon. Vertices should be ordered clockwise.
		@param obj2pos The position of the second convex polygon.
        @return True if there's a separating axis. */
	bool checkAxisSeparation(const std::vector<ugdk::Vector2D>& obj1, const ugdk::Vector2D& obj1pos, 
							 const std::vector<ugdk::Vector2D>& obj2, const ugdk::Vector2D& obj2pos) const;
	bool axisSeparationTest(const ugdk::Vector2D& p1, const ugdk::Vector2D& p2, const ugdk::Vector2D& ref,
							const std::vector<ugdk::Vector2D>& obj, const ugdk::Vector2D& obj2pos) const;
	bool insideSameSpace(const ugdk::Vector2D& line, const ugdk::Vector2D& point) const;
};

} // namespace geometry
} // namespace pyramidworks


#endif /* PYRAMIDWORKS_GEOMETRY_CONVEXPOLYGON_H_ */
