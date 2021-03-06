#pragma once
#include "Ritalgo.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <iterator>
#include <valarray>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <immintrin.h>
#include <bitset>

template <class F, class S>
Ritalgo<F, S>::Ritalgo(std::shared_ptr<Problem> p)
{
  field = std::move(std::unique_ptr<Field>(new F(p->get_field_str())));
  for (auto&& s : p->get_stones_str()) {
    stones.push_back(std::shared_ptr<Stone>(new S(s, stones.size())));
  }
  for (auto && s : stones) {
    std::vector < std::tuple < int, int, int, int >> sub;
    for (int x = -8; x <= 32; ++x) {
      for (int y = -8; y <= 32; ++y) {
        for (int r = 0; r < 2; ++r) {
          for (int a = 0; a < 4; ++a) {
            if (field->appliable(s, x, y, r, a)) {
              sub.push_back(std::make_tuple(x, y, r, a));
            }
          }
        }
      }
    }
    ok_list.push_back(sub);
  }
  remains.resize(stones.size() + 1);
  remains[stones.size()] = 0;
  for (int i = stones.size() - 1; 0 <= i; --i) {
    int cnt = 0;
    for (int x = 0; x < 8; ++x) {
      for (int y = 0; y < 8; ++y) {
        if (stones[i]->at(x, y, 0, 0)) {
          ++cnt;
        }
      }
    }
    remains[i] = cnt + remains[i + 1];
  }
}

template <class F, class S>
Ritalgo<F, S>::Ritalgo(std::shared_ptr<Problem> p, std::unique_ptr<Core> core)
{
  field = std::move(std::unique_ptr<Field>(new F(p->get_field_str())));
  for (auto&& s : p->get_stones_str()) {
    stones.push_back(std::shared_ptr<Stone>(new S(s, stones.size())));
  }
  for (auto && s : stones) {
    std::vector < std::tuple < int, int, int, int >> sub;
    for (int x = -8; x <= 32; ++x) {
      for (int y = -8; y <= 32; ++y) {
        for (int r = 0; r < 2; ++r) {
          for (int a = 0; a < 4; ++a) {
            if (field->appliable(s, x, y, r, a)) {
              sub.push_back(std::make_tuple(x, y, r, a));
            }
          }
        }
      }
    }
    ok_list.push_back(sub);
  }

  remains.resize(stones.size() + 1);
  remains[stones.size()] = 0;
  for (int i = stones.size() - 1; 0 <= i; --i) {
    int cnt = 0;
    for (int x = 0; x < 8; ++x) {
      for (int y = 0; y < 8; ++y) {
        if (stones[i]->at(x, y, 0, 0)) {
          ++cnt;
        }
      }
    }
    remains[i] = cnt + remains[i + 1];
  }

  this->core = std::move(core);
  is_production |= true;
}


template <class F, class S>
void Ritalgo<F, S>::submit(std::unique_ptr<Field> field)
{
  std::stringstream ss;
  ss << *field;
  for (int i = std::get<0>(*(field->get_history().rbegin()))->identify() + 1; i < stones.size(); ++i)
    ss << std::endl;

  boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
  std::string fn = "Rita" + boost::posix_time::to_iso_string(time) + ".txt";
  
  std::ofstream ofs(fn);
  ofs << ss.str();
  ofs.close();

  if (is_production) {
    core->submit(field->score(), field->get_history().size(), fn);
  }
}

