# -*- coding: utf-8 -*-
import copy
import multiprocessing
import random
import sys
import matplotlib
import matplotlib.pyplot as plt
from deap import base
from deap import creator
from deap import tools

#隣接マップの4方向と自分の位置の平滑化
def MakeAdjustMeanMap(ma):
    adjust_mean_map = copy.deepcopy(ma)
    for i in xrange(0, 32):
        for j in xrange(0, 32):
            #隣接度を計算する関数 ma[i][j] == '0'のときはおけないので隣接度0にする
            adjust_sum = 0
            for ydiff in [0, 0, 1, 0, -1]:
                for xdiff in [0, 1, 0, -1, 0]:
                    ny = i+ydiff
                    nx = j+xdiff
                    add = 0
                    if ny < 0 or ny >= 32 or nx <0 or nx >= 32:
                        add = 3
                    else:
                        add = ma[ny][nx]

                    adjust_sum += add
            adjust_sum /= 5 
                        
            adjust_mean_map[i][j] = adjust_sum
    return adjust_mean_map

#隣接マップの作成
def MakeAdjustMap(ma):
    adjust_map = copy.deepcopy(ma)
    for i in xrange(0, 32):
        for j in xrange(0, 32):
            #隣接度を計算する関数 ma[i][j] == '0'のときはおけないので隣接度0にする
            AdjustValue = lambda i, j: sum([1 for y in xrange(i-1, i+2) for x in xrange(j-1, j+2) if  abs(x-j) + abs(y-i) <= 1 and (y < 0 or x < 0 or y >= 32 or x >= 32 or ma[y][x] == '1') and ma[i][j] == '0'])
            adjust_map[i][j] = AdjustValue(i, j)
    return adjust_map

#石の回転
def ChangeStone(st, state):
    if state == 0:  #なにもしない
        pass
    elif state == 1: #裏返し
        for i in xrange(0, 8):
            st[i] = st[i][::-1]
    elif state == 2: #90度回転
        st2 = [[None for col in range(8)] for row in range(8)]
        for i in xrange(0, 8):
            for j in xrange(0, 8):
                st2[i][j] = st[7-j][i]
        st = st2
    elif state == 3: #180度回転
        st = st[::-1]
    elif state == 4: #270度回転
        st2 = [[None for col in range(8)] for row in range(8)]
        for i in xrange(0, 8):
            for j in xrange(0, 8):
                st2[i][j] = st[j][7-i]
        st = st2
    elif state == 5: #裏返し+90度
        for i in xrange(0, 8):
            st[i] = st[i][::-1]
        st2 = [[None for col in range(8)] for row in range(8)]
        for i in xrange(0, 8):
            for j in xrange(0, 8):
                st2[i][j] = st[7-j][i]
        st = st2
    elif state == 6: #裏返し+180度
        for i in xrange(0, 8):
            st[i] = st[i][::-1]
        st = st[::-1]
    elif state == 7: #裏返し+270度
        for i in xrange(0, 8):
            st[i] = st[i][::-1]
        st2 = [[None for col in range(8)] for row in range(8)]
        for i in xrange(0, 8):
            for j in xrange(0, 8):
                st2[i][j] = st[j][7-i]
        st = st2
    else: 
        pass

    return st

#石のセット(Debug)
def SetStoneDebug(ma, stone_n, stones, st, x, y, zk_pos_list):
    score = 0
    f = True                #正常にセットできているか(Debug)
    for i in xrange(0, 8):
        for j in xrange(0, 8):
            ny = y + i
            nx = x + j
            if st[i][j] == '1':
                if ny < 32 and nx < 32 and ma[ny][nx] == '0':
                    ma[ny][nx] = '1'
                    score += 1 
                else:
                    f = False
                    score -= 1000
    #if f: score = -1 
    return score, ma

#石のセット
def SetStone(ma, stone_n, stones, st, x, y, zk_pos_list):
    score = 0
    for zk_pos in zk_pos_list:
        zk_y = y + zk_pos[0]
        zk_x = x + zk_pos[1]
        ma[zk_y][zk_x] = '1'
        score += 1
    return score, ma

