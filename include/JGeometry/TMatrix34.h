#pragma once

#include "JGeometry/TVec3.h"
#include "kamek.h"

namespace JGeometry
{
    template<typename T>
    class TMatrix34
    {
    public:
        void identity();
        void mult(const JGeometry::TVec3<f32> &, JGeometry::TVec3<f32> &);
        void concat(const T &);
        void concat(const T&, const T&);
    
        T val[3][4];
    };
    
    template<typename T>
    class SMatrix34C
    {
    public:
        /* empty */
    };
};