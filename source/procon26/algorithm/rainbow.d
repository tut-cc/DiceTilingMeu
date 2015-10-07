module procon26.algorithm.rainbow;

import std.algorithm        : map, sum;
import std.container        : RedBlackTree;
import std.math;

import procon26.field;
import procon26.problem;
import procon26.stone;
import procon26.util;
import std.parallelism;


version(unittest) import std.stdio;
import std.stdio;



struct RainbowSearchState
{
    alias RBTree = RedBlackTree!(LazyField, "a.numOfAdjacents < b.numOfAdjacents");

    static
    struct Stage
    {
        size_t thr;
        size_t thrSlot;
        size_t minNumOfAdj = size_t.max;
        RBTree[] slots; // indexed by numOfRemainStones
        size_t leastRS = 0;
        size_t limitStoneID = size_t.max;


        bool insert(ref LazyField f)
        {
            if(f.lastStoneID > limitStoneID){
                //writeln("error");
                return false;
            }

            if(f.numOfRemainStones < leastRS)
                return false;

            if((f.numOfRemainStones - leastRS) >= slots.length){
                slots.length = (f.numOfRemainStones - leastRS) + 1;

                if(slots.length > thrSlot){
                    leastRS += (slots.length - thrSlot);
                    slots = slots[$-thrSlot .. $];
                }
            }

            RBTree slot = slots[f.numOfRemainStones - leastRS];
            if(slot is null){
                slot = new RBTree;
                slots[f.numOfRemainStones - leastRS] = slot;
            }

            immutable nAdj = f.numOfAdjacents;

            if(slot.length < thr){
                minNumOfAdj = min(minNumOfAdj, nAdj);
                slot.insert(f);
                return true;
            }else if(nAdj < minNumOfAdj){
                slot.insert(f);
                slot.removeBack();
                minNumOfAdj = slot.back().numOfAdjacents;
                return true;
            }

            return false;
        }


        int opApply(int delegate(ref LazyField) dg)
        {
            int res = 0;
            foreach(k, ref slot; slots) if(slot !is null)
            {
                foreach(ref e; slot[]){
                    res = dg(e);
                    if(res) return res;
                }
            }

            return res;
        }
    }


    bool insert(ref LazyField field)
    {
        return stages[field.numOfEmpty].insert(field);
    }


    Problem problem;
    Stage[] stages;


    GeneralField rainbowSearch()
    {
        foreach_reverse(i, ref stage; stages)
        {
            //writefln("     %s: %s, %s", i, stage.slots[].map!"a is null ? 0 : 1".sum(), stage.slots[].map!"a is null ? 0 : a.length");

            // for GC, release unused state
            if(stage.slots[].map!"a is null ? 0 : a.length".sum()){
                foreach_reverse(j; i+1 .. stages.length)
                    stages[j] = Stage.init;
            }

            foreach(ref LazyField field; stage){
                auto gf = field.commit();
                foreach(ref LazyField next; gf.nextFields(problem))
                    this.insert(next);
            }
        }

        foreach(ref stage; stages)
            if(stage.slots[].map!"a is null ? 0 : a.length".sum()){
                auto tree = new RedBlackTree!(GeneralField, "a.history.length < b.history.length");
                foreach(ref LazyField ee; stage){
                    auto gf = ee.commit();
                    tree.insert(gf);
                }

                return tree.front;
            }

        return null;
    }
}


auto nextFields(GeneralField field, Problem problem)
{
    static struct Result()
    {
        int opApply(int delegate(ref LazyField) dg)
        {
            int res = 0;
            foreach(byte x, byte y; _field.byAdjacentZk){
                foreach(id; _field.history[$-1].stone.id+1 .. min(_problem.stones.length, _field.history[$-1].stone.id+25)){
                    auto problemStone = _problem.stones[id];
                    foreach(ss; problemStone.uniqueState){
                        auto stone = problemStone[ss];
                        foreach(byte xx, byte yy; stone.byZk){
                            byte xxx = cast(byte)(x - xx);
                            byte yyy = cast(byte)(y - yy);
                            if(!_field.isCollided(xxx, yyy, stone)){
                                //LazyField newField = new LazyField(_field, xxx, yyy, stone);
                                auto newField = LazyField(_field, xxx, yyy, stone);
                                res = dg(newField);
                                if(res) return res;
                            }
                        }
                    }
                }
            }
            return res;
        }


      private:
        GeneralField _field;
        Problem _problem;
    }


    return Result!()(field, problem);
}


size_t calcSlotSize(size_t idxOfStage, size_t numOfEmpty)
{
    if(numOfEmpty < 100){
        if(idxOfStage < 50) return 100;
        else return 100;
    }
    else if(numOfEmpty < 500){
        if(idxOfStage < 20) return 32;
        else if(idxOfStage < 50) return 16;
        else if(idxOfStage < 100) return 4;
        else if(idxOfStage < 200) return 4;
        else return 4;
    }else{
        if(idxOfStage < 50) return 32;
        else if(idxOfStage < 100) return 16;
        else if(idxOfStage < 200) return 4;
        else if(idxOfStage < 400) return 2;
        else return 1;
    }
}


