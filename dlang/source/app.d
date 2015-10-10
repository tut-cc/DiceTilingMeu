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

enum string ip = "192.168.1.219:8080";
enum string token = "0123456789abcdef";
enum int problemID = 1;
enum string server = "testform26.procon-online.net";

void main()
{
    auto sw = StopWatch();
    sw.start(); 
    auto input = fallbackGet();

    auto inputLines = input.splitLines.map!chomp().array();
    auto problem = new Problem(inputLines);

    GeneralField res;

    if(problem.numOfEmpty < 300)
        res = simpleRainbowSearchByStone!fallbackPost(problem);
    else
        res = simpleRainbowSearchByXY!fallbackPost(problem);

    sw.stop();

    writeln(res.numOfEmpty);
    writeln(sw.peek.msecs);
    //postAnswer(res.answer).writeln();
    //writeln(res.answer);
    //std.file.write("ans42.txt", res.answer);
}



void fallbackPost(GeneralField gf)
{
    try postToMyServer(gf, ip);
    catch(Exception){
        writeln("!!!!!!Fallback POST!!!!!!");
        std.file.write("ans_" ~ Clock.currTime.toISOString() ~ ".txt", gf.answer);
    }
}


string fallbackGet()
{
    try return getFromMyServer(ip);
    catch(Exception){
        writeln("!!!!!!Fallback GET!!!!!!");
        RequestSpec spec;
        spec.host = server;
        spec.token = token;
        return getProblem(spec, problemID);
    }
}