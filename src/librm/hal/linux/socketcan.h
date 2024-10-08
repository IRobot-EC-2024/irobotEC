/*
  Copyright (c) 2024 XDU-IRobot

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

/**
 * @file  librm/hal/linux/socketcan.h
 * @brief SocketCAN类库
 * @todo  实现tx消息队列
 */

#ifndef LIBRM_HAL_LINUX_SOCKETCAN_H
#define LIBRM_HAL_LINUX_SOCKETCAN_H

#include <string>
#include <deque>
#include <memory>
#include <unordered_map>
#include <mutex>

#include <fcntl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "librm/device/can_device.hpp"
#include "librm/hal/can_interface.h"
#include "librm/core/thread_pool.hpp"

namespace rm::hal::linux_ {

/**
 * @brief CAN设备回调锁
 * @note  SocketCan类会异步地调用不同设备的RxCallback函数，为了保证线程安全，需要分别给每个设备加锁
 */
struct AsyncCanDevice {
  AsyncCanDevice(device::CanDevice &dev) : dev(&dev) {}
  std::mutex mutex;
  device::CanDevice *dev;
};

class SocketCan : public hal::CanInterface {
 public:
  explicit SocketCan(const char *dev);
  SocketCan() = default;
  ~SocketCan() override;

  // 禁止拷贝构造
  SocketCan(const SocketCan &) = delete;
  SocketCan &operator=(const SocketCan &) = delete;

  void SetFilter(u16 id, u16 mask) override;
  void Write(u16 id, const u8 *data, usize size) override;
  void Write() override;
  void Enqueue(u16 id, const u8 *data, usize size, CanTxPriority priority) override;
  void Begin() override;
  void Stop() override;

 private:
  void RecvThread();
  void SendThread();
  void RxCallbackCallWorker(std::unique_ptr<struct ::can_frame> msg);
  void RegisterDevice(device::CanDevice &device, u32 rx_stdid) override;

  int socket_fd_;
  struct ::sockaddr_can addr_;
  struct ::ifreq interface_request_;
  struct ::can_filter filter_;
  struct ::can_frame tx_buf_;
  std::string dev_;
  CanMsg rx_buffer_{};
  std::unique_ptr<core::ThreadPool> thread_pool_{};  // 线程池，用于异步调用设备的回调函数
  std::unordered_map<CanTxPriority, std::deque<std::shared_ptr<CanMsg>>> tx_queue_{
      {CanTxPriority::kHigh, {}},
      {CanTxPriority::kNormal, {}},
      {CanTxPriority::kLow, {}},
  };  // <priority, queue>
  std::unordered_map<u16, AsyncCanDevice *> device_list_{};  // <rx_stdid, device+lock>

  /**
   * @brief 消息队列最大长度
   * @note  在插入或发送消息的时候，如果检测到消息队列长度超过这个值，就会清空消息队列（待发送的消息都会被丢弃）
   * @note  触发清空队列的动作意味着插入消息的速度大于发送消息的速度，应该减少发送消息的数量
   */
  static constexpr usize kQueueMaxSize = 100;

  /**
   * @brief 最大线程数
   * @note  用于创建线程池
   */
  static constexpr usize kMaxThreads = 20;
};

}  // namespace rm::hal::linux_

#endif