module procon26.stone;

import std.algorithm    : map, reduce, max, min, canFind;
import std.array        : array;
import std.conv         : to;
import std.range        : popFront, front, empty;
import std.string       : chomp, splitLines;
import std.traits       : EnumMembers;
import std.container    : RedBlackTree;
import std.typecons     : Rebindable;
import std.format       : formattedWrite;

import procon26.util;

version(unittest) import std.stdio;
debug import std.stdio;


enum StoneState : ubyte
{
    H0 = 0,
    H90,
    H180,
    H270,
    T0,
    T90,
    T180,
    T270,
    unused,
}


string toString(StoneState ss)
{
  final switch(ss)
  {
    case StoneState.H0:     return "H 0";
    case StoneState.H90:    return "H 90";
    case StoneState.H180:   return "H 180";
    case StoneState.H270:   return "H 270";
    case StoneState.T0:     return "T 0";
    case StoneState.T90:    return "T 90";
    case StoneState.T180:   return "T 180";
    case StoneState.T270:   return "T 270";
    case StoneState.unused: return "Unused";
  }
}


final class Stone
{
    enum byte width = 8;
    enum byte height = 8;

    this(size_t id, string[] lines)
    {
        _id = id;
        byte[4][8] rec;

        foreach(byte y; 0 .. 8)
            foreach(byte x; 0 .. 8){
                bool b = lines[y][x] == '1';
                if(b) foreach(ss; EnumMembers!StoneState[0 .. 8]){
                    byte xx, yy;
                    final switch(ss){
                        case StoneState.H0:     yy = cast(byte)(  y); xx = cast(byte)(  x); break;
                        case StoneState.H90:    yy = cast(byte)(  x); xx = cast(byte)(7-y); break;
                        case StoneState.H180:   yy = cast(byte)(7-y); xx = cast(byte)(7-x); break;
                        case StoneState.H270:   yy = cast(byte)(7-x); xx = cast(byte)(  y); break;
                        case StoneState.T0:     yy = cast(byte)(  y); xx = cast(byte)(7-x); break;
                        case StoneState.T90:    yy = cast(byte)(7-x); xx = cast(byte)(7-y); break;
                        case StoneState.T180:   yy = cast(byte)(7-y); xx = cast(byte)(  x); break;
                        case StoneState.T270:   yy = cast(byte)(  x); xx = cast(byte)(  y); break;
                        case StoneState.unused: assert(0);
                    }

                    assert(b);
                    _stones[ss][yy][xx] = true;
                    _bits[ss] |= 1uL << (yy*8 + xx);
                    _xlist[ss] ~= xx;
                    _ylist[ss] ~= yy;
                }
            }

        _zkCNT = _xlist[0].length;

        auto xmms = _xlist[].map!(reduce!(min, max));
        auto ymms = _ylist[].map!(reduce!(min, max));
        foreach(i; 0 .. 8){
            auto xmm = xmms[i],
                 ymm = ymms[i];

            _minRect[i].x = xmm[0];
            _minRect[i].y = ymm[0];
            _minRect[i].w = cast(byte)(xmm[1] - xmm[0] + 1);
            _minRect[i].h = cast(byte)(ymm[1] - ymm[0] + 1);
        }


        foreach(ss; EnumMembers!StoneState[0 .. 8]){
            auto uniq = new RedBlackTree!(Point, `a < b`);
            foreach(i; 0 .. _zkCNT){
                byte x = _xlist[ss][i],
                     y = _ylist[ss][i];
                foreach(byte[2] dxy; cast(byte[2][])[[1, 0], [-1, 0], [0, 1], [0, -1]]){
                    byte xx = cast(byte)(x + dxy[0]),
                         yy = cast(byte)(y + dxy[1]);

                    if(xx < 0 || xx >= 8 || yy < 0 || yy >= 8 || !_stones[ss][yy][xx])
                        uniq.insert(Point(xx, yy));
                }
            }

            foreach(e; uniq[]){
                _adjXList[ss] ~= e.x;
                _adjYList[ss] ~= e.y;
            }
        }

        _uniqState ~= StoneState.H0;

      Lbreak:
        foreach(ss; 1 .. 8)
        {
            bool allUniqueCheck = true;
            foreach(other; 0 .. ss){

                bool isDiff = false;
                foreach(i, x; _xlist[ss]){
                    byte y = _ylist[ss][i],
                         xx = cast(byte)(x - _minRect[ss].x + _minRect[other].x),
                         yy = cast(byte)(y - _minRect[ss].y + _minRect[other].y);

                    if(!isInField!8(xx, yy) || _stones[ss][y][x] != _stones[other][yy][xx]){
                        isDiff = true;
                        break;
                    }
                }

                allUniqueCheck = allUniqueCheck && isDiff;
            }

            if(allUniqueCheck)
                _uniqState ~= cast(StoneState)ss;
        }
    }


