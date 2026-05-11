
#include <mutex>
#include <vector>
#include <unordered_map>

#include "replacer.hpp"

class ClockReplacer : public Replacer {
    public:
      ClockReplacer() {};
      ~ClockReplacer() = default;

      struct ClockEntry {
          int frameID;
          bool roundFlag;
      };

      void Insert(int fid) override;
      void Delete(int fid) override;
      int Evict() override;
      int Size() override;
    private:
      std::mutex latch_;

      std::vector<ClockEntry> clockBuffer_;
      std::unordered_map<int, std::vector<ClockEntry>::iterator> lookupMap_;
};
