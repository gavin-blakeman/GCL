
void CDefaultQueue::processPushBack(std::unique_ptr<CRecord> &&r)
{
  queue.push_back(std::move(r));
}


std::unique_ptr<CRecord> CDefaultQueue::processPopFront()
{
  return std::move(queue.pop_front());
}

bool CDefaultQueue::processEmpty()
{
  return queue.empty();
}