template <class F, class S>
void Ritalgo<F, S>::solve()
{
  std::shared_ptr<Env> env(new Env());
  std::vector<std::shared_ptr<Ant>> ants;
  static constexpr int N = 10;
  for (int i = 0; i < N; ++i) {
    std::shared_ptr<Ant> ptr(new Ant(field -> clone(), stones, ok_list, remains, env));
    ants.push_back(ptr);
  }
  int best_score = 1 << 28;
  std::unique_ptr<Field> best_field;
  for (int ite = 0; ; ++ite) {
    std::cerr << "iteration : " << ite << std::endl;
    double ave_score = 0.0;
    env->eva();
    // for (auto ant : ants) {
#pragma omp parallel for reduction(+:ave_score)
    for (int i = 0; i < ants.size(); ++i) {
      const auto &ant = ants[i];
      ant -> run_over();
      ave_score += (double)ant->score() / N;
      if ( ant -> score() < best_score ) {
        best_score = ant -> score();
        //std::cout << *(ant -> loot()) << std::endl;
        best_field = std::move(ant->loot());
        std::cerr << ant->score() << " ... " << ant -> loot() -> get_history().size() << std::endl;
        submit(best_field->clone());
      }
      else if (ant->score() == best_score) {
        if (ant->loot()->get_history().size() < best_field->get_history().size()) {
          best_field = std::move(ant->loot());
          std::cerr << ant->score() << " ... " << ant->loot()->get_history().size() << std::endl;
          submit(best_field->clone());
        }
      }
    }
    std::cerr << "AVERAGE SCORE : " << ave_score << std::endl;
    for (auto ant : ants) {
      //ant->renew();
      ant->renew(ave_score);
      ant -> reset(field -> clone());
    }
  }
}

/*
template <class F, class S>
template <typename T, typename TSize>
T Ritalgo<F, S>::Env::make(T& ptr, const TSize ts) {
  ptr = new typename std::remove_pointer<T>::type [ts];
  for (int i = 0; i < ts; ++i) {
    ptr[i] = 0.0;
  }
  return ptr;
}

template <class F, class S>
template <typename T, typename TSize, typename... RSize>
T Ritalgo<F, S>::Env::make(T& ptr, const TSize ts, const RSize... rs) {
  ptr = new typename std::remove_pointer<T>::type [ts];
  for (int i = 0; i < ts; ++i) {
    ptr[i] = make(ptr[i], rs...);
  }
  return ptr;
}

template <class F, class S>
template <typename T, typename TSize>
T Ritalgo<F, S>::Env::destroy(T& ptr, const TSize ts) {
  for (int i = 0; i < ts; ++i) {
    ptr[i] = 0;
  }
  delete ptr;
  ptr = nullptr;
  return ptr;
}

template <class F, class S>
template <typename T, typename TSize, typename... RSize>
T Ritalgo<F, S>::Env::destroy(T& ptr, const TSize ts, const RSize... rs) {
  for (int i = 0; i < ts; ++i) {
    ptr[i] = destroy(ptr[i], rs...);
  }
  delete ptr;
  ptr = nullptr;
  return ptr;
}


template <class F, class S>
Ritalgo<F, S>::Env::Env()
{
  env = make(env, 256, 2, 2, 64, 64, 2, 4);
}

template <class F, class S>
Ritalgo<F, S>::Env::~Env()
{
  env = destroy(env, 256, 2, 2, 64, 64, 2, 4);
}
*/

template <class F, class S>
void Ritalgo<F, S>::Env::put(int idx, int add, int x, int y, int rev, int ang, double phe)
{
#ifdef _DEBUG
  if (!( 0 <= x + GETA && x + GETA < 64 && 0 <= y + GETA && y + GETA < 64 ) ) {
    std::cerr << "AHHHHHHHHHH" << std::endl;
  }
#endif
  env[idx][add][x + GETA][y + GETA][rev][ang] += phe;
}

template <class F, class S>
double Ritalgo<F, S>::Env::get(int idx, int add, int x, int y, int rev, int ang) const
{
#ifdef _DEBUG
  if (!( 0 <= x + GETA && x + GETA < 64 && 0 <= y + GETA && y + GETA < 64 ) ) {
    std::cerr << "AHHHHHHHHHH" << std::endl;
  }
#endif
  return env[idx][add][x + GETA][y + GETA][rev][ang];
}

template <class F, class S>
template <typename T, typename TSize>
void Ritalgo<F, S>::Env::eva(T& ptr, const TSize ts)
{
  for (int i = 0; i < ts; ++i) {
    ptr[i] *= EVAPORATE_RATE;
  }
}

template <class F, class S>
template <typename T, typename TSize, typename... RSize>
void Ritalgo<F, S>::Env::eva(T& ptr, const TSize ts, const RSize... rs)
{
  for (int i = 0; i < ts; ++i) {
    eva(ptr[i], rs...);
  }
}

template <class F, class S>
void Ritalgo<F, S>::Env::eva()
{
  eva(env, 256, BEAM, 64, 64, 2, 4);
}

