/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "RecordingBench.h"
#include "SkBBHFactory.h"
#include "SkLiteDL.h"
#include "SkLiteRecorder.h"
#include "SkPictureRecorder.h"

RecordingBench::RecordingBench(const char* name, const SkPicture* pic, bool useBBH, bool lite)
    : fName(name)
    , fUseBBH(useBBH)
    , fLite(lite) {
    // Flatten the source picture in case it's trivially nested (useless for timing).
    SkPictureRecorder rec;
    pic->playback(rec.beginRecording(pic->cullRect(), nullptr,
                                     SkPictureRecorder::kPlaybackDrawPicture_RecordFlag));
    fSrc = rec.finishRecordingAsPicture();
}

const char* RecordingBench::onGetName() {
    return fName.c_str();
}

bool RecordingBench::isSuitableFor(Backend backend) {
    return backend == kNonRendering_Backend;
}

SkIPoint RecordingBench::onGetSize() {
    return SkIPoint::Make(SkScalarCeilToInt(fSrc->cullRect().width()),
                          SkScalarCeilToInt(fSrc->cullRect().height()));
}

void RecordingBench::onDraw(int loops, SkCanvas*) {
    if (fLite) {
        SkLiteRecorder rec;
        while (loops --> 0) {
            sk_sp<SkLiteDL> dl = SkLiteDL::New(fSrc->cullRect());
            rec.reset(dl.get());
            fSrc->playback(&rec);
            dl->makeThreadsafe();
        }

    } else {
        SkRTreeFactory factory;
        SkPictureRecorder recorder;
        while (loops --> 0) {
            fSrc->playback(recorder.beginRecording(fSrc->cullRect(), fUseBBH ? &factory : nullptr));
            (void)recorder.finishRecordingAsPicture();
        }
    }
}