size_t calcThrValue(size_t idxOfStage, size_t numOfEmpty)
{
    if(numOfEmpty < 100){
        if(idxOfStage < 50) return 1024*1024;
        else return 1024*512;
    }
    else if(numOfEmpty < 500){
        if(idxOfStage < 20) return 256;
        else if(idxOfStage < 50) return 64;
        else if(idxOfStage < 100) return 32;
        else if(idxOfStage < 200) return 16;
        else if(idxOfStage < 400) return 8;
        else return 4;
    }else{
        if(idxOfStage < 50) return 256;
        else if(idxOfStage < 100) return 32;
        else if(idxOfStage < 200) return 8;
        else if(idxOfStage < 400) return 4;
        else return 2;
    }
}



GeneralField simpleRainbowSearch(Problem problem)
{
    GeneralField minResult = null;


    size_t[] sumOfZk = new size_t[problem.stones.length];
    foreach_reverse(i, st; problem.stones){
        if(i == problem.stones.length -1)
            sumOfZk[i] = st.numOfZk;
        else
            sumOfZk[i] = sumOfZk[i+1] + st.numOfZk;
    }
    //writeln(sumOfZk);


    size_t[] limitStoneID = new size_t[problem.numOfEmpty + 1];
    {
        foreach_reverse(i, ref e; limitStoneID){
            size_t lastIdx = problem.stones.length-1;
            while(lastIdx > 0 && sumOfZk[lastIdx] < i) --lastIdx;
            lastIdx += 1;
            e = lastIdx;
            lastIdx = problem.stones.length-1;
        }
    }
    //writeln(limitStoneID);
    //return null;


    static
    struct TaskInput
    {
        InstantiatedStone stone;
    }

    TaskInput[] inputs;

    foreach(i, stone; problem.stones)
        foreach(ss; stone.uniqueState)
            inputs ~= TaskInput(stone[ss]);

    foreach(task; inputs.parallel(1)){
        writefln("%s, %s", task.stone.id, task.stone.state);

        //auto st = task.stone[ss];
        auto st = task.stone;

        RainbowSearchState state;
        state.problem = problem;
        state.stages.length = problem.numOfEmpty + 1;
        foreach(i, ref e; state.stages){
            e.thr = size_t.max;
            e.thrSlot = calcSlotSize(i, problem.numOfEmpty);
        }

        size_t cnt;
        foreach(byte x; -8 .. 32) foreach(byte y; -8 .. 32) if(!problem.initField.isCollided(x, y, st))
        {
            ++cnt;
            auto lf = LazyField(problem.stones.length, problem.initField, problem.numOfEmpty, x, y, st);
            state.insert(lf);
        }

        foreach(i, ref e; state.stages){
            e.thr = calcThrValue(i, problem.numOfEmpty);
            e.limitStoneID = limitStoneID[i];
        }

        auto res = state.rainbowSearch();

        if(minResult !is null && res !is null && (minResult.numOfEmpty == res.numOfEmpty ? (minResult.history.length > res.history.length) : minResult.numOfEmpty > res.numOfEmpty)){
            minResult = res;
            writeln("Update: ", res.numOfEmpty);
        }

        if(minResult is null)
            minResult = res;
    }

    return minResult;
}


/+
GeneralField simpleRainbowSearch(Problem problem)
{
    GeneralField minResult = null;


    static
    struct TaskInput
    {
        byte x, y;
    }

    TaskInput[] inputs;

    //foreach(i, stone; problem.stones)
    //    foreach(ss; stone.uniqueState)
    //        inputs ~= TaskInput(stone[ss]);
    foreach(byte x; cast(byte)(problem.minRect.x - 8) .. cast(byte)(problem.minRect.x + problem.minRect.w))
        foreach(byte y; cast(byte)(problem.minRect.y - 8) .. cast(byte)(problem.minRect.y + problem.minRect.h))
            inputs ~= TaskInput(x, y);

    //foreach(task; inputs.parallel(1))
    foreach(task; inputs.parallel(1))
    {
        writefln("%s, %s", task.x, task.y);

        //auto st = task.stone[ss];
        //auto st = task.stone;

        RainbowSearchState state;
        state.problem = problem;
        state.stages.length = problem.numOfEmpty + 1;
        foreach(ref e; state.stages){
            e.thr = size_t.max;
            e.thrSlot = problem.numOfEmpty+1;
        }

        //foreach(byte x; -8 .. 32) foreach(byte y; -8 .. 32) if(!problem.initField.isCollided(x, y, st))
        foreach(stone; problem.stones) foreach(ss; stone.uniqueState)
        {
            auto st = stone[ss];
            if(!problem.initField.isCollided(task.x, task.y, st)){
                auto lf = LazyField(problem.stones.length, problem.initField, problem.numOfEmpty, task.x, task.y, st);
                state.insert(lf);
            }
        }

        foreach(ref e; state.stages){
            e.thr = 1024;
        }

        auto res = state.rainbowSearch();

        if(minResult !is null && res !is null && (minResult.numOfEmpty == res.numOfEmpty ? (minResult.history.length > res.history.length) : minResult.numOfEmpty > res.numOfEmpty)){
            minResult = res;
            writeln("Update: ", res.numOfEmpty);
        }

        if(minResult is null)
            minResult = res;
    }

    return minResult;
}
+/