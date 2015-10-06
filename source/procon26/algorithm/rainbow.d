module procon26.algorithm.rainbow;

import std.algorithm        : map, sum;
import std.container        : RedBlackTree;

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


        bool insert(LazyField f)
        {
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
                foreach(e; slot[]){
                    res = dg(e);
                    if(res) return res;
                }
            }

            return res;
        }
    }


    bool insert(LazyField field)
    {
        return stages[field.numOfEmpty].insert(field);
    }


    Problem problem;
    Stage[] stages;


    LazyField rainbowSearch()
    {
        foreach_reverse(i, ref stage; stages)
        {
            //writefln("     %s: %s, %s", i, stage.slots[].map!"a is null ? 0 : 1".sum(), stage.slots[].map!"a is null ? 0 : a.length".sum());

            // for GC, release unused state
            if(stage.slots[].map!"a is null ? 0 : a.length".sum()){
                foreach_reverse(j; i+1 .. stages.length)
                    stages[j] = Stage.init;
            }

            foreach(LazyField field; stage){
                field.commit();
                foreach(LazyField next; field.nextFields(problem))
                    this.insert(next);
            }
        }

        foreach(ref stage; stages)
            if(stage.slots[].map!"a is null ? 0 : a.length".sum()){
                auto tree = new RedBlackTree!(LazyField, "a.history.length < b.history.length");
                foreach(LazyField ee; stage)
                    tree.insert(ee);

                return tree.front;
            }

        return null;
    }
}


auto nextFields(LazyField field, Problem problem)
{
    static struct Result()
    {
        int opApply(int delegate(ref LazyField) dg) const
        {
            int res = 0;
            foreach(byte x, byte y; _field.byAdjacentZk){
                foreach(id; _field.history[$-1].stone.id+1 .. _problem.stones.length){
                    auto problemStone = _problem.stones[id];
                    foreach(ss; problemStone.uniqueState){
                        auto stone = problemStone[ss];
                        foreach(byte xx, byte yy; stone.byZk){
                            byte xxx = cast(byte)(x - xx);
                            byte yyy = cast(byte)(y - yy);
                            if(!_field.parent.isCollided(xxx, yyy, stone)){
                                LazyField newField = new LazyField(_field, xxx, yyy, stone);
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
        LazyField _field;
        Problem _problem;
    }


    return Result!()(field, problem);
}


LazyField simpleRainbowSearch(Problem problem)
{
    LazyField minResult = null;


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
        foreach(ref e; state.stages){
            e.thr = size_t.max;
            e.thrSlot = 1;
        }

        size_t cnt;
        foreach(byte x; -8 .. 40) foreach(byte y; -8 .. 40) if(!problem.initField.isCollided(x, y, st))
        {
            ++cnt;
            state.insert(new LazyField(problem.stones.length, problem.initField, problem.numOfEmpty, x, y, st));
        }

        foreach(ref e; state.stages){
            e.thr = 1;
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
