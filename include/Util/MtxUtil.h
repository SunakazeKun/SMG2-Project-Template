#pragma once

#include "JGeometry/TVec3.h"
#include "JGeometry/TPosition3.h"
#include "JGeometry/TMatrix34.h"

#include "kamek.h"

namespace MR
{
    void makeMtxTRS(Mtx, f32, f32, f32, f32, f32, f32, f32, f32, f32);
    void makeMtxTRS(Mtx, const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&);

    void makeMtxFrontUpPos(JGeometry::TPosition3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > > *, const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &);
    Mtx* tmpMtxRotYDeg(f32);

    void extractMtxYDir(Mtx, JGeometry::TVec3<f32>*);
    void extractMtxZDir(Mtx, JGeometry::TVec3<f32>*);
};