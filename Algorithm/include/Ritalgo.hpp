#pragma once

#include "algorithm.hpp"
#include "field.hpp"
#include "stone.hpp"
#include "problem.hpp"
#include <memory>
#include <vector>
#include <type_traits>
#include <iostream>
#include <random>

template <class F, class S>
class Ritalgo : public Algorithm {
  private:
    std::unique_ptr<Field> field;
    std::vector<std::shared_ptr<Stone>> stones;
    class Env {
      private:
        static constexpr double EVAPORATE_RATE = 0.95;
        static constexpr int    GETA           = 8;
        union {
          double env[256][2][2][64][64][2][4];
          double raw[256 *2 *2 *64 *64 *2 *4];
        };
/* VER : heap
        double *******env;

        template <typename T, typename TSize>
        T make(T& ptr, const TSize ts);
        template <typename T, typename TSize, typename... RSize>
        T make(T& ptr, const TSize ts, const RSize... rs);

        template <typename T, typename TSize>
        T make(T& ptr, const std::shared_ptr<double>, const TSize ts);
        template <typename T, typename TSize, typename... RSize>
        T make(T& ptr, const std::shared_ptr<double>, const TSize ts, const RSize... rs);

        template <typename T, typename TSize>
        T destroy(T& ptr, const TSize ts);
        template <typename T, typename TSize, typename... RSize>
        T destroy(T& ptr, const TSize ts, const RSize... rs);
*/

        template <typename T, typename TSize>
        static void eva(T& ptr, const TSize ts);
        template <typename T, typename TSize, typename... RSize>
        static void eva(T& ptr, const TSize ts, const RSize... rs);
      public:
        Env() = default;
        ~Env() = default;
/* VER : heap
        Env();
        ~Env();
*/
        void put(int idx, int is, int fir, int x, int y, int rev, int ang, double phe);
        double get(int idx, int is, int fir, int x, int y, int rev, int ang) const;
        void eva();
    };
    class Ant {
      private:
        // TODO: PHEROMONE to constexpr value to function made from average score
        static constexpr double PHEROMONE = 1024.0;
        //static constexpr double ALPHA = 3.0;
        //static constexpr double BETA = 5.0;
        static constexpr double ALPHA = 10;
        static constexpr double BETA = 6.0;

        static double noise(const std::unique_ptr<Field>& src) {
          double sum = 0.0;
          for (int i = 1; i < 31; ++i) {
            for (int j = 1; j < 31; ++j) {
              double ave = 0.0;
              for (int k = -1; k <= 1; ++k) {
                for (int l = -1; l <= 1; ++l) {
                  ave += (1.0 / 9.0) * src->at(j + l, i + k);
                }
              }
              sum += std::pow(src->at(j, i) - ave, 2.0);
            }
          }
          return sum;
        }
        std::unique_ptr<Field> field;
        std::vector<std::shared_ptr<Stone>> stones;
        std::shared_ptr<Env> env;
        std::mt19937 mt;
        std::uniform_real_distribution<double> dist;
        std::uniform_int_distribution<int> skipper;
        double h(const std::shared_ptr<Stone> s, const int x, const int y, const int rev, const int ang) const;
        double v(const int idx, const int is, const int fir, const int x, const int y, const int rev, const int ang, const std::pair<int, int> prev) const;
        double v2(const int idx, const int is, const int fir, const int x, const int y, const int rev, const int ang, const std::pair<int, int> prev) const;
      public:
        Ant(std::unique_ptr<Field> field, const std::vector<std::shared_ptr<Stone>> & stones, std::shared_ptr<Env> env);
        ~Ant() = default;
        void run();
        void renew();
        void renew(double anchor);
        void reset(std::unique_ptr<Field> field);
        int score() const;
        std::unique_ptr<Field> loot() const;
    };
  public:
    Ritalgo(const std::shared_ptr<Problem> p);
    virtual ~Ritalgo() = default;
    virtual void solve();
};

#include "detail/Ritalgo.hpp"

