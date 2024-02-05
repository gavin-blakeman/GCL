namespace GCL::logger::queues
{
  void CBaseQueue::pushBack(std::unique_ptr<CRecord> &&r)
  {
    uniqueLock ul(queueMutex);
    
    processPushBack(std::move(r));
  }
      
  std::unique_ptr<CRecord> CBaseQueue::popFront()
  {
    UniqueLock ul(queueMutex);
    
    return processPopFront();
  }
      
  bool CBaseQueue::empty() const noexcept
  {
    sharedLock sl(queueMutex);
    
    return processEmpty();
  }

}