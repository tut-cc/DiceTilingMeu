module procon26.problem;

import std.conv         : to;
import std.range        : popFront, front, empty;
import std.string       : chomp, splitLines;

version(unittest) import std.stdio;
debug import std.stdio;

import procon26.field;
import procon26.stone;
import procon26.util;


final class Problem
{
    this(string[] lines)
    {
        _initField = new TinyField;
        byte xm = byte.max, ym = byte.max, xM = -1, yM = -1;
        foreach(byte y, line; lines[0 .. 32]){
            foreach(byte x; 0 .. 32){
                if(line[x] == '1'){
                    _initField[x, y] = true;
                }else{
                    xm = min(xm, x); ym = min(ym, y);
                    xM = max(xM, x); yM = max(yM, y);
                    ++_numOfEmpty;
                }
            }
        }

        _minRect.x = xm;
        _minRect.y = ym;
        _minRect.w = cast(byte)(xM - xm + 1);
        _minRect.h = cast(byte)(yM - ym + 1);

        lines = lines[32 .. $];

        lines.popFront();       // only \r\n

        size_t nStones = lines.front.to!size_t();
        foreach(i; 0 .. nStones){
            lines.popFront();
            _stones ~= new Stone(i, lines[0 .. 8]);
            lines = lines[8 .. $];
        }

        _adjacents = new TinyField;
        StructLargeField!(byte, 32) adjs;
        foreach(byte xx, byte yy; _initField.byZk){
            foreach(d; [[1, 0], [-1, 0], [0, -1], [0, 1]]) {
                byte xxx = cast(byte)(xx + d[0]),
                     yyy = cast(byte)(yy + d[1]);

                if(isInField!32(xxx, yyy) && !_initField[xxx, yyy]){
                    _adjacents[xxx, yyy] = true;
                    auto v = adjs[xxx, yyy];
                    size_t inc = incValue(v);

                    adjs[xxx, yyy] += inc;
                }
            }
        }

        foreach(byte i; 0 .. 32){
            if(!_initField[0, i]) _adjacents[0, i] = true;
            if(!_initField[i, 0]) _adjacents[0, i] = true;
            if(!_initField[31, i]) _adjacents[31, i] = true;
            if(!_initField[i, 31]) _adjacents[i, 31] = true;
        }

        foreach(byte x;  0 .. 32) foreach(byte y; 0 .. 32) {
            _difficulty[x, y] += adjs[x, y];
            _sumOfDifficulty += adjs[x, y];
            foreach(d; [[1, 0], [-1, 0], [0, -1], [0, 1]]) {
                byte xx = cast(byte)(x + d[0]),
                     yy = cast(byte)(y + d[1]);

                if(isInField!32(xx, yy)){
                    _difficulty[x, y] += adjs[xx, yy];
                    _sumOfDifficulty += adjs[xx, yy];
                }
            }
        }
    }


    ref const(TinyField) initField() const pure nothrow @safe @nogc @property
    {
        return _initField;
    }


    inout(Stone)[] stones() inout pure nothrow @safe @nogc @property
    {
        return _stones;
    }


    Rectangle minRect() const pure nothrow @safe @nogc @property
    {
        return _minRect;
    }


    size_t numOfEmpty() const pure nothrow @safe @nogc @property
    {
        return _numOfEmpty;
    }


    ref const(StructLargeField!(size_t, 32)) difficulty() const pure nothrow @safe @nogc @property
    {
        return _difficulty;
    }


    size_t sumOfDifficulty() const pure nothrow @safe @nogc @property
    {
        return _sumOfDifficulty;
    }


    auto byAdjacentZk() const pure nothrow @safe @nogc @property
    {
        return _adjacents.byZk();
    }


  private:
    TinyField _initField;
    Stone[] _stones;
    TinyField _adjacents;
    Rectangle _minRect;
    size_t _numOfEmpty;
    StructLargeField!(size_t, 32) _difficulty;
    size_t _sumOfDifficulty;
}


unittest
{
    string input = 
`00000000000000001111111111111111
00000000000000001111111111111111
01000000000000001111111111111111
00000000000000001111111111111111
00000000000000001111111111111111
00000000000000001111111111111111
00000000000000001111111111111111
00000100000000001111111111111111
00000000000000001111111111111111
00000000000000001111111111111111
00000000000000001111111111111111
00000000010000001111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111
11111111111111111111111111111111

4
01000000
01000000
01000000
01000000
01000000
01000000
01110000
00000000

00000000
00010000
00010000
01111000
00000000
00000000
00000000
00000000

00000000
01100000
01100000
01100000
01100000
00000000
00000000
00000000

10000000
11000000
01100000
00110000
00011000
00001100
00000110
00000011`;

    auto inputLines = input.splitLines.map!chomp().array();
    auto problem = new Problem(inputLines);
    assert(!problem.initField[ 0,  0]);
    assert( problem.initField[ 1,  2]);
    assert( problem.initField[ 5,  7]);
    assert( problem.initField[ 9, 11]);
    assert(problem.minRect == Rectangle(0, 0, 16, 12));
    assert(problem.numOfEmpty == 16*12 - 3);

    assert(problem.stones.length == 4);
    foreach(i; 0 .. 4)
    {
        auto st = problem.stones[i];
        assert(st.id == i);
        assert(st.numOfZk == [9, 6, 8, 15][i]);
    }

    size_t cnt;
    foreach(x, y; problem.initField.byEmptyZk()){
        //writefln("%s : %s", x, y);
        ++cnt;
    }

    assert(cnt == problem.numOfEmpty);

    foreach(x, y; problem.initField.byZk())
        ++cnt;

    assert(cnt == 32 * 32);
}
