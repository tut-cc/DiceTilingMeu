module procon26.http;

version(Procon26LocalTest) {}
else:

import std.net.curl;
import std.format;
import std.uri;
import std.array;
import std.stdio;
import std.string;

import procon26.field;


struct RequestSpec
{
    string host = `testform26.procon-online.net`;
    string token = `0123456789abcdef`;
}


string getProblem(RequestSpec spec, uint num)
{
    string url = format("http://%s/quest%s.txt?token=%s", spec.host, num, spec.token);
    writeln(url);

    return std.net.curl.get(url).dup;
}


string postAnswer(RequestSpec spec, string ans)
{
    string url = format("http://%s/answer", spec.host);

    auto app = appender!string();
    HTTP http = HTTP();

    immutable boundary = `cce6735153bf14e47e999e68bb183e70a1fa7fc89722fc1efdf03a917340`;   // 適当な文字列
    http.addRequestHeader("Content-Type", "multipart/form-data; boundary=" ~ boundary);

    app.put("--"); app.put(boundary); app.put("\r\n");
    app.put(`Content-Disposition: form-data; name="token"`); app.put("\r\n");
    app.put("\r\n");
    app.put(spec.token); app.put("\r\n");
    app.put("--"); app.put(boundary); app.put("\r\n");
    app.put(`Content-Disposition: form-data; name="answer"; filename="answer.txt"`); app.put("\r\n");
    app.put(`Content-Type: text/plain`); app.put("\r\n");
    app.put("\r\n");
    app.put(ans); app.put("\r\n");
    app.put("--"); app.put(boundary); app.put("--\r\n");

    return std.net.curl.post(url, app.data, http).dup;
}


string getFromMyServer(string serverAddress)
{
    string url = format("http://%s/problem", serverAddress);
    return std.net.curl.get(url).dup;
}


string postToMyServer(GeneralField field, string serverAddress)
{
    string url = format("http://%s/update", serverAddress);

    auto app = appender!string();
    HTTP http = HTTP();

    immutable boundary = `cce6735153bf14e47e999e68bb183e70a1fa7fc89722fc1efdf03a917340`;   // 適当な文字列
    http.addRequestHeader("Content-Type", "multipart/form-data; boundary=" ~ boundary);

    app.put("--"); app.put(boundary); app.put("\r\n");
    app.put(`Content-Disposition: form-data; name="score"`); app.put("\r\n");
    app.put("\r\n");
    app.formattedWrite("%s", field.numOfEmpty); app.put("\r\n");
    app.put("--"); app.put(boundary); app.put("\r\n");
    app.put(`Content-Disposition: form-data; name="stones"`); app.put("\r\n");
    app.put("\r\n");
    app.formattedWrite("%s", field.history.length); app.put("\r\n");
    app.put("--"); app.put(boundary); app.put("\r\n");
    app.put(`Content-Disposition: form-data; name="answer"; filename="answer.txt"`); app.put("\r\n");
    app.put(`Content-Type: text/plain`); app.put("\r\n");
    app.put("\r\n");
    app.put(field.answer); app.put("\r\n");
    app.put("--"); app.put(boundary); app.put("--\r\n");

    return std.net.curl.post(url, app.data, http).dup;
}
