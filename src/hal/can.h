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
 * @file  hal/can.h
 * @brief 根据HAL库的宏定义决定Can是bxCAN还是fdCAN，并且在can_interface.h里提供一个接口类CanBase实现多态
 */

#ifndef EC_LIB_HAL_CAN_H
#define EC_LIB_HAL_CAN_H

#include "bxcan.h"
#include "fdcan.h"

namespace irobot_ec::hal {
#if defined(HAL_CAN_MODULE_ENABLED)
using Can = BxCan;
#elif defined(HAL_FDCAN_MODULE_ENABLED)
using Can = FdCan;  // TODO: 实现FdCan类
#endif
}  // namespace irobot_ec::hal

#endif  // EC_LIB_HAL_CAN_H
