//============================================================================
// Name            : tree.hpp
// Author          : Wolkow Daniel
// Description     : This hpp implements set container
//============================================================================
#ifndef MTL_TREE_HPP 
#define MTL_TREE_HPP 1

#include <memory>

namespace bst 
{
    template <
        typename KeyT, typename Allocator = std::allocator<KeyT> 
                >
    class BaseTree 
    {
    public:
        //----------------------------------------------------------------------------------
        typedef KeyT                                                        key_type;
        typedef KeyT                                                        value_type;
        typedef size_t                                                      size_type;
        typedef std::ptrdiff_t                                              difference_type;
        typedef typename std::allocator_traits<Allocator>::pointer          pointer;
        typedef typename std::allocator_traits<Allocator>::const_pointer    const_pointer;
        //----------------------------------------------------------------------------------
    };
}
#endif
