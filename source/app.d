module app;

import std.stdio;
import std.string;
import std.range;
import std.array;
import std.file;
import std.datetime;

import procon26.field,
       procon26.algorithm.rainbow,
       procon26.problem,
       procon26.stone,
       procon26.util,
       procon26.http;

void main()
{
    auto sw = StopWatch();
    sw.start(); 
    auto input = readText("9.txt");
    //auto input = getProblem(1);

    auto inputLines = input.splitLines.map!chomp().array();
    auto problem = new Problem(inputLines);

    auto res = simpleRainbowSearch(problem);
    sw.stop();

    writeln(res.numOfEmpty);
    writeln(sw.peek.msecs);
    //writeln(res.answer);
    std.file.write("ans9.txt", res.answer);
}
