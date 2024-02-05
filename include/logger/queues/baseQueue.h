#ifndef GCL_LOGGER_QUEUES_BASEQUEUE_H
#define GCL_LOGGER_QUEUES_BASEQUEUE_H

namespace GCL::logger::queues
{
  class CBaseQueue
  {
    public:
      void pushBack(std::unique_ptr<CRecord> &&);
      std::unique_ptr<CRecord> popFront();
      
      bool empty() const noexcept;
      
    protected:
      using mutex_type = std::shared_mutex;
      using uniqueLock = std::unique_lock<mutex_type>;
      using sharedLock = std::shared_lock<mutex_type>;
    
      mutable mutex_type queueMutex;
      
    private:      
    
      virtual void processPushBack(std::unique_ptr<CRecord> &&) = 0;
      virtual std::unique_ptr<CRecord> processPopFront() = 0;
      virtual bool processEmpty() = 0;
  }
}

#endif // GCL_LOGGER_QUEUES_BASEQUEUE_H