    bool opIndex(StoneState state, byte x, byte y) const pure nothrow @safe @nogc
    {
        return _stones[state][y][x];
    }


    InstantiatedStone opIndex(StoneState state) pure nothrow @safe @nogc
    {
        return InstantiatedStone(this, state);
    }


    size_t id() const pure nothrow @safe @nogc @property
    {
        return _id;
    }


    size_t numOfZk() const pure nothrow @safe @nogc @property
    {
        return _zkCNT;
    }


    auto byZk(StoneState state) const pure nothrow @safe @nogc @property
    {
        return byXYList(_xlist[state], _ylist[state]);
    }


    auto byAdjacentZk(StoneState state) const pure nothrow @safe @nogc @property
    {
        return byXYList(_adjXList[state], _adjYList[state]);
    }


    ulong bitField(StoneState state) const pure nothrow @safe @nogc @property
    {
        return _bits[state];
    }


    const(byte)[] xlist(StoneState state) const pure nothrow @safe @nogc
    {
        return _xlist[state];
    }


    const(byte)[] ylist(StoneState state) const pure nothrow @safe @nogc
    {
        return _ylist[state];
    }


    Rectangle minRect(StoneState state) const pure nothrow @safe @nogc
    {
        return _minRect[state];
    }


    const(StoneState)[] uniqueState() const pure nothrow @safe @nogc
    {
        return _uniqState;
    }


    void toString(scope void delegate(const(char)[]) sink) const
    {
        sink.formattedWrite("%s", _id);
    }


  private:
    size_t _id;
    size_t _zkCNT;
    bool[8][8][8] _stones;
    ulong[8] _bits;
    Rectangle[8] _minRect;
    byte[][8] _xlist;
    byte[][8] _ylist;
    byte[][8] _adjXList;
    byte[][8] _adjYList;
    StoneState[] _uniqState;


    static
    auto byXYList(const byte[] xlist, const byte[] ylist) pure nothrow @safe @nogc @property
    {
        static struct Result()
        {
            bool empty() const pure nothrow @safe @nogc { return _xs.length == 0; }


            Point front() const pure nothrow @safe @nogc @property
            {
                return typeof(return)(_xs[0], _ys[0]);
            }


            void popFront() pure nothrow @safe @nogc
            {
                _xs = _xs[1 .. $];
                _ys = _ys[1 .. $];
            }


            int opApply(T)(int delegate(ref T, ref T) dg) const
            {
                int res = 0;

                foreach(i; 0 .. _xs.length){
                    T x = _xs[i], y = _ys[i];
                    res = dg(x, y);
                    if(res) return res;
                }

                return res;
            }


            int opApply(int delegate(ref Point) dg) const
            {
                int res = 0;

                foreach(i; 0 .. _xs.length){
                    Point p = Point(_xs[i], _ys[i]);
                    res = dg(p);
                    if(res) return res;
                }

                return res;
            }


            size_t length() const pure nothrow @safe @nogc @property
            {
                return _xs.length;
            }


            alias opDollar = length;


          private:
            const(byte)[] _xs;
            const(byte)[] _ys;
        }

        return Result!()(xlist, ylist);
    }
}

