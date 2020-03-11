/**
 * @brief Simple wrapper for Qt-style operators for std::vector<>
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#ifndef _STL_UTILITIES
	#define _STL_UTILITIES

#include <vector>

namespace std
{

// Qt-style vector stuff
// Generalize for other push_back'able containers?
template<typename T, typename Item>
inline vector<T>& operator<<(vector<T> &v, const Item &item)
{
    v.push_back(item);
    return v;
}

template<typename T, typename Item>
inline vector<T>& operator<<(vector<T> &v, const vector<Item> &other)
{
    std::copy(other.begin(), other.end(), back_inserter(v));
    return v;
}

} // namespace std

#endif // _STL_UTILITIES
