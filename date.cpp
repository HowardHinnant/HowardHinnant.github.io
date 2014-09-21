//  chrono.cpp
//
//  (C) Copyright Howard Hinnant
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#include <algorithm>
#include <ctime>
#include "date"

#ifdef _LIBCPP_BEGIN_NAMESPACE_STD
_LIBCPP_BEGIN_NAMESPACE_STD
#else
namespace std {
#endif
namespace chrono
{

const weekday sun(0);
const weekday mon(1);
const weekday tue(2);
const weekday wed(3);
const weekday thu(4);
const weekday fri(5);
const weekday sat(6);

const month jan(1);
const month feb(2);
const month mar(3);
const month apr(4);
const month may(5);
const month jun(6);
const month jul(7);
const month aug(8);
const month sep(9);
const month oct(10);
const month nov(11);
const month dec(12);

__day_spec __make_spec(UInt8_t s) noexcept {return __day_spec(s);}

const __day_spec _1st = __make_spec(1);
const __day_spec _2nd = __make_spec(2);
const __day_spec _3rd = __make_spec(3);
const __day_spec _4th = __make_spec(4);
const __day_spec _5th = __make_spec(5);
const __day_spec last = __make_spec(6);

static
const int
db[2][13] = {{-1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364},
             {-1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365}};

static
const unsigned char
mb[2][366] =
{
{
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12
},
{
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12
}
};

static
inline
bool
is_leap(int y) noexcept
{
    return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
}

static
UInt32_t
inline
days_in_years(int y)
{
    y += 32799;
    return y*365 + y/4 - y/100 + y/400;
}

static
int
to_year_and_doy(int& doy, UInt32_t x)
{
    int y = static_cast<int>(static_cast<long long>(x+2) * 400 / 146097);
    const int ym1 = y - 1;
    doy = x - (y*365 + y/4 - y/100 + y/400);
    const int doy1 = x - (ym1*365 + ym1/4 - ym1/100 + ym1/400);
    const int N = std::numeric_limits<int>::digits - 1;
    const int mask1 = doy >> N; // arithmetic rshift - not portable - but nearly universal
    const int mask0 = ~mask1;
    doy = (doy & mask0) | (doy1 & mask1);
    y = (y & mask0) | (ym1 & mask1);
    y -= 32799;
    return y;
}

year::year(Int32_t v)
    : y_(v)
{
    if (!(-32768 <= v && v <= 32767))
        throw bad_date("year " + std::to_string(v) + " is out of range");
}

month::month(int v)
    : m_(v)
{
    if (!(1 <= v && v <= 12))
        throw bad_date("month " + std::to_string(v) + " is out of range");
}

weekday::weekday(int v)
    : wd_(v)
{
    if (v < 0 || v > 6)
        throw bad_date("week day " + std::to_string(v) + " is out of range");
}

day
weekday::operator[](int n) const
{
    if (n < 1 || n > 5)
        throw bad_date("week day argument " + std::to_string(n) + " is out of range");
    return (*this)[__day_spec(n)];
}

#if DESIGN == 1

date::date() noexcept
    : x_(11979588),
      y_(0),
      m_(1),
      d_(1),
      leap_(1),
      n_(7),
      dow_(7)
{
}

#elif DESIGN == 2

date::date() noexcept
    : x_(11979588),
      n_(7),
      dow_(7)
{
}

#elif DESIGN == 3

date::date() noexcept
    : y_(0),
      m_(1),
      d_(1),
      leap_(1),
      n_(7),
      dow_(7)
{
}

#endif

date
date::today() noexcept
{
    time_t systime;
    time(&systime);
    tm now;
    localtime_r(&systime, &now);
    return date(chrono::year(now.tm_year+1900, no_check),
                chrono::month(now.tm_mon+1, no_check),
                chrono::day(now.tm_mday), no_check);
}

#ifdef _LIBCPP_VERSION

date::date(std::chrono::system_clock::time_point tp)
{
    time_t systime = std::chrono::system_clock::to_time_t(tp);
    tm now;
    gmtime_r(&systime, &now);
    *this = date(chrono::year(now.tm_year+1900, no_check),
                 chrono::month(now.tm_mon+1, no_check),
                 chrono::day(now.tm_mday), no_check);
}

date::operator std::chrono::system_clock::time_point() const
{
    tm now = {0};
    now.tm_year = year() - 1900;
    now.tm_mon = month() - 1;
    now.tm_mday = day();
    time_t t = timegm(&now);
    return std::chrono::system_clock::from_time_t(t);
}

#endif

#if DESIGN == 1

date::date(chrono::year y, chrono::month m, chrono::day d)
    : y_(y.y_),
      m_(m.m_),
      d_(d.d_),
      n_(d.n_),
      dow_(d.dow_)
{
    leap_ = is_leap(y_);
    const int* year_data = db[leap_];
    if (n_ != 7)  // if a __day_spec is involved
    {
        if (dow_ == 7)  // if we want nth day of month
        {
            if (n_ == 6)  // want last day of month
                d_ = year_data[m_] - year_data[m_-1];
            else
                d_ = n_;  // want nth day of month
        }
        else  // we want nth weekday of month
        {
            // dow_ = [0 - 6]
            // n_ = [1 - 6] 6 means last
            Int32_t fy =  days_in_years(y.y_);
            int n_days_in_month = year_data[m_] - year_data[m_-1];
            int d;
            if (n_ == 6)
            {
                int ldow = (fy + year_data[m_] + 1) % 7;
                d = n_days_in_month;
                if (dow_ < ldow)
                    d -= ldow - dow_;
                else if (dow_ > ldow)
                    d -= 7 - (dow_ - ldow);
            }
            else
            {
                int fdow = (fy + year_data[m_-1] + 2) % 7;
                d = 1 + (n_-1) * 7;
                if (dow_ < fdow)
                    d += 7 - (fdow - dow_);
                else if (dow_ > fdow)
                    d += dow_ - fdow;
                if (d > n_days_in_month)
                    throw bad_date("day " + std::to_string(d) +
                                  " is out of range for " + std::to_string(y_) +
                                  '-' + std::to_string(m_));
            }
            d_ = d;
            x_ = fy + year_data[m_-1] + d_;
            return;
        }
    }
    else
    {
        if (!(1 <= d_ && d_ <= year_data[m_] - year_data[m_-1]))
            throw bad_date("day " + std::to_string(d) +
                          " is out of range for " + std::to_string(y_) +
                          '-' + std::to_string(m_));
    }
    x_ = days_in_years(y.y_) + year_data[m_-1] + d_;
}

date::date(chrono::year y, chrono::month m, chrono::day d, no_check_t)
    : y_(y.y_),
      m_(m.m_),
      d_(d.d_),
      n_(d.n_),
      dow_(d.dow_)
{
    leap_ = is_leap(y_);
    const int* year_data = db[leap_];
    if (n_ != 7)  // if a __day_spec is involved
    {
        if (dow_ == 7)  // if we want nth day of month
        {
            if (n_ == 6)  // want last day of month
                d_ = year_data[m_] - year_data[m_-1];
            else
                d_ = n_;  // want nth day of month
        }
        else  // we want nth weekday of month
        {
            // dow_ = [0 - 6]
            // n_ = [1 - 6] 6 means last
            Int32_t fy =  days_in_years(y.y_);
            int n_days_in_month = year_data[m_] - year_data[m_-1];
            int d;
            if (n_ == 6)
            {
                int ldow = (fy + year_data[m_] + 1) % 7;
                d = n_days_in_month;
                if (dow_ < ldow)
                    d -= ldow - dow_;
                else if (dow_ > ldow)
                    d -= 7 - (dow_ - ldow);
            }
            else
            {
                int fdow = (fy + year_data[m_-1] + 2) % 7;
                d = 1 + (n_-1) * 7;
                if (dow_ < fdow)
                    d += 7 - (fdow - dow_);
                else if (dow_ > fdow)
                    d += dow_ - fdow;
            }
            d_ = d;
            x_ = fy + year_data[m_-1] + d_;
            return;
        }
    }
    x_ = days_in_years(y.y_) + year_data[m_-1] + d_;
}

#elif DESIGN == 2

date::date(chrono::year y, chrono::month m, chrono::day d)
    : n_(d.n_),
      dow_(d.dow_)
{
    bool leap = is_leap(y.y_);
    const int* year_data = db[leap];
    if (n_ != 7)  // if a __day_spec is involved
    {
        if (dow_ == 7)  // if we want nth day of month
        {
            if (n_ == 6)  // want last day of month
                d.d_ = year_data[m.m_] - year_data[m.m_-1];
            else
                d.d_ = n_;  // want nth day of month
        }
        else  // we want nth weekday of month
        {
            // dow_ = [0 - 6]
            // n_ = [1 - 6] 6 means last
            Int32_t fy =  days_in_years(y.y_);
            int n_days_in_month = year_data[m.m_] - year_data[m.m_-1];
            int d;
            if (n_ == 6)
            {
                int ldow = (fy + year_data[m.m_] + 1) % 7;
                d = n_days_in_month;
                if (dow_ < ldow)
                    d -= ldow - dow_;
                else if (dow_ > ldow)
                    d -= 7 - (dow_ - ldow);
            }
            else
            {
                int fdow = (fy + year_data[m.m_-1] + 2) % 7;
                d = 1 + (n_-1) * 7;
                if (dow_ < fdow)
                    d += 7 - (fdow - dow_);
                else if (dow_ > fdow)
                    d += dow_ - fdow;
                if (d > n_days_in_month)
                    throw bad_date("day " + std::to_string(d) +
                                  " is out of range for " + std::to_string(y.y_) +
                                  '-' + std::to_string(m.m_));
            }
            x_ = fy + year_data[m.m_-1] + d;
            return;
        }
    }
    else
    {
        if (!(1 <= d.d_ && d.d_ <= year_data[m.m_] - year_data[m.m_-1]))
            throw bad_date("day " + std::to_string(d) +
                          " is out of range for " + std::to_string(y.y_) +
                          '-' + std::to_string(m.m_));
    }
    x_ = days_in_years(y.y_) + year_data[m.m_-1] + d.d_;
}

#elif DESIGN == 3

date::date(chrono::year y, chrono::month m, chrono::day d)
    : y_(y.y_),
      m_(m.m_),
      d_(d.d_),
      n_(d.n_),
      dow_(d.dow_)
{
    leap_ = is_leap(y_);
    const int* year_data = db[leap_];
    if (n_ != 7)  // if a __day_spec is involved
    {
        if (dow_ == 7)  // if we want nth day of month
        {
            if (n_ == 6)  // want last day of month
                d_ = year_data[m_] - year_data[m_-1];
            else
                d_ = n_;  // want nth day of month
        }
        else  // we want nth weekday of month
        {
            // dow_ = [0 - 6]
            // n_ = [1 - 6] 6 means last
            Int32_t fy =  days_in_years(y.y_);
            int n_days_in_month = year_data[m_] - year_data[m_-1];
            int d;
            if (n_ == 6)
            {
                int ldow = (fy + year_data[m_] + 1) % 7;
                d = n_days_in_month;
                if (dow_ < ldow)
                    d -= ldow - dow_;
                else if (dow_ > ldow)
                    d -= 7 - (dow_ - ldow);
            }
            else
            {
                int fdow = (fy + year_data[m_-1] + 2) % 7;
                d = 1 + (n_-1) * 7;
                if (dow_ < fdow)
                    d += 7 - (fdow - dow_);
                else if (dow_ > fdow)
                    d += dow_ - fdow;
                if (d > n_days_in_month)
                    throw bad_date("day " + std::to_string(d) +
                                  " is out of range for " + std::to_string(y_) +
                                  '-' + std::to_string(m_));
            }
            d_ = d;
        }
    }
    else
    {
        if (!(1 <= d_ && d_ <= year_data[m_] - year_data[m_-1]))
            throw bad_date("day " + std::to_string(d) +
                          " is out of range for " + std::to_string(y_) +
                          '-' + std::to_string(m_));
    }
}

#endif

#if DESIGN == 1

date&
date::operator+=(days d)
{
    x_ += d.count();
    if (!(11322 <= x_ && x_ <= 23947853))
        throw bad_date("year is out of range [-32768, 32767]");
    int doy;
    y_ = to_year_and_doy(doy, x_);
    leap_ = is_leap(y_);
    m_ = mb[leap_][doy];
    d_ = static_cast<UInt16_t>(doy - db[leap_][m_-1]);
    n_ = 7;
    dow_ = 7;
    return *this;
}

#elif DESIGN == 2

date&
date::operator+=(days d)
{
    x_ += d.count();
    if (!(11322 <= x_ && x_ <= 23947853))
        throw bad_date("year is out of range [-32768, 32767]");
    n_ = 7;
    dow_ = 7;
    return *this;
}

#elif DESIGN == 3

date&
date::operator+=(days d)
{
    UInt32_t x = day_number_from_ymd() + d.count();
    if (!(11322 <= x && x <= 23947853))
        throw bad_date("year is out of range [-32768, 32767]");
    int doy;
    y_ = to_year_and_doy(doy, x_);
    leap_ = is_leap(y_);
    m_ = mb[leap_][doy];
    d_ = static_cast<UInt16_t>(doy - db[leap_][m_-1]);
    n_ = 7;
    dow_ = 7;
    return *this;
}

#endif

#if DESIGN == 1 || DESIGN == 3

date&
date::operator+=(months mn)
{
    Int32_t y = y_;
    Int32_t m = m_;
    m += mn.count();
    if (m < 1)
    {
        int dy = (12 - m) / 12;
        y -= dy;
        m += 12 * dy;
    }
    else if (m > 12)
    {
        int dy = (m - 1) / 12;
        y += dy;
        m -= 12 * dy;
    }
    *this = date(chrono::year(y), chrono::month(m, no_check),
                 chrono::day(d_, n_, dow_));
    return *this;
}

date&
date::operator+=(years yr)
{
    *this = date(chrono::year(y_ + yr.count()), chrono::month(m_, no_check),
                 chrono::day(d_, n_, dow_));
    return *this;
}

#elif DESIGN == 2

date&
date::operator+=(months mn)
{
    int doy;
    int y = to_year_and_doy(doy, x_);
    const bool leap = is_leap(y);
    int m = mb[leap][doy];
    int d = doy - db[leap][m-1];
    m += mn.x_;
    if (m < 1)
    {
        int dy = (12 - m) / 12;
        y -= dy;
        m += 12 * dy;
    }
    else if (m > 12)
    {
        int dy = (m - 1) / 12;
        y += dy;
        m -= 12 * dy;
    }
    *this = date(chrono::year(y), chrono::month(m, nocheck),
                 chrono::day(d_, n_, dow_));
    return *this;
}

date&
date::operator+=(years yr)
{
    int doy;
    const int y = to_year_and_doy(doy, x_);
    const bool leap = is_leap(y);
    const int m = mb[leap][doy];
    const int d = doy - db[leap][m-1];
    *this = date(chrono::year(y + yr.x_), chrono::month(m, no_check),
                 chrono::day(d, n_, dow_));
    return *this;
}

#endif

#if DESIGN == 2

UInt16_t
date::day_from_day_number() const noexcept
{
    int doy;
    const int y = to_year_and_doy(doy, x_);
    const bool leap = is_leap(y);
    const m = mb[leap][doy];
    return static_cast<UInt16_t>(doy - db[leap][m-1]);
}

UInt16_t
date::month_from_day_number() const noexcept
{
    int doy;
    const int y = to_year_and_doy(doy, x_);
    const bool leap = is_leap(y);
    return mb[leap][doy];
}

Int16_t
date::year_from_day_number() const noexcept
{
    int doy;
    return static_cast<Int16_t>(to_year_and_doy(doy, x_));
}

bool
date::leap_from_day_number() const noexcept
{
    int doy;
    const int y = to_year_and_doy(doy, x_);
    return is_leap(y);
}

#endif

#if DESIGN == 3

UInt32_t
date::day_number_from_ymd() const noexcept
{
    return days_in_years(y_) + db[leap_][m_-1] + d_;
}

#endif

// year_month

bool
year_month::is_leap_year() const noexcept
{
    return is_leap(y_);
}

year_month&
year_month::operator+=(months mn)
{
    Int32_t y = y_;
    Int32_t m = m_;
    m += mn.count();
    if (m < 1)
    {
        int dy = (12 - m) / 12;
        y -= dy;
        m += 12 * dy;
    }
    else if (m > 12)
    {
        int dy = (m - 1) / 12;
        y += dy;
        m -= 12 * dy;
    }
    y_ = chrono::year(y);
    m_ = chrono::month(m);
    return *this;
}

months
operator-(year_month x, year_month y) noexcept
{
    Int32_t y1 = x.y_;
    Int32_t m1 = x.m_;
    Int32_t y2 = y.y_;
    Int32_t m2 = y.m_;
    return months(y1*12 + m1 - (y2*12 + m2));
}

}  // chrono

#ifdef _LIBCPP_END_NAMESPACE_STD
_LIBCPP_END_NAMESPACE_STD
#else
}  // std
#endif

