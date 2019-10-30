#include "Primitive.h"
#include "SpacePartition.h"
#include "BoundingBox.h"
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class CompareMaxEdge
{
    public:
        CompareMaxEdge(int _axis) : axis(_axis) {}

        bool operator () (const PrimBox& a, const PrimBox& b)
            {return (a.box.GetMaxValue(axis) < b.box.GetMaxValue(axis));}

    private:
        int axis;
};

class CompareMinEdge
{
    public:
        CompareMinEdge(int _axis) : axis(_axis) {}

        bool operator () (const PrimBox& a, const PrimBox& b)
            {return (a.box.GetMinValue(axis) < b.box.GetMinValue(axis));}

    private:
        int axis;
};




SpacePartition::SpacePartition(int _axis) : axis(_axis)
{
}

SpacePartition::~SpacePartition()
{
    if(over)            delete over;
    if(under)           delete under;
    if(leaf_primitives) delete leaf_primitives;
}

void SpacePartition::BuildFromList(const vector<unique_ptr<Primitive>>& primitives)
{
    vector<PrimBox> primboxes;
    PrimBox primbox;

    // Need to calculate a bounding box for the partition as a whole
    BoundingBox total_box;
    (*(primitives.begin()))->GetBoundingBox(total_box);

    for (const auto& prim_ptr: primitives)
    {
        primbox.prim = prim_ptr.get();
        primbox.prim->GetBoundingBox(primbox.box);
        primboxes.push_back(primbox);

        total_box.ExpandToFit(primbox.box);
    }

    BuildFromList(primboxes, total_box);
}


void SpacePartition::BuildFromList(const vector<PrimBox>& primitives,
                                   const BoundingBox& box)
{
    cout << "Space partition building from list with " << primitives.size()
         << " primitives." << endl;

    bounding_box = box;

    BoundingBox over_box, under_box;
    bool need_to_split = FindPartitionPlane(primitives, over_box, under_box);

    if(need_to_split)
    {
        vector<PrimBox> over_primitives;
        vector<PrimBox> under_primitives;

        PrimBox primbox;

        for(vector<PrimBox>::const_iterator it = primitives.begin();
            it != primitives.end(); ++it)
        {
            // TODO - using bounding boxes like this is not optimal.  Instead,
            // what we need to do is create a box for the SpacePartition and
            // see if the primitives intersect it.

            primbox = *it;

            if(IsOverBoundary(primbox))
            {
                over_primitives.push_back(primbox);
            }
           
            if(IsUnderBoundary(primbox))
            {
                under_primitives.push_back(primbox);
            }
        }

        over = new SpacePartition((axis + 1) % 3);
        under = new SpacePartition((axis + 1) % 3);
        leaf_primitives = NULL;

        over->BuildFromList(over_primitives, over_box);
        under->BuildFromList(under_primitives, under_box);
    }
    else
    {
        over = NULL;
        under = NULL;
        leaf_primitives = new list<Primitive*>();

        for(unsigned int i = 0; i < primitives.size(); ++i)
        {
            leaf_primitives->push_back(primitives[i].prim);
        }
    }
}


