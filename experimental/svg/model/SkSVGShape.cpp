/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkSVGRenderContext.h"
#include "SkSVGShape.h"

SkSVGShape::SkSVGShape(SkSVGTag t) : INHERITED(t) {}

void SkSVGShape::onRender(const SkSVGRenderContext& ctx) const {
    if (const SkPaint* fillPaint = ctx.presentationContext().fillPaint()) {
        this->onDraw(ctx.canvas(), ctx.lengthContext(), *fillPaint);
    }

    if (const SkPaint* strokePaint = ctx.presentationContext().strokePaint()) {
        this->onDraw(ctx.canvas(), ctx.lengthContext(), *strokePaint);
    }
}

void SkSVGShape::appendChild(sk_sp<SkSVGNode>) {
    SkDebugf("cannot append child nodes to an SVG shape.\n");
}