#隣接度の合計が最も高くなる座標を探索
def GetGoodLocation(adjust_map, ma, stone, zk_pos_list):
    loc = (-10, -10)
    max_point = -10

    for y in xrange(-7, 32):
        for x in xrange(-7, 32):
            adjust_point = 0            #隣接度の合計
            for zk_pos in zk_pos_list:
                zk_y = y + zk_pos[0]
                zk_x = x + zk_pos[1]

                #そこにおけないならポイントは無効
                if zk_y < 0 or zk_x < 0 or zk_y >= 32 or zk_x >= 32 :
                    adjust_point = -10
                    break
                if ma[zk_y][zk_x] == '1':
                    adjust_point = -10
                    break
                adjust_point += adjust_map[zk_y][zk_x] #隣接度を足す
            if adjust_point > max_point:
                max_point = adjust_point
                loc = (y, x)

    if max_point == 0: loc = (-10, -10) #隣接度の合計が0ということは１つも隣接していない。これは許されない
    return loc[0], loc[1]

#GAの初期化
def InitGA(ma, stone_n, stones, zk_list):
    creator.create("FitnessMax", base.Fitness, weights=(1.0,))
    creator.create("Individual", list, fitness=creator.FitnessMax)
    toolbox = base.Toolbox()
    toolbox.register("attr_bool", random.randint, 0, 7)
    #使うか使わないかの自由度は持たせない方がいいようなので現在stone_nになっている
    toolbox.register("individual", tools.initRepeat, creator.Individual, toolbox.attr_bool, stone_n)
    #toolbox.register("individual", tools.initRepeat, creator.Individual, toolbox.attr_bool, 2*stone_n)
    toolbox.register("population", tools.initRepeat, list, toolbox.individual)

    #評価関数
    def evalOneMax(individual):
        nowmap = copy.deepcopy(ma)
        totalscore = 0
        for i in xrange(0, stone_n):
            #used   = individual[2*i]  #この石を使うか
            #state   = individual[2*i+1]   #回転裏返しの状態
            used   = 1  #この石を使うか
            state   = individual[i]   #回転裏返しの状態
            if used == 0: #この石使わないとき
                continue 
            
            #回転後の石の配置を求める
            st = ChangeStone(copy.deepcopy(stones[i]), state)
            #隣接mapを求める
            adjust_map = MakeAdjustMap(nowmap)
            #adjust_mean_map = MakeAdjustMeanMap(adjust_map)
            #石を置く場所を決める
            y, x = GetGoodLocation(adjust_map, nowmap, st, zk_list[i][state])
            if y == -10 and x == -10: #石をおける場所がないということなのでこの石を使わないようにする
                #individual[2*i] = 0
                individual[i] = 0
                continue
            #石をセット
            score, newmap = SetStoneDebug(list(nowmap), stone_n, stones, st, x, y, zk_list[i][state])
            '''
            for line in newmap:
                print "".join(line)
            print "==================="
            '''
            nowmap = newmap
            if score == -1: return 0, #debug

            #評価値に加算
            totalscore += score

        return totalscore,

    toolbox.register("evaluate", evalOneMax)
    toolbox.register("mate", tools.cxTwoPoints)
    toolbox.register("mutate", tools.mutFlipBit, indpb=0.05)
    toolbox.register("select", tools.selTournament, tournsize=3)
    
    return toolbox

#入力
def InputMap():
    #マップの読み込み
    ma = []
    for i in xrange(0, 32):
        ma.append(list(raw_input().strip()))
    
    raw_input()

    #石の読み込み
    stone_n = int(raw_input())
    stones = []
    for i in xrange(0, stone_n):
        stone = []
        for j in xrange(0, 8):
            stone.append(list(raw_input().strip()))
        stones.append(stone)
        sys.stdin.readline()

    return ma, stone_n, stones

#出力(graph output 込み) 一からシミュレートしている
def OutputMap(ma, individual, stone_n, stones, zk_list):
    f = open('result.txt', 'w')
    nowmap = copy.deepcopy(ma)

    #グラフの用意
    fig = plt.figure()
    ax = fig.add_subplot(111)

    for i in xrange(0, stone_n):
        #used   = individual[2*i] % 2  #この石を使うか
        #state   = individual[2*i+1]   #回転裏返しの状態
        used   = 1  #この石を使うか
        state   = individual[i]   #回転裏返しの状態
        side = 'H'                  #表か裏か
        angle = 0                   #角度

        if used == 0: #この石使わないとき
            f.write('\n')  #結果出力
            continue 

        if state == 1 or state == 5 or state == 6 or state == 7: #裏の時
            side = 'T'
        #角度の設定
        if state == 2 or state == 5:
            angle = 90
        if state == 3 or state == 6:
            angle = 180
        if state == 4 or state == 7:
            angle = 180

        #回転後の石の配置を求める
        st = ChangeStone(copy.deepcopy(stones[i]), state)
        #隣接mapを求める
        adjust_map = MakeAdjustMap(nowmap)
        #石を置く場所を決める
        y, x = GetGoodLocation(adjust_map, nowmap, st, zk_list[i][state])
        if y == -10 and x == -10: #石をおける場所がないということなのでこの石を使わないようにする
            #individual[2*i] = 0
            individual[i] = 0
            f.write('\n')  #結果出力
            continue
        else:
            f.write("%d %d %c %d\n" % (x, y, side, angle)) #結果出力
        #石をセットしてスコアを計算
        score, newmap = SetStoneDebug(list(nowmap), stone_n, stones, st, x, y, zk_list[i][state])
        if score == -1: return 0,
        '''
        for line in newmap:
            print "".join(line)
        print "==================="
        '''
        nowmap = list(newmap)


    #マップの配置済みのところを調べてgraph表示
    cnt = 0
    for i in xrange(0, 32):
        for j in xrange(0, 32):
            if nowmap[i][j] == '1':
                cnt += 1
                rect = plt.Rectangle((j, -i), 1, 1, fc="#770000")
                ax.add_patch(rect)

    print "remain cell=",1024 - cnt
    plt.xlim(0, 32)
    plt.ylim(-31, 1)
    plt.show()

