import vibe.d;
import vibe.core.file;

import std.stdio;
import std.concurrency;
import core.thread;
import std.conv;
import std.json;
import std.file;
import core.sync.mutex;


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


TaskMutex mtxProblem;
JSONValue serverSettings;



shared static this()
{
    serverSettings = readText("setting.json").parseJSON();

    auto router = new URLRouter;
    router.post("/update", &update);
    router.get("/hello", &hello);
    router.get("/problem", &problem);

    auto settings = new HTTPServerSettings;
    settings.port = 8080;
    settings.bindAddresses = ["::8080", serverSettings["ip"].str];
    listenHTTP(settings, router);
    mtxUpdate = new TaskMutex;
    mtxProblem = new TaskMutex;

    bestAnswer = Answer(int.max, int.max, "");

    spawn(&postThread);

    logInfo("Please open http://" ~ serverSettings["ip"].str ~ ":8080/ in your browser.");
}


void problem(HTTPServerRequest req, HTTPServerResponse res)
{
    __gshared string problem;

    synchronized(mtxProblem)
    {
        if(problem !is null)
        {
            writeln("have-cache");
            res.writeBody(problem);
            return;
        }

        requestHTTP("http://" ~ serverSettings["server"].str ~ "/quest" ~ serverSettings["problem"].str ~ ".txt?token=" ~ serverSettings["token"].str,
            (scope req){},
            (scope res){
                problem = res.bodyReader.readAllUTF8();
            }
        );
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

    writeln("!!!!!!!!!!GET!!!!!!!!!!!");
    writeln(*ans);
    bool bUpdate = false;

    if(*ans < bestAnswer){
        bestAnswer = *ans;
        bUpdate = true;
        synchronized(mtxUpdate){
            ansReq = ans;
        }
        ans = null;
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
        while(1){
            synchronized(mtxUpdate){
                if(ansReq !is null)
                    break;
            }
            Thread.sleep(dur!"msecs"(200));
        }

        synchronized(mtxUpdate){
            writeln("!!!!!!!!!!POST!!!!!!!!!!!", Clock.currTime);
            //writeln(*ansReq);
            postAnswer(ansReq.answer);
            ansReq = null;
        }
        Thread.sleep(dur!"msecs"(1000));
    }
}


string postAnswer(string ans)
{
    import std.net.curl;

    string url = format("http://%s/answer", serverSettings["server"].str);

    auto app = appender!string();
    HTTP http = HTTP();

    immutable boundary = `cce6735153bf14e47e999e68bb183e70a1fa7fc89722fc1efdf03a917340`;   // 適当な文字列
    http.addRequestHeader("Content-Type", "multipart/form-data; boundary=" ~ boundary);

    app.put("--"); app.put(boundary); app.put("\r\n");
    app.put(`Content-Disposition: form-data; name="token"`); app.put("\r\n");
    app.put("\r\n");
    app.put(serverSettings["token"].str); app.put("\r\n");
    app.put("--"); app.put(boundary); app.put("\r\n");
    app.put(`Content-Disposition: form-data; name="answer"; filename="answer.txt"`); app.put("\r\n");
    app.put(`Content-Type: text/plain`); app.put("\r\n");
    app.put("\r\n");
    app.put(ans); app.put("\r\n");
    app.put("--"); app.put(boundary); app.put("--\r\n");

    return std.net.curl.post(url, app.data, http).dup;
}
