module procon26.field;

import std.algorithm    : canFind;
import std.typecons     : Rebindable;
import std.array        : array, appender;
import std.format       : formattedWrite;

import std.stdio;
debug import std.stdio;

import procon26.problem;
import procon26.stone;
import procon26.util;


//byte upConvert(byte x)
//{
//    return cast(byte)[8, 27, 64][x/8];
//}


mixin template TinyFieldMember()
{
    enum byte width = 32;
    enum byte height = 32;

  //private
  //{
    uint[32] _bitField;
  //}


    bool opIndex(byte x, byte y) const pure nothrow @safe @nogc
    {
        return !!(_bitField[y] & (1 << x));
    }


    void opIndexAssign(bool b, byte x, byte y) pure nothrow @safe @nogc
    {
        if(b)
            _bitField[y] |= 1 << x;
        else
            _bitField[y] &= ~(1 << x);
    }

/+
    /*
    can optimize by bit-wise operators
    */
    bool isCollided(byte x, byte y, InstantiatedStone stone) const
    {
        auto mr = stone.minRect;
        if(!(   mr.x + x >= 0 && mr.x + mr.w + x < 32
             && mr.y + y >= 0 && mr.y + mr.h + y < 32))
            return true;

        ulong bits = stone.bitField;
        foreach(byte yy; mr.y .. cast(byte)(mr.y + mr.h)){
            uint bb = (bits >> (yy*8)) & 0b11111111;
            if(x < 0) bb >>= -x;
            else      bb <<= x;

            yy += y;
            if(_bitField[yy] & bb) return true;
        }
        return false;
    }
+/
    bool isCollided(byte x, byte y, InstantiatedStone stone) const
    {
        auto mr = stone.minRect;
        if(!(   mr.x + x >= 0 && mr.x + mr.w + x <= 32
             && mr.y + y >= 0 && mr.y + mr.h + y <= 32))
            return true;

        foreach(byte xx, byte yy; stone.byZk)
        {
            xx += x;
            yy += y;
            if(this[xx, yy]) return true;
        }
        return false;
    }

    /*
    can optimize by bit-wise operators
    */
    void putStone(byte x, byte y, InstantiatedStone stone)
    in{
        assert(!isCollided(x, y, stone));
    }
    body{
        auto mr = stone.minRect;
        ulong bits = stone.bitField;
        foreach(byte yy; mr.y .. cast(byte)(mr.y + mr.h)){
            uint bb = (bits >> (yy*8)) & 0b11111111;
            if(x < 0) bb >>= -x;
            else      bb <<= x;

            yy += y;
            _bitField[yy] |= bb;
        }
    }


    /*
    can optimize by bit-wise operators
    */
    void putStoneInv(byte x, byte y, InstantiatedStone stone)
    {
        auto mr = stone.minRect;
        ulong bits = stone.bitField;
        foreach(byte yy; mr.y .. cast(byte)(mr.y + mr.h)){
            uint bb = (bits >> (yy*8)) & 0b11111111;
            if(x < 0) bb >>= -x;
            else      bb <<= x;

            yy += y;
            _bitField[yy] &= ~bb;
        }
    }
}


final class TinyField
{
    mixin TinyFieldMember!();

    TinyField dup() const pure nothrow @safe @property
    {
        TinyField field = new TinyField;
        field._bitField = this._bitField;

        return field;
    }
}


struct StructTinyField
{
    mixin TinyFieldMember!();
}


