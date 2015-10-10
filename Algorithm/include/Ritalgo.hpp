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
    std::vector <std::vector < std::tuple < int, int, int, int >>> ok_list;
    class Env {
      public:
        static constexpr int    BEAM = 4;
      private:
        static constexpr double EVAPORATE_RATE = 0.95;
        static constexpr int    GETA           = 8;
        union {
          double env[256][BEAM][64][64][2][4];
          double raw[256 *BEAM *64 *64 *2 *4];
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
        void put(int idx, int add, int x, int y, int rev, int ang, double phe);
        double get(int idx, int add, int x, int y, int rev, int ang) const;
        void eva();
    };
    class Ant {
      private:
        // TODO: PHEROMONE to constexpr value to function made from average score
        static constexpr double PHEROMONE = 1024.0;
        //static constexpr double ALPHA = 3.0;
        //static constexpr double BETA = 5.0;
        static constexpr double ALPHA = 10.0;
        static constexpr double BETA = 5.0;
        static constexpr int    BEAM = Env::BEAM;

        std::unique_ptr<Field> field;
        std::vector<std::shared_ptr<Stone>> stones;
        const std::vector<std::vector<std::tuple<int, int, int, int>>> &ok_list;
        std::shared_ptr<Env> env;
        std::mt19937 mt;
        std::uniform_real_distribution<double> dist;
        std::uniform_int_distribution<int> skipper;
        double h(const std::shared_ptr<Stone> s, const int x, const int y, const int rev, const int ang) const noexcept;
        double v(const int idx, const int add, const int x, const int y, const int rev, const int ang) const;
        double v2(const int idx, const int add, const int x, const int y, const int rev, const int ang) const noexcept;
      public:
        Ant(std::unique_ptr<Field> field, const std::vector<std::shared_ptr<Stone>> & stones, const std::vector<std::vector<std::tuple<int, int, int, int>>> & ok_list, std::shared_ptr<Env> env);
        ~Ant() = default;
        void run() noexcept;
        void run_over() noexcept;
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

