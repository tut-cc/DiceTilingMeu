import vibe.d;

static struct Score
{
    int score;
    int stones;

    int opCmp(const ref Score rhs) const pure nothrow @safe @nogc
    {
        if(this.score != rhs.score) return this.score - rhs.score;
        else return this.stones - rhs.stones;
    }
}


Score bestScore = Score(int.max, int.max);


shared static this()
{
    auto router = new URLRouter;
    router.get("/update", &update);

    auto settings = new HTTPServerSettings;
    settings.port = 8080;
    settings.bindAddresses = ["::1", "127.0.0.1"];
    listenHTTP(settings, router);

    logInfo("Please open http://127.0.0.1:8080/ in your browser.");
}

import std.conv;

void update(HTTPServerRequest req, HTTPServerResponse res)
{
    if("score" !in req.query || "stones" !in req.query){
        res.writeBody(req.query.to!string);
        res.writeBody("fail");
        return;
    }

    Score score = Score(req.query["score"].to!int,
                        req.query["stones"].to!int);

    if(score < bestScore){
        bestScore = score;
        res.writeBody("update");
    }else
        res.writeBody("reject");
}
