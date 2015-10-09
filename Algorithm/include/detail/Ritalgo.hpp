#pragma once
#include "Ritalgo.hpp"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <valarray>

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
}

template <class F, class S>
void Ritalgo<F, S>::solve()
{
  std::shared_ptr<Env> env(new Env());
  std::vector<std::shared_ptr<Ant>> ants;
  for (int i = 0; i < 100; ++i) {
    std::shared_ptr<Ant> ptr(new Ant(field -> clone(), stones, ok_list, env));
    ants.push_back(ptr);
  }
  int best_score = 1 << 28;
  std::unique_ptr<Field> best_field;
  for (int ite = 0; ite < 32; ++ite) {
    std::cerr << "iteration : " << ite << std::endl;
    double ave_score = 0.0;
    env->eva();
    // for (auto ant : ants) {
#pragma omp parallel for reduction(+:ave_score)
    for (int i = 0; i < ants.size(); ++i) {
      const auto &ant = ants[i];
      ant -> run_over();
      ave_score += (double)ant->score() / 100;
      if ( ant -> score() < best_score ) {
        best_score = ant -> score();
        //std::cout << *(ant -> loot()) << std::endl;
        best_field = std::move(ant->loot());
        std::cerr << ant->score() << " ... " << ant -> loot() -> get_history().size() << std::endl;
      }
      else if (ant->score() == best_score) {
        if (ant->loot()->get_history().size() < best_field->get_history().size()) {
          best_field = std::move(ant->loot());
          std::cerr << ant->score() << " ... " << ant->loot()->get_history().size() << std::endl;
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
  std::cout << *best_field;
  for (int i = std::get<0>(*(best_field->get_history().rbegin()))->identify() + 1; i < stones.size(); ++i)
    std::cout << std::endl;
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
void Ritalgo<F, S>::Env::put(int idx, int is, int fir, int x, int y, int rev, int ang, double phe)
{
#ifdef _DEBUG
  if (!( 0 <= x + GETA && x + GETA < 64 && 0 <= y + GETA && y + GETA < 64 ) ) {
    std::cerr << "AHHHHHHHHHH" << std::endl;
  }
#endif
  env[idx][is][fir][x + GETA][y + GETA][rev][ang] += phe;
}

template <class F, class S>
double Ritalgo<F, S>::Env::get(int idx, int is, int fir, int x, int y, int rev, int ang) const
{
#ifdef _DEBUG
  if (!( 0 <= x + GETA && x + GETA < 64 && 0 <= y + GETA && y + GETA < 64 ) ) {
    std::cerr << "AHHHHHHHHHH" << std::endl;
  }
#endif
  return env[idx][is][fir][x + GETA][y + GETA][rev][ang];
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
  eva(env, 256, 2, 2, 64, 64, 2, 4);
}


template <class F, class S>
Ritalgo<F, S>::Ant::Ant(std::unique_ptr<Field> field, const std::vector<std::shared_ptr<Stone>> & stones, const std::vector<std::vector<std::tuple<int, int, int, int>>> & ok_list, std::shared_ptr<Env> env) : dist(0.0, 1.0), skipper(0, stones.size()-1), ok_list(ok_list)
{
  this -> field = std::move(field);
  std::copy(stones.begin(), stones.end(), std::back_inserter(this -> stones));
  this -> env = env;

  std::random_device rd;
  this -> mt . seed(rd());
}

template <class F, class S>
double Ritalgo<F, S>::Ant::h(std::shared_ptr<Stone> s, int x, int y, int rev, int ang) const noexcept
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
      if (s -> at(j, i, rev, ang)) {
        ++fill;
        std::queue<std::pair<int,int>> queue;
        queue.push({j, i});
        while (queue.size()) {
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
              //++cell[dy + 1][dx + 1];
              //chck[dy + 1][dx + 1] |= true;
              continue;
            }

            if ( dx < 0 || 8 <= dx || dy < 0 || 8 <= dy ) {
              if (field->at(nx, ny)) {
                ++sum;
                ++cell[dy + 1][dx + 1];
              }
              else {
                --cell[dy + 1][dx + 1];
              }
              chck[dy + 1][dx + 1] |= true;
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
                ++cell[dy + 1][dx + 1];
              }
              else {
                --cell[dy + 1][dx + 1];
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
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      if (chck[i][j])
        acm += std::pow(4.0, cell[i][j]);
    }
  }

  double dist = [&]() {
    auto ct = s->center(rev, ang);
    std::valarray<double> arr = {ct.first + x, ct.second + y};
    arr -= 16;
    arr *= arr;
    return std::sqrt(arr.sum() + 0.1);
//    return arr.min() + 1.0;
  }();

  return acm;
}

// 
template <class F, class S>
double Ritalgo<F, S>::Ant::v(const int idx, const int is, const int fir, const int x, const int y, const int rev, const int ang) const
{
  const double hv = (is ? h(stones[idx], x, y, rev, ang) : fir ? 0.0 : 1.0) /* * ( std::exp(-4.0 * std::log(2.0) * std::pow((double)idx / stones.size(), 2.0) )) /**/;
  const double ph = env -> get(idx, is, fir, x, y, rev, ang);
  return std::pow(hv + 1, BETA) * std::pow(ph + 1, ALPHA);
}

template <class F, class S>
double Ritalgo<F, S>::Ant::v2(const int idx, const int next, const int fir, const int x, const int y, const int rev, const int ang) const noexcept
{
  const double hv = h(stones[next], x, y, rev, ang) * std::exp(-2.0 * (next - idx)) /* * std::exp(2.0 * std::sqrt(std::log(2.0)) * idx / stones.size()) */;
  const double ph = env->get(next, 1, fir, x, y, rev, ang);
  //return std::exp(hv * BETA) * std::exp(ph * ALPHA);
  return std::pow(hv, BETA) * std::exp(ph * ALPHA);
}


template <class F, class S>
void Ritalgo<F, S>::Ant::run() noexcept
{
  int fir = 1;
  // for diversity
  for (unsigned int idx = 0; idx < stones.size(); ++idx) {
    std::vector<std::tuple<int, int, int, int, int, int>> list;
    std::vector<double> roulette;
    double accum = 0.0;
    double avev2 = 0.0;
    int    cntv2 = 0;
    double maxv2 = 0.0;
    // ATTENTION : variable "accum" maybe going to overflow.
    for (const auto & t : ok_list[idx]) {
      int x, y, rev, ang;
      std::tie(x, y, rev, ang) = t;
      if (field->appliable(stones[idx], x, y, rev, ang)) {
        list.push_back(std::make_tuple(1, fir, x, y, rev, ang));
        double vv2 = v2(idx, idx, fir, x, y, rev, ang);
        accum += vv2;
        avev2 += vv2;
        maxv2 = std::max(vv2, maxv2);
        ++cntv2;
        //accum += v(idx, 1, fir, x, y, rev, ang);
        roulette.push_back(accum);
      }
    }
    avev2 /= cntv2;
    list.push_back(std::make_tuple(0, fir, 0, 0, 0, 0));
    accum += avev2;
    roulette.push_back(accum);
    auto action_id = std::distance(roulette.begin(),
      std::lower_bound(roulette.begin(), roulette.end(), dist(mt) * accum)
      );
    int is, x, y, rev, ang;
#ifdef _DEBUG
    std::cerr << action_id << "/" << list.size() << "..." << accum << std::endl;
#endif
    std::tie(is, std::ignore, x, y, rev, ang) = list[action_id];
    if (!is) continue;
    //field -> apply( stones[idx], prev.first + x, prev.second + y, rev, ang );
    field->apply(stones[idx], x, y, rev, ang);
    fir &= 0;
  }
}


template <class F, class S>
void Ritalgo<F, S>::Ant::run_over() noexcept
{
  int fir = 1;
  // for diversity
  //int who = skipper(mt);
  unsigned int idx = 0;
  for (; idx < stones.size(); ++idx) {
    //if (idx == who)continue;
    std::vector<std::tuple<int, int, int, int, int, int>> list;
    std::vector<double> roulette;
    double accum = 0.0;
    for (unsigned int next = idx; next < idx + 4 && next < stones.size(); ++next) {
      // ATTENTION : variable "accum" maybe going to overflow.
      for (const auto & t : ok_list[next]) {
        int x, y, rev, ang;
        std::tie(x, y, rev, ang) = t;
        if (field->appliable(stones[next], x, y, rev, ang)) {
          list.push_back(std::make_tuple(next, fir, x, y, rev, ang));
          accum += v2(idx, next, fir, x, y, rev, ang);
          roulette.push_back(accum);
        }
      }
    }
    if (list.size() == 0) {
      continue;
    }
    auto action_id = std::distance(roulette.begin(),
      std::lower_bound(roulette.begin(), roulette.end(), dist(mt) * accum)
      );
    int tidx, x, y, rev, ang;
#ifdef _DEBUG
    std::cerr << action_id << "/" << list.size() << "..." << accum << std::endl;
#endif
    std::tie(tidx, std::ignore, x, y, rev, ang) = list[action_id];
    //field -> apply( stones[idx], prev.first + x, prev.second + y, rev, ang );
    field->apply(stones[tidx], x, y, rev, ang);
    fir &= 0;
    idx = tidx;
  }
}

template <class F, class S>
void Ritalgo<F, S>::Ant::renew()
{
  int index = 0;
  int fir = 1;
  const double l = field -> score() + 1.0;
  for ( auto e : field -> get_history() ) {
    for(; index < std::get<0>(e) -> identify(); ++index) {
      env -> put(index, 0, fir, 0, 0, 0, 0, (PHEROMONE / l)
        /* * (1.0 - std::exp(-100.0*std::pow((double)index, 2.0))) /**/
        );
    }
    int x, y, rev, ang;
    std::tie(std::ignore, x, y, rev, ang) = e;
    env -> put(index++, 1, fir, x, y, rev, ang, (PHEROMONE / l)
      /* * (1.0 - std::exp(-100.0*std::pow((double)index, 2.0))) /**/
      );
  }
}

template <class F, class S>
void Ritalgo<F, S>::Ant::renew(double anchor)
{
  int index = 0;
  int fir = 1;
  //int last = std::get<0>(*(field->get_history().rbegin()))->identify();
  int last = 255;
  double a = last*last / std::log(25.0); // at x = 0 => reward was 1/25
  for (auto e : field->get_history()) {
    for (; index < std::get<0>(e)->identify(); ++index) {
      env->put(index, 0, fir, 0, 0, 0, 0, (std::pow(anchor, 3) - std::pow(score(), 3.0)) / std::pow(anchor, 3)
        /**/ * 50.0 * std::exp((-1 / a)*std::pow(last - index, 2.0)) /**/
        );
    }
    int x, y, rev, ang;
    std::tie(std::ignore, x, y, rev, ang) = e;
    env->put(index++, 1, fir, x, y, rev, ang, (std::pow(anchor, 3) - std::pow(score(), 3.0)) / std::pow(anchor, 3)
      /**/ * 50.0 * std::exp((-1 / a)*std::pow(last - index, 2.0)) /**/
      );
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

