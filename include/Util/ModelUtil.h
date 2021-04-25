#pragma once

class LiveActor;
class J3DModel;

namespace MR
{
    bool isExistCollisionResource(const LiveActor *, const char *);

    J3DModel* getJ3DModel(const LiveActor *);
};