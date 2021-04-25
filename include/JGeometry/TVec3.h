#pragma once

namespace JGeometry
{
    void negateInternal(const f32 *, f32 *);

    template<typename T>
    class TVec3
    {
    public:
        TVec3() { }
        TVec3(T);
        TVec3(const TVec3<T> &);
        TVec3(T a, T b, T c) : x(a), y(b), z(c) { }

        template<typename T>
        TVec3(T, T, T);
        
        f32 dot(const JGeometry::TVec3<T> &) const;
        void add(const JGeometry::TVec3<T> &) const;

        template<typename T>
        void set(const TVec3<T> &);
    
        template<typename T>
        void set(T, T, T);

        void set(TVec3<T> const &);
        void zero();

        bool epsilonEquals(const TVec3<T> &, T) const;

        void sub(const TVec3<T> &);
        T squared() const;

        void operator =(const TVec3<T> &);
        TVec3<T> operator +(const TVec3<T>&) const;
        TVec3<T> operator +=(const TVec3<T> &);
        TVec3<T> operator -(const TVec3<T> &); // mi
        TVec3<T> operator -=(const TVec3<T> &); // ami
        TVec3<T> operator *=(T); // amu
        TVec3<T> operator *(T) const; // ml

        void scale(T);
        void scale(JGeometry::TVec3<T> &, T);
        T x; // _0
        T y; // _4
        T z; // _8
    };
};