template <class F, class S>
Ritalgo<F, S>::Ant::Ant(std::unique_ptr<Field> field, const std::vector<std::shared_ptr<Stone>> & stones, const std::vector<std::vector<std::tuple<int, int, int, int>>> & ok_list, const std::vector<int> &remains, std::shared_ptr<Env> env) : dist(0.0, 1.0), skipper(0, stones.size()-1), ok_list(ok_list), remains(remains), normal(5, 2)
{
  this -> field = std::move(field);
  std::copy(stones.begin(), stones.end(), std::back_inserter(this -> stones));
  this -> env = env;

  std::random_device rd;
  this -> mt . seed(rd());
}

static double mypow(double v, int n) noexcept
{
  if (n == 1) {
    return v;
  }
  double r = 1.0;
  double t = mypow(v, n / 2);
  r *= t * t;
  if (n & 1) {
    r *= v;
  }
  return r;
}

static uint64_t popcnt(uint64_t n) noexcept
{
  uint64_t i;
  uint64_t step = 4;
  uint64_t sum = 0, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
  for (i = 1; i <= (n & ~(step - 1)); i += step) {
    sum1 += _mm_popcnt_u64(i);
    sum2 += _mm_popcnt_u64(i+1);
    sum3 += _mm_popcnt_u64(i+2);
    sum4 += _mm_popcnt_u64(i+3);
  }
  sum = sum1 + sum2 + sum3 + sum4;
  for (; i <= n; ++i) {
    sum += _mm_popcnt_u64(i);
  }
  return sum;
}

