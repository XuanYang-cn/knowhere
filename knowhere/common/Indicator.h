// Copyright (C) 2019-2020 Zilliz. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under the License

#pragma once

#include <atomic>
#include <vector>

namespace knowhere {

enum IndicatorType {
    DISK_IO_NUM = 0,
    MAX_INDICATOR_TYPE,
};

class Indicator {
 public:
    Indicator() : value_(0) {
    }

    int64_t
    Get() {
        return value_.load();
    }

    void
    Set(int64_t v) {
        value_.store(v);
    }

    void
    Inc() {
        Add(1);
    }

    void
    Dec() {
        Sub(1);
    }

    void
    Add(int64_t v) {
        value_.fetch_add(v);
    }

    void
    Sub(int64_t v) {
        value_.fetch_sub(v);
    }

 private:
    std::atomic<int64_t> value_;
};

class IndicatorCollector {
 public:
    IndicatorCollector(): data_(MAX_INDICATOR_TYPE) {
    }

    static IndicatorCollector&
    GetInstance() {
        static IndicatorCollector instance;
        return instance;
    }

    Indicator&
    Get(IndicatorType idx) {
        return data_.at(idx);
    }

 private:
    std::vector<Indicator> data_;
};

}  // namespace knowhere
