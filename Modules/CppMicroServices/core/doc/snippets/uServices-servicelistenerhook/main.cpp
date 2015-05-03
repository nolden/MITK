#include <usServiceListenerHook.h>

#include <map>

US_USE_NAMESPACE

//! [1]
class MyServiceListenerHook : public ServiceListenerHook
{
private:

  class Tracked
  {
    // Do some work during construction and destruction
  };

  US_UNORDERED_MAP_TYPE<ListenerInfo, Tracked> tracked;

public:

  void Added(const std::vector<ListenerInfo>& listeners) override
  {
    for (const auto & listener : listeners)
    {
      // Lock the tracked object for thread-safe access

      if (listener.IsRemoved()) return;
      tracked.insert(std::make_pair(listener, Tracked()));
    }
  }

  void Removed(const std::vector<ListenerInfo>& listeners) override
  {
    for (const auto & listener : listeners)
    {
      // Lock the tracked object for thread-safe access

      // If we got a corresponding "Added" event before, the Tracked
      // destructor will do some cleanup...
      tracked.erase(listener);
    }
  }
};
//! [1]

int main(int /*argc*/, char* /*argv*/[])
{
  return 0;
}
