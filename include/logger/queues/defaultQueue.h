#ifndef GCL_LOGGER_QUEUES_DEFAULTQUEUE_H
#define GCL_LOGGER_QUEUES_DEFAULTQUEUE_H

namespace GCL::logger
{
  class CDefaultQueue : public CBaseQueue
  {
    public:      
      CDefaultQueue() = default;
      
    private:      
      std::queue<std::unique_ptr<CRecord>> queue;
    
      virtual void processPushBack(std::unique_ptr<CRecord> &&);
      virtual std::unique_ptr<CRecord> processPopFront();
      virtual bool processEmpty();
  }
}

#endif // GCL_LOGGER_QUEUES_DEFAULTQUEUE_H