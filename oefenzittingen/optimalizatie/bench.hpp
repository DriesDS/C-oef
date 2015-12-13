#ifndef DIRKN_BENCH_HPP
#define DIRKN_BENCH_HPP

#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <iterator>
#include <cstdint>

#include <sys/resource.h>

/**
  * @brief Read the cycle counter from the Pentium processor.
  */
inline std::uint64_t rdtsc()
// This must be a function to be inlined according to icc.
{
    std::uint32_t lo, hi;
    asm volatile ("rdtsc\n\t" : "=a" (lo), "=d" (hi));
    return (std::uint64_t) hi << 32 | lo;
}

/**
  * @brief Class to measure time.
  *
  * These classes are the @e strategy part in the <em>strategy pattern<em>.
  * Nothing is in here, we add this parent only to group the effective Timer
  * classes together.
  *
  * Descendants must implement
  *     - <code>typedef some_type unit_t</code>: the type used to measure time.
  *     - <code>void start()</code>
  *     - <code>void stop()</code>
  *     - <code>unit_t getdelta() const</code>
  */
//class Timer { };

/**
  * @brief Timer making use of the intel cycle counter.
  */
class TscTimer //: public Timer
{
protected:
    std::uint64_t _t1, _t2;
    std::uint64_t _delta0;
    void callibrate()
    {
        const std::size_t n = 3; // this is also done 3 times in the note from intel
        std::uint64_t t[n];
        _delta0 = 0; // just safety for if this gets called more than once
        for(std::size_t i = 0; i < n; ++i) {
            start();
            stop();
            t[i] = getdelta();
        }
        _delta0 = static_cast<std::uint64_t>(std::accumulate(t, t+n, 0.) / n);
    }
public:
    typedef std::uint64_t unit_t;
    
    TscTimer() : _t1(0), _t2(0), _delta0(0)
    {
        callibrate();
    }
    inline void start()
    {
        _t1 = rdtsc();
    }
    inline void stop()
    {
        _t2 = rdtsc();
    }
    std::uint64_t getdelta() const
    {
        std::uint64_t t = (_t2 > _t1) ? _t2 - _t1 : 0;
        return t > _delta0 ? (t - _delta0) : t;
    }
};

/**
  * @brief Timer making use of the standard unix @c getrusage().
  */
class RusageTimer //: public Timer
{
protected:
    struct rusage _t1, _t2;
    double _delta0;
    /**
      * @brief Callibrate the measurement routines.
      *
      * Measure how long it takes to get start and stop time samples and keep
      * this in _delta0. The function getdelta substracts this amount from
      * measured times. For most systems this will be 0, since the measurement
      * is too coarse grained.
      *
      * This function gets called from the constructor, so there is no need to
      * call it yourself.
      */
    void callibrate()
    {
        const std::size_t n = 100;
        double t[n];
        _delta0 = 0; // just safety for if this gets called more than once
        for(std::size_t i = 0; i < n; ++i) {
            start();
            stop();
            t[i] = getdelta();
        }
        _delta0 = std::accumulate(t, t+n, 0.) / n;
    }
public:
    typedef double unit_t;

    RusageTimer() : _t1(), _t2(_t1), _delta0(0)
    {
        callibrate();
    }
    inline void start()
    {
        getrusage(RUSAGE_SELF, &_t1);
    }
    inline void stop()
    {
        getrusage(RUSAGE_SELF, &_t2);
    }
    /**
      * @brief Return the passed time in msec.
      */
    double getdelta() const
    {
        double t;
        t = ((_t2.ru_utime.tv_sec - _t1.ru_utime.tv_sec) +
             (_t2.ru_stime.tv_sec - _t1.ru_stime.tv_sec)) * 1.0e3 +
            ((_t2.ru_utime.tv_usec - _t1.ru_utime.tv_usec) +
             (_t2.ru_stime.tv_usec - _t1.ru_stime.tv_usec)) * 1.0e-3;
        return t > _delta0 ? (t - _delta0) : t;
    }
};