unittest
{
    string[] lines = [`01000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,];

    Stone stone1 = new Stone(0, lines);
    auto field = new TinyField();

    field[0, 0] = true;
    assert(field[0, 0]);
    assert(!field[0, 1]);

    assert(!field.isCollided( 0,  0, stone1[StoneState.H0]));
    assert( field.isCollided(-1,  0, stone1[StoneState.H0]));
    assert(!field.isCollided(29,  0, stone1[StoneState.H0]));
    assert( field.isCollided(30,  0, stone1[StoneState.H0]));
    assert( field.isCollided(31,  0, stone1[StoneState.H0]));
    assert( field.isCollided(32,  0, stone1[StoneState.H0]));
    assert(!field.isCollided( 0, 30, stone1[StoneState.H0]));
    assert( field.isCollided( 0, 31, stone1[StoneState.H0]));
    assert( field.isCollided( 0, 32, stone1[StoneState.H0]));

    assert(!field[2, 1]);
    field.putStone(1, 1, stone1[StoneState.H0]);
    assert(field[2, 1]);
}


struct CommitContent
{
    InstantiatedStone stone;
    byte x, y;


    bool isNull() const pure nothrow @safe @nogc @property
    {
        return stone.origin is null;
    }
}


final class GeneralField
{
    enum byte width = 32;
    enum byte height = 32;


    this(const ref StructTinyField field,
         const ref byte[32][32] adjMem,
         const ref bool[32][32] adjField,
         const(CommitContent)[] history,
         size_t numOfEmpty, size_t numOfAdjacents, size_t numOfRemainStones) pure nothrow @safe @nogc
    {
        _field = field;
        _adjMem = adjMem;
        _adjacent = adjField;
        _history = history;
        _numOfEmpty = numOfEmpty;
        _numOfAdjacents = numOfAdjacents;
        _numOfRemainStones = numOfRemainStones;
    }


    bool opIndex(byte x, byte y) const pure nothrow @safe @nogc @property
    {
        return _field[x, y];
    }


    auto byAdjacentZk() const pure nothrow @safe @nogc @property
    {
        return _adjacent.byZk!"field[y][x]"(Rectangle(0, 0, 32, 32));
    }


    const(CommitContent)[] history() const pure nothrow @safe @nogc @property
    {
        return _history;
    }


    size_t numOfEmpty() const pure nothrow @safe @nogc @property
    {
        return _numOfEmpty;
    }


    size_t numOfAdjacents() const pure nothrow @safe @nogc @property
    {
        return _numOfAdjacents;
    }


    size_t numOfRemainStones() const pure nothrow @safe @nogc @property
    {
        return _numOfRemainStones;
    }


    string answer() const @property
    {
        auto app = appender!string();
        ptrdiff_t lastId = -1;
        foreach(ref e; _history){
            foreach(i; lastId+1 .. e.stone.id)
                app.put("\r\n");

            app.formattedWrite("%s %s %s\r\n", e.x, e.y, e.stone.state.toString);
            lastId = e.stone.id;
        }

        foreach(i; 0 .. _numOfRemainStones)
            app.put("\r\n");

        return app.data;
    }


    ref const(StructTinyField) field() const pure nothrow @safe @nogc @property
    {
        return _field;
    }


    bool isCollided(byte x, byte y, InstantiatedStone stone) const
    {
        return _field.isCollided(x, y, stone);
    }


  private:
    StructTinyField _field;
    byte[32][32] _adjMem;
    bool[32][32] _adjacent;
    const(CommitContent)[] _history;
    size_t _numOfEmpty;
    size_t _numOfAdjacents;
    size_t _numOfRemainStones;
}


struct LazyField
{
    this(size_t numOfRemainStones, const TinyField initField, size_t numOfEmpty, byte x, byte y, InstantiatedStone stone)
    {
        StructTinyField field;
        field._bitField = initField._bitField;

        byte[32][32] adjMem;
        size_t numOfAdj = 0;

        foreach(byte xx, byte yy; initField.byZk){
            foreach(dx; [1, -1, 0, 0]) foreach(dy; [0, 0, -1, -1]){
                byte xxx = cast(byte)(xx + dx),
                     yyy = cast(byte)(yy + dy);

                if(isInField!32(xxx, yyy) && !initField[xxx, yyy]){
                    size_t inc = adjMem[yyy][xxx] ? adjMem[yyy][xxx]*3 : 1;
                    //size_t inc = 1;
                    adjMem[yyy][xxx] += inc;
                    numOfAdj += inc;
                }
            }
        }

        foreach(byte i; 0 .. 32)
        {
            if(!initField[0, i]){
                size_t inc = adjMem[i][0] ? adjMem[i][0]*3 : 1;
                adjMem[i][0] += inc;
                numOfAdj += inc;
            }
            if(!initField[31, i]){
                size_t inc = adjMem[i][31] ? adjMem[i][31]*3 : 1;
                adjMem[i][31] += inc;
                numOfAdj += inc;
            }
            if(!initField[i, 0]){
                size_t inc = adjMem[0][i] ? adjMem[0][i]*3 : 1;
                adjMem[0][i] += inc;
                numOfAdj += inc;
            }
            if(!initField[i, 31]){
                size_t inc = adjMem[31][i] ? adjMem[31][i]*3 : 1;
                adjMem[31][i] += inc;
                numOfAdj += inc;
            }
        }

        bool[32][32] adj;

        GeneralField iniGF = new GeneralField(field, adjMem, adj, null, numOfEmpty, numOfAdj, numOfRemainStones);

        this(iniGF, x, y, stone);
    }


    this(GeneralField parent, byte x, byte y, InstantiatedStone stone)
    {
        _parent = parent;
        _commit = CommitContent(stone, x, y);
        _numOfEmpty = parent._numOfEmpty;
        _numOfAdjacents = parent._numOfAdjacents;
        _numOfRemainStones = parent._numOfRemainStones;

        if(parent._history.length){
            _numOfRemainStones -= (_commit.stone.id - parent._history[$-1].stone.id);
        }else{
            _numOfRemainStones -= _commit.stone.id + 1;
        }

        _numOfEmpty -= stone.numOfZk;

        // 石を置くことで消える隣接マスの計数
        foreach(byte xx, byte yy; stone.byZk){
            xx += x;
            yy += y;

            _numOfAdjacents -= parent._adjMem[yy][xx];
        }

        // 石を置くことで新たに追加される隣接マスの計数
        foreach(byte xx, byte yy; stone.byAdjacentZk){
            xx += x;
            yy += y;
            if(isInField!32(xx, yy) && !parent._field[xx, yy]) _numOfAdjacents += parent._adjMem[yy][xx] ? parent._adjacent[yy][xx]*3 : 1;
            //if(isInField!32(xx, yy) && !parent._field[xx, yy]) _numOfAdjacents += parent._adjacent[yy][xx] ? (parent._adjacent[yy][xx].upConvert - parent._adjacent[yy][xx]) : 1;
        }
    }


    GeneralField commit() const
    {
        StructTinyField tf;
        byte[32][32] adjMem;
        bool[32][32] adj;

        tf._bitField = _parent._field._bitField;
        //adj._bitField = _parent._adjacent._bitField;
        adjMem = _parent._adjMem;
        adj = _parent._adjacent;

        foreach(byte x, byte y; _commit.stone.byZk){
            x += _commit.x;
            y += _commit.y;
            tf[x, y] = true;
            adj[y][x] = false;
            adjMem[y][x] = 0;
        }

        foreach(byte x, byte y; _commit.stone.byAdjacentZk){
            x += _commit.x;
            y += _commit.y;
            if(x < 0 || x >= 32 || y < 0 || y >= 32) continue;

            if(!tf[x, y]){
                adjMem[y][x] += adjMem[y][x] ? adjMem[y][x]*3 : 1;
                //adjMem[y][x] += 1;
                adj[y][x] = true;
            }
        }

        auto hists = _parent.history;
        hists ~= _commit;

        return new GeneralField(tf, adjMem, adj, hists, _numOfEmpty, _numOfAdjacents, _numOfRemainStones);
    }


    size_t numOfEmpty() const pure nothrow @safe @nogc { return _numOfEmpty; }
    size_t numOfAdjacents() const pure nothrow @safe @nogc { return _numOfAdjacents; }
    size_t numOfRemainStones() const pure nothrow @safe @nogc { return _numOfRemainStones; }

    size_t lastStoneID() const pure nothrow @safe @nogc { return _commit.stone.id; }


  private:
    GeneralField _parent;
    CommitContent _commit;
    size_t _numOfIsolated;
    size_t _numOfEmpty;
    size_t _numOfAdjacents;
    size_t _numOfRemainStones;
}


unittest
{
    TinyField tf = new TinyField;
    tf[0, 0] = true;

    string[] lines = [`10000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,];

    Stone stone1 = new Stone(1, lines);
    assert(stone1.uniqueState == [StoneState.H0]);

    auto instSt1 = stone1[StoneState.H0];

    auto lf = LazyField(5, tf, 32*32-1, 1, 0, instSt1);
    //assert(lf.hasCommit);
    //assert(lf[0, 0]);
    //assert(lf[1, 0]);
    //assert(lf.history.length == 1);
    assert(lf.numOfEmpty == 32*32 - 2);
    assert(lf.numOfAdjacents == 2);
    assert(lf.numOfRemainStones == 3);

    auto gf = lf.commit();
    //assert(!lf.hasCommit);
    assert(gf[0, 0]);
    assert(gf[1, 0]);
    assert(gf.history.length == 1);
    assert(gf.numOfEmpty == 32*32 - 2);
    assert(gf.numOfAdjacents == 2);
    assert(gf.numOfRemainStones == 3);

    foreach(byte x, byte y; gf.byAdjacentZk)
        assert([Point(1, 1), Point(2, 0)].canFind(Point(x, y)));

    {
        size_t cnt;
        foreach(byte x, byte y; gf.byAdjacentZk())
            ++cnt;
        assert(cnt == 2);
    }

    auto stone3 = new Stone(3, lines);
    auto instSt3 = stone3[StoneState.H0];
    lf = LazyField(gf, 2, 0, instSt3);
    //assert(lf.hasCommit);
    //assert(lf[0, 0]);
    //assert(lf[1, 0]);
    //assert(lf[2, 0]);
    //assert(lf.history.length == 2);
    assert(lf.numOfEmpty == 32*32 - 3);
    assert(lf.numOfAdjacents == 3);
    assert(lf.numOfRemainStones == 1);

    gf = lf.commit();
    //assert(!lf.hasCommit);
    assert(gf[0, 0]);
    assert(gf[1, 0]);
    assert(gf[2, 0]);
    assert(gf.history.length == 2);
    assert(gf.numOfEmpty == 32*32 - 3);
    assert(gf.numOfAdjacents == 3);
    assert(gf.numOfRemainStones == 1);

    foreach(byte x, byte y; gf.byAdjacentZk)
        assert([Point(1, 1), Point(2, 1), Point(3, 0)].canFind(Point(x, y)));

    {
        size_t cnt;
        foreach(byte x, byte y; gf.byAdjacentZk())
            ++cnt;
        assert(cnt == 3);
    }

    assert(array(map!chomp(splitLines(gf.answer))) == [``, `1 0 H 0`, ``, `2 0 H 0`, ``]);
}


/+
/*
can optimize by bit-wise operators
*/
bool isCollided(F)(F field, byte x, byte y, InstantiatedStone stone)
{
    auto mr = stone.minRect;
    if(!(   mr.x + x >= 0 && mr.x + mr.w + x < 32
         && mr.y + y >= 0 && mr.y + mr.h + y < 32))
        return true;

    foreach(byte zx, byte zy; stone.byZk){
        zx += x;
        zy += y;
        if(field[zx, zy]) return true;
    }
    return false;
}
+/