#include <cassert>
#include <chrono>
#include <cmath>
#include <queue>

namespace mirazabal
{

template<typename T>
class Codel
{
  using time_stamp = std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>;

 const std::chrono::microseconds target_ = std::chrono::microseconds(10000);// =	std::chrono::milliseconds(5);
  const std::chrono::microseconds interval_ = std::chrono::microseconds(100000);// I do not understand why static constexpr in the shared lib does not work... // <>  TARGET =  MS2TIME(5);// 5ms TARGET queue delay
  //	time_t INTERVAL = MS2TIME(100); // 100ms sliding-minimum window
 static constexpr uint16_t maxpacket_ = 1500; // = 512;
  // Maximum packet size in bytes
  // (SHOULD use interface MTU)

  struct Packet
  {
    T data_;
    time_stamp time_;
  };

  struct dodequeue_result
  {
    Packet const * p;
    bool ok_to_drop;
  };

  time_stamp first_above_time_; // = 0; //
  time_stamp drop_next_;// = 0;
  uint32_t count_ ; //= 0;
  uint32_t lastcount_; // = 0;
  bool dropping_ ; //= false;
  std::queue<Packet> q_;

  time_stamp control_law(time_stamp t, uint32_t count)
  {
    auto temp =  interval_.count() / std::sqrt(count);
    return t +  static_cast<std::chrono::microseconds>(static_cast<int64_t>(temp));
  }

  inline
    time_stamp time_now()
    {
      return std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
    }

  dodequeue_result dodequeue(time_stamp now)
  {
    assert(size() != 0);
    auto const& p = q_.front();
    // To span a large range of bandwidths, CoDel runs two
    // different AQMs in parallel. One is sojourn-time-based
    // and takes effect when the time to send an MTU-sized
    // packet is less than TARGET.  The 1st term of the "if"
    // below does this.  The other is backlog-based and takes
    // effect when the time to send an MTU-sized packet is >=
    // TARGET. The goal here is to keep the output link
    // utilization high by never allowing the queue to get
    // smaller than the amount that arrives in a typical
    // interarrival time (MTU-sized packets arriving spaced
    // by the amount of time it takes to send such a packet on
    // the bottleneck). The 2nd term of the "if" does this.
    auto sojourn_time = now - p.time_;
    if (sojourn_time < target_ ){ //|| bytes() <= maxpacket_) {
      // went below - stay below for at least INTERVAL
      first_above_time_ = time_stamp(std::chrono::microseconds(0));
      return {&p, false};
    }

    if (first_above_time_ == time_stamp(std::chrono::microseconds(0))) {
      // just went above from below. if still above at
      // first_above_time, will say it’s ok to drop.
      first_above_time_ = now + interval_; // INTERVAL;
    } else if((now >= first_above_time_) ){
      return {&p, true}; // ok_to_drop = true;
    }

      return {&p, false};
    }

    public:
    Codel() : first_above_time_(time_stamp(std::chrono::microseconds(0) )), drop_next_{time_stamp(std::chrono::microseconds{0})}, count_{0}, lastcount_{0}, dropping_{false}
    {

    }

    inline
      void pop()
      {
        q_.pop();
      }

    inline
      size_t size()
      {
        return q_.size();
      }


    void enqueue(T const& data)
    {
      Packet p;
      p.data_ = data;
      p.time_ = time_now();
      q_.push(p);
    }

    T const* dequeue()
    {

      if(size() == 0){
        first_above_time_ = time_stamp(std::chrono::microseconds(0));
        return nullptr;
      }

      auto now = time_now();
      auto r = dodequeue(now);
      uint32_t delta;
      if (dropping_) {
        if (! r.ok_to_drop) {
          // sojourn time below TARGET - leave drop state
          dropping_ = false;
          return &(r.p->data_);
        }
        // Time for the next drop. Drop current packet and dequeue
        // next.  If the dequeue doesn’t take us out of dropping
        // state, schedule the next drop. A large backlog might
        // result in drop rates so high that the next drop should
        // happen now, hence the ’while’ loop.
        while (now >= drop_next_ && dropping_) {
          pop(); //drop(r.p);
          if(size() == 0){
            first_above_time_ = time_stamp(std::chrono::microseconds(0));
            return nullptr;
          }
          r = dodequeue(now);
          if (! r.ok_to_drop) {
            // leave drop state
            dropping_ = false;
          } else {
            ++count_;
            // schedule the next drop.
            drop_next_ = control_law(drop_next_, count_);
          }
        }
        // If we get here we’re not in drop state. The ’ok_to_drop’
        // return from dodequeue means that the sojourn time has been
        // above ’TARGET’ for ’INTERVAL’ so enter drop state.
      } else if (r.ok_to_drop) {
        pop(); //drop(r.p);
        if(size() == 0){
          first_above_time_ = time_stamp(std::chrono::microseconds(0));
          return nullptr;
        }
        r = dodequeue(now);
        dropping_ = true;
        // If min went above TARGET close to when it last went
        // below, assume that the drop rate that controlled the
        // queue on the last cycle is a good starting point to
        // control it now. (’drop_next’ will be at most ’INTERVAL’
        // later than the time of the last drop so ’now - drop_next’
        // is a good approximation of the time from the last drop
        // until now.) Implementations vary slightly here; this is
        // the Linux version, which is more widely deployed and
        // tested.
        delta = count_ - lastcount_;
        count_ = 1;
        if ((delta > 1) && (now - drop_next_ < 16*interval_))
          count_ = delta;
        drop_next_ = control_law(now, count_);
        lastcount_ = count_;
      }
      return &(r.p->data_);
    }

  };

} // end namespace mirazabal
