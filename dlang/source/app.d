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

//version = Procon26LocalTest;

version(Procon26LocalTest) {} else
{
  __gshared JSONValue serverSettings;
}


void main()
{
  version(Procon26LocalTest) {} else
  {
    serverSettings = readText(buildPath("..", "dsetting.json")).parseJSON();
  }

    auto sw = StopWatch();
    sw.start(); 

  version(Procon26LocalTest)
  {
    auto input = readText("35.txt");
  }
  else
  {
    auto input = fallbackGet();
  }


    auto inputLines = input.splitLines.map!chomp().array();
    auto problem = new Problem(inputLines);

    GeneralField res;

    //if(problem.numOfEmpty < 300)
        res = simpleRainbowSearchByStone!fallbackPost(problem);
    //else
        //res = simpleRainbowSearchByXY!fallbackPost(problem);

    sw.stop();

    writeln(res.numOfEmpty);
    writeln(sw.peek.msecs);
}



void fallbackPost(GeneralField gf)
{
  version(Procon26LocalTest)
  {
    writefln("Answer(%s, %s)", gf.numOfEmpty, gf.history.length);
    std.file.write("ans_" ~ Clock.currTime.toISOString() ~ ".txt", gf.answer);
  }
  else
  {
    try postToMyServer(gf, serverSettings["ip"].str ~ ":8080");
    catch(Exception ex){
        writeln(ex);
        writeln("!!!!!!Fallback POST!!!!!!");
        std.file.write("ans_" ~ Clock.currTime.toISOString() ~ ".txt", gf.answer);
    }
  }
}


version(Procon26LocalTest) {} else
{
  string fallbackGet()
  {
      try return getFromMyServer(serverSettings["ip"].str ~ ":8080");
      catch(Exception ex){
          writeln(ex);
          writeln("!!!!!!Fallback GET!!!!!!");
          RequestSpec spec;
          spec.host = serverSettings["server"].str;
          spec.token = serverSettings["token"].str;
          return getProblem(spec, cast(int)serverSettings["problem"].integer);
      }
  }
}
