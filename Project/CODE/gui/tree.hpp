#ifndef TREE_H
#define TREE_H

#include "layout.hpp"
#include <vector>

struct TreeNode {
    struct LayoutBase *Parent;
    std::vector<struct LayoutBase *>Children;
};

#endif
