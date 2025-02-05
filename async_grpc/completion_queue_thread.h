/*
 * Copyright 2017 The Cartographer Authors
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

#ifndef CPP_GRPC_COMMON_COMPLETION_QUEUE_THREAD_H_
#define CPP_GRPC_COMMON_COMPLETION_QUEUE_THREAD_H_

#include <grpcpp/grpcpp.h>
#include <memory>
#include <thread>

namespace async_grpc {

class CompletionQueueThread {
 public:
  using CompletionQueueRunner =
      std::function<void(::grpc::ServerCompletionQueue*)>;

  explicit CompletionQueueThread(
      std::unique_ptr<::grpc::ServerCompletionQueue> completion_queue);

  ::grpc::ServerCompletionQueue* completion_queue();

  void Start(CompletionQueueRunner runner);
  void Shutdown();

 private:
  std::unique_ptr<::grpc::ServerCompletionQueue> completion_queue_;
  std::unique_ptr<std::thread> worker_thread_;
};

}  // namespace async_grpc

#endif  // CPP_GRPC_COMMON_COMPLETION_QUEUE_THREAD_H_
