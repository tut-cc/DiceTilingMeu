module procon26.util;

import carbon.functional;

version(unittest) import std.stdio;
debug import std.stdio;

import procon26.stone;

struct Rectangle
{
    byte x, y;
    byte w, h;
}


struct Point
{
    byte x, y;


    int opCmp(ref const Point rhs) const
    {
        if(x == rhs.x)
            return y - rhs.y;
        else
            return x - rhs.x;
    }
}


auto byZk(alias pred = "field[x, y]", T)(const ref T field)
//if(is(typeof((byte x, byte x){ mixin(pred); }))
//&& is(typeof((){ size_t x = field.width + field.height; })))
{
    return .byZk!pred(field, Rectangle(0, 0, cast(byte)field.width, cast(byte)field.height));
}


auto byZk(alias pred = "field[x, y]", T)(const ref T field, Rectangle rect)
//if(is(typeof((byte x, byte x){ mixin(pred); })))
{
    static struct Result()
    {
        int opApply(int delegate(ref byte, ref byte) dg)
        {
            alias field = _field;

            int res = 0;
            foreach(byte y; _rect.y .. cast(byte)(_rect.y + _rect.h))
                foreach(byte x; _rect.x .. cast(byte)(_rect.x + _rect.w)){
                    if(mixin(pred)){
                        res = dg(x, y);
                        if(res) return res;
                    }
                }

            return res;
        }

      private:
        const(T) _field;
        Rectangle _rect;
    }


    return Result!()(field, rect);
}


auto byEmptyZk(T)(const ref T field)
//if(is(typeof((byte x, byte x){ mixin(pred); }))
//&& is(typeof((){ size_t x = field.width + field.height; })))
{
  static if(is(typeof(field.minRect)))
    return .byZk!"!(field[x, y])"(field, field.minRect);
  else
    return .byZk!"!(field[x, y])"(field);
}


auto byEmptyZk(T)(const ref T field, Rectangle rect)
//if(is(typeof((byte x, byte x){ mixin(pred); })))
{
    return .byZk!"!(field[x, y])"(field, rect);
}


auto isInField(size_t N = 32, T)(T x, T y)
{
    return x >= 0 && x < N && y >= 0 && y < N;
}
