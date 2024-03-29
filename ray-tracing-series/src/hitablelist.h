/**
 * MIT License
 * Copyright (c) 2019 Guillaume Riby <guillaumeriby@gmail.com>
 *
 * GitHub repository - https://github.com/griby/ray-tracing-series
 *
 * A ray tracer implementation based on the Ray Tracing in One Weekend Book Series by Peter Shirley - https://raytracing.github.io/
 */

#pragma once

#include <memory>
#include <vector>

#include "hitable.h"

namespace rts // for ray tracing series
{
    class HitableList final : public Hitable
    {
    public:
        HitableList() : m_list() {}

        void reserve(std::size_t capacity) { m_list.reserve(capacity); }
        void add(std::unique_ptr<const Hitable> value) { m_list.push_back(std::move(value)); }

        virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;

    private:
        std::vector<std::unique_ptr<const Hitable>> m_list;
    };
}
