// #pragma once

// #include <functional>
// #include <memory>

// namespace math {
//     class MathMaster {
//         float sin(const float x);
//         float cos(const float x);
//         float tan(const float x);
//     };

//     // Mod function for floating-point types using std::fmod
//     template <typename T>
//     typename std::enable_if<std::is_floating_point<T>::value, T>::type mod(const T a, const T b) {
//         #ifndef RELEASE
//         if (0 == b) {
//             throw std::runtime_error("Division by zero");
//         }
//         #endif
//         return std::fmod(a, b);
//     }

//     // Mod function for integer-like types using %
//     template <typename T>
//     typename std::enable_if<std::is_integral<T>::value, T>::type mod(const T a, const T b) {
//         #ifndef RELEASE
//         if (0 == b) {
//             throw std::runtime_error("Division by zero");
//         }
//         #endif
//         return a % b;
//     }

//     template <class T> class LookUpTable {
//       public:
//         LookUpTable(
//             const std::function<T(T)> &function, const size_t size,
//             const T start, const T end, const std::function<T(T, T, T)> &rebound
//         )
//             : _rebound(rebound), _start(start), _end(end), _size(size) {
//             _step = (end - start) / static_cast<T>(size - 1);
//             _v = std::make_unique<T[]>(size);

//             for (size_t i = 0; i < size; ++i) {
//                 T x = start + i * _step;
//                 _v[i] = function(x);
//             }
//         }

//         T interpolate (const T x) const {
//             // Linearly interpolate between two closest values in the table
//             size_t index = static_cast<size_t>((x - _start) / _step);

//             T x1 = _start + index * _step;
//             T x2 = _start + (index + 1) * _step;

//             T y1 = _v[index];
//             T y2 = _v[index + 1];

//             // Linear interpolation
//             return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
//         }

//         T get_raw (const T x) const {
//             return _v[static_cast<size_t>(round((x - _start) / _step))];
//         }

//         T get_rounded (const T x) const {
//             return _v[static_cast<size_t>((x - _start) / _step)];
//         }

//         T get_by_index (const size_t i) const {
//             return _v[i];
//         }

//         T get(const T x) const {
//             T _x = (x >= _end || x < _start) ? _rebound(x, _start, _end) : x;
//             return interpolate(_x);
//         }

//         T operator(const T x) const {
//             return get(x);
//         }

//       private:
//         std::function<T(T, T, T)> _rebound;
//         T _start;
//         T _end;
//         T _step;
//         size_t _size;
//         std::unique_ptr<T[]> _v;
//     };
// } // namespace utils