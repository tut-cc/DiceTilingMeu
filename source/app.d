module app;

import std.stdio;
import std.string;
import std.range;
import std.array;
import std.file;

import procon26.field,
       procon26.algorithm.rainbow,
       procon26.problem,
       procon26.stone,
       procon26.util;

void main()
{
    auto input = readText("15.txt");
    auto inputLines = input.splitLines.map!chomp().array();
    auto problem = new Problem(inputLines);
    auto res = simpleRainbowSearch(problem);

    writeln(res.numOfEmpty);
    //writeln(res.answer);
    std.file.write("ans15.txt", res.answer);
    //foreach(e; res.history)
    //    writeln(e.stone.id);
}