template <class F, class S>
double Ritalgo<F, S>::Ant::h(std::shared_ptr<Stone> s, int x, int y, int rev, int ang) noexcept
{
  /**
  int sum = 0;
  int wall = 0;
  int fill = 0;
  bool done[8][8] = {};
  int cell[10][10] = {};
  bool chck[10][10] = {};
  bool term[8][8] = {};
  bool flag = true;
  for (int i = 0; flag && i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (s -> at(j, i, rev, ang)) {
        std::queue<std::pair<int,int>> queue;
        queue.push({j, i});
        while (queue.size()) {
          ++fill;
          static constexpr int ofs[4][2] = {
            {0, 1},
            {1, 0},
            {-1, 0},
            {0, -1}
          };
          auto e = queue.front();
          queue.pop();
          for (int k = 0; k < 4; ++k) {
            int dx = e.first  + ofs[k][0];
            int dy = e.second + ofs[k][1];
            int nx = x + dx;
            int ny = y + dy;

            if (ny < 0 || 32 <= ny || nx < 0 || 32 <= nx) {
              ++wall;
              --cell[dy + 1][dx + 1];
              chck[dy + 1][dx + 1] |= true;
              term[e.second][e.first] |= true;
              continue;
            }

            if ( dx < 0 || 8 <= dx || dy < 0 || 8 <= dy ) {
              if (field->at(nx, ny)) {
                ++sum;
                --cell[dy + 1][dx + 1];
              }
              else {
                ++cell[dy + 1][dx + 1];
              }
              chck[dy + 1][dx + 1] |= true;
              term[e.second][e.first] |= true;
              continue;
            }

            if (s -> at( dx, dy, rev, ang ) ) {
              if (done[dy][dx]) {
                continue;
              }
              queue.push({dx, dy});
              done[dy][dx] |= true;
              continue;
            }
            else {
              if (field->at(nx, ny)) {
                ++sum;
                --cell[dy + 1][dx + 1];
              }
              else {
                ++cell[dy + 1][dx + 1];
              }
              chck[dy + 1][dx + 1] |= true;
              term[e.second][e.first] |= true;
            }
          }
        }
        flag &= false;
        break;
      }
    }
  }
  double acm = 0.0;
  int cnt = 0;
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      if (!chck[i][j]) continue;
      acm += std::pow(4.0, cell[i][j]);
      ++cnt;
    }
  }
  int tc = 0;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (term[i][j])
        ++tc;
    }
  }
  acm /= cnt;

  return 1.0/acm;
  /**/
  unsigned int raw_f[10] = {};
  unsigned int raw_s[10] = {};
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {

      int dx = j - 1;
      int dy = i - 1;
      int nx = dx + x;
      int ny = dy + y;

      if (nx < 0 || 32 <= nx || ny < 0 || 32 <= ny) {
        raw_f[i] |= 1 << j;
        continue;
      }
      if (field->at(nx, ny)) {
        raw_f[i] |= 1 << j;
      }
      if (dx < 0 || 8 <= dx || dy < 0 || 8 <= dy) {
        continue;
      }
      if (s->at(dx, dy, rev, ang)) {
        raw_f[i] |= 1 << j;
        raw_s[i] |= 1 << j;
      }
    }
  }

  //std::string ll;
  //std::getline(std::cin, ll);

  //test
  //for (int i = 0; i < 10; ++i) {
  //  std::string line;
  //  std::getline(std::cin, line);
  //  raw_f[i] = 0;
  //  for (int j = 0; j < 10; ++j) {
  //    int b = (line.at(j) == '1');
  //    raw_f[i] |= b << j;
  //  }
  //}
  //for (int i = 0; i < 10; ++i) {
  //  std::string line;
  //  std::getline(std::cin, line);
  //  raw_s[i] = 0;
  //  for (int j = 0; j < 10; ++j) {
  //    int b = (line.at(j) == '1');
  //    raw_s[i] |= b << j;
  //  }
  //}

  // up
  double up = [&]() {
    unsigned int score = 0;
    unsigned int count = 0;
    for (int i = 1; i <= 8; ++i) {
      unsigned int d1 = raw_s[i] & ~raw_s[i - 1];
      unsigned int sc = _mm_popcnt_u32(d1);
      d1 &= raw_f[i - 1];
      unsigned int ss = _mm_popcnt_u32(d1);

      count += sc;
      score += ss;
    }
    return ::pow((double)score / count, 4) * 5;
  }();
  // down
  double down = [&]() {
    unsigned int score = 0;
    unsigned int count = 0;
    for (int i = 1; i <= 8; ++i) {
      unsigned int d1 = raw_s[i] & ~raw_s[i + 1];
      unsigned int sc = _mm_popcnt_u32(d1);
      d1 &= raw_f[i + 1];
      unsigned int ss = _mm_popcnt_u32(d1);

      count += sc;
      score += ss;
    }
    return ::pow((double)score / count, 4) * 5;
  }();
  // right
  double right = [&]() {
    unsigned int score = 0;
    unsigned int count = 0;
    unsigned int arr[8];
    for (int i = 1; i <= 8; ++i) {
      unsigned int d1 = (raw_s[i] >> 1) & ~raw_s[i];
      unsigned int sc = _mm_popcnt_u32(d1);
      d1 &= raw_f[i];
      unsigned int ss = _mm_popcnt_u32(d1);

      arr[i - 1] = d1;

      count += sc;
      score += ss;
    }
    int line = 0;
    for (int i = 0; i < 8 - 1; ++i) {
      line += _mm_popcnt_u32(arr[i] & arr[i+1]);
    }
    return ::pow((double)score / count, 4) * 5;
  }();
  // left
  double left = [&]() {
    unsigned int score = 0;
    unsigned int count = 0;
    unsigned int arr[8];

    for (int i = 1; i <= 8; ++i) {
      unsigned int d1 = (raw_s[i] << 1) & ~raw_s[i];
      unsigned int sc = _mm_popcnt_u32(d1);
      d1 &= raw_f[i];
      unsigned int ss = _mm_popcnt_u32(d1);

      arr[i - 1] = d1;

      count += sc;
      score += ss;
    }
    int line = 0;
    for (int i = 0; i < 8 - 1; ++i) {
      line += _mm_popcnt_u32(arr[i] & arr[i + 1]);
    }
    return ::pow((double)score / count, 4) * 5;
  }();

  return (up + down + left + right);
}

