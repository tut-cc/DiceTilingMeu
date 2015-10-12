module procon26.algorithm.rainbow;

import std.algorithm        : map, sum, sort, uniq;
import std.container        : RedBlackTree;
import std.math;

import core.sync.mutex;

import procon26.field;
import procon26.problem;
import procon26.stone;
import procon26.util;
import std.parallelism;
import std.random;
import std.range;


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
            //writefln("        %s, %s, %s, %s", f.numOfEmpty, f.numOfRemainStones, f.numOfAdjacents, slots[].map!"a is null ? 0 : 1".sum());

            if(f.lastStoneID > limitStoneID){
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
                foreach(ref LazyField next; gf.nextFields(problem)){
                    this.insert(next);
                }
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
            foreach(id; _field.history[$-1].stone.id+1 .. min(_problem.stones.length, _field.history[$-1].stone.id+5)){
                auto problemStone = _problem.stones[id];
                foreach(ss; problemStone.uniqueState){
                    auto stone = problemStone[ss];
                    bool[40][40] bs;

                    foreach(byte x, byte y; _field.byAdjacentZk)
                        foreach(byte xx, byte yy; stone.byZk){
                            byte xxx = cast(byte)(x - xx);
                            byte yyy = cast(byte)(y - yy);
                            bs[yyy+8][xxx+8] = true;
                        }

                    foreach(byte x; -8 .. 32) foreach(byte y; -8 .. 32){
                        if(bs[y+8][x+8] && !_field.isCollided(x, y, stone)){
                            auto newField = LazyField(_field, x, y, stone);
                            res = dg(newField);
                            if(res) return res;
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
    if(numOfEmpty < 200){
        if(idxOfStage < 50) return 2;
        else return 3;
    }
    else if(numOfEmpty < 500){
        if(idxOfStage < 20) return 32;
        else if(idxOfStage < 50) return 20;
        else if(idxOfStage < 100) return 20;
        else if(idxOfStage < 200) return 20;
        else return 20;
    }
    else{
        if(idxOfStage < 50) return 32;
        else if(idxOfStage < 100) return 16;
        else if(idxOfStage < 200) return 10;
        else if(idxOfStage < 400) return 5;
        else return 3;
    }
}


size_t calcThrValue(size_t idxOfStage, size_t numOfEmpty)
{
    if(numOfEmpty < 200){
        if(idxOfStage < 50) return 5;
        else return 10;
    }
    else if(numOfEmpty < 500){
        if(idxOfStage < 20) return 256;
        else if(idxOfStage < 50) return 64;
        else if(idxOfStage < 100) return 32;
        else if(idxOfStage < 200) return 40;
        else if(idxOfStage < 400) return 40;
        else return 40;
    }else{
        if(idxOfStage < 50) return 256;
        else if(idxOfStage < 100) return 32;
        else if(idxOfStage < 200) return 10;
        else if(idxOfStage < 400) return 5;
        else return 3;
    }
}


void recalculateLimitStomeID(Problem problem, in size_t[] sumOfZk, size_t[] limitStoneID, size_t smallestResult)
{
    foreach_reverse(i, ref e; limitStoneID){
        size_t lastIdx = problem.stones.length-1;
        size_t target = max(i, smallestResult);

        while(lastIdx > 0 && sumOfZk[lastIdx] < target) --lastIdx;
        lastIdx += 1;
        e = lastIdx;
        lastIdx = problem.stones.length-1;
    }
}



GeneralField simpleRainbowSearch(alias resultConsumer, alias genField, alias fieldToTask, Task)(Problem problem, Task[] inputs)
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

    recalculateLimitStomeID(problem, sumOfZk, limitStoneID, 0);

    //alias RBTree = RedBlackTree!(GeneralField, "a.numOfEmpty == b.numOfEmpty ? a.history.length < b.history.length : a.numOfEmpty < b.numOfEmpty");
    //RBTree heap = new RBTree;
    //RBTree badHeap = new RBTree;
    //size_t heapMaxSize = inputs.length / 4;
    Mutex mtx = new Mutex;
    size_t incIdx = 1;

    Task[] taskList = inputs;
    while(incIdx < 20)
    {
        writefln("!!!!!!! Start Stage%s !!!!!!!", incIdx);

        foreach(task; taskList.parallel(1)){
            if(minResult !is null)
                writefln("%s, now best score is (%s, %s)", task, minResult.numOfEmpty, minResult.history.length);
            else
                writefln("%s", task);

            RainbowSearchState state;
            state.problem = problem;
            state.stages.length = problem.numOfEmpty + 1;

            foreach(i, ref e; state.stages){
                e.thr = size_t.max;
                e.thrSlot = calcSlotSize(i, problem.numOfEmpty) * incIdx^^2;
            }

            genField(problem, task, state);

            foreach(i, ref e; state.stages){
                e.thr = calcThrValue(i, problem.numOfEmpty) * incIdx^^2;
                e.limitStoneID = limitStoneID[i];
            }

            auto res = state.rainbowSearch();

            synchronized(mtx){
                //if(res !is null){
                //    heap.insert(res);
                //    if(heap.length > heapMaxSize){
                //        auto gf = heap.back();
                //        heap.removeBack();
                //        if(uniform(0, 3) == 0)
                //            badHeap.insert(gf);
                //    }
                //}

                if((minResult is null && res !is null) || (minResult !is null && res !is null && (minResult.numOfEmpty == res.numOfEmpty ? (minResult.history.length > res.history.length) : minResult.numOfEmpty > res.numOfEmpty))){
                    import std.datetime;
                    minResult = res;
                    writefln("Update: (%s, %s) on %s", res.numOfEmpty, res.history.length, task);
                    //std.file.write("ans_" ~ Clock.currTime.toISOString() ~ ".txt", minResult.answer);
                    resultConsumer(minResult);
                }
            }
        }

        ++incIdx;
        //writeln(heap.length);
        //writeln(badHeap.length);
        //Task[] tasks = heap[].map!(fieldToTask).chain(badHeap[].map!(fieldToTask)).array();
        //tasks.sort!"a < b"();
        //tasks = tasks.uniq.array();
    }

    return minResult;
}


GeneralField simpleRainbowSearchByStone(alias resultConsumer)(Problem problem)
{
    static
    struct InputTask
    {
        InstantiatedStone stone;

        bool opEquals(const ref InputTask rhs) const pure nothrow @safe @nogc
        {
            return this.stone.id == rhs.stone.id && this.stone.state == rhs.stone.state;
        }


        int opCmp(const ref InputTask rhs) const pure nothrow @safe @nogc
        {
            if(this.stone.id == rhs.stone.id) return .opCmp(this.stone.state, rhs.stone.state);
            else return .opCmp(this.stone.id, rhs.stone.id);
        }
    }

    InputTask[] inputs;
    foreach(i, stone; problem.stones)
        foreach(ss; stone.uniqueState)
            inputs ~= InputTask(stone[ss]);

    return simpleRainbowSearch!(
        resultConsumer,
        function(Problem p, InputTask t, ref RainbowSearchState s)
        {
            foreach(byte x; -8 .. 32) foreach(byte y; -8 .. 32) if(!p.initField.isCollided(x, y, t.stone))
            {
                auto lf = LazyField(p, p.stones.length, p.initField, p.numOfEmpty, x, y, t.stone);
                s.insert(lf);
            }
        },
        function(GeneralField field)
        {
            //return field.history[0].stone;
            auto st = field.history[0].stone;
            return InputTask(field.problem.stones[st.id][st.state]);
        }
    )(problem, inputs);
}


GeneralField simpleRainbowSearchByXY(alias resultConsumer)(Problem problem)
{
    static
    struct InputTask
    {
        byte x, y;

        bool opEquals(const ref InputTask rhs) const pure nothrow @safe @nogc
        {
            return this.x == rhs.x && this.y == rhs.y;
        }


        int opCmp(const ref InputTask rhs) const pure nothrow @safe @nogc
        {
            if(this.x == rhs.x) return .opCmp(this.y, rhs.y);
            else return .opCmp(this.x, rhs.x);
        }
    }

    InputTask[] inputs;

    auto pmr = problem.minRect;
    foreach(byte x; cast(byte)(pmr.x - 8) .. cast(byte)(pmr.x + pmr.w))
        foreach(byte y; cast(byte)(pmr.y - 8) .. cast(byte)(pmr.y + pmr.h))
            inputs ~= InputTask(x, y);

    inputs.randomShuffle();

    return simpleRainbowSearch!(
        resultConsumer,
        function(Problem p, InputTask t, ref RainbowSearchState s)
        {
            foreach(i, ref e; p.stones){
                foreach(ss; e.uniqueState){
                    auto st = e[ss];
                    if(!p.initField.isCollided(t.x, t.y, st)){
                        auto lf = LazyField(p, p.stones.length, p.initField, p.numOfEmpty, t.x, t.y, st);
                        s.insert(lf);
                    }
                }
            }
        },
        function(GeneralField field)
        {
            auto firstCommit = field.history[0];
            return InputTask(firstCommit.x, firstCommit.y);
        }
    )(problem, inputs);
}