#pragma once
#include "Ritalgo.hpp"
#include <iostream>
#include <algorithm>
#include <iterator>

template <class F, class S>
Ritalgo<F, S>::Ritalgo(std::shared_ptr<Problem> p)
{
  field = std::move(std::unique_ptr<Field>(new F(p -> get_field_str())));
  for( auto&& s : p -> get_stones_str() ) {
    stones.push_back(std::shared_ptr<Stone>(new S(s, stones.size())));
  }
}

template <class F, class S>
void Ritalgo<F, S>::solve()
{
  std::shared_ptr<Env> env(new Env());
  std::vector<std::shared_ptr<Ant>> ants;
  for (int i = 0; i < 100; ++i) {
    std::shared_ptr<Ant> ptr(new Ant(field -> clone(), stones, env));
    ants.push_back(ptr);
  }
  std::cerr << "チェストー" << std::endl;
  for (;;) {
    for (auto ant : ants) {
      ant -> run();
    }
    for (auto ant : ants) {
      ant -> renew();
      ant -> reset(field -> clone());
    }
    env -> eva();
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
void Ritalgo<F, S>::Env::put(int idx, int is, int fir, int x, int y, int rev, int ang, double phe)
{
  env[idx][is][fir][x][y][rev][ang] += phe;
}

template <class F, class S>
double Ritalgo<F, S>::Env::get(int idx, int is, int fir, int x, int y, int rev, int ang) const
{
  return env[idx][is][fir][x][y][rev][ang];
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
Ritalgo<F, S>::Ant::Ant(std::unique_ptr<Field> field, const std::vector<std::shared_ptr<Stone>> & stones, std::shared_ptr<Env> env) : dist(0.0, 1.0)
{
  this -> field = std::move(field);
  std::copy(stones.begin(), stones.end(), std::back_inserter(this -> stones));
  this -> env = env;

  std::random_device rd;
  this -> mt . seed(rd());
}

template <class F, class S>
double Ritalgo<F, S>::Ant::h(std::shared_ptr<Stone> s, int x, int y, int rev, int ang) const
{
  int sum = 0;
  bool done[8][8] = {};
  bool flag = true;
  for (int i = 0; flag && i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (s -> at(j, i, rev, ang)) {
        // 幅優先
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
          for (int k = 0; k < 4; ++k) {
            int dx = e.first  + ofs[k][0];
            int dy = e.second + ofs[k][1];
            if ( dx < 0 || 8 <= dx || dy < 0 || 8 <= dy ) {
              continue;
            }
            if ( done[dy][dx] && s -> at( dx, dy, rev, ang ) ) {
              queue.push({dx, dy});
              done[dy][dx] |= true;
              continue;
            }

            int nx = x + dx;
            int ny = y + dy;
            if ( field -> at( nx, ny ) ) {
              ++sum;
            }
          }
          queue.pop();
        }
        flag &= false;
        break;
      }
    }
  }
  return sum;
}

template <class F, class S>
double Ritalgo<F, S>::Ant::v(const int idx, const int is, const int fir, const int x, const int y, const int rev, const int ang, const std::pair<int, int> prev) const
{
  const double hv = is ? h(stones[idx], prev.first + x, prev.second + y, rev, ang) : 0.0;
  const double ph = env -> get(idx, is, fir, x, y, rev, ang);
  return std::pow(hv + 1, BETA) * std::pow(ph + 1, ALPHA);
}

template <class F, class S>
void Ritalgo<F, S>::Ant::run()
{
  int fir = 1;
  for (int idx = 0; idx < 256; ++idx) {
    std::vector<std::tuple<int, int, int, int, int, int>> list;
    std::vector<double> roulette;
    double accum = 0.0;
    list.push_back(std::make_tuple(0, fir, 0, 0, 0, 0));
    accum += v(idx, 0, fir, 0, 0, 0, 0, {0, 0});
    roulette.push_back(accum);
    static constexpr int GETA = 7;
    const std::pair<int, int> zr = {
      -7,
      fir ? 31 : 7
    };
    const std::pair<int, int> prev = [&](){
      if (fir) {
        return std::make_pair(0, 0);
      }
      int gx, gy;
      std::tie(std::ignore, gx, gy, std::ignore, std::ignore) = *(field -> get_history() .rend());
      return std::make_pair(gx, gy);
    }();
    for (int x = zr.first; x <= zr.second; ++x) {
      for (int y = zr.first; y <= zr.second; ++y) {
        for (int rev = 0; rev < 2; ++rev) {
          for (int ang = 0; ang < 4; ++ang) {
            if ( field -> appliable(stones[idx], prev.first + x, prev.second + y, rev, ang) ) {
              list.push_back(std::make_tuple(1, fir, x, y, rev, ang));
              accum += v(idx, 1, fir, x + GETA, y + GETA, rev, ang, prev);
              roulette.push_back(accum);
            }
          }
        }
      }
    }
    auto action_id = std::distance(roulette.begin(),
        std::lower_bound(roulette.begin(), roulette.end(), dist(mt) * accum)
    );
    int is, x, y, rev, ang;
    std::tie(is, std::ignore, x, y, rev, ang) = list[action_id];
    if (!is) continue;
    field -> apply( stones[idx], x, y, rev, ang );
  }
}

template <class F, class S>
void Ritalgo<F, S>::Ant::renew()
{
  int index = 0;
  for ( auto e : field -> get_history() ) {
    // TODO:
    // WANTED: Field -> score
  }
}

template <class F, class S>
void Ritalgo<F, S>::Ant::reset(const std::unique_ptr<Field> field)
{
}
