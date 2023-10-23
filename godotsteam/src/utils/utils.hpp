#pragma once

#include <algorithm>


namespace wbi 
{
    template<typename T, typename Iterator, typename Predicate>
    T* ptrfind_if(Iterator begin, Iterator end, Predicate predicate)
    {
        auto it = std::find_if(container.begin(), container.end(), predicate);
        if(it == end)
        {
            return nullptr;
        }
        return &*it;
    }
    
}