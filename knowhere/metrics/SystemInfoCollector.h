// Copyright (C) 2019-2020 Zilliz. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under the License.

#pragma once

#include <mutex>
#include <thread>

#include "metrics/Prometheus.h"

#define GAUGE(name, description)                  \
    prometheus::Family<prometheus::Gauge>& name = \
        prometheus::BuildGauge().Name(cluster_name + #name).Help(description).Register(prometheus.registry());

namespace knowhere {

class SystemInfoCollector {
 public:
    static SystemInfoCollector&
    GetInstance() {
        static SystemInfoCollector instance;
        return instance;
    }

    void
    Start();

    void
    Stop();

 private:
    SystemInfoCollector() = default;

    void
    collector_function();

    double
    cpu_utilization_ratio();

    double
    cpu_temperature_celsius();

    double
    mem_usage();

    double
    mem_available();

    double
    network_receive_total();

    double
    network_transport_total();

 private:
    bool running_ = false;
    std::mutex mutex_;

    std::thread collector_thread_;

    int64_t base_network_in_octets_ = 0;
    int64_t base_network_out_octets_ = 0;

    clock_t base_cpu_ = clock_t();
    clock_t base_sys_cpu_ = clock_t();
    clock_t base_user_cpu_ = clock_t();

    /* metrics */
    template <typename T>
    using Family = ::prometheus::Family<T>;
    using Gauge = ::prometheus::Gauge;
    using Counter = ::prometheus::Counter;

    Family<Counter>& knowhere_keeping_alive_ = ::prometheus::BuildCounter()
                                                   .Name("knowhere_uptime")
                                                   .Help("total seconds of the serve alive")
                                                   .Register(Prometheus::GetInstance().registry());
    Counter& knowhere_keeping_alive_counter_ = knowhere_keeping_alive_.Add({});

    /* cpu_utilization_ratio */
    Family<Gauge>& knowhere_cpu_util_ratio_ = ::prometheus::BuildGauge()
                                                  .Name("knowhere_cpu_util_ratio")
                                                  .Help("knowhere cpu utilization ratio")
                                                  .Register(Prometheus::GetInstance().registry());
    Gauge& knowhere_cpu_util_ratio_gauge_ = knowhere_cpu_util_ratio_.Add({});

    Family<Gauge>& knowhere_cpu_temp_ = ::prometheus::BuildGauge()
                                            .Name("knowhere_cpu_temp_celsius")
                                            .Help("cpu temperature")
                                            .Register(Prometheus::GetInstance().registry());
    Gauge& knowhere_cpu_temp_gauge_ = knowhere_cpu_temp_.Add({});

    Family<Gauge>& knowhere_mem_usage_ = ::prometheus::BuildGauge()
                                             .Name("knowhere_mem_usage")
                                             .Help("memory usage")
                                             .Register(Prometheus::GetInstance().registry());
    Gauge& knowhere_mem_usage_gauge_ = knowhere_mem_usage_.Add({});

    Family<Gauge>& knowhere_mem_avail_ = ::prometheus::BuildGauge()
                                             .Name("knowhere_mem_available")
                                             .Help("memory available")
                                             .Register(Prometheus::GetInstance().registry());
    Gauge& knowhere_mem_avail_gauge_ = knowhere_mem_avail_.Add({});

    Family<Gauge>& knowhere_network_rx_bytes_total_ = ::prometheus::BuildGauge()
                                                          .Name("knowhere_network_rx_bytes_total")
                                                          .Help("network total receive bytes")
                                                          .Register(Prometheus::GetInstance().registry());
    Gauge& knowhere_network_rx_bytes_total_gauge_ = knowhere_network_rx_bytes_total_.Add({});

    Family<Gauge>& knowhere_network_tx_bytes_total_ = ::prometheus::BuildGauge()
                                                          .Name("knowhere_network_tx_bytes_total")
                                                          .Help("network total transport bytes")
                                                          .Register(Prometheus::GetInstance().registry());
    Gauge& knowhere_network_tx_bytes_total_gauge_ = knowhere_network_tx_bytes_total_.Add({});
};

}  // namespace knowhere