/**
  * @brief A benchmark class which accepts the code as a template argument.
  *
  * To use this benchmark class you have to encapsulate the algorithm in a
  * class and provide @c operator() to call the algorithm. If @c operator() is
  * inline as well as the timer routines then this class incurs no overhead.
  *
  * The timings array contains the warmup timings, but these are ignored for
  * the calculations of the statistics. This means that the statistics are over
  * <tt>_n - _warmup</tt> timings.
  */
template <typename _Algorithm,
          std::size_t _n=53, std::size_t _warmup=0, // _warmup < _n since the warmup times are part of the timings array
          class _Timer=TscTimer>
class Bench
{
public:
    typedef typename _Timer::unit_t unit_t;
protected:
    _Timer _timer;
    _Algorithm _alg;        ///< Algorithm to time.
    unit_t _t[_n];          ///< Array of times.
    unit_t _maxt;
    unit_t _mint;
    unit_t _meant; // note: calculated as double (against overflow) then converted back to long long
    double _mediant;
    void calculate_statistics()
    {
        _maxt = *std::max_element(_t+_warmup, _t+_n);
        _mint = *std::min_element(_t+_warmup, _t+_n);
        _meant = static_cast<unit_t>(std::accumulate(_t+_warmup, _t+_n, 0.0) / (_n-_warmup)); // accumulating in double
        unit_t t2[_n];
        std::copy(_t, _t+_n, t2);
        std::sort(t2+_warmup, t2+_n);
        auto size = _n - _warmup;
        auto mid = size / 2;
        _mediant = size % 2 == 0 ? (t2[mid] + t2[mid-1]) / 2.0 : t2[mid];
    }
public:
    Bench(bool time_right_away = true, _Algorithm alg = _Algorithm()) : _timer(), _alg(alg), _maxt(0), _mint(0), _meant(0)
    {
        if(time_right_away) time();
    }
    Bench(std::ostream& out, _Algorithm alg = _Algorithm()) : _timer(), _alg(alg), _maxt(0), _mint(0), _meant(0)
    {
        time();
        out << *this;
    }
    void time()
    {
        for(std::size_t i = 0; i < _n; ++i)
        {
            _timer.start();
            _alg();
            _timer.stop();
            _t[i] = _timer.getdelta();
            _alg.use_result(); // to not optimize out the _alg() call...
        }
        calculate_statistics();
    }
    unit_t mean() const
    {
      return _meant;
    }
    std::ostream& print_on(std::ostream& out, std::size_t max_timings_to_print=10) const
    {
        out << "Statistics for " << "_Z" << typeid(_Algorithm).name() << ":" << std::endl;
        out << "     timer: " << "_Z" << typeid(_Timer).name() << std::endl;
        out << "      runs: " << _n << std::endl;
        out << "    warmup: " << _warmup << " (these are ignored in the stats)" << std::endl;
        out << "igntimings: ";
        std::copy(_t, _t+std::min(max_timings_to_print, _warmup), std::ostream_iterator<unit_t>(out, " "));
        if(_warmup > max_timings_to_print) out << "...";
        out << std::endl;
        out << "efftimings: ";
        std::copy(_t+_warmup, _t+_warmup+std::min(max_timings_to_print, _n-_warmup), std::ostream_iterator<unit_t>(out, " "));
        if(_n > max_timings_to_print + _warmup) out << "...";
        out << std::endl;
        out << "       max: " << std::right << std::setw(20) << _maxt << std::endl;
        out << "       min: " << std::right << std::setw(20) << _mint << std::endl;
        out << "      mean: " << std::right << std::setw(20) << _meant << std::endl;
        out << "    median: " << std::right << std::setw(20) << _mediant << std::endl;
        out << "   max/min: " << std::right << std::setw(20) << (_maxt*1.0/_mint) << std::endl;
        return out;
    }
    std::ostream& dump(std::ostream& out) const
    {
        std::copy(_t, _t+_n, std::ostream_iterator<unit_t>(out, "\n"));
        return out;
    }
};

template <typename _Algorithm, 
          std::size_t _n, std::size_t _warmup,
          class _Timer>
std::ostream& 
operator<<(std::ostream& out, const Bench<_Algorithm, _n, _warmup, _Timer>& b)
{
    return b.print_on(out);
}

#endif
