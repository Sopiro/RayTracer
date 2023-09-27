#pragma once

#include "aabb.h"
#include "growable_array.h"
#include "intersectable.h"

#define nullNode (-1)

namespace spt
{

constexpr Vec3 aabb_margin{ Float(0.0) };
constexpr Float aabb_multiplier{ Float(1.0) };

class Intersectable;

class BVH
{
public:
    using Data = Intersectable;

    struct Node
    {
        bool IsLeaf() const
        {
            return child1 == nullNode;
        }

        int32 id;
        AABB aabb;

        NodeProxy parent;
        NodeProxy child1;
        NodeProxy child2;

        NodeProxy next;
        bool moved;

        Data* data; // user data
    };

    BVH();

    virtual ~BVH() noexcept;

    BVH(const BVH&) = delete;
    BVH& operator=(const BVH&) = delete;

    BVH(BVH&&) noexcept;
    BVH& operator=(BVH&&) noexcept;

    void Reset();

    NodeProxy CreateNode(Data* data, const AABB& aabb);
    bool MoveNode(NodeProxy node, AABB aabb, const Vec3& displacement, bool force_move);
    void RemoveNode(NodeProxy node);

    bool TestOverlap(NodeProxy nodeA, NodeProxy nodeB) const;
    const AABB& GetAABB(NodeProxy node) const;
    void ClearMoved(NodeProxy node) const;
    bool WasMoved(NodeProxy node) const;
    Data* GetData(NodeProxy node) const;

    void Query(const Vec3& point, const std::function<bool(NodeProxy, Data*)>& callback) const;
    void Query(const AABB& aabb, const std::function<bool(NodeProxy, Data*)>& callback) const;
    template <typename T>
    void Query(const Vec3& point, T* callback) const;
    template <typename T>
    void Query(const AABB& aabb, T* callback) const;

    void RayCast(const Ray& r,
                 Float t_min,
                 Float t_max,
                 const std::function<Float(const Ray&, Float, Float, Intersectable*)>& callback) const;
    template <typename T>
    void RayCast(const Ray& r, Float t_min, Float t_max, T* callback) const;

    void Traverse(const std::function<void(const Node*)>& callback) const;
    template <typename T>
    void Traverse(T* callback) const;

    Float GetTreeCost() const;

    void Rebuild();

private:
    friend class Aggregate;
    friend class Scene;

    NodeProxy nodeID;
    NodeProxy root;

    Node* nodes;
    int32 nodeCount;
    int32 nodeCapacity;

    NodeProxy freeList;

    NodeProxy AllocateNode();
    void FreeNode(NodeProxy node);

    NodeProxy InsertLeaf(NodeProxy leaf);
    void RemoveLeaf(NodeProxy leaf);

    void Rotate(NodeProxy node);
    void Swap(NodeProxy node1, NodeProxy node2);

    static Float SAH(const AABB& aabb);
};

inline bool BVH::TestOverlap(NodeProxy nodeA, NodeProxy nodeB) const
{
    assert(0 <= nodeA && nodeA < nodeCapacity);
    assert(0 <= nodeB && nodeB < nodeCapacity);

    return nodes[nodeA].aabb.TestOverlap(nodes[nodeB].aabb);
}

inline const AABB& BVH::GetAABB(NodeProxy node) const
{
    assert(0 <= node && node < nodeCapacity);

    return nodes[node].aabb;
}

inline void BVH::ClearMoved(NodeProxy node) const
{
    assert(0 <= node && node < nodeCapacity);

    nodes[node].moved = false;
}

inline bool BVH::WasMoved(NodeProxy node) const
{
    assert(0 <= node && node < nodeCapacity);

    return nodes[node].moved;
}

inline BVH::Data* BVH::GetData(NodeProxy node) const
{
    assert(0 <= node && node < nodeCapacity);

    return nodes[node].data;
}

inline Float BVH::GetTreeCost() const
{
    Float cost = Float(0.0);

    Traverse([&cost](const Node* node) -> void { cost += SAH(node->aabb); });

    return cost;
}

template <typename T>
void BVH::Query(const Vec3& point, T* callback) const
{
    if (root == nullNode)
    {
        return;
    }

    GrowableArray<NodeProxy, 256> stack;
    stack.Emplace(root);

    while (stack.Count() != 0)
    {
        NodeProxy current = stack.Pop();

        if (nodes[current].aabb.TestPoint(point) == false)
        {
            continue;
        }

        if (nodes[current].IsLeaf())
        {
            bool proceed = callback->QueryCallback(current, nodes[current].data);
            if (proceed == false)
            {
                return;
            }
        }
        else
        {
            stack.Emplace(nodes[current].child1);
            stack.Emplace(nodes[current].child2);
        }
    }
}

template <typename T>
void BVH::Query(const AABB& aabb, T* callback) const
{
    if (root == nullNode)
    {
        return;
    }

    GrowableArray<NodeProxy, 256> stack;
    stack.Emplace(root);

    while (stack.Count() != 0)
    {
        NodeProxy current = stack.Pop();

        if (nodes[current].aabb.TestOverlap(aabb) == false)
        {
            continue;
        }

        if (nodes[current].IsLeaf())
        {
            bool proceed = callback->QueryCallback(current, nodes[current].data);
            if (proceed == false)
            {
                return;
            }
        }
        else
        {
            stack.Emplace(nodes[current].child1);
            stack.Emplace(nodes[current].child2);
        }
    }
}

template <typename T>
void BVH::Traverse(T* callback) const
{
    if (root == nullNode)
    {
        return;
    }

    GrowableArray<NodeProxy, 256> stack;
    stack.Emplace(root);

    while (stack.Count() != 0)
    {
        NodeProxy current = stack.Pop();

        if (!nodes[current].IsLeaf())
        {
            stack.Emplace(nodes[current].child1);
            stack.Emplace(nodes[current].child2);
        }

        const Node* node = nodes + current;
        callback->TraverseCallback(node);
    }
}

template <typename T>
void BVH::RayCast(const Ray& r, Float t_min, Float t_max, T* callback) const
{
    Vec3 p1 = r.At(t_min);
    Vec3 p2 = r.At(t_max);
    Float t = t_max;

    AABB rayAABB;
    rayAABB.min = Min(p1, p2);
    rayAABB.max = Max(p1, p2);

    GrowableArray<int32, 256> stack;
    stack.Emplace(root);

    while (stack.Count() > 0)
    {
        int32 nodeID = stack.Pop();
        if (nodeID == nullNode)
        {
            continue;
        }

        const Node* node = nodes + nodeID;
        if (node->aabb.TestOverlap(rayAABB) == false)
        {
            continue;
        }

        if (node->aabb.Intersect(r, t_min, t) == false)
        {
            continue;
        }

        if (node->IsLeaf())
        {
            Float value = callback->RayCastCallback(r, t_min, t, node->data);
            if (value <= t_min)
            {
                return;
            }
            else
            {
                // Update ray AABB
                t = value;
                Vec3 newEnd = r.At(t);

                rayAABB.min = Min(p1, newEnd);
                rayAABB.max = Max(p1, newEnd);
            }
        }
        else
        {
            stack.Emplace(node->child1);
            stack.Emplace(node->child2);
        }
    }
}

inline Float BVH::SAH(const AABB& aabb)
{
#if 0
    return aabb.GetVolume();
#else
    return aabb.GetSurfaceArea();
#endif
}

} // namespace spt