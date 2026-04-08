#pragma once

#include <inno_driver/common/inno_common.hpp>

#include <queue>
#include <mutex>
#include <thread>
namespace innolight
{
namespace lidar
{
inline void inno_msleep(uint64_t ms)
{
  std::this_thread::sleep_for(std::chrono::microseconds(ms));
}
template <typename T>
class SyncQueue
{
public:
  inline size_t Push(const T& value)
  {
    m_mtx.lock();
    m_queue.push(value);
    m_size = m_queue.size();
    m_mtx.unlock();

    return m_size;
  }
  inline size_t GetSize() const
  {
    return m_size;
  }

  inline T Pop()
  {
    T value = NULL;

    m_mtx.lock();
    if (!m_queue.empty())
    {
      value = m_queue.front();
      m_queue.pop();
    }
    m_mtx.unlock();

    return value;
  }

  inline T PopWait(uint32_t msec = 1)
  {
    T value = NULL;
    bool empty = true;

    m_mtx.lock();
    if (!m_queue.empty())
    {
      value = m_queue.front();
      m_queue.pop();
      empty = false;
    }
    m_mtx.unlock();

    if (empty)
    {
	    inno_msleep(msec);
    }

    return value;
  }

  inline void Clear()
  {
    std::queue<T> empty;

    m_mtx.lock();
    std::swap(empty, m_queue);
    m_mtx.unlock();
  }

private:
  size_t        m_size;
  std::mutex    m_mtx;
  std::queue<T> m_queue;
};
}  // namespace lidar
}  // namespace innolight