bool SpacePartition::FindPartitionPlane(const vector<PrimBox>& primitives,
                                        BoundingBox& over_box, BoundingBox& under_box)
{
    // For each primitive...
    //      Suppose we put the plane on the right edge?
    //          What would be the surface area of the bounding box for the stuff on either side?
    //          And how many primitives would be contained therein?
    //          Figure the cost of this split as:
    //              surface area * # primitives, over side + surface area * # primitives, under side
    //      Do the same for the left edge...
    //      Is either of those costs the minimum so far?

    // So........  sort by right edge
    // Pick a splitting plane
    // Counting the primitives on either side is easy...
    // Is finding the bounding box hard?
    // For the right side, yes :(

    // So wait..... let's do it this way...

    // Sort by right edge
    // Loop through each primitive...
    //      For each, store the # of primitives on the left
    //      Also store the surface area at each step
    // Do the same thing, looping in the other direction
    // Now you have an easy way to find the cost of a split at any point

    // Oh wait, do we even need a sort here?
    // Yeah, you do....

    // Now about that sorting...
    // How you gonna do that?
    // C++ std::sort?
    // That's quicksort, no?  Which sucks on already-sorted data...
    // n/m, it's introsort, good.

    // Hang on, that's not going to work :/
    // When you sort by right edge, you'll be able to tell the cost of
    // the right-side box for any particular split, but there's no way
    // to know which primitives go on the left side...
    
    // Could we find that out by stepping through the left edge sorted
    // list?  Think so...

    // -----------------------------------------------------------------------------------

    // TODO - get this working
    /*
    vector<PrimBox> max_edge_sorted_prims(primitives);
    vector<PrimBox> min_edge_sorted_prims(primitives);

    sort(max_edge_sorted_prims.begin(), max_edge_sorted_prims.end(), CompareMaxEdge(axis));
    sort(min_edge_sorted_prims.begin(), min_edge_sorted_prims.end(), CompareMaxEdge(axis));

    int num_prims = primitives.size();

    vector<float> max_edge_costs(num_prims);
    vector<float> min_edge_costs(num_prims);

    // For each possible max-edge split, figure out the cost of each side

    PrimBox current_primbox;

    int over_side_count;
    int under_side_count = 0;
    float over_side_area;
    float under_side_area;

    float first_max_edge = max_edge_sorted_prims[0].box.GetMaxValue(axis);


    while(min_edge_sorted_prims[under_side_count].box.GetMinValue(axis) <= first_max_edge)
    {
        ++under_side_count;
    }

    for(int i = 0; i < num_prims; ++i)
    {
        // Ok, so we're going to try to put a splitting plane on the max edge
        // of this primitive...

        // The number of primitives over this plane is easy to figure out
        over_side_count = num_prims - i - 1;

        // To find the area of the over-primitives, we need to find a bounding box
        // for them.  This can shrink every time we move up a primitive...   

        // The number of primitives under it is a little harder, but we know it's
        // at least as many as there were for the previous plane
        while(min_edge_sorted_prims[under_side_count].box.GetMinValue(axis) <=
              max_edge_sorted_prims[i].box.GetMaxValue(axis))
        {
            ++under_side_count;
        }
    }




    // Out of all that stuff we just calculated, which is best?

    float min_cost = max_edge_costs[0];
    splitting_plane = max_edge_sorted_prims[0].box.GetMaxValue(axis);

    for(i = 1; i < max_edge_costs; ++i)
    {
        if(max_edge_costs[i] < min_cost)
        {
            min_cost = max_edge_costs[i];
            splitting_plane = max_edge_sorted_prims[0].box.GetMaxValue(axis);
        }
    }
    */
    


    // Crappy O(n^2) version  (makes baby Jesus cry)

    // This version will also over-estimate the bounding boxes of the over/under
    // sets, since it allows the boxes to cross the splitting plane.  That's bad.

    int num_prims = primitives.size();
    if(num_prims <= 3) return false;

    float lowest_cost = bounding_box.GetSurfaceArea() * num_prims * 0.99;
    bool split = false;
    float current_cost;

    BoundingBox trial_over_box, trial_under_box;

    for(int i = 0; i < num_prims; ++i)
    {
        current_cost = AttemptSplit(primitives, primitives[i].box.GetMaxValue(axis),
                                    trial_over_box, trial_under_box);

        if(current_cost < lowest_cost)
        {
            split = true;
            lowest_cost = current_cost;
            splitting_plane = primitives[i].box.GetMaxValue(axis);
            over_box = trial_over_box;
            under_box = trial_under_box;
        }

        current_cost = AttemptSplit(primitives, primitives[i].box.GetMinValue(axis),
                                    trial_over_box, trial_under_box);

        if(current_cost < lowest_cost)
        {
            split = true;
            lowest_cost = current_cost;
            splitting_plane = primitives[i].box.GetMinValue(axis);
            over_box = trial_over_box;
            under_box = trial_under_box;
        }
    }

    return split;
}



