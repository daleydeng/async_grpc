/*
 * Copyright 2018 The Cartographer Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CPP_GRPC_RETRY_H
#define CPP_GRPC_RETRY_H

#include <set>

#include "async_grpc/common/optional.h"
#include "async_grpc/common/time.h"
#include "grpcpp/grpcpp.h"

namespace async_grpc {

using common::Duration;
using common::optional;

using RetryStrategy = std::function<optional<Duration>(
    int /* failed_attempts */, const ::grpc::Status &)>;
using RetryIndicator =
    std::function<bool(int /* failed_attempts */, const ::grpc::Status &)>;
using RetryDelayCalculator = std::function<Duration(int /* failed_attempts */)>;

RetryStrategy CreateRetryStrategy(RetryIndicator retry_indicator,
                                  RetryDelayCalculator retry_delay_calculator);

RetryIndicator CreateLimitedRetryIndicator(int max_attempts);
RetryIndicator CreateUnlimitedRetryIndicator();
RetryIndicator CreateUnlimitedRetryIndicator(
    const std::set<::grpc::StatusCode> &unrecoverable_codes);
RetryDelayCalculator CreateBackoffDelayCalculator(Duration min_delay,
                                                  float backoff_factor);
RetryDelayCalculator CreateConstantDelayCalculator(Duration delay);
RetryStrategy CreateLimitedBackoffStrategy(Duration min_delay,
                                           float backoff_factor,
                                           int max_attempts);
RetryStrategy CreateUnlimitedConstantDelayStrategy(Duration delay);
RetryStrategy CreateUnlimitedConstantDelayStrategy(
    Duration delay, const std::set<::grpc::StatusCode> &unrecoverable_codes);

bool RetryWithStrategy(RetryStrategy retry_strategy,
                       std::function<::grpc::Status()> op,
                       std::function<void()> reset = nullptr);

}  // namespace async_grpc

#endif  // CPP_GRPC_RETRY_H
