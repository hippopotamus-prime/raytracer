#ifndef SPACE_PARTITION_
#define SPACE_PARTITION_

// Nicer way of storing primitives than in a flat list.
// Implemented with a K-D Tree.

#include <vector>
#include <memory>
#include <list>
#include "Primitive.h"
#include "BoundingBox.h"
#include "VectorMath.h"

struct PrimBox
{
    BoundingBox box;
    Primitive* prim;
};

class SpacePartition
{
    public:
        SpacePartition(int _axis = 0);
        ~SpacePartition();

        void BuildFromList(const std::vector<std::unique_ptr<Primitive>>& primitives);
        double TraceRay(const Point& src, const Vector& ray, double hither,
                        Primitive*& object, Vector& normal);

        size_t GetSize();
        void MarkPrimitives();

    protected:
        void BuildFromList(const std::vector<PrimBox>& primitives,
                           const BoundingBox& box);
        bool FindPartitionPlane(const std::vector<PrimBox>& primitives,
                                BoundingBox& over_box, BoundingBox& under_box);
        float AttemptSplit(const std::vector<PrimBox>& primitives, float trial_plane,
                           BoundingBox& over_box, BoundingBox& under_box);
        bool IsOverBoundary(const PrimBox& primbox);
        bool IsUnderBoundary(const PrimBox& primbox);

    private:
        int axis;
        float splitting_plane;
        SpacePartition* over;
        SpacePartition* under;
        std::list<Primitive*>* leaf_primitives;
        BoundingBox bounding_box;

        // TODO - make leaf_primitives a union
};



#endif



