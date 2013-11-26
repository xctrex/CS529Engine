#include "Containers.h"

namespace Framework
{
    template <typename T>
    std::unique_ptr<T> p_to_unique_ptr(T* p){ return std::unique_ptr<T>(p); }
}
