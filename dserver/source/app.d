import vibe.d;
import vibe.core.file;

import std.stdio;
import std.concurrency;
import core.thread;
import std.conv;
import std.json;
import std.file;
import std.path;
import core.sync.mutex;
import procon26.http;


struct Answer
{
    int score;
    int stones;
    string answer;

    int opCmp(const ref Answer rhs) const pure nothrow @safe @nogc
    {
        if(this.score != rhs.score) return this.score - rhs.score;
        else return this.stones - rhs.stones;
    }
}


Answer bestAnswer;
__gshared immutable(Answer)* ansReq;
__gshared Mutex mtxUpdate;

__gshared JSONValue serverSettings;



shared static this()
{
    serverSettings = readText(buildPath("..", "dsetting.json")).parseJSON();

    auto router = new URLRouter;
    router.post("/update", &update);
    router.get("/hello", &hello);
    router.get("/problem", &problem);

    auto settings = new HTTPServerSettings;
    settings.port = 8080;
    settings.bindAddresses = [serverSettings["ip"].str];
    listenHTTP(settings, router);
    mtxUpdate = new TaskMutex;

    bestAnswer = Answer(int.max, int.max, "");

    spawnLinked(&postThread);

    logInfo("Please open http://" ~ serverSettings["ip"].str ~ ":8080/ in your browser.");
}


void problem(HTTPServerRequest req, HTTPServerResponse res)
{
    static TaskMutex mtx;
    static string problem;

    if(mtx is null) mtx = new TaskMutex;

    synchronized(mtx)
    {
        if(problem !is null)
        {
            writeln("have-cache");
            res.writeBody(problem);
            return;
        }

        /*
        requestHTTP("http://" ~ serverSettings["server"].str ~ "/quest" ~ serverSettings["problem"].str ~ ".txt?token=" ~ serverSettings["token"].str,
            (scope req){},
            (scope res){
                problem = res.bodyReader.readAllUTF8();
            }
        );*/
        RequestSpec spec;
        spec.token = serverSettings["token"].str;
        spec.host = serverSettings["server"].str;
        problem = getProblem(spec, cast(int)serverSettings["problem"].integer);
    }

    res.writeBody(problem);
}


void hello(HTTPServerRequest req, HTTPServerResponse res)
{
    res.writeBody("hello, world\r\n");
}


void update(HTTPServerRequest req, HTTPServerResponse res)
{
    if("score" !in req.form || "stones" !in req.form || "answer" !in req.files)
    {
        res.writeBody(req.form.to!string);
        return;
    }

    auto pf = "answer" in req.files;
    string answer = (cast(char[])readFile(pf.tempPath)).dup;

    auto ans = new immutable(Answer)(req.form["score"].to!int,
                                     req.form["stones"].to!int,
                                     answer);

    //writefln("query: %s", req.form);
    bool bUpdate = false;
    synchronized(mtxUpdate){
        if(*ans < bestAnswer){
            bestAnswer = *ans;
            bUpdate = true;
            ansReq = ans;
            ans = null;
        }
    }


    if(bUpdate)
        res.writeBody("update\r\n");
    else
        res.writeBody("reject\r\n");
}


void postThread()
{
    while(1)
    {
        try{
            while(1){
                synchronized(mtxUpdate){
                    if(ansReq !is null)
                        break;
                }
                Thread.sleep(dur!"msecs"(200));
            }

            try{
                synchronized(mtxUpdate){
                    writeln("!!!!!!!!!!POST!!!!!!!!!!!", Clock.currTime);

                    RequestSpec spec;
                    spec.token = serverSettings["token"].str;
                    spec.host = serverSettings["server"].str;
                    postAnswer(spec, ansReq.answer).writeln();

                    ansReq = null;
                }
            }
            catch(Exception ex){ writeln(ex); }
            Thread.sleep(dur!"msecs"(1000));
        }
        catch(Exception ex)
        {
            writeln(ex);
        }
    }
}