template <class F, class S>
std::pair<double, double> Ritalgo<F, S>::Ant::h2(std::shared_ptr<Stone> s, int x, int y, int rev, int ang) const noexcept
{
  int sum = 0;
  int wall = 0;
  int fill = 0;
  bool done[8][8] = {};
  int cell[10][10] = {};
  bool chck[10][10] = {};
  bool flag = true;
  for (int i = 0; flag && i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (s->at(j, i, rev, ang)) {
        std::queue<std::pair<int, int>> queue;
        queue.push({ j, i });
        while (queue.size()) {
          ++fill;
          static constexpr int ofs[4][2] = {
            { 0, 1 },
            { 1, 0 },
            { -1, 0 },
            { 0, -1 }
          };
          auto e = queue.front();
          queue.pop();
          for (int k = 0; k < 4; ++k) {
            int dx = e.first + ofs[k][0];
            int dy = e.second + ofs[k][1];
            int nx = x + dx;
            int ny = y + dy;

            if (ny < 0 || 32 <= ny || nx < 0 || 32 <= nx) {
              ++wall;
              --cell[dy + 1][dx + 1];
              chck[dy + 1][dx + 1] |= true;
              continue;
            }

            if (dx < 0 || 8 <= dx || dy < 0 || 8 <= dy) {
              if (field->at(nx, ny)) {
                ++sum;
                --cell[dy + 1][dx + 1];
              }
              else {
                ++cell[dy + 1][dx + 1];
              }
              chck[dy + 1][dx + 1] |= true;
              continue;
            }

            if (s->at(dx, dy, rev, ang)) {
              if (done[dy][dx]) {
                continue;
              }
              queue.push({ dx, dy });
              done[dy][dx] |= true;
              continue;
            }
            else {
              if (field->at(nx, ny)) {
                ++sum;
                --cell[dy + 1][dx + 1];
              }
              else {
                ++cell[dy + 1][dx + 1];
              }
              chck[dy + 1][dx + 1] |= true;
            }
          }
        }
        flag &= false;
        break;
      }
    }
  }
  double acm = 0.0;
  int cnt = 0;
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      if (!chck[i][j]) continue;
      acm += std::pow(4.0, cell[i][j]);
      ++cnt;
    }
  }
  acm /= cnt;

  return{ 1.0 / acm, fill / acm };
}

template <class F, class S>
double Ritalgo<F, S>::Ant::v(const int idx, const int add, const int x, const int y, const int rev, const int ang) noexcept
{
  //const double a = -log(4.0) / ((BEAM - 1) * (BEAM - 1));
  //const double a = -1024.0 / ::mypow((double)remains[idx + add] / field->score(), 10);
  const double a = (-log(4.0) / (BEAM - 1));
  //std::cerr << add << " : " << std::exp(add * a) << std::endl;
  const double hv = h(stones[idx + add], x, y, rev, ang) /**/ * std::exp(add * a) /**/ /* * std::exp(2.0 * std::sqrt(std::log(2.0)) * idx / stones.size()) */;
//  const double hv = h(stones[idx + add], x, y, rev, ang) /**/ * std::exp(a * add * add) /**/ /* * std::exp(2.0 * std::sqrt(std::log(2.0)) * idx / stones.size()) */;
  const double ph = env->get(idx + add, std::min(add, (BEAM - 1)), x, y, rev, ang);
  //return std::pow(hv, BETA) * std::exp(ph * ALPHA);
  return ::mypow(hv, DBETA) * std::exp(ph * ALPHA);
}

template <class F, class S>
std::pair<double, double> Ritalgo<F, S>::Ant::v2(const int idx, const int add, const int x, const int y, const int rev, const int ang) const noexcept
{
  //const double a = -log(4.0) / ((BEAM - 1) * (BEAM - 1));
  const double a = -1024.0 / ::mypow((double)remains[idx + add] / field->score(), 10);
  //std::cerr << add << " : " << std::exp(add * a) << std::endl;
  auto hv_ = h2(stones[idx + add], x, y, rev, ang);
  const std::pair<double, double> hv = { hv_.first * std::exp(add * a), hv_.second * std::exp(add * a) };
  //  const double hv = h(stones[idx + add], x, y, rev, ang) /**/ * std::exp(a * add * add) /**/ /* * std::exp(2.0 * std::sqrt(std::log(2.0)) * idx / stones.size()) */;
  const double ph = env->get(idx + add, std::min(add, (BEAM - 1)), x, y, rev, ang);
  //return std::pow(hv, BETA) * std::exp(ph * ALPHA);
  return{ ::mypow(hv.first, DBETA) * std::exp(ph * ALPHA), ::mypow(hv.second, DBETA) * std::exp(ph * ALPHA) };
}

