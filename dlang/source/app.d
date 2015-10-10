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
    auto input = readText("42.txt");
    //auto input = getProblem(1);

    auto inputLines = input.splitLines.map!chomp().array();
    auto problem = new Problem(inputLines);

    GeneralField res;

    if(problem.numOfEmpty < 300)
        res = simpleRainbowSearchByStone(problem);
    else
        res = simpleRainbowSearchByXY(problem);

    sw.stop();

    writeln(res.numOfEmpty);
    writeln(sw.peek.msecs);
    //postAnswer(res.answer).writeln();
    //writeln(res.answer);
    //std.file.write("ans42.txt", res.answer);
}
