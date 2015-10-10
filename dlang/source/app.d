module app;

import std.stdio;
import std.string;
import std.range;
import std.array;
import std.file;
import std.datetime;
import std.json;
import std.path;

import procon26.field,
       procon26.algorithm.rainbow,
       procon26.problem,
       procon26.stone,
       procon26.util,
       procon26.http;

//enum string ip = "192.168.1.219:8080";
//enum string token = "11e2351a1459e3fc";
//enum int problemID = 1;
//enum string server = "172.16.1.2";

JSONValue serverSettings;

void main()
{
    serverSettings = readText(buildPath("..", "dsetting.json")).parseJSON();

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
    try postToMyServer(gf, serverSettings["ip"].str);
    catch(Exception){
        writeln("!!!!!!Fallback POST!!!!!!");
        std.file.write("ans_" ~ Clock.currTime.toISOString() ~ ".txt", gf.answer);
    }
}


string fallbackGet()
{
    try return getFromMyServer(serverSettings["ip"].str);
    catch(Exception){
        writeln("!!!!!!Fallback GET!!!!!!");
        RequestSpec spec;
        spec.host = serverSettings["server"].str;
        spec.token = serverSettings["token"].str;
        return getProblem(spec, cast(int)serverSettings["problem"].integer);
    }
}