unittest
{
    string[] lines = [`01000000`,
                      `01000000`,
                      `01000000`,
                      `01000000`,
                      `01000000`,
                      `01000000`,
                      `01110000`,
                      `00000000`,];

    Stone stone1 = new Stone(0, lines);
    assert(stone1.numOfZk == 9);
    assert(stone1.xlist(StoneState.H0) == cast(byte[])[1, 1, 1, 1, 1, 1, 1, 2, 3]);
    assert(stone1.ylist(StoneState.H0) == cast(byte[])[0, 1, 2, 3, 4, 5, 6, 6, 6]);
    assert(stone1.minRect(StoneState.H0) == Rectangle(1, 0, 3, 7));
    assert(stone1.minRect(StoneState.H90) == Rectangle(1, 1, 7, 3));
    assert(stone1.uniqueState.length == 8);

    foreach(p; [Point(0, 0), Point(0, 1), Point(0, 2), Point(0, 3), Point(0, 4), Point(0, 5), Point(0, 6),
                Point(1, 7), Point(2, 7), Point(3, 7),
                Point(4, 6),
                Point(3, 5), Point(2, 5), Point(2, 4), Point(2, 3), Point(2, 2), Point(2, 1), Point(2, 0),
                Point(1, -1)]){
        assert(stone1.byAdjacentZk(StoneState.H0).canFind(p));
    }

    foreach(p; [Point(0, -1), Point(0, 7), Point(4, 7), Point(4, 5), Point(2, -1), Point(7, 7)]){
        assert(!stone1.byAdjacentZk(StoneState.H0).canFind(p));
    }

    foreach(ss; EnumMembers!StoneState[0 .. 8]){
        foreach(byte x, byte y; stone1.byZk(ss))
            assert(!stone1.byAdjacentZk(ss).canFind(Point(x, y)));
    }

    foreach(ss; EnumMembers!StoneState[0 .. 8]){
        size_t cnt;
        foreach(ubyte x; 0 .. 8) foreach(ubyte y; 0 .. 8) cnt += stone1[ss, x, y] ? 1 : 0;
        assert(cnt == 9);

        cnt = 0;
        foreach(ubyte x, ubyte y; stone1.byZk(ss))
            ++cnt;
        assert(cnt == 9);
    }

    assert(stone1[StoneState.H0, 1, 0]);
    assert(stone1[StoneState.H0, 1, 1]);
    assert(stone1[StoneState.H0, 1, 2]);
    assert(stone1[StoneState.H0, 1, 3]);
    assert(stone1[StoneState.H0, 1, 4]);
    assert(stone1[StoneState.H0, 1, 5]);
    assert(stone1[StoneState.H0, 1, 6]);
    assert(stone1[StoneState.H0, 2, 6]);
    assert(stone1[StoneState.H0, 3, 6]);

    assert(stone1[StoneState.H90, 7, 1]);
    assert(stone1[StoneState.H90, 6, 1]);
    assert(stone1[StoneState.H90, 5, 1]);
    assert(stone1[StoneState.H90, 4, 1]);
    assert(stone1[StoneState.H90, 3, 1]);
    assert(stone1[StoneState.H90, 2, 1]);
    assert(stone1[StoneState.H90, 1, 1]);
    assert(stone1[StoneState.H90, 1, 2]);
    assert(stone1[StoneState.H90, 1, 3]);

    assert(stone1[StoneState.H180, 6, 7]);
    assert(stone1[StoneState.H180, 6, 6]);
    assert(stone1[StoneState.H180, 6, 5]);
    assert(stone1[StoneState.H180, 6, 4]);
    assert(stone1[StoneState.H180, 6, 3]);
    assert(stone1[StoneState.H180, 6, 2]);
    assert(stone1[StoneState.H180, 6, 1]);
    assert(stone1[StoneState.H180, 5, 1]);
    assert(stone1[StoneState.H180, 4, 1]);

    assert(stone1[StoneState.H270, 0, 6]);
    assert(stone1[StoneState.H270, 1, 6]);
    assert(stone1[StoneState.H270, 2, 6]);
    assert(stone1[StoneState.H270, 3, 6]);
    assert(stone1[StoneState.H270, 4, 6]);
    assert(stone1[StoneState.H270, 5, 6]);
    assert(stone1[StoneState.H270, 6, 6]);
    assert(stone1[StoneState.H270, 6, 5]);
    assert(stone1[StoneState.H270, 6, 4]);

    /* T0 */
    assert(stone1[StoneState.T0, 6, 0]);
    assert(stone1[StoneState.T0, 6, 1]);
    assert(stone1[StoneState.T0, 6, 2]);
    assert(stone1[StoneState.T0, 6, 3]);
    assert(stone1[StoneState.T0, 6, 4]);
    assert(stone1[StoneState.T0, 6, 5]);
    assert(stone1[StoneState.T0, 6, 6]);
    assert(stone1[StoneState.T0, 5, 6]);
    assert(stone1[StoneState.T0, 4, 6]);
}

unittest
{
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
}

unittest
{
    string[] lines = [`11000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,
                      `00000000`,];

    Stone stone1 = new Stone(1, lines);
    assert(stone1.uniqueState == [StoneState.H0, StoneState.H90]);
}


struct InstantiatedStone
{
    bool opIndex(byte x, byte y) const pure nothrow @safe @nogc
    {
        return _stone[_state, x, y];
    }


    size_t id() const pure nothrow @safe @nogc @property
    {
        return _stone.id;
    }

    size_t numOfZk() const pure nothrow @safe @nogc @property
    {
        return _stone.numOfZk;
    }


    ulong bitField() const pure nothrow @safe @nogc @property
    {
        return _stone.bitField(_state);
    }


    auto byZk() const pure nothrow @safe @nogc @property
    {
        return _stone.byZk(_state);
    }


    auto byAdjacentZk() const pure nothrow @safe @nogc @property
    {
        return _stone.byAdjacentZk(_state);
    }


    const(byte)[] xlist() const pure nothrow @safe @nogc
    {
        return _stone.xlist(_state);
    }


    const(byte)[] ylist() const pure nothrow @safe @nogc
    {
        return _stone.ylist(_state);
    }


    Rectangle minRect() const pure nothrow @safe @nogc
    {
        return _stone.minRect(_state);
    }


    inout(Stone) origin() inout pure nothrow @safe @nogc @property
    {
        return _stone;
    }


    StoneState state() const pure nothrow @safe @nogc @property
    {
        return _state;
    }


  private:
    Stone _stone;
    StoneState _state;
}