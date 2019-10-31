#ifndef SPACE_PARTITION_
#define SPACE_PARTITION_

// Nicer way of storing primitives than in a flat list.
// Implemented with a K-D Tree.

#include <vector>
#include <memory>
#include <variant>
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
        SpacePartition(int axis = 0);

        void BuildFromList(const std::vector<std::unique_ptr<Primitive>>& primitives);
        double TraceRay(const Point& src, const Vector& ray, double hither,
                        const Primitive*& object, Vector& normal) const;

        size_t GetSize() const noexcept;

    protected:
        void BuildFromList(const std::vector<PrimBox>& primitives);
        bool FindPartitionPlane(const std::vector<PrimBox>& primitives,
            BoundingBox& over_box,
            BoundingBox& under_box);
        float AttemptSplit(const std::vector<PrimBox>& primitives,
            float trial_plane,
            BoundingBox& over_box,
            BoundingBox& under_box) const;
        bool IsOverBoundary(const PrimBox& primbox) const noexcept;
        bool IsUnderBoundary(const PrimBox& primbox) const noexcept;

    private:
        struct NonLeafChildren
        {
            std::unique_ptr<SpacePartition> over;
            std::unique_ptr<SpacePartition> under;
        };

        struct LeafChildren
        {
            std::vector<const Primitive*> primitives;
        };

        std::variant<NonLeafChildren, LeafChildren> _children;
        BoundingBox _bounding_box;
        float _splitting_plane;
        int _axis;
};



#endif



