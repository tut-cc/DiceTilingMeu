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
    std::shared_ptr<Ant> ptr(new Ant(field -> clone(), stones));
    ants.push_back(ptr);
  }
  std::cerr << "チェストー" << std::endl;
  for (;;) {
    for (auto ant : ants) {
      ant -> run();
    }
    for (auto ant : ants) {
      ant -> renew(env);
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
Ritalgo<F, S>::Ant::Ant(std::unique_ptr<Field> field, const std::vector<std::shared_ptr<Stone>> & stones)
{
  this -> field = std::move(field);
  std::copy(stones.begin(), stones.end(), std::back_inserter(this -> stones));
}

template <class F, class S>
void Ritalgo<F, S>::Ant::run()
{
  // TODO:
}

template <class F, class S>
void Ritalgo<F, S>::Ant::renew(std::shared_ptr<Env> env)
{
  int index = 0;
  for ( auto e : field -> get_history() ) {
    // TODO:
    // WANTED: Field -> score
  }
}