#ズクの座標リストの生成
def MakeZkList(stone_n, stones):
    zk_list = []
    for stone in stones:
        zk_state_list = [] #石が回転した時のそれぞれの座標リスト
        for i in xrange(0, 8):
            #石の回転
            change_stone = ChangeStone(copy.deepcopy(stone), i)
            #1つの石のズク状態リストに追加
            zk_state_list.append([(y, x) for y in xrange(0, 8) for x in xrange(0, 8) if change_stone[y][x] == '1'])
        #石リストに追加
        zk_list.append(zk_state_list)
    return zk_list

def main():
    START_N = 10
    CXPB, MUTPB, NGEN = 0.5, 0.2, 3

    #入力
    ma, stone_n, stones = InputMap()

    #石の1になっている座標リストの取得
    zk_list = MakeZkList(stone_n, stones)
    adjust_map = MakeAdjustMap(ma)

    '''
    for x in adjust_map:
        for y in x:
            print y,
        print ""
    '''

    #GA初期化
    toolbox = InitGA(copy.deepcopy(ma), stone_n, stones, zk_list)
    #並列計算
    #pool = multiprocessing.Pool(8)
    #toolbox.register("map", pool.map)

    #乱数の種
    random.seed(64)
    #初期個体群を生成
    pop = toolbox.population(n=START_N)

    print("Start of evolution")
    best_ind = tools.selBest(pop, 1)[0]

    #初期の個体群の評価
    fitnesses = list(map(toolbox.evaluate, pop))
    for ind, fit in zip(pop, fitnesses):
        ind.fitness.values = fit

    print(" Evaluated %i individuals" % len(pop))

    #進化計算開始
    for g in range(NGEN):
        print("-- Generation %i --" % g)

        #次世代の個体群を選択
        offspring = toolbox.select(pop, len(pop))
        #個体群のクローンを生成
        offspring = list(map(toolbox.clone, offspring))

        #選択した個体群に交差と突然変異を適応する
        #偶数番目と奇数番目の個体を取り出して交差
        for child1, child2 in zip(offspring[::2], offspring[1::2]):
            if random.random() < CXPB:
                toolbox.mate(child1, child2)
                del child1.fitness.values
                del child2.fitness.values

        for mutant in offspring:
            if random.random() < MUTPB:
                toolbox.mutate(mutant)
                del mutant.fitness.values

        # 適合度が計算されていない個体を集めて適合度を計算
        invalid_ind = [ind for ind in offspring if not ind.fitness.valid]
        fitnesses = map(toolbox.evaluate, invalid_ind)
        for ind, fit in zip(invalid_ind, fitnesses):
            ind.fitness.values = fit

        print(" Evaluated %i individuals" % len(invalid_ind))

        #次世代群をoffspringにする
        pop[:] = offspring

        #すべての個体の適合度を配列にする
        fits = [ind.fitness.values[0] for ind in pop]

        length = len(pop)
        mean = sum(fits) / length
        sum2 = sum(x*x for x in fits)
        std = abs(sum2 / length - mean**2)**0.5

        print(" Min %s" % min(fits))
        print(" Max %s" % max(fits))
        print(" Avg %s" % mean)
        print(" Std %s" % std)

    print("-- End of (successful) evolution --")

    best_ind = tools.selBest(pop, 1)[0]
    #結果とグラフの出力
    OutputMap(copy.deepcopy(ma), best_ind, stone_n, stones, zk_list)
    print("Best individual is %s, %s" % (best_ind, best_ind.fitness.values))

    #pool.close()

if __name__ == "__main__":main()