float SpacePartition::AttemptSplit(const vector<PrimBox>& primitives, float trial_plane,
                                   BoundingBox& over_box, BoundingBox& under_box)
{
    int num_prims = primitives.size();

    int over_count = 0;
    int under_count = 0;

    over_box.Zero();
    under_box.Zero();

    for(int j = 0; j < num_prims; ++j)
    {
        // See if we should add another primitive to the under-side set

        if(primitives[j].box.GetMinValue(axis) <= trial_plane)
        {
            ++under_count;

            if(under_count == 1)
            {
                under_box = primitives[j].box;
            }
            else
            {
                under_box.ExpandToFit(primitives[j].box);
            }
        }

        // See if we should add another primitive to the over-side set

        if(primitives[j].box.GetMaxValue(axis) > trial_plane)
        {
            ++over_count;

            if(over_count == 1)
            {
                over_box = primitives[j].box;
            }
            else
            {
                over_box.ExpandToFit(primitives[j].box);
            }

        }
    }

    // Calculate the cost of that whole thing...

    float cost = over_box.GetSurfaceArea() * over_count +
                 under_box.GetSurfaceArea() * under_count;

    return cost;
}



bool SpacePartition::IsOverBoundary(const PrimBox& primbox)
{
    return (primbox.box.GetMaxValue(axis) > splitting_plane);
}

bool SpacePartition::IsUnderBoundary(const PrimBox& primbox)
{
    return (primbox.box.GetMinValue(axis) <= splitting_plane);
}


void SpacePartition::MarkPrimitives()
{
    if(leaf_primitives)
    {
        for(list<Primitive*>::iterator it = leaf_primitives->begin();
            it != leaf_primitives->end(); ++it)
        {
            (*it)->SetMark(true);
        }
    }
    else
    {
        over->MarkPrimitives();
        under->MarkPrimitives();
    }
}


size_t SpacePartition::GetSize()
{
    if(leaf_primitives)
    {
        return leaf_primitives->size();
    }
    else
    {
        return over->GetSize() + under->GetSize();
    }
}


double SpacePartition::TraceRay(const Point& src, const Vector& ray, double hither,
                                Primitive*& object, Vector& normal)
{
    double result = -1.0;

    // Does the ray hit the bounding box for this partition?

    if(bounding_box.IntersectRay(src, ray, hither))
    {
        // If this is a leaf, intersect all the prmitives

        if(leaf_primitives)
        {
            bool intersected = false;
            double distance;
            double min = -1;
            Vector temp_normal;

            for(list<Primitive*>::iterator it = leaf_primitives->begin();
                it != leaf_primitives->end(); ++it)
            {
                distance = (*it)->IntersectRay(src, ray, hither, temp_normal);

                if(distance >= hither)
                {
                    if(!intersected || (distance < min))
                    {
                        intersected = true;
                        min = distance;
                        object = *it;
                        normal = temp_normal;
                    }
                }
            }

            result = min;
        }
        else
        {
            // Not a leaf node, so intersect the two sub-partitions.
            // First one is chosen based on which side of the splitting
            // plane the ray starts on.

            if(src.GetValue(axis) < splitting_plane)
            {
                result = under->TraceRay(src, ray, hither, object, normal);
                // Need to check the other side of the partition in two cases:
                // - If the ray didn't hit anything, obviously
                // - If the ray hit something that spans both halves and the
                //   intersection is on the other side of the splitting plane;
                //   in this case we can't say whether or not the found
                //   intersection is actually the closest one

                if((result < hither)
                || ((src + ray * result).GetValue(axis) > splitting_plane))
                {
                    result = over->TraceRay(src, ray, hither, object, normal);
                }
            }
            else
            {
                result = over->TraceRay(src, ray, hither, object, normal);

                if((result < hither)
                || ((src + ray * result).GetValue(axis) < splitting_plane))
                {
                    result = under->TraceRay(src, ray, hither, object, normal);
                }
            }
        }
    }

    return result;
}


