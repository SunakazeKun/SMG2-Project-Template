#pragma once

#include "JMap/JMapInfoIter.h"

namespace MR
{
    bool getJMapInfoArg0NoInit(const JMapInfoIter &iter, s32 *out);
    bool getJMapInfoArg0NoInit(const JMapInfoIter &iter, f32 *out);
    bool getJMapInfoArg0NoInit(const JMapInfoIter &iter, bool *out);

    bool getJMapInfoArg1NoInit(const JMapInfoIter &iter, s32 *out);
    bool getJMapInfoArg1NoInit(const JMapInfoIter &iter, f32 *out);
    bool getJMapInfoArg1NoInit(const JMapInfoIter &iter, bool *out);

    bool getJMapInfoArg2NoInit(const JMapInfoIter &iter, s32 *out);
    bool getJMapInfoArg2NoInit(const JMapInfoIter &iter, f32 *out);
    bool getJMapInfoArg2NoInit(const JMapInfoIter &iter, bool *out);

    bool getJMapInfoArg3NoInit(const JMapInfoIter &iter, s32 *out);
    bool getJMapInfoArg3NoInit(const JMapInfoIter &iter, f32 *out);
    bool getJMapInfoArg3NoInit(const JMapInfoIter &iter, bool *out);

    bool getJMapInfoArg4NoInit(const JMapInfoIter &iter, s32 *out);
    bool getJMapInfoArg4NoInit(const JMapInfoIter &iter, f32 *out);
    bool getJMapInfoArg4NoInit(const JMapInfoIter &iter, bool *out);

    bool getJMapInfoArg5NoInit(const JMapInfoIter &iter, s32 *out);
    bool getJMapInfoArg5NoInit(const JMapInfoIter &iter, f32 *out);
    bool getJMapInfoArg5NoInit(const JMapInfoIter &iter, bool *out);

    bool getJMapInfoArg6NoInit(const JMapInfoIter &iter, s32 *out);
    bool getJMapInfoArg6NoInit(const JMapInfoIter &iter, f32 *out);
    bool getJMapInfoArg6NoInit(const JMapInfoIter &iter, bool *out);

    bool getJMapInfoArg7NoInit(const JMapInfoIter &iter, s32 *out);
    bool getJMapInfoArg7NoInit(const JMapInfoIter &iter, f32 *out);
    bool getJMapInfoArg7NoInit(const JMapInfoIter &iter, bool *out);

    bool isObjectName(const JMapInfoIter &, const char *);
};