template <class F, class S>
void Ritalgo<F, S>::Ant::run_over() noexcept
{
  int fir = 1;
  // for diversity
  //int who = skipper(mt);
  unsigned int idx = 0;
  unsigned int pre = 0;
  for (; idx < stones.size(); ++idx) {
    //if (idx == who)continue;
    std::vector<std::tuple<int, int, int, int, int, int>> list;
    std::vector<double> roulette;
    //std::vector<double> roulette2;
    double accum = 0.0;

    if (remains[idx] > field->score() && dist(mt) < 0.05) {
      continue;
    }
    //double accum2 = 0.0;
    if ((double)remains[idx] / field->score() < 1.2) {
      // DO NOT SKIP
      for (const auto & t : ok_list[idx]) {
        int x, y, rev, ang;
        std::tie(x, y, rev, ang) = t;
        if (field->appliable(stones[idx], x, y, rev, ang)) {
          list.push_back(std::make_tuple(idx, 0, x, y, rev, ang));
          //const std::pair<double, double> res = v2(pre, add + idx - pre, x, y, rev, ang);
          //accum += res.first;
          //roulette.push_back(accum);
          //accum2 += res.second;
          //roulette2.push_back(accum2);
          accum += v(idx, 0, x, y, rev, ang);
          roulette.push_back(accum);
        }
      }
    }
    else {
      for (unsigned int add = 0; add < (BEAM - 1) && idx + add < stones.size(); ++add) {
        const unsigned int next = idx + add;
        for (const auto & t : ok_list[next]) {
          int x, y, rev, ang;
          std::tie(x, y, rev, ang) = t;
          if (field->appliable(stones[next], x, y, rev, ang)) {
            list.push_back(std::make_tuple(next, 0, x, y, rev, ang));
            //const std::pair<double, double> res = v2(pre, add + idx - pre, x, y, rev, ang);
            //accum += res.first;
            //roulette.push_back(accum);
            //accum2 += res.second;
            //roulette2.push_back(accum2);
            accum += v(pre, add + idx - pre, x, y, rev, ang);
            roulette.push_back(accum);
          }
        }
      }
    }
    if (list.size() == 0) {
      continue;
    }
    //auto action_id = [&]() {
    //  return dist(mt) < 0.5 ?
    //    std::distance(roulette.begin(),
    //      std::lower_bound(roulette.begin(), roulette.end(), dist(mt) * accum)
    //      ) :
    //    std::distance(roulette2.begin(),
    //      std::lower_bound(roulette2.begin(), roulette2.end(), dist(mt) * accum2)
    //      );
    //}();
    auto action_id = std::distance(roulette.begin(),
      std::lower_bound(roulette.begin(), roulette.end(), dist(mt) * accum)
      );
    int tidx, x, y, rev, ang;
#ifdef _DEBUG
    std::cerr << action_id << "/" << list.size() << "..." << accum << std::endl;
#endif
    std::tie(tidx, std::ignore, x, y, rev, ang) = list[action_id];
    field->apply(stones[tidx], x, y, rev, ang);
    fir &= 0;
    idx = tidx;
    pre = tidx;
  }
}

template <class F, class S>
void Ritalgo<F, S>::Ant::renew(double anchor)
{
  int index = 0;
  int fir = 1;
  //int last = std::get<0>(*(field->get_history().rbegin()))->identify();
  int last = 255;
  int prev = 0;
  const double a = last*last / std::log(25.0); // at x = 0 => reward was 1/25
  for (auto e : field->get_history()) {
    index = std::get<0>(e)->identify();
    int x, y, rev, ang;
    std::tie(std::ignore, x, y, rev, ang) = e;
   
    env->put(index, std::min(index - prev, (BEAM - 1)), x, y, rev, ang, (std::pow(anchor, 3) - std::pow(score(), 3.0)) / std::pow(anchor, 3)
      /** * 50.0 * std::exp((-1 / a)*std::pow(last - index, 2.0)) /**/
      );
    prev = index;
  }
}

template <class F, class S>
void Ritalgo<F, S>::Ant::reset(std::unique_ptr<Field> field)
{
  this -> field = std::move(field);
}

template <class F, class S>
int Ritalgo<F, S>::Ant::score() const
{
  return field -> score();
}

template <class F, class S>
std::unique_ptr<Field> Ritalgo<F, S>::Ant::loot() const
{
  return std::move(field -> clone());
}

